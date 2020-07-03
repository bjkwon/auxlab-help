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

vector<CBIF> InitBuiltInFunctionList()
{
	vector<CBIF> out;
	CBIF pp;

	pp.name = "tone";
	pp.alwaysstatic = true;
	out.push_back(pp);

	pp.name = "sam";
	pp.alwaysstatic = false;
	out.push_back(pp);

	pp.name = "fm";
	pp.alwaysstatic = true;
	out.push_back(pp);

	pp.alwaysstatic = true;
	const char *f0[] = { "noise", "gnoise", "silence", "dc",0 };
	for (int k = 0; f0[k]; k++)
	{
		pp.name = f0[k];
		out.push_back(pp);
	}

	pp.name = "wave";
	pp.alwaysstatic = true;
	out.push_back(pp);
	pp.name = "issame";
	out.push_back(pp);

	pp.name = "wavwrite";
	pp.alwaysstatic = false;
	out.push_back(pp);

	pp.name = "write";
	pp.alwaysstatic = false;
	out.push_back(pp);
	pp.name = "dtype";
	out.push_back(pp);
	pp.name = "fprintf";
	out.push_back(pp);
	pp.name = "fread";
	out.push_back(pp);
	pp.name = "fwrite";
	out.push_back(pp);
	pp.name = "fclose";
	out.push_back(pp);

	pp.name = "ismember";
	out.push_back(pp);
	pp.name = "erase";
	out.push_back(pp);
	pp.name = "head";
	out.push_back(pp);

	pp.name = "setfs"; // check this... is narg1 one correct?
	pp.alwaysstatic = true;
	out.push_back(pp);

	pp.name = "audio";
	pp.alwaysstatic = false;
	out.push_back(pp);

	pp.name = "squeeze";
	pp.alwaysstatic = false;
	out.push_back(pp);


	// begin narg 2 and 2
	pp.alwaysstatic = false;

	pp.name = "ramp";
	out.push_back(pp);

	pp.name = "timestretch";
	out.push_back(pp);
	
	pp.name = "pitchscale";
	out.push_back(pp);

	pp.name = "respeed";
	out.push_back(pp);

	pp.name = "movespec";
	out.push_back(pp);
	
	
	// end narg 2 and 2

	const char *f1[] = { "hann", "hamming", 0 };
	for (int k = 0; f1[k]; k++)
	{
		pp.name = f1[k];
		out.push_back(pp);
	}

	pp.name = "blackman";
	out.push_back(pp);

	const char *f2[] = { "lpf", "hpf", 0 };
	for (int k = 0; f2[k]; k++)
	{
		pp.name = f2[k];
		out.push_back(pp);
	}

	const char *f3[] = { "bpf", "bsf", 0 };
	for (int k = 0; f3[k]; k++)
	{
		pp.name = f3[k];
		out.push_back(pp);
	}

	pp.name = "left";
	out.push_back(pp);
	pp.name = "right";
	out.push_back(pp);

	pp.name = "std";
	out.push_back(pp);
	pp.name = "atmost";
	out.push_back(pp);
	pp.name = "atleast";
	out.push_back(pp);

	const char *f4[] = { "min", "max", 0 };
	for (int k = 0; f4[k]; k++)
	{
		pp.name = f4[k];
		out.push_back(pp);
	}


	pp.name = "diff";
	out.push_back(pp);
	pp.name = "cumsum";
	out.push_back(pp);
	pp.name = "rms";
	out.push_back(pp);
	pp.name = "rmsall";
	out.push_back(pp);

	pp.name = "rand";
	out.push_back(pp);
	pp.name = "irand";
	out.push_back(pp);
	pp.name = "randperm";
	out.push_back(pp);
	pp.name = "zeros";
	out.push_back(pp);
	pp.name = "ones";
	out.push_back(pp);
	pp.name = "cell";
	out.push_back(pp);
	pp.name = "clear";
	out.push_back(pp);

	pp.name = "matrix";
	out.push_back(pp);

	pp.name = "conv";
	out.push_back(pp);
	pp.name = "interp";
	out.push_back(pp);

	pp.alwaysstatic = true;
	pp.name = "fopen";
	out.push_back(pp);
	pp.name = "fdelete";
	out.push_back(pp);
	pp.name = "sprintf";
	out.push_back(pp);
	pp.name = "tic";
	out.push_back(pp);
	pp.name = "toc";
	out.push_back(pp);

	//where is '#'????

	pp.name = "getfs";
	out.push_back(pp);

	pp.name = "file";
	out.push_back(pp);
	pp.name = "include";
	out.push_back(pp);

	pp.stat = 2;
	pp.name = "delete";
	out.push_back(pp);

	pp.stat = 0;
	pp.name = "dir";
	out.push_back(pp);

	pp.alwaysstatic = false;
	pp.name = "isempty";
	out.push_back(pp);
	pp.name = "isaudio";
	out.push_back(pp);
	pp.name = "isvector";
	out.push_back(pp);
	pp.name = "isstring";
	out.push_back(pp);
	pp.name = "iscell";
	out.push_back(pp);
	pp.name = "isclass";
	out.push_back(pp);
	pp.name = "isbool";
	out.push_back(pp);
	pp.name = "istseq";
	out.push_back(pp);
	pp.name = "isstereo";
	out.push_back(pp);
	pp.name = "isaudioat";
	out.push_back(pp);

	pp.name = "tsq_isrel";
	out.push_back(pp);
	pp.name = "tsq_gettimes";
	out.push_back(pp);
	pp.name = "tsq_settimes";
	out.push_back(pp);
	pp.name = "tsq_getvalues";
	out.push_back(pp);
	pp.name = "tsq_setvalues";
	out.push_back(pp);

	pp.name = "size";
	out.push_back(pp);

	pp.name = "filt";
	out.push_back(pp);
	pp.name = "filtfilt";
	out.push_back(pp);

	pp.name = "fft";
	out.push_back(pp);

	pp.name = "envelope";
	out.push_back(pp);
	pp.name = "hilbert";
	out.push_back(pp);
	pp.name = "ifft";
	out.push_back(pp);
	pp.name = "sort";
	out.push_back(pp);

	pp.alwaysstatic = false;
	pp.name = "vector";
	out.push_back(pp);
	pp.stat = 2;
	pp.name = "play";
	out.push_back(pp);
	pp.stat = 0;
	pp.name = "qstop";
	out.push_back(pp);
	pp.name = "stop";
	out.push_back(pp);
	pp.name = "pause";
	out.push_back(pp);
	pp.name = "resume";
	out.push_back(pp);
	pp.name = "status";
	out.push_back(pp);

	pp.alwaysstatic = true;
	pp.name = "msgbox";
	out.push_back(pp);

	pp.alwaysstatic = false;
	pp.name = "inputdlg";
	out.push_back(pp);
	pp.name = "input";
	out.push_back(pp);

	pp.alwaysstatic = false;
	pp.name = "and";
	out.push_back(pp);
	pp.name = "or";
	out.push_back(pp);

	pp.alwaysstatic = true;
	pp.name = "eval";
	out.push_back(pp);
	pp.alwaysstatic = false;
	pp.name = "str2num";
	out.push_back(pp);
	pp.name = "pow";
	out.push_back(pp);
	const char *fmath1[] = { "abs", "sqrt", "conj", "real", "imag", "angle", "sign", "sin", "cos", "tan", "asin", "acos", "atan", "log", "log10", "exp", "round", "fix", "ceil", "floor", 0 };
	for (int k = 0; fmath1[k]; k++)
	{
		pp.name = fmath1[k];
		out.push_back(pp);
	}
	const char *fmath2[] = { "mod", 0 };
	for (int k = 0; fmath2[k]; k++)
	{
		pp.name = fmath2[k];
		out.push_back(pp);
	}
	pp.alwaysstatic = false;
	const char *f5[] = { "begint", "dur", "endt", "sum", "mean", "length", 0 };
	for (int k = 0; f5[k]; k++)
	{
		pp.name = f5[k];
		out.push_back(pp);
	}

	pp.alwaysstatic = true;
	pp.name = "figure";
	out.push_back(pp);

	pp.stat = 2;
	pp.name = "plot";
	out.push_back(pp);
	pp.name = "line";
	out.push_back(pp);

	pp.stat = 0;
	pp.alwaysstatic = false;
	pp.name = "axes";
	out.push_back(pp);

	pp.name = "text";
	out.push_back(pp);

	pp.name = "delete";
	out.push_back(pp);

	pp.name = "replicate";
	out.push_back(pp);
	pp.name = "repaint";
	out.push_back(pp);

	pp.alwaysstatic = false;
	pp.name = "contig";
	out.push_back(pp);
	pp.name = "nullin";
	out.push_back(pp);

	return out;
}

void mathfunctionlist()
{
	const char *fmath1[] = { "abs", "sqrt", "conj", "real", "imag", "angle", "db", "sgn", "sin", "cos", "tan", "asin", "acos", "atan", "log", "log10", "exp", "round", "fix", "ceil", "floor", "^", "mod", 0 };
	FILE* fp = fopen("mathfunctions", "wt");
	for (int k = 0; fmath1[k]; k++)
	{
		fprintf(fp, "%s\n", fmath1[k]);
	}
	fclose(fp);
}
