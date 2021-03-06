// auxhtmlhelp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "audfret.h"
class CBIF // Built-In Function class
{
public:
	CBIF() { alwaysstatic = false; stat = 0; };
	virtual ~CBIF() {};
	string name;
	string funcsignature;
	int stat; //0: non-static, 1: static; 2: hybrid
	bool alwaysstatic; // true means the function cannot be called as a member function
	int narg1, narg2;
};

class mol
{
public:
	int nItems;
	double ddres;
	int (mol::*pf)();
	int func1();
	int func2();
	int func3();
	mol() { nItems = 1; ddres = .1; };
	virtual ~mol() {};
};

int mol::func1()
{
	return nItems;
}

int mol::func2()
{
	return nItems+1;
}

int mol::func3()
{
	return nItems+2;
}

vector<CBIF> out;
vector<CBIF> InitBuiltInFunctionList();
void mathfunctionlist();

#define CH(X) \
if ((X)==0) {printf("%s: %s\nPress any key and enter\n", filename, errstr); fgetc(stdin); return count; }

int makeTextFiles(const char *path)
{
	char buf[2048], filename[256], errstr[256];
	strcpy(buf, " ");
	int count(1);
	for (vector<CBIF>::iterator it = out.begin(); it != out.end(); count++, it++)
	{
		sprintf(filename, "%s%s.txt", path, (*it).name.c_str());
		CH(printfINI(errstr, filename, "FUNCTION NAME", "%s", (*it).name.c_str()))
		int b = it->stat == 2 ? 2: it->alwaysstatic ? 1 : 0;
		CH(printfINI(errstr, filename, "STATIC", "%d", b))
		CH(printfINI(errstr, filename, "FUNCTION DESCRIPTION", "%s", buf))
		CH(printfINI(errstr, filename, "PARAMETERS", "%s", buf))
		CH(printfINI(errstr, filename, "PARAM TYPE DELIM BY TAB", "%s", buf))
		CH(printfINI(errstr, filename, "PARAM DESCRIPTION--MULTILINES", "%s", buf))
		CH(printfINI(errstr, filename, "PARAM UNIT--MULTILINES", "%s", buf))
		CH(printfINI(errstr, filename, "NOTES", "%s", buf))
		CH(printfINI(errstr, filename, "EXAMPLES", "%s", buf))
		CH(printfINI(errstr, filename, "ALGORITHM", "%s", buf))
		CH(printfINI(errstr, filename, "SEE ALSO", "%s", buf))
		CH(printfINI(errstr, filename, "OUTPUT ARGS BY TAB", "%s", "y"))
		CH(printfINI(errstr, filename, "OUTPUT ARGS--MULTILINES", "%s", "#q1 is"))
		printf("%d: %s saved.\n", count, (*it).name.c_str());
	}
	return count;
}

void replace(string &body, string findit, string replacewith)
{
	size_t pos = body.find(findit);
	while (pos != string::npos)
	{
		body.replace(pos, findit.length(), replacewith);
		pos = body.find(findit, pos + replacewith.length());
	}
}

//If the heading is not found, this stops
#define READ(TOKEN,ITEM) \
{ if(ReadINI(errstr, filename,TOKEN,ITEM)<0) { sprintf(estr, "In %s, %s", filename, errstr); return 0;}}

size_t replaceParamString(string &in, string marker, vector<string> param)
{ // This assumes that all params are from param1 to param9
	size_t pos1, pos2;
	pos1 = in.find(marker);
	while (pos1 != string::npos)
	{
		pos2 = pos1 + 3; // #p_integer: 3 characters
		string sub = in.substr(pos1 + 2, pos2 - pos1 - 1);
		int id;
		if (sscanf(sub.c_str(), "%d", &id) == EOF)
		{
			printf_s("Invalid parameter marker...%s where is id?\n", sub.c_str());
			return 0;
		}
		if (id > param.size())
		{
			printf_s("%s\nparameter index exceeding string containing %s\n", in.c_str(), marker.c_str());
			return 0;
		}
		string sub2 = "<b><font face=\"Courier New\">";
		sub2 += param[id - 1] + "</font></b>";
		in.replace(pos1, pos2 - pos1, sub2);
		pos1 = in.find(marker, pos1);
	}
	return in.length();
}

string makeli(string in, vector<string> param1, vector<string> param2)
{ // each line from in is enclosed with a <li> tag
	string out;
	vector<string> lines;
	size_t res = str2vect(lines, in.c_str(), '\n');
	if (lines.back().empty()) { lines.pop_back(); res--; }

	for (size_t k = 0; k < res; k++)
	{
		if (replaceParamString(lines[k], "#p", param1) == 0) return "";
		if (replaceParamString(lines[k], "#q", param2) == 0) return "";
		out += "<li>";
		out += lines[k];
		out += "</li>";
	}
	return out;
}

#include <direct.h>

#define SEEALSO_STRING "<table id=\"notes\"><tr><th>See Also</th></tr></table>"

int text2htm(char *path1, char *path2, char *estr)
{
	string sbuf;
	char filename[256], targetfilename[256], templatefilename[256], errstr[256];
	sprintf(templatefilename, "%s%s", path1, "template.html");
	int res, count(0);
	string fname;
	size_t nItems, nItems2, pos0, pos2, pos1;
	string head1("[PARAM DESCRIPTION--MULTILINES]");
	string head2("[PARAM UNIT--MULTILINES]");
	string head3("[NOTES]");
	WIN32_FIND_DATA ls;
	strcpy(targetfilename, path2);
	HANDLE hFind = FindFirstFile(targetfilename, &ls);
	if (hFind== INVALID_HANDLE_VALUE)
	{
		if (!_mkdir(targetfilename))
			printf_s("Directory %s created.\n", targetfilename);
		else
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				printf("%s found.\n", targetfilename);
			else
			{
				printf_s("Cannot create the directory %s created.\n", targetfilename);
				return -1;
			}
		}
	}
	else
	{
		if (!(ls.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			printf_s("%s already exists and not a directory name\n", targetfilename);
			return -1;
		}
	}
	for (vector<CBIF>::iterator it = out.begin(); it != out.end(); it++)
	{
		//if ((*it).name == "fread")
		//	sbuf = "sdfsd";
		sprintf(filename, "%s%s.txt", path1, (*it).name.c_str());
		FILE *fp = fopen(filename, "rt");
		if (!fp) continue;
		fclose(fp);
		res = GetFileText(templatefilename, "rt", sbuf);
		if (res==-1) return -1; // template not found
		replace(sbuf, "%", "%%");

		sprintf(targetfilename, "%s%s.html", path2, (*it).name.c_str());
		
		string fdescription, notes, seealso, examples, algorit;
		vector<string> param, param_desc, param_unit, param_type, outarg, outarg_desc;
		string tp("");
		READ("FUNCTION NAME", fname);
		READ("PARAMETERS", tp);
		nItems = str2vect(param, tp.c_str(), " \t");
		vector<string> defaultvalue(nItems, "");
		for (size_t k = 0; k < nItems; k++)
		{
			vector<string> temp;
			size_t homany = str2vect(temp, param[k].c_str(), "=");
			if (homany > 1)
			{
				defaultvalue[k] = temp[1];
				param[k] = temp[0];
			}
		}
		READ("OUTPUT ARGS BY TAB", tp);
		nItems2 = str2vect(outarg, tp.c_str(), "\t");

		READ("FUNCTION DESCRIPTION", fdescription);
		replaceParamString(fdescription, "#p", param);
		replaceParamString(fdescription, "#q", outarg);

		res = GetFileText(filename, "rt", tp);

		pos0 = tp.find(head1);
		pos1 = tp.find(head2);
		pos2 = tp.find(head3);
		size_t len1 = head1.length();
		size_t len2 = head2.length();
		string tp2 = tp.substr(pos0 + len1, pos1 - pos0 - len1);
		replace(tp2, "\r\n", "\n");
		size_t dd = str2vect(param_desc, tp2, '\n');
		if (param_desc.back().empty()) {
			param_desc.pop_back(); dd--;
		}
		if (dd != nItems) { sprintf(estr, "In %s, %s has %d items (should be %d).", filename, "PARAM DESCRIPTION--MULTILINES", dd, nItems); return 0; }
		tp2 = tp.substr(pos1 + len2, pos2 - pos1- len2);
		replace(tp2, "\r\n", "\n");
		dd = str2vect(param_unit, tp2, '\n');
		if (param_unit.back().empty()) {
			param_unit.pop_back(); dd--;
		}
		if (dd != nItems) { sprintf(estr, "In %s, %s has %d items (should be %d).", filename, "PARAM UNIT--MULTILINES", dd, nItems); return 0; }
		READ("PARAM TYPE DELIM BY TAB", tp);
		dd = str2vect(param_type, tp.c_str(), "\t");
		if (dd != nItems) { sprintf(estr, "In %s, %s has %d items (should be %d).", filename, "PARAM TYPE", dd, nItems); return 0; }
		READ("OUTPUT ARGS--MULTILINES", tp);
		dd = str2vect(outarg_desc, tp.c_str(), "\n");
		if (dd != nItems2) { sprintf(estr, "In %s, %s has %d items (should be %d).", filename, "OUTPUT ARGS--MULTILINES", dd, nItems2); return 0; }

		READ("NOTES", notes);
		READ("EXAMPLES", examples);
		READ("SEE ALSO", seealso);
		
		tp = "";
		replace(sbuf, "_fname", fname);
		replace(sbuf, "_fdescription", fdescription);
		//usage
		string lhs;
		if (outarg.empty() || (outarg.size() == 1 && outarg.front().empty()))
			lhs = "";
		else if (outarg.size() == 1)
		{
			lhs = outarg.front();
			lhs += " = ";
		}
		else
		{
			lhs = "[";
			for (vector<string>::iterator it = outarg.begin(); it != outarg.end(); )
			{
				lhs += *it;
				it++;
				if (it == outarg.end())
				{
					lhs += "] = ";
					break;
				}
				else
					lhs += ", ";
			}
		}
		tp2 = lhs;
		tp2 += fname;
		tp2 += " ( ";
		vector<string>::iterator it3, it4, it2 = defaultvalue.begin();
		for (vector<string>::iterator it = param.begin(); it != param.end(); it++, it2++)
		{
			if (!tp.empty()) tp += ',';
			if (it == param.begin() && !param.front().empty() && param.front().at(0) == '_')
				tp += param.front().substr(1);
			else
				tp += *it;
			if (!(*it2).empty())
			{
				tp += "[=";
				tp += *it2;
				tp += "]";
			}
		}
		tp += " )";
		tp2 += tp;
		if ((*it).stat == 2)
		{
			int tcount(0);
			it2 = defaultvalue.begin();
			if (param.front().at(0) == '_')
			{
				param.front() = param.front().substr(1);
				string tp3(lhs);
				tp = fname + " ( ";
				it2 = defaultvalue.begin() + 1;
				int tcount(0);
				for (vector<string>::iterator it = param.begin() + 1; it != param.end(); it++, it2++, tcount++)
				{
					if (tcount) tp += ',';
					tp += *it;
					if (!(*it2).empty())
					{
						tp += "[=";
						tp += *it2;
						tp += "]";
					}
				}
				tp += " )";
				tp3 += tp;
				tp3 += "<br>";
				tp2.insert(0, tp3);

				tp2 += "<br>";
				tp2 += lhs;
				it2 = defaultvalue.begin() + 1;
				tp = param.front() + ".";
				tp += fname + " ( ";
				tcount = 0;
				for (vector<string>::iterator it = param.begin() + 1; it != param.end(); it++, it2++, tcount++)
				{
					if (tcount) tp += ',';
					tp += *it;
					if (!(*it2).empty())
					{
						tp += "[=";
						tp += *it2;
						tp += "]";
					}
				}
				tp += " )";
				tp2 += tp;
			}
		}
		else if (!(*it).alwaysstatic) 
		{
			tp2 += "<br>";
			tp2 += lhs;
			it2 = defaultvalue.begin()+1;
			tp = param.front() + ".";
			tp += fname + " ( ";
			int tcount(0);
			for (vector<string>::iterator it = param.begin()+1; it != param.end(); it++, it2++, tcount++)
			{
				if (tcount) tp += ',';
				tp += *it;
				if (!(*it2).empty())
				{
					tp += "[=";
					tp += *it2;
					tp += "]";
				}
			}
			tp += " )";
			tp2 += tp;
		}
		//If OPERATOR is found
		tp.clear();
		ReadINI(errstr, filename, "OPERATOR", tp);
		if (!tp.empty())
		{
			tp2 += "<br>";
			tp2 += lhs + " ";
			tp2 += param.front() + " ";
			if (tp == "%") tp += tp;
			tp2 += tp + " ";
			tp2 += *(param.begin() + 1);
		}
		replace(sbuf, "to be auto-generated", tp2);
		//argument table
		pos1 = sbuf.find("<tr><td>_param");
		pos2 = sbuf.find("unit</td></tr>") + string("unit</td></tr>").length();
		it2 = param_type.begin();
		it3 = param_desc.begin();
		it4 = param_unit.begin();
		tp.clear();
		for (vector<string>::iterator it = param.begin(); it != param.end(); it++, it2++, it3++, it4++)
		{
			tp += "<tr>";
			if (it == param.begin()) tp += "<td width=\"15%%\">";
			else tp += "<td>";
			tp += "<b><font face=\"Courier New\">";	tp += *it + "</font></b></td>";
			if (it == param.begin()) tp += "<td width=\"10%%\">";
			else tp += "<td>";
			tp += *it2 + "</td>";
			if (it == param.begin()) tp += "<td width=\"50%%\">";
			else tp += "<td>";
			tp += *it3 + "</td>";
			if (it == param.begin()) tp += "<td width=\"25%%\">";
			else tp += "<td>";
			if ((*it4).find("(n/a)")!=string::npos)		tp += *it4 + "</td>";
			tp += "</tr>"; tp += "\n";
		}
		sbuf.replace(pos1, pos2 - pos1, tp);
		//outputs
		if (outarg.empty())
			replace(sbuf, "<tr><td><ul><li>auto-generate notes. If empty, delete from (tr)(td) til (/td)(/tr)</li></ul></td></tr>", "");
		else
		{
			string newstring = "\n  <tr><td><ul>";
			string tstr;
			it2 = outarg_desc.begin();
			for (vector<string>::iterator it = outarg.begin(); it != outarg.end(); it++, it2++)
			{
				tstr = makeli(*it2, param, outarg);
				newstring += tstr;
	//			newstring += "\n";
			}
			//Locate <tr><th>Outputs</th></tr>; tp is the position right after this 
			size_t tp = sbuf.find("<tr><th>Outputs</th></tr>") + string("<tr><th>Outputs</th></tr>").size();
			newstring += "</ul></td></tr>";
			sbuf.insert(tp, newstring);
		}
		//notes
		if (notes.empty())
			replace(sbuf, "<tr><td><ul><li>auto-generate notes. If empty, delete from (tr)(td) til (/td)(/tr)</li></ul></td></tr>", "");
		else
		{
			notes = makeli(notes, param, outarg);
			replace(sbuf, "<li>auto-generate notes. If empty, delete from (tr)(td) til (/td)(/tr)</li>", notes);
		}
		//examples
		if (examples.empty())
			replace(sbuf, "<tr><td><ul><li>auto-generate examples. If empty, delete from (tr)(td) til (/td)(/tr)</li></ul></td></tr>", "");
		else
		{
			examples = makeli(examples, param, outarg);
			replace(sbuf, "<li>auto-generate examples. If empty, delete from (tr)(td) til (/td)(/tr)</li>", examples);
		}
		//see also
		pos0 = sbuf.find(SEEALSO_STRING);
		if (pos0 == string::npos)
		{
			sprintf(estr, "Critical error: template.html doesn't have %s\n", SEEALSO_STRING);
			return 0;
		}
		pos1 = pos0 + string(SEEALSO_STRING).length();
		vector<string> items;
		nItems = str2vect(items, seealso.c_str(), " \t\r\n");
		tp = "";
		for (unsigned int k = 0; k < nItems; k++)
		{
			tp += string("<a href=\"") + items[k] + ".html\">" + items[k];
			if (k < nItems-1) tp += "</a>&nbsp;&#124; ";
			else  tp += " </a>";
		}
		sbuf.insert(pos1, tp);

		//if ALGORITHM exists
		if (ReadINI(errstr, filename, "ALGORITHM", algorit) > 0)
		{
			pos0 = sbuf.find("</body>");
			if (pos0 == string::npos)
			{
				sprintf(estr, "Critical error: template.html doesn't have </body>\n");
				return 0;
			}
			tp = "<p></p><table id=\"notes\"><tr><th>Algorithm</th></tr></table>";
			tp += algorit;
			tp += "<p></p>";
			sbuf.insert(pos0, tp);
		}
		tp2 = "AUX function ";
		tp2 += fname;
		replace(sbuf, "AUX HELP template", tp2);
		fp = fopen(targetfilename, "wt");
		if (fprintf(fp, sbuf.c_str()))
		{
			count++;
			printf("%s saved.\n", targetfilename);
		}
		fclose(fp);
	}
	printf("Total %d files generated.\n", count);
	return 1;
}


/*
int t2txt(char *path)
{ // I have a bunch of old txt files that I renamed to *.t.
	// They need to be updated with the new format
	// This function does the batch work.
	// 6/9/2018
	WIN32_FIND_DATA ls;
	int res, count(0);
	FILE *fp;
	string sbuf;
	string buffer = "[OUTPUT ARGS BY TAB] y\n[OUTPUT ARGS--MULTILINES] #q1 is ";
	char search[256], fname[256], fullfileInput[256], errstr[256];
	sprintf(search, "%s\\*.t", path);
	HANDLE hFind = FindFirstFile(search, &ls);
	if (hFind == INVALID_HANDLE_VALUE)
		printf("No *.t files found.\n");
	else
	{
		do {
			//Read the content
			sprintf(fullfileInput, "%s%s", path, ls.cFileName);
			res = GetFileText(fullfileInput, "rt", sbuf);
			if (res>0)
			{
				//make new file
				sprintf(fname, "%s%sxt", path, ls.cFileName);
				fp = fopen(fname, "wt");
				fprintf(fp, sbuf.c_str());
				fprintf(fp, buffer.c_str());
				fclose(fp);
				printf("%s created.\n", fname);
				count++;
			}
		} while (FindNextFile(hFind, &ls));
		printf("A total of %d txt files created.\n", count);
		return count;
	}
}
*/

int main(int argc, char **argv)
{
	char iniFile[256], name[256], buf[256] = {};
	char estr[256], path1[256] = {}, path2[256] = {}, path3[256] = {};
	DWORD dw = sizeof(buf);
	GetComputerName(buf, &dw);
	_splitpath(argv[0], NULL, NULL, name, NULL);
	sprintf(iniFile, "%s.%s.ini", name, buf);
	/* ini file has the following tags
	TXT_IN_PATH: This is where the prepared txt files and template.html are located 
	HTML_OUT_PATH: output path
	GENERATE_BLANK_TXT_PATH: This is where the blank txt files are generated.
	*/

	string read;
	if (ReadINI(estr, iniFile, "TXT_IN_PATH", read) >= 0)
		strcpy(path1, read.c_str());
	else
	{
		printf("ReadINI returns negative. file: %s, estr: %s\n", iniFile, estr);
		printf("What is the path for the prepared txt files?\n");
		fgets(path1, 256, stdin);
		if (!printfINI(estr, iniFile, "TXT_IN_PATH", "%s", path1))
			printf("printfINI error for %s--%s\n", path1, estr);
	}
	if (ReadINI(estr, iniFile, "HTML_OUT_PATH", read) >= 0)
		strcpy(path2, read.c_str());
	else
	{
		printf("What is the path to generate html files?\n");
		fgets(path2, 256, stdin);
		if (!printfINI(estr, iniFile, "HTML_OUT_PATH", "%s", path2))
			printf("printfINI error for %s--%s\n", path2, estr);
	}
	if (ReadINI(estr, iniFile, "GENERATE_BLANK_TXT_PATH", read) >= 0)
		strcpy(path3, read.c_str());
	else
	{
		printf("Where do you want to generate blank txt files (you need to have created the folder)?\n");
		fgets(path3, 256, stdin);
		if (!printfINI(estr, iniFile, "GENERATE_BLANK_TXT_PATH", "%s", path3))
			printf("printfINI error for %s--%s\n", path3, estr);
	}
	if (path1[strlen(path1) - 1] != '\\') strcat(path1, "\\");
	if (path2[strlen(path2) - 1] != '\\') strcat(path2, "\\");
	if (path3[strlen(path3) - 1] != '\\') strcat(path3, "\\");

	int res;
	out = InitBuiltInFunctionList();
	FILE *fp = fopen("funct_list", "wt");
	for (vector<CBIF>::iterator it = out.begin(); it != out.end(); it++)
	{
		fprintf(fp, "%s\n", (*it).name.c_str());
	}
	fclose(fp);
	printf("funct_list created with %d files.\n", out.size());
	printf("Do you want to generate blank txt files? ");
	char c = fgetc(stdin);
	if (c == 'Y' || c == 'y')
	{
		int count = makeTextFiles(path3);
		printf("%d blank files generated.\n", count);
	}

//	res = t2txt(path);
//	mathfunctionlist();

	res = text2htm(path1, path2, estr);
	if (res == -1)
		printf("template.html not found.\n");
	else if (res == 0) // other error
		printf("%s\n", estr);

   return 0;
}

