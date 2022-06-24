
#ifndef _CJ_YMD_H_
#define _CJ_YMD_H_



///////////////////////////////////////////////////////////////////////////////
// pragma pack 1 byte alignment - start
///////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

typedef struct cj_wdays_t {

	union {
		struct {
			cjc			last;  // v[0], last wday
			cjc         sun;   // v[1], sunday
			cjc         mon;   // v[2], monday
			cjc         tue;   // tuesday
			cjc         wed;   // wednesday
			cjc         thu;   // thursday
			cjc         fri;   // friday
			cjc         sat;   // saturday
		} chk;

		cjuc			v[8];
	};

} cj_wdays;


typedef struct cj_months_t {

	union {
		struct {
			cjc          m0;    // v[0], last month=0
			cjc          m1;    // v[1], jan = 1
			cjc          m2;	// v[2], feb = 2
			cjc          m3;	// march = 3
			cjc          m4;	// april
			cjc          m5;	// may
			cjc          m6;    // june
			cjc          m7;    // july

			cjc		     m8;    // aug
			cjc		     m9 ;   // sep
			cjc		     m10;   // oct
			cjc		     m11;   // nov
			cjc		     m12;   // dec
			cjc		     m_res1;
			cjc		     m_res2;
			cjc		     m_res3;

		} chk;

		cjc				v[16];

	} dat;

} cj_months;


typedef struct cj_days_t {

	union {
		struct {
			cjc         d_last;  // v[0], last day
			cjc         d1;		 // v[1], day 1
			cjc         d2;		 // v[2], day 2
			cjc         d3;
			cjc         d4;
			cjc         d5;
			cjc         d6;
			cjc         d7;

			cjc		    d8;
			cjc         d9;
			cjc         d10;
			cjc         d11;
			cjc         d12;
			cjc         d13;
			cjc         d14;
			cjc         d15;

			cjc		    d16;
			cjc         d17;
			cjc         d18;
			cjc         d19;
			cjc         d20;
			cjc         d21;
			cjc         d22;
			cjc         d23;

			cjc		    d24;
			cjc         d25;
			cjc         d26;
			cjc         d27;
			cjc         d28;
			cjc         d29;
			cjc         d30;
			cjc         d31;

		} chk;

		cjc				v[32];
	} dat;

} cj_days;


typedef struct cj_weeks_t {

	union {
		struct {
			cjc			w_last;		// v[0]
			cjc			w1;			// v[1], 1st week
			cjc			w2;			// v[2], 2nd week
			cjc			w3;			// 3rd week
			cjc			w4;			// 4th week
			cjc         w_res1;
			cjc         w_res2;
			cjc         w_res3;
		} chk;

		cjc				v[8];
	} dat;

} cj_weeks;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////////
// pragma pack 1 byte alignment - end
///////////////////////////////////////////////////////////////////////////////



typedef struct cj_per_every_day_t {
	cji					per_day;	// repeat every n day

} cj_per_every_day;


typedef struct cj_per_every_week_t {

	cji					per_week;	// repeat every n week
	cj_wdays			wdays;		// do checked day

} cj_per_every_week;


typedef struct cj_per_every_month_t {

	cj_months					month;

	// 0: every day , 1: every_week
	cji							per_type;

	union {
		// per_type=0, day
		cj_days					days;	// repeat checked day

		// per_type=1, week
		struct {
			cj_weeks			weeks;
			cj_wdays			wdays;
		} per_week;
	};

} cj_per_every_month;


typedef enum cj_period_type_t {
	CJ_PERIOD_NONE,
	CJ_PERIOD_ONCE,
	CJ_PERIOD_EVERY_DAY,
	CJ_PERIOD_EVERY_WEEK,
	CJ_PERIOD_EVERY_MONTH,
} cj_period_type;


#endif
