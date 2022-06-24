
#include "cj.h"
#include "cjthread.h"

//#ifdef WIN32
//
////
////#define PTW32_STATIC_LIB  // link pthread32 library as static
////
////#ifndef _TIMESPEC_DEFINED
////#define _TIMESPEC_DEFINED
////#endif
////// 1. keep pthread library header including orderer
////// 2. libwebsocket.s header file must be later pthread.h
//////	  or it raises error  'typedef int pid_t' in sched.h
////
////#endif 
////
////#include <pthread.h>
//
//#endif

#ifdef WIN32

CJEXTERNC void cjsleep_us(cjui us) {
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (__int64)us);

	// CreateWiatbleTimer reqiure windows xp or higher
	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

CJEXTERNC void cjsleep_ms(cjui ms) {
	Sleep(ms);
}

#else

	#error "not implemented , cj_sleep_ms"

#endif


CJEXTERNC cji cjatomic_inc(cji* out) {
#ifdef WIN32
	return InterlockedIncrement(out);
#else
	//__atomic_add_fetch (type *ptr, type val, int memorder)
	return __atomic_fetch_add(out, 1, __ATOMIC_SEQ_CST);
#endif
}

CJEXTERNC cji cjatomic_dec(cji* out) {
#ifdef WIN32
	// win32=LONG=32bit  win64 LONG=32bit
	//LONG InterlockedIncrement(LONG volatile* Addend);
	return InterlockedDecrement(out);
#else
	return __atomic_fetch_sub(out, 1, __ATOMIC_SEQ_CST);
#endif
}

CJEXTERNC cji cjthread_yield(void) {
#ifdef WIN32
	// return Sleep(0);
	return SwitchToThread();
#else
	return sched_yield();
#endif
}

