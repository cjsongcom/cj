
#ifndef _CJGETOPT_H_
#define _CJGETOPT_H_


#ifdef __cplusplus
extern "C" {
#endif


//
// cj_getopt
//

struct cjgetopt_option
{
	const char* name;
	int has_arg;
	int* flag;
	int val;
};

struct cjgetopt
{
	int opterr;					/* if error message should be printed */
	int optind;					/* index into parent argv vector */
	int optopt;					/* character checked for validity */
	int optreset;				/* reset getopt */
	char* optarg;				/* argument associated with option */

	char* __progname;
	char* place;
};

#define cjgetopt_no_argument       0
#define cjgetopt_required_argument 1
#define cjgetopt_optional_argument 2


//void cjgetopt_init(struct cjgetopt* ctx);

void cjgetopt_reset(struct cjgetopt* ctx);

int cjgetopt_getopt(struct cjgetopt* ctx, int nargc, char* const nargv[], const char* ostr);

int cjgetopt_getoptlong(struct cjgetopt* ctx, int nargc, char** nargv, char* options,
	struct cjgetopt_option* long_options, int* index);


#ifdef __cplusplus
}
#endif



#endif
