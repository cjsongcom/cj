
#define CJ_TEST_TIME

#if defined(CJ_TEST) && defined(CJ_TEST_TIME)

#include <cj/cj.h>
#include <cj/cjtime.h>


typedef struct cjtest_ymd_hms_t {
	cji year;
	cji month;
	cji day;
	cji hour;
	cji minute;
	cji second;
	cji zone_offset;

} cjtest_ymd_hms;


static cjrst cjtest_cjtime_make_epoch(cjepoch* out_locale_epoch, cjepoch* out_epoch,
	cjtest_ymd_hms* data) {

	cjtime_tm tm = { 0 };
	tm.tm_sec = data->second;		// seconds after the minute - [0, 60] including leap second
	tm.tm_min = data->minute;		// minutes after the hour - [0, 59]
	tm.tm_hour = data->hour;		// hours since midnight - [0, 23]
	tm.tm_mday = data->day;			// day of the month - [1, 31]
	tm.tm_mon = data->month - 1;	// months since January - [0, 11]
	tm.tm_year = data->year - 1900; // years since 1900
	// ignored tm.tm_wday =			// days since Sunday - [0, 6]
	// ignored tm.tm_yday =			// days since January 1 - [0, 365]
	tm.tm_isdst = 0;				// daylight savings time flag
	
	cjepoch locale_epoch
		= cj_get_locale_tm(&tm);

	cjepoch epoch
		= cj_get_tm(&tm);


#ifdef CJ_USE_TIME64

	#if defined(CJPF_WIN)
	cjstd_printf("[cjtest][cjtime][cjmktime] epoch from locale tm =" CJ_PRFMT_I64 "\n", locale_epoch);
	cjstd_printf("[cjtest][cjtime][cjmktime] epoch from tm=" CJ_PRFMT_I64  "\n", epoch);
	#else
	cjstd_printf("[cjtest][cjtime][cjmktime] epoch from locale tm =" CJ_PFMT_I64  "\n", locale_epoch);
	cjstd_printf("[cjtest][cjtime][cjmktime] epoch from tm=%Il \n", epoch);
	#endif

#else

	cjstd_printf("[cjtest][cjtime][cjmktime] epoch from locale tm =%d \n", locale_epoch);
	cjstd_printf("[cjtest][cjtime][cjmktime] epoch from tm=%d \n", epoch);

#endif

	return 0;
}


//static cjrst cjtest_cjtime_show_local_time_str() {
//
//	cjepoch epoch = 0;
//	cjepoch epoch_locale = 0;
//
//	cjepoch_get(&epoch);
//
//	cjtime_tm tm;
//	epoch_locale = epoch + cjdatetime_get_timezone_sec();
//
//	cjmc buf_local_datetime_str_locale[64] = { 0 };
//	cjmc buf_local_datetime_str_utc[64] = { 0 };
//
//	cjmc buf_iso8601_datetime_str_locale[64] = { 0 };
//	cjmc buf_iso8601_datetime_str_utc[64] = { 0 };
//
//	cjepoch_get_local_datetime_str(buf_local_datetime_str_utc,
//		sizeof(buf_local_datetime_str_utc), epoch);
//
//	cjepoch_get_local_datetime_str(buf_local_datetime_str_locale,
//		sizeof(buf_local_datetime_str_locale), epoch_locale);
//
//	cjepoch_get_i8601_datetime_str(buf_iso8601_datetime_str_utc,
//		sizeof(buf_iso8601_datetime_str_utc), epoch, cjtrue);
//
//	cjepoch_get_i8601_datetime_str(buf_iso8601_datetime_str_locale,
//		sizeof(buf_iso8601_datetime_str_locale), epoch_locale, cjfalse);
//
//	cjstd_printf("\n\n");
//	cjstd_printf("=== locale timezone second =%d \n", cjdatetime_get_timezone_sec());
//	cjstd_printf("\n\n");
//	cjstd_printf("=== local datetime fmt ===\n");
//	cjstd_printf("[cjtest][cjtime][local datetime fmt] epoch=" CJ_PRFMT_I64 ", epoch_locale=" CJ_PRFMT_I64  "\n", epoch, epoch_locale);
//	cjstd_printf("[cjtest][cjtime][local datetime fmt] utc=    %s \n", buf_local_datetime_str_utc);
//	cjstd_printf("[cjtest][cjtime][local datetime fmt] locale= %s \n", buf_local_datetime_str_locale);
//	cjstd_printf("\n");
//	cjstd_printf("=== iso8601 datetime fmt ===\n");
//	cjstd_printf("[cjtest][cjtime][iso8601 datetime fmt] epoch=" CJ_PRFMT_I64  ", epoch_locale=" CJ_PRFMT_I64  "\n", epoch, epoch_locale);
//	cjstd_printf("[cjtest][cjtime][iso8601 datetime fmt] utc=    %s \n", buf_iso8601_datetime_str_utc);
//	cjstd_printf("[cjtest][cjtime][iso8601 datetime fmt] locale= %s \n", buf_iso8601_datetime_str_locale);
//	cjstd_printf("\n\n");
//
//	return 0;
//}


//
//#include <stdio.h>
//#include <time.h>
//
//char* wday[] = { "Sunday", "Monday", "Tuesday", "Wednesday",
//				 "Thursday", "Friday", "Saturday" };
//
//int main(void) {
//	time_t t1, t3;
//	struct tm* t2;
//
//	t1 = time(NULL);
//	t2 = localtime(&t1);
//	t2->tm_mday += 40;
//	t2->tm_hour += 16;
//	t3 = mktime(t2);
//
//	printf("40 days and 16 hours from now, it will be a %s \n",
//		wday[t2->tm_wday]);
//}
//
///*******************  Output should be similar to:  ***************
//
//40 days and 16 hours from now, it will be a Sunday
//*/


//
//static cjrst cjtest_cjtime_get_utctime() {
//
//	cjtm tm;
//	cjtime_t v = cjmktime(&tm);
//
//#if defined(CJPF_WIN)
//
//#ifdef CJ_USE_TIME64
//	cjstd_printf("[cjtest][cjtime][cjmktime] time_t=%I64d", v);
//#else
//	cjstd_printf("[cjtest][cjtime][cjmktime] time_t=%d", v);
//#endif
//
//#else
//	cjstd_printf("[cjtest][cjtime][cjmktime] time_t=%ll", v);
//#endif
//
//	return 0;
//}
//

//
//static cjtm* cjtest_cjtime_conv_time(cjtime_t time_v, cjbool is_local_time) {
//
//	cjtm* rst = cjNULL;
//
//	if (is_local_time) {
//		rst = cjlocaltime(&time_v);
//	} else {
//		rst = cjgmtime(&time_v);
//	}		
//}


CJEXTERNC cjrst cjtest_cjtime_main(void* prm) {

	cjtest_ymd_hms test_data = {
		.year = 2022,
		.month = 6,
		.day = 1,

		.hour = 15,
		.minute = 30,
		.second = 12,

		.zone_offset = 0
	};


	// build epoch, locale-epoch from given parameter
	cjepoch locale_epoch, epoch;

	cjtest_cjtime_make_epoch(&locale_epoch, &epoch, &test_data);

//	cjtest_cjtime_show_local_time_str();

//	cjtest_cjtime_cjdatetime_timezone_offset();


	//
	// epoch to time string
	//














//	cjtime_t cur_utctime  = cjtime_utc();
//	cjtime_t cur_localtime = cjtime_local();


	//cjtest_ymd_hms dt;
	//cjtest_cjtime_extract_datetime(&dt, &localtime, "local_time");




	/*cjdatetime dt;

	cji year = 2022;
	cji month = 6;
	cji day = 6;
	cji hour = 18;
	cji minute = 10;
	cji second = 1;

	dt = cjdatetime_get_localtime(year, month, day, hour, minute, second);*/

	
	return 0;
}


#endif