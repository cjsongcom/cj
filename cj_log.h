

#ifndef _CJ_LOG_H_
#define _CJ_LOG_H_

// max log buf siz=1024-x 
#define CJL_BUF_SIZ  1024
// functionname,line
#define CJL_BUF_PREFIX_SIZ  128

#define CJL_FATAL	(1 << 0)
#define CJL_ERR		(1 << 1)
#define	CJL_WARN	(1 << 2)
#define	CJL_NOTICE	(1 << 3)
#define	CJL_INFO	(1 << 4)
#define	CJL_DBG		(1 << 5)

// frequency low
#define	CJL_DBG1	(1 << 6)
#define	CJL_DBG2	(1 << 7)
#define	CJL_DBG3	(1 << 8)
#define	CJL_DBG4	(1 << 9)
// realtime debug log
#define	CJL_DBG5	(1 << 10)
#define	CJL_DBG6	(1 << 11)


//
// libwebsocket
//

#define CJL_FATAL_LWS     (1 << 15)
#define CJL_ERR_LWS       (1 << 16)
#define CJL_WARN_LWS	  (1 << 17)
#define CJL_INFO_LWS      (1 << 18)
#define CJL_DBG_LWS       (1 << 19)
#define CJL_DBG1_LWS      (1 << 20)

#define CJL_FATAL_LWSCB   (1 << 21)
#define CJL_ERR_LWSCB     (1 << 22)
#define CJL_WARN_LWSCB	  (1 << 23)
#define CJL_INFO_LWSCB	  (1 << 24)
#define CJL_DBG_LWSCB     (1 << 25)
#define CJL_DBG1_LWSCB    (1 << 26)


//#define	CJL_PARSER	(1 << 5)
//#define	CJL_HEADER	(1 << 6)
//#define	CJL_EXT		(1 << 7)
//#define	CJL_CLIENT	(1 << 8)
//#define	CJL_LATENCY	(1 << 9)
//#define	CJL_USER	(1 << 10)
//#define	CJL_THREAD	(1 << 11)


// do not use 31,32, prevent overflow 
#define	CJL_COUNT	(30)   


// add timestamp prefix at log
#define CJL_OPT_TIMESTAMP_PREFIX
//#define CJL_OPT_SIMPLE_LOG

//#define CJL_OPT_SINGLE_THREAD
// static char buf[256] or  char buf[1024];


#ifdef __cplusplus
extern "C" {
#endif


cjbool cjl_init(const char* log_path, cjui log_filter, void* prm);
void cjl_finalize();


// default=CJL_ERR | CJL_WARN | CJL_NOTICE
void cjl_set_log_level(cjui level);

//static void loggger(int level, const char* line)
//{
//	char buf_prefix[50];
//	cjl_add_timestamp(level, buf_prefix, sizeof(buf_prefix));
//	fprintf(stderr, "%s%s", buf_prefix, line);
//}

void cjl_set_logger(void (*func)(cjui level, const char* line));

int cjl_add_timestamp(cjui level, char* p, int len);

int cjl_visible(cjui level);

void _cjl_log(cjui filter, const char* logtype, const char* filename, unsigned int fileline,
	const char* funcname, const char* format, ...);


typedef const char* (*pfn_cjws_lws_get_cb_reason_str)(unsigned int);

void cjl_log_register_lws_cb_reason_str(pfn_cjws_lws_get_cb_reason_str pfn);


void _cjl_log_lwscb(cjui filter, const char* logtype, const char* filename, unsigned int fileline, 
	const char* funcname, unsigned int cb_reason, const char* format, ...);

void _cjl_logv(cjui filter, const char* logtype, const char* filename, unsigned int fileline,
	const char* funcname, const char* format, const char* ext1, va_list vl);


#ifdef __cplusplus
}
#endif


#if defined(_DEBUG)

#if defined(CJL_WITH_NO_LOGS)
#define _CJL_INIT (CJL_ERR | CJL_USER)
#else
#define _CJL_INIT ((1 << CJL_COUNT) - 1)
#endif
#else
/* not _DEBUG */
#define _CJL_INIT (CJL_ERR | CJL_USER | CJL_WARN | CJL_NOTICE)
#endif

#if defined(CJL_LOGGING_BITFIELD_SET)
#define _CJL_LBS (CJL_LOGGING_BITFIELD_SET)
#else
#define _CJL_LBS 0
#endif

#if defined(CJL_LOGGING_BITFIELD_CLEAR)
#define _CJL_LBC (CJL_LOGGING_BITFIELD_CLEAR)
#else
#define _CJL_LBC 0
#endif

#define _CJL_ENABLED_LOGS		(((_CJL_INIT) | (_CJL_LBS)) & ~(_CJL_LBC))
#define _CJL_ENABLED_LOGS_LWS   (CJL_FATAL_LWS | CJL_ERR_LWS | CJL_WARN_LWS | CJL_INFO_LWS | CJL_DBG_LWS | CJL_DBG1_LWS | CJL_ERR_LWSCB | CJL_WARN_LWSCB | CJL_INFO_LWSCB | CJL_DBG_LWSCB | CJL_DBG1_LWSCB)


#if (_CJL_ENABLED_LOGS & CJL_FATAL)
#define cjl_fatal(...) _cjl_log(CJL_FATAL, "FATAL", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_fatal(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_ERR)
#define cjl_err(...) _cjl_log(CJL_ERR, "ERR", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_err(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_WARN)
#define cjl_warn(...) _cjl_log(CJL_WARN, "WARN", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_warn(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_NOTICE)
#define cjl_notice(...) _cjl_log(CJL_NOTICE, "NOTICE", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_notice(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_INFO)
#define cjl_info(...) _cjl_log(CJL_INFO, "INFO", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_info(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG)
#define cjl_dbg(...) _cjl_log(CJL_DBG, "DBG", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG1)
#define cjl_dbg1(...) _cjl_log(CJL_DBG1, "DBG1", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg1(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG2)
#define cjl_dbg2(...) _cjl_log(CJL_DBG2, "DBG2", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg2(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG3)
#define cjl_dbg3(...) _cjl_log(CJL_DBG3, "DBG3", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg3(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG4)
#define cjl_dbg4(...) _cjl_log(CJL_DBG4, "DBG4", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg4(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG5)
#define cjl_dbg5(...) _cjl_log(CJL_DBG5, "DBG5", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg5(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG6)
#define cjl_dbg6(...) _cjl_log(CJL_DBG6, "DBG6", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg6(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG7)
#define cjl_dbg7(...) _cjl_log(CJL_DBG7, "DBG7", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg7(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS & CJL_DBG8)
#define cjl_dbg8(...) _cjl_log(CJL_DBG8, "DBG8", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg8(...) do {} while(0)
#endif


//
// libwebsocket support
//

#if (_CJL_ENABLED_LOGS_LWS & CJL_FATAL_LWS)
#define cjl_fatal_lws(...) _cjl_log(CJL_FATAL_LWS, "FATAL_LWS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_fatal_lws(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_ERR_LWS)
#define cjl_err_lws(...) _cjl_log(CJL_ERR_LWS, "ERR_LWS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_err_lws(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_WARN_LWS)
#define cjl_warn_lws(...) _cjl_log(CJL_WARN_LWS, "WARN_LWS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_warn_lws(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_INFO_LWS)
#define cjl_info_lws(...) _cjl_log(CJL_INFO_LWS, "INFO_LWS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_info_lws(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_DBG_LWS)
#define cjl_dbg_lws(...) _cjl_log(CJL_DBG_LWS, "DBG_LWS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg_lws(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_DBG1_LWS)
#define cjl_dbg1_lws(...) _cjl_log(CJL_DBG1_LWS, "DBG1_LWS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_dbg1_lws(...) do {} while(0)
#endif

//
// lwscb
//

#if (_CJL_ENABLED_LOGS_LWS & CJL_FATAL_LWSCB)
#define cjl_fatal_lwscb(...) _cjl_log(CJL_FATAL_LWSCB, "FATAL_LWSCB", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_fatal_lwscb(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_ERR_LWSCB)
#define cjl_err_lwscb(...) _cjl_log(CJL_ERR_LWSCB, "ERR_LWSCB", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define cjl_err_lwscb(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_WARN_LWSCB)
#define cjl_warn_lwscb(...) _cjl_log_lwscb(CJL_WARN_LWSCB, "WARN_LWSCB", __FILE__, __LINE__, __FUNCTION__, reason, __VA_ARGS__)
#else
#define cjl_warn_lwscb(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_INFO_LWSCB)
#define cjl_info_lwscb(...) _cjl_log_lwscb(CJL_INFO_LWSCB, "INFO_LWSCB", __FILE__, __LINE__, __FUNCTION__, reason, __VA_ARGS__)
#else
#define cjl_info_lwscb(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_DBG_LWSCB)
#define cjl_dbg_lwscb(...) _cjl_log_lwscb(CJL_DBG_LWSCB, "DBG_LWSCB", __FILE__, __LINE__, __FUNCTION__, reason, __VA_ARGS__)
#else
#define cjl_dbg_lwscb(...) do {} while(0)
#endif

#if (_CJL_ENABLED_LOGS_LWS & CJL_DBG1_LWSCB)
#define cjl_dbg1_lwscb(...) _cjl_log_lwscb(CJL_DBG1_LWSCB, "DBG_LWSCB", __FILE__, __LINE__, __FUNCTION__, reason, __VA_ARGS__)
#else
#define cjl_dbg1_lwscb(...) do {} while(0)
#endif


//
//#if (_CJL_ENABLED_LOGS & CJL_PARSER)
//#define cjl_parser(...) _cjl_log(CJL_PARSER,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#else
//#define cjl_parser(...) do {} while(0)
//#endif
//
//#if (_CJL_ENABLED_LOGS & CJL_HEADER)
//#define cjl_header(...) _cjl_log(CJL_HEADER,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#else
//#define cjl_header(...) do {} while(0)
//#endif
//
//#if (_CJL_ENABLED_LOGS & CJL_EXT)
//#define cjl_ext(...) _cjl_log(CJL_EXT,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#else
//#define cjl_ext(...) do {} while(0)
//#endif
//
//#if (_CJL_ENABLED_LOGS & CJL_CLIENT)
//#define cjl_client(...) _cjl_log(CJL_CLIENT,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#else
//#define cjl_client(...) do {} while(0)
//#endif
//
//#if (_CJL_ENABLED_LOGS & CJL_LATENCY)
//#define cjl_latency(...) _cjl_log(CJL_LATENCY,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#else
//#define cjl_latency(...) do {} while(0)
//#endif
//
//#if (_CJL_ENABLED_LOGS & CJL_THREAD)
//#define cjl_thread(...) _cjl_log(CJL_THREAD,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#else
//#define cjl_thread(...) do {} while(0)
//#endif
//
//#if (_CJL_ENABLED_LOGS & CJL_USER)
//#define cjl_user(...) _cjl_log(CJL_USER,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#else
//#define cjl_user(...) do {} while(0)
//#endif



#endif  // end of #ifdef _CJ_LOG_H_
