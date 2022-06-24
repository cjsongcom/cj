
#include "cj.h"
#include "cj_log.h"
#include "cj_log_spdlog.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


struct cj_log_spdlog_prm gs_log_spdlog_prm;


//#include "spdlog/sinks/rotating_file_sink.h"
//void rotating_example()
//{
//    // Create a file rotating logger with 5mb size max and 3 rotated files
//    auto max_size = 1048576 * 5;
//    auto max_files = 3;
//    auto logger = spdlog::rotating_logger_mt("some_logger_name", "logs/rotating.txt", max_size, max_files);
//}

//#include "spdlog/sinks/daily_file_sink.h"
//void daily_example()
//{
//    // Create a daily logger - a new file is created every day on 2:30am
//    auto logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
//}


//Backtrace support
//// Loggers can store in a ring buffer all messages (including debug/trace) and display later on demand.
//// When needed, call dump_backtrace() to see them
//
//spdlog::enable_backtrace(32); // Store the latest 32 messages in a buffer. Older messages will be dropped.
//// or my_logger->enable_backtrace(32)..
//for (int i = 0; i < 100; i++)
//{
//    spdlog::debug("Backtrace message {}", i); // not logged yet..
//}
//// e.g. if some error happened:
//spdlog::dump_backtrace(); // log them now! show the last 32 messages
//
//// or my_logger->dump_backtrace(32)..


// periodically flush all *registered* loggers every 3 seconds:
// warning: only use if all your loggers are thread safe ("_mt" loggers)
//spdlog::flush_every(std::chrono::seconds(3));


//#include "spdlog/sinks/syslog_sink.h"
//void syslog_example()
//{
//    std::string ident = "spdlog-example";
//    auto syslog_logger = spdlog::syslog_logger_mt("syslog", ident, LOG_PID);
//    syslog_logger->warn("This is warning that will end up in syslog.");
//}


//#include "spdlog/spdlog.h"
//#include "spdlog/sinks/stdout_color_sinks.h"
//void stdout_example()
//{
//    // create color multi threaded logger
//    auto console = spdlog::stdout_color_mt("console");
//    auto err_logger = spdlog::stderr_color_mt("stderr");
//    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");
//}

static std::shared_ptr<spdlog::logger> g_sp_spdlogger;


void cj_logger_spdlog(cjui level, const char* line)
{
    if (!g_sp_spdlogger)
        return;

    if (level == CJL_INFO)
        spdlog::info(line);
    else if (level == CJL_ERR)
        spdlog::error(line);
    else if (level == CJL_WARN)
        spdlog::warn(line);
    else
        spdlog::debug(line);
}

static bool init_spdlog(cj_log_spdlog_prm* prm)
{
 //   auto console = spdlog::stdout_color_mt("console");
//    auto err_logger = spdlog::stderr_color_mt("stderr");

    //auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/stock_agent.log");
    auto file_logger = spdlog::basic_logger_mt(
        prm->log_grp_name, prm->log_file_path);


    g_sp_spdlogger = file_logger;

    spdlog::set_default_logger(file_logger);

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    spdlog::get(prm->log_grp_name)->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");

    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");
    spdlog::debug("This message should be displayed..");

    spdlog::flush_on(spdlog::level::debug);

    // change log pattern

    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");

    // Set the default logger to file logger
//    auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
//    spdlog::set_default_logger(file_logger);

    return true;
}

void finalize_spdlog()
{
    g_sp_spdlogger = NULL;
}


//
// entry
//

#ifdef __cplusplus
extern "C" {
#endif

cjbool cj_log_spdlog_init(const char* log_path, cjui log_filter, void* prm)
{
    cjassert(log_path);

    if (!log_path)
        return cjfalse;

    cjmemzero(&gs_log_spdlog_prm, sizeof(struct cj_log_spdlog_prm));

    cjstrcpy_s(gs_log_spdlog_prm.log_file_path,
        sizeof(gs_log_spdlog_prm.log_file_path),
        log_path);

    if (!log_filter)
    {
        //log_filter = CJL_ERR | CJL_WARN | CJL_INFO | \
        //    CJL_DBG | CJL_DBG1 | CJL_DBG2 | CJL_DBG3 | CJL_DBG4 | \
        //    CJL_DBG5 | CJL_DBG6 | CJL_DBG7 | CJL_DBG8 | \
        //    CJL_NOTICE;

        log_filter = _CJL_ENABLED_LOGS;
        //| _CJL_ENABLED_LOGS_LWS;
    }

    cjstrcpy_s(gs_log_spdlog_prm.log_grp_name,
        sizeof(gs_log_spdlog_prm.log_grp_name),
        "[default]");;

    gs_log_spdlog_prm.log_filter = log_filter;
    gs_log_spdlog_prm.prm = prm;

    init_spdlog(&gs_log_spdlog_prm);

    cjl_set_log_level(gs_log_spdlog_prm.log_filter);

    cjl_set_logger(cj_logger_spdlog);

	return cjtrue;
}

void cj_log_spdlog_finalize()
{
    finalize_spdlog();
}

#ifdef __cplusplus
}
#endif
