
#define CJ_DATETIME

#if defined(CJ_TEST) && defined(CJ_DATETIME)

#include <cj/cj.h>
#include <cj/cjdatetime.h>


static cjrst cjtest_cjdatetime() {

	cjlocaldatetime local_dt;
	cjmc localdatetime_str[CJ_MAX_SIZ_DATETIME_STR] = { 0 };

	cj_get_localdatetime(&local_dt);
	cj_get_local_datetime_str(localdatetime_str,
		sizeof(localdatetime_str) / sizeof(localdatetime_str[0]),
		&local_dt);

	cjstd_printf("\n\n");
	cjstd_printf("=== cjlocaldatetime ===\n");
	cjstd_printf("[cjtest][cjdatetime][cjlocaldatetime] %s \n",
		localdatetime_str);


	cjdatetime dt;
	cjmc iso8601_locale_str[CJ_MAX_SIZ_DATETIME_STR] = { 0 };
	cjmc iso8601_utc_str[CJ_MAX_SIZ_DATETIME_STR] = { 0 };

	cj_get_datetime(&dt);
	cj_get_datetime_str(iso8601_locale_str, sizeof(iso8601_locale_str) / sizeof(iso8601_locale_str[0]), &dt);
	cj_get_datetime_utc_str(iso8601_utc_str, sizeof(iso8601_utc_str) / sizeof(iso8601_utc_str[0]), &dt);

	cjstd_printf("\n");
	cjstd_printf("=== cjdatetime (locale) ===\n");
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] epoch(utc)=" CJEPOCH_PRFMT "\n", dt.epoch);
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] LOCALE TIMEZONE Second=%d\n", dt.timezone_sec);

	cjstd_printf("[cjtest][cjdatetime][cjdatetime] iso8601 datetime LOCALE =%s\n", iso8601_locale_str);
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] iso8601 datetime UTC    =%s\n", iso8601_utc_str);

	cjstd_printf("\n\n");


	return 0;
}


static cjrst cjtest_cjdatetime_parse(const cjmc* datetime_str) {

	cjassert(datetime_str);

	cjdatetime datetime;
	cjmc iso8601_locale_str[CJ_MAX_SIZ_DATETIME_STR] = { 0 };
	cjmc iso8601_utc_str[CJ_MAX_SIZ_DATETIME_STR] = { 0 };

	if (!cj_parse_datetime_str(&datetime, datetime_str)) {
		cjstd_printf("=== cjdatetime parse failed %s ===\n", datetime_str);
		return cjrstfail;
	}

	cj_get_datetime_str(iso8601_locale_str, sizeof(iso8601_locale_str) / sizeof(iso8601_locale_str[0]), &datetime);
	cj_get_datetime_utc_str(iso8601_utc_str, sizeof(iso8601_utc_str) / sizeof(iso8601_utc_str[0]), &datetime);


	cjstd_printf("\n");
	cjstd_printf("=== cjdatetime parse ===\n");
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] input=%s", datetime_str);
	cjstd_printf("\n");
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] epoch(utc)=" CJEPOCH_PRFMT "\n", datetime.epoch);
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] LOCALE TIMEZONE Second=%d\n", datetime.timezone_sec);
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] iso8601 datetime LOCALE =%s\n", iso8601_locale_str);
	cjstd_printf("[cjtest][cjdatetime][cjdatetime] iso8601 datetime UTC    =%s\n", iso8601_utc_str);

	cjstd_printf("\n");

	return cjrstok;
}

CJEXTERNC cjrst cjtest_cjdatetime_main(void* prm) {

//	cjtest_cjdatetime();

	cjtest_cjdatetime_parse("2019-09-07T15:51:23.000+09:00");
	cjtest_cjdatetime_parse("2019-09-07T15:51:23.000-03:00");
	cjtest_cjdatetime_parse("2019-09-07T15:51:25+09:00");
	cjtest_cjdatetime_parse("2019-09-07T15:51:25-02:00");
	cjtest_cjdatetime_parse("2019-09-07T15:52Z");
	cjtest_cjdatetime_parse("2019-09-07T15:53");


	return 0;
}


#endif

