
#include <cj/cj.h>
#include <cj/cjtime.h>

#include <cj/cjdatetime.h>


static cji gs_timzone_second;
static char gs_timzone_name[32];

static cjbool gs_timezone_info_init;


///////////////////////////////////////////////////////////////////////////////
//  cjdatetime
///////////////////////////////////////////////////////////////////////////////

CJEXTERNC cjlocaldatetime* cj_get_localdatetime(cjlocaldatetime* out) {

	cjassert(out);
	cj_get_locale_tm(&out->tm);

	return out;
}

CJEXTERNC cji cj_get_local_datetime_str(cjmc* buf, cjstr_siz buf_siz,
	cjlocaldatetime* datetime) {

	cjtime_tm* tm;
	cjtime_tm _tm;

	if (datetime) {
		tm = &datetime->tm;
	} else {
		cj_get_locale_tm(&_tm);
		tm = &_tm;
	}	

	return cjtime_tm_printf(buf, buf_siz, CJ_FMT_LOCALDATETIME_STR, tm);
}

CJEXTERNC cjdatetime* cj_get_datetime(cjdatetime* out) {

	cjassert(out);

	out->epoch = cj_get_locale_tm(&out->tm);
	out->timezone_sec = cj_get_locale_timezone_sec();

	return out;
}

CJEXTERNC cji cj_get_locale_timezone_sec() {

#ifdef CJPF_WIN

	if (!gs_timezone_info_init) {
		_tzset();

		gs_timzone_second = _timezone;  // > KST: -32400
		gs_timzone_second *= (-1);  // KST = UST + 32400(9 hour)

		cjstrcpy_s(gs_timzone_name, 32, *_tzname);

		gs_timezone_info_init = cjtrue;
	}

	return gs_timzone_second;

#else

#error "impl, not yet"

#endif

}

CJEXTERNC cji cj_get_datetime_str(cjmc* buf, cjstr_siz buf_siz, cjdatetime* datetime) {

	cjdatetime _datetime;

	if (!datetime)
		datetime = cj_get_datetime(&_datetime);

	if(datetime->timezone_sec == 0)
		return cjtime_tm_printf(buf, buf_siz, CJ_FMT_DATETIME_STR_UTC, &datetime->tm);

	cji append_pos;

	append_pos = cjtime_tm_printf(buf, buf_siz, CJ_FMT_LOCALDATETIME_STR, &datetime->tm);

	if (append_pos == 0 || append_pos == -1)
		return append_pos;

	if (datetime->timezone_sec == 0) {
		buf[append_pos] = 'Z';
		buf[append_pos + 1] = cjNULL;

		return append_pos + 2;
	}

	cjmc tz_char = '+';
	cji tz_hh = 0;
	cji tz_mm = 0;

	if (datetime->timezone_sec < 0)
		tz_char = '-';

	cji sec = cjabs(datetime->timezone_sec);

	tz_hh = sec / 3600;
	tz_mm = (sec - tz_hh * 3600);

	if (tz_mm > 0)
		tz_mm /= 60;
		
	return append_pos + cjsnprintf(&buf[append_pos], buf_siz - append_pos, "%c%02d:%02d", tz_char, tz_hh, tz_mm);
}

CJEXTERNC cji cj_get_datetime_utc_str(cjmc* buf, cjstr_siz buf_siz, cjdatetime* datetime) {

	cjepoch epoch;

	if(datetime && datetime->timezone_sec == 0)
		return cjtime_tm_printf(buf, buf_siz, CJ_FMT_DATETIME_STR_UTC, &datetime->tm);

	if (datetime)
		epoch = datetime->epoch - datetime->timezone_sec;
	else
		cj_get_epoch(&epoch);
	 
	cjtime_tm tm;
	cj_conv_epoch_to_tm(&epoch, &tm);

	return cjtime_tm_printf(buf, buf_siz, CJ_FMT_DATETIME_STR_UTC, &tm);
}

CJEXTERNC cjrst cj_parse_datetime_str_ex(cjepoch* out_epoch, cjtime_tm* out_tm, 	
	cji* out_timezone_sec, const cjmc* datetime_str, cjbool parse_as_localtime) {

	cji timezone_sec = 0;
	const cjmc* cp;
	char c;

	if (!datetime_str)
		return -100;

	// get datepart
	const cjmc* t = cjstrchr(datetime_str, 'T');

	if (!t)
		return -110;
		
	const cjmc* datepart_st = datetime_str;
	const cjmc* datepart_en = t;

//	cjmc datepart[CJ_MAX_SIZ_DATE_STR + 1] = { 0 };

	// [0] year, [1] month [2] day
	cjmc parsed_date[3][5] = { 0 };
	cji max_parse_date_siz[3] = { 4, 2, 2 };

	cji parse_date_phase = 0;

	cji i = 0;
	cp = datepart_st;

	while (cp < datepart_en) {

		if (i > max_parse_date_siz[parse_date_phase])
			return -200;

		c = *cp;

		if (!isdigit(c)) {

			if (c == '-') {
				parse_date_phase++;

				if (parse_date_phase > 2) {
					break;

				} else {
					i = 0;
					cp++;

					continue;
				}

			} else {
				return -210;
			}
		}

		parsed_date[parse_date_phase][i] = c;

		i++;
		cp++;
	}


	//
	// timepart + timezone
	//

	cjmc timepart[CJ_MAX_SIZ_TIME_TIMEZONE_STR+1] = { 0 };

	cjstrncpy_s(timepart, CJ_MAX_SIZ_TIME_TIMEZONE_STR+1,
		t + 1, CJ_MAX_SIZ_TIME_TIMEZONE_STR);

	const cjmc* timepart_st = timepart;
	const cjmc* timepart_en = cjNULL;

	const cjmc* timezone_st = cjNULL;
//	const cjmc* timezone_en = cjNULL;

	timezone_st = cjstrchr(timepart_st, 'Z');

	if (!timezone_st)
		timezone_st = cjstrchr(timepart_st, '+');

	if (!timezone_st)
		timezone_st = cjstrchr(timepart_st, '-');

	if (timezone_st)
		timepart_en = timezone_st;

	if (!timepart_en) {

		// no timezone str is founded
		if(!parse_as_localtime)
			return -300;  

		timepart_en = cjstrfind_null_term(timepart);
	}

	// parse time part
	i = 0;
	cp = timepart_st;

	// [0] hour, [1] minute [2] sec
	cjmc parsed_time[3][3] = { 0 };
	cji parse_time_phase = 0;

	// don't parse millesecond  12:10.20.XXX	
	const cjmc* cm = timepart_st;
	while (cm <= timepart_en) {
		if (*cm == '.') {
			timepart_en = cm - 1;
			break;
		}
		cm++;
	}

	cp = timepart_st;
	while (cp < timepart_en) {

		if (i > 2)
			return -310;

		c = *cp;

		if (!isdigit(c)) {

			if (c == ':') {
				parse_time_phase++;

				if (parse_time_phase > 3) {
					break;

				} else {
					i = 0;
					cp++;

					continue;
				}

			} else {
				// invalid str
				return -320;
			}
		}

		parsed_time[parse_time_phase][i] = c;

		i++;
		cp++;
	}


	//
	// timezone
	//

	if (parse_as_localtime)
		goto skip_parse_timezone;


	i = 0;
	cp = timezone_st;

	// parse +/-/Z
	c = *cp;

	cjbool is_utc = cjfalse;
	cjbool is_locale_plus = cjfalse;

	if (c == 'Z') {
		is_utc = cjtrue;
	} else if(c == '+' ) {
		is_locale_plus = cjtrue;
	} else if (c == '-') {
		is_locale_plus = cjfalse;
	} else {
		// invalid timezone start string
		return -400;
	}

	cp++;

	// [0] +-Z, [1] timezone hour [2] timezone minute
	cjmc parsed_timezone[2][3] = { 0 };
	cji parse_timezone_phase = 0;

	while (*cp) {

		if (i > 2)
			return -410;

		c = *cp;

		if (!isdigit(c)) {

			if (c == ':') {
				parse_timezone_phase++;

				if (parse_time_phase > 2) {
					break;

				} else {
					i = 0;
					cp++;

					continue;
				}

			} else {
				// invalid str
				return -420;
			}

		}

		parsed_timezone[parse_timezone_phase][i] = c;

		i++;
		cp++;
	}

	if (is_utc) {
		timezone_sec = 0;

	} else {

		if (parse_time_phase != 2)
			return -430;  // invalid timezone string, 

		cji timezone_hour = cjatoi(parsed_timezone[0]);
		cji timezone_min  = cjatoi(parsed_timezone[1]);

		timezone_sec = timezone_hour * 3600 + timezone_min * 60;

		if (!is_locale_plus)
			timezone_sec *= -1;
	}

skip_parse_timezone:

	//
	// build tm
	//

	cjmemzero(out_tm, sizeof(cjtime_tm));

	// 24hour format  :  00:00(AM12:00) ~ 23:59 (PM11:59)
	// 124hour format :  AM 12:00 ~ PM11:59
	out_tm->tm_hour = cjatoi(parsed_time[0]);     // hour [0~23]
	out_tm->tm_min = cjatoi(parsed_time[1]);      // minute [0~59]
	out_tm->tm_sec = cjatoi(parsed_time[2]);      // second [0~60]

	out_tm->tm_year = cjatoi(parsed_date[0]) - 1900;  // year after 1900
	out_tm->tm_mon = cjatoi(parsed_date[1]) - 1;   // month [0~11]
	out_tm->tm_mday = cjatoi(parsed_date[2]);  // day [1~31]

	if (!cj_check_datetime_tm_value(out_tm))
		return -500; // value ranage in out_tm is invalid


	if (parse_as_localtime) {
		*out_timezone_sec = 0;
		*out_epoch = cj_get_epoch_from_locale_tm(out_tm);

	} else {
		*out_epoch = cj_get_epoch_from_tm(out_tm);
		*out_timezone_sec = timezone_sec;
	}

	return 0;
}

CJEXTERNC cjbool cj_parse_datetime_str(cjdatetime* out, const cjmc* datetime_str) {

	cjdatetime dt;

	if (cjrstok != cj_parse_datetime_str_ex(&dt.epoch, &dt.tm, &dt.timezone_sec, 
		datetime_str, cjfalse))
		return cjfalse;

	*out = dt;

	return cjtrue;
}

CJEXTERNC cjbool cj_check_datetime_tm_value(const cjtime_tm* tm) 	{

	cjbool rst = cjfalse;

	if (tm) {

		if (tm->tm_hour < 0 || tm->tm_hour > 23)
			return cjfalse;

		if (tm->tm_min < 0 || tm->tm_min > 59)
			return cjfalse;

		if (tm->tm_sec < 0 || tm->tm_min > 60)
			return cjfalse;

		if (tm->tm_year < 0)
			return cjfalse;

		if (tm->tm_mon < 0 || tm->tm_mon > 11)
			return cjfalse;

		if (tm->tm_mday < 1 || tm->tm_mday > 31)
			return cjfalse;

		return cjtrue;
	}

	return rst;
}



//
//void cjdatetime_get_str(cjmc* out, cjstr_siz out_siz, const cjdatetime* dt) {
//
//}
//
//cjdatetime cjdatetime_get_localtime(cji year, cji month, cji day, cji hour, 
//	cji minute, cji second)	{
//
////	cjtm tm = { 0 };
////	cjtime_t time_t;
////
////	tm.tm_sec = second;
////	tm.tm_min = minute;
////	tm.tm_hour = hour;
////	tm.tm_mday = day;
////	tm.tm_mon = month - 1;
////	tm.tm_year = year - 1900;
////
////	//tm.tm_wday = days since Sunday - [0, 6]
////	//tm.tm_yday = days since January 1 - [0, 365]
////	//tm.tm_isdst = aylight savings time flag
////
////#if defined(CJPF_WIN32)
////	time_t = mktime(&tm);
////#elif defined(CJPF_WIN64)
////	time_t = _mktime64(&st_tm);
////#else
////	time_t = mktime(&st_tm);
////#endif	
//
//}
//
//cjdatetime cjdatetime_get_utctime(cji year, cji month, cji day, cji hour, cji minute, cji second) {
//	
//	cjdatetime rst = { 0 };
//	
//	/*struct tm st_tm;
//
//	st_tm.tm_year = YYYY - 1900;
//	st_tm.tm_mon = MM - 1;
//
//
//
//	st_tm.tm_mday = DD;
//
//
//
//	st_tm.tm_hour = hh;
//
//
//
//	st_tm.tm_min = mi;
//
//
//
//	st_tm.tm_sec = ss;
//
//	return _mkgmtime(&st_tm);*/
//
//	return rst;
//}
//
