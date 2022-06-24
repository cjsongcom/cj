
#ifdef WIN32

#ifndef _TIMESPEC_DEFINED
#define _TIMESPEC_DEFINED
#endif
// 1. keep pthread library header including orderer
// 2. libwebsocket.s header file must be later pthread.h
//	  or it raises error  'typedef int pid_t' in sched.h

#endif

#include <pthread.h>
#include "cj.h"


//
// cjfunction impl
//


CJEXTERNC static void __cjstd_printf(const cjc* fmt, va_list vl) {

	cjmc buf[2048] = { 0 };
	cji n = vsnprintf(buf, sizeof(buf), fmt, vl);

	fprintf(stdout, buf);
}

CJEXTERNC void _cjstd_printf(const cjc* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);
	__cjstd_printf(fmt, ap);
	va_end(ap);
}


