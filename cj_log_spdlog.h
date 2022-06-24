
#ifndef _CJ_LOG_SPDLOG_H_
#define _CJ_LOG_SPDLOG_H_


#ifdef __cplusplus
extern "C" {
#endif

	struct cj_log_spdlog_prm
	{
		// "basic_logger"
		char log_grp_name[CJ_MAX_SHORT_NAME];

		// "log/log.log"
		char log_file_path[CJ_MAX_PATH];

		/* 
		    CJL_ERR | CJL_WARN | CJL_INFO | \
            CJL_DBG | CJL_DBG1 | CJL_DBG2 | CJL_DBG3 | CJL_DBG4 | \
            CJL_DBG5 | CJL_DBG6 | CJL_DBG7 | CJL_DBG8 | \
            CJL_NOTICE
		*/
		cjui log_filter;

		void* prm;
	};

	cjbool cj_log_spdlog_init(const char* log_path, cjui log_filter, void* prm);
	void cj_log_spdlog_finalize();


#ifdef __cplusplus
}
#endif


#endif
