
#include "cj.h"
#include "cj_log.h"


#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifdef WIN32
// for struct timeval
#include <winsock.h>
#endif


//#ifdef WIN32
//#include "cj_unix_w32_lib.h"
//#endif

#include "cj_log_spdlog.h"


//
// cjl_log
//

static cjui cjl_log_level = CJL_ERR | CJL_WARN | CJL_NOTICE;

#ifndef CJL_OPT_SIMPLE_LOG
// [2020/07/27 00:52:47:2540] I: test
// [2020/07/27 00:52:48:5270] D: debug msg
// E =error log,   I=Info log,  D=debug log
static const cjc* cjl_log_level_names = "EWNIDPHXCLUT??";
#endif

void cjl_emit_stderr(cjui level, const cjc* line);

static void (*cjl_emit)(cjui level, const cjc* line)
#ifndef CJ_OPT_SIMPLE_LOG
= cjl_emit_stderr
#else
= cjl_emit_optee;
#endif
;


//
// cjl_timestamp
//

#ifdef CJL_OPT_TIMESTAMP_PREFIX

cji cjl_add_timestamp(cjui level, cjc* p, cji len)
{

#ifndef CJL_OPT_SIMPLE_LOG
	time_t o_now;
	cjulonglong now;
//	unsigned long long now;
	struct cjtimeval tv;
	struct tm* ptm = NULL;

#ifndef WIN32
	struct tm tm;
#endif

	cji n;

	cjgettimeofday(&tv, NULL);

	o_now = tv.tv_sec;
	now = ((cjulonglong)tv.tv_sec * 10000) + (tv.tv_usec / 100);

#ifdef WIN32
	ptm = localtime(&o_now);
#else
	if (localtime_r(&o_now, &tm))
		ptm = &tm;
#endif

	p[0] = '\0';

	for (n = 0; n < CJL_COUNT; n++) 
	{
		if (level != (1 << n))
			continue;

		if (ptm)
		{
			n = cjsnprintf(p, len,
				"[%04d/%02d/%02d %02d:%02d:%02d:%04d] %c: ",
				ptm->tm_year + 1900,
				ptm->tm_mon + 1,
				ptm->tm_mday,
				ptm->tm_hour,
				ptm->tm_min,
				ptm->tm_sec,
				(cji)(now % 10000), cjl_log_level_names[n]);
		}
		else
		{
			n = cjsnprintf(p, len, "[%llu:%04d] %c: ",
				(cjulonglong) now / 10000,
				(cji)(now % 10000), cjl_log_level_names[n]);
		}

		return n;
	}

#else
	p[0] = '\0';
#endif

	return 0;
}

#endif // end of ifdef CJL_OPT_TIMESTAMP_PREFIX


//
// _cjl_logv
//

//#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


void _cjl_logv(cjui filter, const char* logtype, const char* filename,
	unsigned int fileline, const char* funcname, const char* ext1, const cjc* format, va_list vl)
{

#ifdef CJL_OPT_SINGLE_THREAD
	static cjc buf[CJL_BUF_SIZ];
#else
	cjc buf[CJL_BUF_PREFIX_SIZ + CJL_BUF_SIZ];
#endif

	cji n;
	cji np;

	if (!(cjl_log_level & filter))
		return;

	const char* nf = cjnull;

	if (0 && filename)
	{
		nf = strrchr(filename, '/');

		if (!nf)
			nf = strrchr(filename, '\\');

		if(nf)
			nf++;
		else
			nf = filename;
	}
	else
	{
		nf = "";
	}

	if(!ext1)
		ext1 = "";

	np = snprintf(buf, CJL_BUF_PREFIX_SIZ, "[%s] %s %s(0,%d):%s ", logtype, nf, funcname,fileline, ext1);

	n = vsnprintf(buf+np, sizeof(buf) - np, format, vl);
	//n = vsnprintf(buf, sizeof(buf) - 1, format, vl);
	/* vnsprintf returns what it would have written, even if truncated */

	//(void)n;
	n = n + np;
	
	if (n > (cji)sizeof(buf) - 1) {
		n = sizeof(buf) - 5;
		buf[n++] = '.';
		buf[n++] = '.';
		buf[n++] = '.';
		buf[n++] = '\n';
		buf[n] = '\0';
	}
	if (n > 0)
		buf[n] = '\0';

	cjl_emit(filter, buf);
}

void _cjl_log(cjui filter, const char* logtype, const char* filename,
	unsigned int fileline, const char* funcname, const cjc* format, ...)
{
	va_list ap;

	va_start(ap, format);
	_cjl_logv(filter, logtype, filename, fileline, funcname, cjnull, format, ap);
	va_end(ap);
}


//
// lws support
//

static pfn_cjws_lws_get_cb_reason_str gs_pfn_cjws_lws_get_cb_reason_str = cjnull;

void cjl_log_register_lws_cb_reason_str(pfn_cjws_lws_get_cb_reason_str pfn)
{
	gs_pfn_cjws_lws_get_cb_reason_str = pfn;
}

void _cjl_log_lwscb(cjui filter, const char* logtype, const char* filename, 
	unsigned int fileline, const char* funcname, unsigned int cb_reason, const char* format, ...)
{
	va_list ap;

	const char* cb_reason_str = "";

	if(gs_pfn_cjws_lws_get_cb_reason_str)
		cb_reason_str = gs_pfn_cjws_lws_get_cb_reason_str(cb_reason);

	va_start(ap, format);
	_cjl_logv(filter, logtype, filename, fileline, funcname, cb_reason_str, format, ap);
	va_end(ap);
}

void cjl_set_log_level(cjui level)
{
	cjl_log_level = level;
}

void cjl_set_logger(void (*func)(cjui level, const cjc* line))
{
	if (func)
		cjl_emit = func;
}

cji cjl_visible(cjui level)
{
	return cjl_log_level & level;
}


#ifndef CJ_LOG_PLAIN_LOGGING

static const cjc* const colours[] = {
	"[31;1m", /* LLL_ERR */
	"[36;1m", /* LLL_WARN */
	"[35;1m", /* LLL_NOTICE */
	"[32;1m", /* LLL_INFO */
	"[34;1m", /* LLL_DEBUG */
	"[33;1m", /* LLL_PARSER */
	"[33m", /* LLL_HEADER */
	"[33m", /* LLL_EXT */
	"[33m", /* LLL_CLIENT */
	"[33;1m", /* LLL_LATENCY */
		"[0;1m", /* LLL_USER */
	"[31m", /* LLL_THREAD */
};

static cjc tty;

static void _cjl_emit_stderr(cjui level, const cjc* line, cji add_timestamp)
{
	cjc buf[50];

	cji n;
	cji m = CJ_ARY_SIZ(colours) - 1;

	if (!tty)
	{
#ifdef WIN32
		tty = (cjc)_isatty(2) | (cjc)2;
#else
		tty = isatty(2) | 2;
#endif
	}

	buf[0] = '\0';

#if defined(CJL_OPT_TIMESTAMP_PREFIX)
	if (add_timestamp)
		cjl_add_timestamp(level, buf, sizeof(buf));
#endif

	if (tty == 3) 
	{
		n = 1 << (CJ_ARY_SIZ(colours) - 1);

		while (n) 
		{
			if (level & n)
				break;

			m--;
			n >>= 1;
		}

		fprintf(stderr, "%c%s%s%s%c[0m", 27, colours[m], buf, line, 27);
	}
	else
	{
		fprintf(stderr, "%s%s", buf, line);
	}

}

void cjl_emit_stderr(cjui level, const cjc* line)
{
	_cjl_emit_stderr(level, line, 1);
}

void cjl_emit_stderr_notimestamp(cjui level, const cjc* line)
{
	_cjl_emit_stderr(level, line, 0);
}


//
// cj log initalize / finalize
//

cjbool cjl_init(const char* log_path, cjui log_filter, void* prm)
{
	if (!log_path || *log_path == '\0')
		return cjfalse;

	cj_log_spdlog_init(log_path, log_filter, prm);

	return cjtrue;
}

void cjl_finalize()
{
	cj_log_spdlog_finalize(NULL);
}


#endif
