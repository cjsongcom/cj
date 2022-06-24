
#ifndef _CJDATETIME_H_
#define _CJDATETIME_H_

#include <cj/cjtime.h>

/*
 	  |------------------------|
	  |  OS Time  (Epoch, UTC) |
	  |------------------------|
				|
			Locale Settings (/etc/localtime/ -> /usr/share/zoneinfo/Asia/Seoul)
				|
		 Locale Timezone Seoul(34000 second=9Hour)
				 |
	LocalDateTime (cjlocaldatetime) = Epoch(UTC) + Locale Timezone Second(34000)
*/

/////////////////////////////////////////////////////////////////////////
//  DateTime Format
/////////////////////////////////////////////////////////////////////////

/*
  0. System time = epoch
    > number of seconds that have elapsed since January 1, 1970

  1. cjlocaldatetime: LocalDateTime
	> https://docs.oracle.com/javase/8/docs/api/java/time/LocalDateTime.html
	> A date-time WITHOUT A TIMEZONE in the ISO-8601 calendar system
	> does not store or represent a time-zone, it is a description of the date
	> YYYY-MM-DDThh:mm:ss.ms, 2007-12-03T10:15:30

  2. cjdatetime: ISO8601 DateTime Format
	> YYYY-MM-DDThh:mm:ss[.mmm][Z|+-HH:mm][+-HHmm][+-HH]
	> Part
	  >> DATE : YYYY-MM-DD
	  >> TIME : Thh:mm:ss[.mmm]
		>>> 'T' character between date and time,
		>>> 'T' indicating the start of the time element
	  >> TIMEZONE : Z, +-HH:mm

	> TIMEZONE : [Z|+-HH:mm][+-HHmm][+-HH]
	  >> Z: end with 'Z'
		>>> zero timezone time, UTC Time
		>>> yyyy-mm-ddThh:mm:ss.msZ
		>>> 2020-04-30T00:00:00.000Z
		>>> AWS Lambda, AZure Function use
	  >> +HH:MM:
		>>> 2020-02-08T09:00:00+07:00
		>>> seven hours from UTC. In other words it¡¯s at 9 a.m. in time zone +7.
		>>> the + or - values indicate how far ahead or behind a time zone is
	   >>> from the UTC (Coordinated Universal Time) zone.

  3. Example

	3.1 LocalDateTime (cjlocaldatetime)
	  > 2007-12-03T10:15:30
	  > no timezone info

	3.2 ISO8601 DateTime (cjdatetime)
	  > Check 'T' before time string begin

	  > 2020-04-30T00:00:00.000Z           ; (UTC)
	  > 2019-09-07T15:50:00.000+09:00      ; KST(UTC+9)
	  > 2019-09-07T-15:50Z                 ; 3:30 pm on September 7, 2019 in the time zone of universal time
	  > 2019-09-07T15:50-04:00             ; 3:30 pm on September 7, 2019 in the time zone New York
												(UTC with daylight saving time)
*/

//enum {
//	CJDATETIME_LOCALDATETIME_STR_MAX_SIZ	= 64
//};


#define cjtimezone						timezone

// no timezone designator
#define CJ_FMT_LOCALDATETIME_STR			"%Y-%m-%dT%H:%M:%S.000"

#define CJ_FMT_DATETIME_STR_UTC				"%Y-%m-%dT%H:%M:%S.000Z"

#define CJ_FMT_DATETIME_STR_OS_LOCALE		"%Y-%m-%dT%H:%M:%S.000%z"


enum {
	// Local DateTime
	// 2007-12-03T10:15:30
	// ISO8601 Datetime
	// 2019-09-07T15:50:00.000+09:00      : KST South Korea
	// 2019-09-07T15:50:00.000-03:00      : ADT Canada - Newfoundland and Labrador
	// 2019-09-07T15:50:00.000Z           : UTC

	CJ_MAX_SIZ_DATETIME_STR = 64,

	// 2019-09-07
	CJ_MAX_SIZ_DATE_STR= 10,

	// 15:50:00.000
	CJ_MAX_SIZ_TIME_STR = 12,

	// +09:00 -09:00 Z
	CJ_MAX_SIZ_TIMEZONE_STR = 6,

	CJ_MAX_SIZ_TIME_TIMEZONE_STR = CJ_MAX_SIZ_TIME_STR + CJ_MAX_SIZ_TIMEZONE_STR
};


typedef struct cjdatetime_t {
	cjepoch						epoch;  // always UTC, zerotimezone

	// timezone_sec:
	//  > KST : 32400 sec
	// KST: 32400, 9hour = 32400 sec,
	// KST = UST + 32400,
	// ex: UTC=1654065012
	//     KST=1654097412=(UTC+32400)
	cji							timezone_sec;

	cjtime_tm					tm;
	
} cjdatetime;


typedef struct cjlocaldatetime_t {
	cjtime_tm					tm;
} cjlocaldatetime;



//
// localdatetime
// > locale timezone applied
//

CJEXTERNC cjlocaldatetime* cj_get_localdatetime(cjlocaldatetime* out);

// LocalDateTime (cjlocaldatetime)
// > YYYY-MM-DDThh:mm:ss.ms , 2007-12-03T10:15:30
// set datetime to null to get current datetime
CJEXTERNC cji cj_get_local_datetime_str(cjmc* buf, cjstr_siz buf_siz,
	cjlocaldatetime* datetime);


//
// datetime
// > ISO-8601 calendar system DateTime 
//

CJEXTERNC cjdatetime* cj_get_datetime(cjdatetime* out);

// get seconds West of UTC
// local datetime epoch = UTC epoch + local timezone second
CJEXTERNC cji cj_get_locale_timezone_sec();

// iso8601 datetime format
// ex: 2020-04-30T00:00:00.000Z       ; UTC:Z
//     2019-09-07T15:50:00.000+09:00  ; KST:UTC+9
// set datetime to null to get current datetime str
CJEXTERNC cji cj_get_datetime_str(cjmc* buf, cjstr_siz buf_siz,
	cjdatetime* datetime);
CJEXTERNC cji cj_get_datetime_utc_str(cjmc* buf, cjstr_siz buf_siz,
	cjdatetime* datetime);

CJEXTERNC cjbool cj_parse_datetime_str(cjdatetime* out, const cjmc* datetime_str);

CJEXTERNC cjbool cj_parse_datetime_str_ex(cjepoch* out_epoch, cjtime_tm* out_tm,
	cji* out_timezone_sec, const cjmc* datetime_str, cjbool parse_as_localtime);

CJEXTERNC cjbool cj_check_datetime_tm_value(const cjtime_tm* tm);


// sun=0, mon=1, tue=2 ..
typedef enum cj_wday_t {
	CJ_WDAY_LAST= 0,
	CJ_WDAY_SUN,
	CJ_WDAY_MON,
	CJ_WDAY_TUE,
	CJ_WDAY_WED,
	CJ_WDAY_THU,
	CJ_WDAY_FRI,
	CJ_WDAY_SAT,
	CJ_WDAY_MAX
} cj_wday;

typedef cjc			cj_wday_chk[CJ_WDAY_MAX];


// jan=0, feb=1, mar=2 ..
typedef enum cj_month_t {
	CJ_MONTH_LAST= 0,
	CJ_MONTH_JAN,
	CJ_MONTH_FEB,
	CJ_MONTH_MAR,
	CJ_MONTH_APR,
	CJ_MONTH_MAY,
	CJ_MONTH_JUN,
	CJ_MONTH_JUL,
	CJ_MONTH_AUG,
	CJ_MONTH_SEP,
	CJ_MONTH_OCT,
	CJ_MONTH_NOV,
	CJ_MONTH_DEC,
	CJ_MONTH_MAX
} cj_month;

typedef cjc			cj_month_chk[CJ_MONTH_MAX];


typedef enum cj_week_t {
	CJ_WEEK_LAST,
	CJ_WEEK_FIR,
	CJ_WEEK_SEC,
	CJ_WEEK_THIRD,
	CJ_WEEK_FORTH,	
	CJ_WEEK_MAX
} cj_week;

typedef cjc			cj_week_chk[CJ_WEEK_MAX];


typedef enum cj_day_t {
	CJ_DAY_LAST,
	CJ_DAY_1,  CJ_DAY_2, CJ_DAY_3, CJ_DAY_4, CJ_DAY_5,
	CJ_DAY_6,  CJ_DAY_7, CJ_DAY_8, CJ_DAY_9, CJ_DAY_10,
	CJ_DAY_11, CJ_DAY_12, CJ_DAY_13, CJ_DAY_14, CJ_DAY_15,
	CJ_DAY_16, CJ_DAY_17, CJ_DAY_18, CJ_DAY_19, CJ_DAY_20,
	CJ_DAY_21, CJ_DAY_22, CJ_DAY_23, CJ_DAY_24, CJ_DAY_25,
	CJ_DAY_26, CJ_DAY_27, CJ_DAY_28, CJ_DAY_29, CJ_DAY_30,
	CJ_DAY_31, 
	CJ_DAY_MAX
} cj_day;

typedef cjc			cj_day_chk[CJ_DAY_MAX];





#endif
