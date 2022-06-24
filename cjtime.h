
#ifndef  _CJTIME_H_
#define  _CJTIME_H_

#define CJ_USE_TIME64



// signed int :  -2,147,483,648 ~ 2,147,483,647
// expired  epoch (03:14:07 UTC on 19 January 2038)
// https://www.mongodb.com/blog/post/the-2038-problem-and-how-to-solve-it-for-mongodb
typedef cji cjtime_sec32;      
typedef cji64 cjtime_sec64;

// millsecond=2,147,483,647 => 1970 jan 25  20:31:23 => about 25.X days
typedef cji cjtime_ms32; 
typedef cji64 cjtime_ms64;


/*---------------------------------------------------------------------------*/
#if defined(CJPF_WIN)
/*---------------------------------------------------------------------------*/

#ifndef _TIMEZONE_DEFINED 
struct timezone
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};
#endif

// defined at winsock2.h
//struct timeval {
//	long    tv_sec;         /* seconds */
//	long    tv_usec;        /* and microseconds */
//};


struct timeval64 {
	cji64 tv_sec;   // seconds
	cji tv_usec;    // microseconds
};

typedef struct tm										cjtime_tm;

// rst: 0 for success or EINVAL
//  > second : tv->tv_sec
//  > microsec : tv->tv_usec
CJEXTERNC cji cjgettimeofday32(struct timeval* tv, struct timezone* tz);
CJEXTERNC cji cjgettimeofday64(struct timeval64* tv, struct timezone* tz);

CJEXTERNC cji cjgettime_sec32(void);
CJEXTERNC cji64 cjgettime_sec64(void);
CJEXTERNC cji cjgettime_ms32(void);
CJEXTERNC cji64 cjgettime_ms64(void);

CJEXTERNC cjbool cjcheck_time_interval_sec32(cjtime_sec32* last_sec,
	cjtime_sec32 limit_interval_sec);
CJEXTERNC cjbool cjcheck_time_interval_sec64(cjtime_sec64* last_sec,
	cjtime_sec64 limit_interval_sec);


/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

// posix os (unix, ios, apple)



/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////
// epch function
///////////////////////////////////////////////////////////////////////////////

/*
  epoch:
    The Unix epoch (or Unix time or POSIX time or Unix timestamp) is 
	the number of seconds that have elapsed since January 1, 1970 (midnight UTC/GMT), 
	not counting leap seconds (in ISO 8601: 1970-01-01T00:00:00Z). 
	Literally speaking the epoch is Unix time 0 (midnight 1/1/1970), 
	but 'epoch' is often used as a synonym for Unix time.
*/

// time_t, __time32_t, __time64_t, 
// epoch = UTC = number of seconds that have elapsed since January 1, 1970 
typedef long											cjepoch32;
typedef long long										cjepoch64;


// epoch=UTC
CJEXTERNC cjepoch32 cj_get_tm32(cjtime_tm* out);
CJEXTERNC cjepoch64 cj_get_tm64(cjtime_tm* out);

// locale timezone applied datetime
CJEXTERNC cjepoch32 cj_get_locale_tm32(cjtime_tm* out);
CJEXTERNC cjepoch64 cj_get_locale_tm64(cjtime_tm* out);

#define cjtime_tm_printf(buf, buf_siz, time_fmt, time_tm) \
	strftime(buf, buf_siz, time_fmt, time_tm)


#if defined(CJPF_WIN)

// get epoch second (UTC) from os
#define cj_get_epoch32										_time32
#define cj_get_epoch64										_time64

// gmtime(),   _gmtime32   : thread NOT safe
// gmtime_r(), _gmtime32_s : thread safe
// convert epoch(UTC) to time_tm (passthru)
#define cj_conv_epoch32_to_tm(epoch,out_tm)					_gmtime32_s(out_tm, epoch)
#define cj_conv_epoch64_to_tm(epoch,out_tm)					_gmtime64_s(out_tm, epoch)

// localtime(),   _localtime32   : thread NOT safe
// localtime_r(), _localtime32_s : thread safe
// convert epoch(UTC) to locale time tm
//  > convert with locale setting => locale offset, DST, 
#define cj_conv_epoch32_to_locale_tm(epoch,out_tm)			_localtime32_s(out_tm, epoch)
#define cj_conv_epoch64_to_locale_tm(epoch,out_tm)			_localtime64_s(out_tm, epoch)


#ifdef CJ_USE_TIME64

// __time64_t=long long
typedef cjepoch64											cjepoch;

#define cj_get_epoch										cj_get_epoch64

// make epoch from locale tm(locale time)
// ignored param: tm.tm_wday, tm.tm_yday
// rst: epoch(UTC) or -1: failed
#define cj_get_epoch_from_locale_tm							_mktime64

// make epoch(utc) tm
#define cj_get_epoch_from_tm								_mkgmtime64

#define cj_get_tm											cj_get_tm64
#define cj_get_locale_tm									cj_get_locale_tm64

#define cj_conv_epoch_to_tm(epoch, out_tm)					cj_conv_epoch64_to_tm(epoch, out_tm)
#define cj_conv_epoch_to_locale_tm(epoch, out_tm)			cj_conv_epoch64_to_locale_tm(epoch, out_tm)


#else 

// __time32_t=long
typedef cjepoch32											cjepoch;

#define cj_get_epoch										cj_get_epoch32

#define cj_get_epoch_from_locale_tm							_mktime32

// make epoch(utc) tm
#define cj_get_epoch_from_tm								_mkgmtime32

#define cj_get_tm											cj_get_tm32
#define cj_get_locale_tm									cj_get_locale_tm32

#define cj_conv_epoch_to_tm(epoch, out_tm)					cj_conv_epoch32_to_tm(epoch, out_tm)
#define cj_conv_epoch_to_locale_tm(epoch, out_tm)			cj_conv_epoch32_to_locale_tm(epoch, out_tm)

#endif


#elif defined(CJPF_POSIX)


// get epoch second (UTC) from os
#define cj_get_epoch32										time32
#define cj_get_epoch64										time64

// gmtime() : thread NOT safe
// gmtime_r(), gmtime64_r() : thread safe
#define cj_conv_epoch32_to_tm(epoch,out_tm)					gmtime_r(epoch,out_tm)
#define cj_conv_epoch64_to_tm(epoch,out_tm)					gmtime64_r(epoch,out_tm)

// localtime() : thread NOT safe
// localtime_r(), localtime64_r() : thread safe
#define cj_conv_epoch32_to_locale_tm(epoch,out_tm)			localtime_r(epoch,out_tm)
#define cj_conv_epoch64_to_locale_tm(epoch,out_tm)			localtime64_r(epoch,out_tm)


#ifdef CJ_USE_TIME64

// time_t = long
// long: 32bit in posix32 ,  long: 64bit in posix64
typedef cjepoch64											cjepoch;

#define cj_get_epoch										cj_get_epoch64


// make epoch from locale tm
// ignored param: tm.tm_wday, tm.tm_yday
// rst: epoch(UTC) or -1: failed
// long: 64bit in posix64
#define cj_get_epoch_from_locale_tm							mktime

// make epoch(utc) tm
#define cj_get_epoch_from_tm								mkgmtime

#define cj_get_tm											cj_get_tm64
#define cj_get_locale_tm									cj_get_locale_tm64

#define cj_conv_epoch_to_tm(epoch, out_tm)					cj_conv_epoch64_to_tm(epoch, out_tm)
#define cj_conv_epoch_to_locale_tm(epoch, out_tm)			cj_conv_epoch64_to_locale_tm(epoch, out_tm)


#else 


// __time32_t=long
typedef cjepoch_sec32										cjepoch;

#define cj_get_epoch										cj_get_epoch32

// long: 32bit in posix32
#define cj_get_epoch_from_locale_tm							mktime

// make epoch(utc) tm
#define cj_get_epoch_from_tm								mkgmtime

#define cj_get_tm											cj_get_tm32
#define cj_get_locale_tm									cj_get_locale_tm32

#define cj_conv_epoch_to_tm(epoch, out_tm)					cjepoch_epoch32_to_tm(epoch, out_tm)
#define cj_conv_epoch_to_locale_tm(epoch, out_tm)			cjepoch_epoch32_to_locale_tm(epoch, out_tm)


#endif


#endif   // end of -defined(CJPF_POSIX)-


#ifdef CJ_USE_TIME64

#define cjtime_sec						cjtime_sec64
#define cjtime_ms						cjtime_ms64

#define cjgettimeofday					cjgettimeofday64		
#define cjgettime_sec					cjgettime_sec64
#define cjgettime_ms					cjgettime_ms64
#define cjcheck_time_interval_sec		cjcheck_time_interval_sec64

#define cjtimeval						timeval64

#ifdef CJPF_WIN
#define CJEPOCH_PRFMT					"%I64d"
#else
#define CJEPOCH_PRFMT					"%lld"
#endif



#else

#define cjtime_sec						cjtime_sec32
#define cjtime_ms						cjtime_ms32


#define cjgettimeofday					cjgettimeofday32
#define cjgettime_sec					cjgettime_sec32
#define cjgettime_ms					cjgettime_ms32
#define cjcheck_time_interval_sec		cjcheck_time_interval_sec32

#define cjtimeval						timeval32

#define CJEPOCH_PRFMT(x)				"%Id"


#endif



#endif