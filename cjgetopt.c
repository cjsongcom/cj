
#include "cj.h"
#include "cjgetopt.h"


#define __P(x) x
#define _DIAGASSERT(x) assert(x)

#define	BADCH	(int)'?'
#define	BADARG	(int)':'
#define	EMSG	""

#define lws_ptr_diff(head, tail) \
			((int)((char *)(head) - (char *)(tail)))


static char* cjgetopt_progname(char* nargv0)
{
	char* tmp;

	_DIAGASSERT(nargv0 != NULL);

	tmp = strrchr(nargv0, '/');

	if (tmp)
		tmp++;
	else
		tmp = nargv0;

	return(tmp);
}


void cjgetopt_reset(struct cjgetopt* ctx)
{
	cjmemzero(ctx, sizeof(struct cjgetopt));

	// skip first argument, argv[0]
	ctx->optind = 1; /* index into parent argv vector */
	ctx->opterr = 1; /* if error message should be printed */

	ctx->__progname = cjnull;
	ctx->place = EMSG;

	// reset
	ctx->optreset = 1;
}


int cjgetopt_getopt(struct cjgetopt* ctx, int nargc, char* const nargv[], const char* ostr)
{
	//static char* __progname = 0;
	//static char* place = EMSG;		/* option letter processing */

	char* oli;				/* option letter list index */

	//__progname = __progname ? __progname : _progname(*nargv);
	ctx->__progname = ctx->__progname ? ctx->__progname : cjgetopt_progname(*nargv);

	_DIAGASSERT(ctx != NULL);
	_DIAGASSERT(nargv != NULL);
	_DIAGASSERT(ostr != NULL);

	if (ctx->optreset || !*ctx->place)
	{
		/* update scanning pointer */
		ctx->optreset = 0;

		if (ctx->optind >= nargc || *(ctx->place = nargv[ctx->optind]) != '-')
		{
			ctx->place = EMSG;

			return -1;
		}

		if (ctx->place[1] && *++ctx->place == '-'  /* found "--" */
			&& ctx->place[1] == '\0')
		{
			++ctx->optind;
			ctx->place = EMSG;

			return -1;
		}
	}
	
	/* option letter okay? */
	if ((ctx->optopt = (int)*ctx->place++) == (int)':' ||
		!(oli = strchr(ostr, ctx->optopt)))
	{
		/*
		 * if the user didn't specify '-' as an option,
		 * assume it means -1.
		 */
		if (ctx->optopt == (int)'-')
			return (-1);

		if (!*ctx->place)
			++ctx->optind;

		if (ctx->opterr && *ostr != ':')
		{
			(void)fprintf(stderr, "%s: illegal option -- %c\n", ctx->__progname, ctx->optopt);
		}

		return (BADCH);
	}

	if (*++oli != ':') /* don't need argument */ 
	{		
		ctx->optarg = NULL;

		if (!*ctx->place)
			++ctx->optind;
	}
	else 
	{
		/* need an argument */
		if (*ctx->place) /* no white space */
		{
			ctx->optarg = ctx->place;
		}
		else if (nargc <= ++ctx->optind) /* no arg */
		{	
			ctx->place = EMSG;

			if (*ostr == ':')
				return (BADARG);

			if (ctx->opterr)
			{
				(void)fprintf(stderr, "%s: option requires an argument -- %c\n",
					ctx->__progname, ctx->optopt);
			}

			return (BADCH);
		}
		else
		{
			/* white space */
			ctx->optarg = nargv[ctx->optind];
		}

		ctx->place = EMSG;
		++ctx->optind;
	}

	return ctx->optopt;  /* dump back option letter */
}


//
// cjgetopt_long
//

/*
 * getopt --
 *	Parse argc/argv argument vector.
 */

static int cjgetopt_internal(struct cjgetopt* ctx, int nargc, char* const* nargv, const char* ostr)
{
	//static char* place = EMSG;		/* option letter processing */
	char* oli;						/* option letter list index */

	_DIAGASSERT(ctx != NULL);
	_DIAGASSERT(nargv != NULL);
	_DIAGASSERT(ostr != NULL);

	if (ctx->optreset || !*ctx->place)
	{
		/* update scanning pointer */
		ctx->optreset = 0;

		if (ctx->optind >= nargc || *(ctx->place = nargv[ctx->optind]) != '-')
		{
			ctx->place = EMSG;
			return (-1);
		}

		if (ctx->place[1] && *++ctx->place == '-')
		{
			/* found "--" */
			/* ++optind; */
			ctx->place = EMSG;
			return (-2);
		}
	}
	
	/* option letter okay? */
	if ((ctx->optopt = (int)*ctx->place++) == (int)':' || !(oli = strchr(ostr, ctx->optopt)))
	{
		/* if the user didn't specify '-' as an option, assume it means -1 */
		if (ctx->optopt == (int)'-')
			return (-1);

		if (!*ctx->place)
			++ctx->optind;

		if (ctx->opterr && *ostr != ':')
		{
			(void)fprintf(stderr, "%s: illegal option -- %c\n", cjgetopt_progname(nargv[0]), ctx->optopt);
		}

		return (BADCH);
	}

	if (*++oli != ':')
	{	
		/* don't need argument */
		ctx->optarg = NULL;

		if (!*ctx->place)
			++ctx->optind;
	}
	else 
	{
		/* need an argument */
		if (*ctx->place) /* no white space */
		{
			ctx->optarg = ctx->place;
		}
		else if (nargc <= ++ctx->optind)
		{
			/* no arg */
			ctx->place = EMSG;

			if ((ctx->opterr) && (*ostr != ':'))
			{
				(void)fprintf(stderr, "%s: option requires an argument -- %c\n", 
					cjgetopt_progname(nargv[0]), ctx->optopt);
			}

			return (BADARG);
		}
		else
		{
			/* white space */
			ctx->optarg = nargv[ctx->optind];
		}

		ctx->place = EMSG;
		++ctx->optind;
	}

	return (ctx->optopt);			/* dump back option letter */
}


#if 0
/*
 * getopt --
 *	Parse argc/argv argument vector.
 */
int
getopt2(nargc, nargv, ostr)
int nargc;
char* const* nargv;
const char* ostr;
{
	int retval;

	if ((retval = getopt_internal(nargc, nargv, ostr)) == -2) {
		retval = -1;
		++optind;
	}
	return(retval);
}
#endif


//
// cjgetopt_long
//
/*
 * getopt_long --
 *	Parse argc/argv argument vector.
 */

int cjgetopt_getoptlong(struct cjgetopt* ctx, int nargc, char** nargv, char* options,
	struct cjgetopt_option* long_options, int* index)
{
	int retval;

	_DIAGASSERT(ctx != NULL);
	_DIAGASSERT(nargv != NULL);
	_DIAGASSERT(options != NULL);
	_DIAGASSERT(long_options != NULL);

	/* index may be NULL */
	if ((retval = cjgetopt_internal(ctx, nargc, nargv, options)) == -2) 
	{
		char* current_argv = nargv[ctx->optind++] + 2, * has_equal;
		int i, current_argv_len, match = -1;

		if (*current_argv == '\0')
			return(-1);

		if ((has_equal = strchr(current_argv, '=')) != NULL)
		{
			current_argv_len = lws_ptr_diff(has_equal, current_argv);
			has_equal++;
		}
		else
		{
			current_argv_len = (int)strlen(current_argv);
		}

		for (i = 0; long_options[i].name; i++)
		{
			if (strncmp(current_argv, long_options[i].name, current_argv_len))
				continue;

			if (strlen(long_options[i].name) == (unsigned)current_argv_len)
			{
				match = i;
				break;
			}

			if (match == -1)
				match = i;
		}

		if (match != -1) 
		{
			if (long_options[match].has_arg == cjgetopt_required_argument ||
				long_options[match].has_arg == cjgetopt_optional_argument)
			{
				if (has_equal)
					ctx->optarg = has_equal;
				else
					ctx->optarg = nargv[ctx->optind++];
			}

			if ((long_options[match].has_arg == cjgetopt_required_argument) && (ctx->optarg == NULL))
			{
				/* Missing argument, leading : indicates no error should be generated */
				if ((ctx->opterr) && (*options != ':'))
				{
					(void)fprintf(stderr, "%s: option requires an argument -- %s\n",
						cjgetopt_progname(nargv[0]), current_argv);
				}

				return (BADARG);
			}
		}
		else 
		{
			/* No matching argument */
			if ((ctx->opterr) && (*options != ':'))
			{
				(void)fprintf(stderr, "%s: illegal option -- %s\n", 
					cjgetopt_progname(nargv[0]), current_argv);
			}

			return (BADCH);
		}

		if (long_options[match].flag)
		{
			*long_options[match].flag = long_options[match].val;
			retval = 0;
		}
		else
		{
			retval = long_options[match].val;
		}

		if (index)
			*index = match;
	}

	return(retval);
}
