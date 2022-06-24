
#include <cj/cj.h>
#include <cj/cjtime.h>


/*---------------------------------------------------------------------------*/
#ifdef WIN32
/*---------------------------------------------------------------------------*/

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timeval {
	long    tv_sec;         /* seconds */
	long    tv_usec;        /* and microseconds */
};

CJEXTERNC cji cjgettimeofday32(struct timeval* tv, struct timezone* tz) {
	FILETIME ft;
	unsigned __int64 v = 0;
	static int tzflag;
	long timezone_v;
	long daylight_v;

	if (tv) {
		GetSystemTimeAsFileTime(&ft);

		v |= ft.dwHighDateTime;
		v <<= 32;
		v |= ft.dwLowDateTime;

		/*converting file time to unix epoch*/
		v /= 10;  /*convert into microseconds*/
		v -= DELTA_EPOCH_IN_MICROSECS;
		tv->tv_sec = (long)(v / 1000000UL);
		tv->tv_usec = (long)(v % 1000000UL);
	}

	if (tz) {
	    if (!tzflag) {
			_tzset();
			tzflag++;
		}

		_get_timezone(&timezone_v);
		_get_daylight(&daylight_v);

		tz->tz_minuteswest = timezone_v / 60;
		tz->tz_dsttime = daylight_v;
	}

	return 0;
}

CJEXTERNC cji cjgettimeofday64(struct timeval64* tv, struct timezone* tz) {

	FILETIME ft;
	unsigned __int64 v = 0;
	static int tzflag;
	long timezone_v;
	long daylight_v;

	if (tv) {
		GetSystemTimeAsFileTime(&ft);

		v |= ft.dwHighDateTime;
		v <<= 32;
		v |= ft.dwLowDateTime;

		/*converting file time to unix epoch*/
		v /= 10;  /*convert into microseconds*/
		v -= DELTA_EPOCH_IN_MICROSECS;
		tv->tv_sec = (long)(v / 1000000UL);
		tv->tv_usec = (long)(v % 1000000UL);
	}

	if (tz) {
		if (!tzflag) {
			_tzset();
			tzflag++;
		}

		_get_timezone(&timezone_v);
		_get_daylight(&daylight_v);

		tz->tz_minuteswest = timezone_v / 60;
		tz->tz_dsttime = daylight_v;
	}

	return 0;
}


/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#error "implementation not yet"




/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/


//
// time management
//

CJEXTERNC cji cjgettime_sec32(void)
{
	struct timeval tv;
	cjgettimeofday32(&tv, NULL);

	return tv.tv_sec;
}

CJEXTERNC cji64 cjgettime_sec64(void)
{
	struct timeval64 tv;
	cjgettimeofday64(&tv, cjNULL);

	return tv.tv_sec;
}

CJEXTERNC cji cjgettime_ms32(void)
{
	struct timeval tv;
	cji ms;

	cjgettimeofday32(&tv, cjNULL);
	ms = (tv.tv_sec * (cji)1000) + (tv.tv_usec / 1000);

	return ms;
}

CJEXTERNC cji64 cjgettime_ms64(void)
{
	struct timeval64 tv;
	cji64 ms64;

	cjgettimeofday64(&tv, cjNULL);
	ms64 = (tv.tv_sec * (cji64)1000) + (tv.tv_usec / 1000);

	return ms64;
}

CJEXTERNC cjepoch32 cj_get_tm32(cjtime_tm* out) {
	cjepoch32 sys_epoch;

	if (cj_get_epoch32(&sys_epoch) == -1) {
		cjassert(0 && "failed to get epoch32");
		return -1;
	}

	if (!out)
		return sys_epoch;

	if(0 == cj_conv_epoch32_to_tm(&sys_epoch, out))
		return sys_epoch;

	return -1;
}

CJEXTERNC cjepoch64 cj_get_tm64(cjtime_tm* out) {
	cjepoch64 sys_epoch;

	if (cj_get_epoch64(&sys_epoch) == -1) {
		cjassert(0 && "failed to get epoch64");
		return -1;
	}

	if (!out)
		return sys_epoch;

	if (0 == cj_conv_epoch64_to_tm(&sys_epoch, out))
		return sys_epoch;

	return -1;
}

CJEXTERNC cjepoch32 cj_get_locale_tm32(cjtime_tm* out) {
	cjepoch32 sys_epoch;
	
	if (cj_get_epoch32(&sys_epoch) == -1) {
		cjassert(0 && "failed to get epoch32");
		return -1;
	}

	if (!out)
		return sys_epoch;

	if (0 == cj_conv_epoch32_to_locale_tm(&sys_epoch, out))
		return sys_epoch;

	return -1;
}

CJEXTERNC cjepoch64 cj_get_locale_tm64(cjtime_tm* out) {
	cjepoch64 sys_epoch;

	if (cj_get_epoch64(&sys_epoch) == -1) {
		cjassert(0 && "failed to get epoch64");
		return -1;
	}

	if (!out)
		return sys_epoch;

	if (0 == cj_conv_epoch64_to_locale_tm(&sys_epoch, out))
		return sys_epoch;

	return -1;
}

CJEXTERNC cjbool cjcheck_time_interval_sec32(cjtime_sec32* last_sec,
	cjtime_sec32 limit_interval_sec)
{
	struct timeval tv;

	cjgettimeofday32(&tv, cjNULL);

	if (tv.tv_sec - (*last_sec) < limit_interval_sec)
		return cjfalse;

	*last_sec = tv.tv_sec;

	return cjtrue;
}

CJEXTERNC cjbool cjcheck_time_interval_sec64(cjtime_sec64* last_sec,
	cjtime_sec64 limit_interval_sec)
{
	struct timeval64 tv;

	cjgettimeofday64(&tv, cjNULL);

	if (tv.tv_sec - (*last_sec) < limit_interval_sec)
		return cjfalse;

	*last_sec = tv.tv_sec;

	return cjtrue;
}
