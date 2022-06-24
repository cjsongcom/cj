
/*
  chunjong simple common library
  2022. chunjong.song, wiper90x@nate.com  

  https://github.com/cjsongcom

  updates:   
*/

#ifndef _CJ_H_
#define _CJ_H_


#if defined(WIN32) || defined(WIN64)

#define CJPF_WIN

#if defined(WIN32)
#define CJPF_WIN32
#define CJ32BIT
#else
#define CJPF_WIN64
#define CJ64BIT
#endif

#define CJ_PRFMT_I64		"%I64d"


#else

#define CJPF_POSIX

#define CJPF_LINUX

#if __GNUC__
#if __x86_64__ || __ppc64__
#define CJ64BIT
#else
#define CJ32BIT
#endif
#endif

#define CJ_PRFMT_I64		"%Ild"


#endif



#ifdef CJPF_WIN
#define _CRT_SECURE_NO_WARNINGS 1
#endif


#include <time.h>

#ifdef CJPF_WIN
// to support struct __time32_t, __time64_t
#include <sys/timeb.h>
#endif

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>

#include <locale.h>


#define cjNULL          0
#define cjnull			0
#define cjnullptr		nullptr

#define cjansi_null		(char)0
#define cjuni_null		()

#define CJ_MAX_PATH						256

#define CJ_MAX_URI						1024

#define CJ_MAX_SHORT_NAME				32
#define CJ_MAX_MID_NAME					64
#define CJ_MAX_NAME						128
#define CJ_MAX_TAG						128

#define CJ_MAX_SHORT_ERR_MSG			64
#define CJ_MAX_LONG_ERR_MSG				1024
#define CJ_MAX_ERR_MSG					256

// number string 
// 64bit: -9,223,372,036,854,775,808~ 9,223,372,036,854,775,807  
// unsigned 64bit: 18446744073709551615 = 20 str
#define CJ_MAX_NUM_STR					32


#define CJ_UNUSED(x)					{(void)(x);}


//#define cjsiz_t			size_t

typedef size_t  cjsiz_t, cjsiz;

// maximum memory
typedef size_t  cjmsiz_t, cjmsiz;

// int 4byte
typedef int cji;
typedef unsigned int cjui;

typedef __int64 cji64;
typedef unsigned __int64 cjui64;

typedef char cjc;		// char 1byte
typedef unsigned char cjuc;		// char 1byte

typedef short cjw;		// short
typedef unsigned short cjuw;		// short

// float 4byte
typedef float cjfloat;
// double 8byte
typedef double cjdouble;


///////////////////////////////////////////////////////
// sizeof(long) in platform
// win32(32bit) : 32bit,  
// win64(64bit) : 32bit
// linux(32bit) : 32bit
// linux(64bit) : 64bit  <<<
///////////////////////////////////////////////////////


typedef int					  cjlong32;     // keep 4byte in 64bit platform
typedef long long			  cjlong64;
typedef long long			  cjlonglong;

typedef unsigned int          cjulong32;    // keep 4byte in 64bit platform
typedef unsigned long long    cjulong64;
typedef unsigned long long    cjulonglong;

typedef void cjvoid;

typedef size_t cjsiz_t;


typedef cji   cjidx;



//
// character
//

typedef char cjmc;				// multi-byte char	1byte, ansi, cp949

#ifdef WIN32
typedef wchar_t cjwc;			// wide character	2byte
#else
typedef unsigned short cjwc;    // wide character	2byte
#endif

typedef unsigned char cju8c;	// utf-8 character	


// true/false
typedef enum { cjfalse, cjtrue } cjbool;

typedef int cjrst;
enum { cjok = 0, cjfail = -1 };
enum { cjrstok = 0, cjrstfail = -1 };

inline cjbool is_valid_cjrst(cjrst rst) {
	return rst == cjrstok ? cjtrue : cjfalse;
}


#if !defined(CJ_ARY_SIZ)
#define CJ_ARY_SIZ(_x) (sizeof(_x) / sizeof(_x[0]))
#endif

#if !defined(CJA_SIZ)
#define CJA_SIZ(_x) (sizeof(_x) / sizeof(_x[0]))
#endif


#ifdef __cplusplus 
#include <string>
#include <vector>
#include <map>

#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>
#endif


#ifdef __cplusplus
#define CJEXTERN extern
#define CJEXTERNC extern "C"
#define CJEXTERNC_BEGIN extern "C" {
#define CJEXTERNC_END }
#else
#define CJEXTERN extern
#define CJEXTERNC 
#define CJEXTERNC_BEGIN 
#define CJEXTERNC_END 
#endif

//
// cjdefine
//

#define CJDEBUG

#ifdef CJDEBUG
#define _CJDEBUG_MEM

#include <assert.h>

#define cjassert(x)             assert(x)
#define cjrAssert(x)            assert(x)

#else

#define cjAssert(x)
#define cjRAssert(x)

#endif


#define cjabs(x)  abs(x)


#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

// avoid ws2def.h(91) : warning C4005: 'AF_IPX' : macro redefinition
// http://social.msdn.microsoft.com/Forums/en/windowssdk/thread/4a90b143-1fb8-43e9-a54c-956127e0c579
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#else

# include <sys/time.h>

#endif

//
// cj string types
//

typedef char cjansi;
typedef wchar_t cjutf16le;
typedef cjutf16le cjuni;
typedef cjutf16le cjstrch;

// wide character
#define CJWC(x)  L##x

// utf-8 unicode
#define CJU8C(x) x

// mulit-byte, ansi, ascii, defaut
#define CJMC(x)	x

// cj-param-character, cjpstring
//#define CJPC(x) x

// current character
//#define CJCC(x) x

// cj param character is utf-8 (unsigned char)

// use wide char (utf-16 le)
#define CJ_USE_WIDE

#ifdef CJ_USE_WIDE
	typedef cjwc		cjch;
#else
	typedef cjmc		cjch;
#endif


typedef cji cjstr_siz;

typedef cjui cjflag;

typedef struct {
		cji64						v;
} cj_duration, cj_interval;


#ifdef __cplusplus

typedef std::wstring  cjwstring;
typedef std::string   cjastring;

// cjparam string, unsigned char, utf-8
typedef std::string   cjpstring;

//#ifdef _UNICODE
//typedef std::wstring cjstring;
//#else
//typedef std::string cjstring;
//#endif

typedef std::vector<std::wstring>	cjwstring_ary;
typedef std::vector<std::string>	cjastring_ary;
//typedef std::vector<cjstring>		cjstring_ary;
typedef std::vector<cjpstring>	    cjpstring_ary;


#endif

#include <cj/cj_mem.h>
#include <cj/cjary.h>

#include <cj/cjtime.h>
#include <cj/cjthread.h>
#include <cj/cjuuid.h>

#include <cj/cj_string.h>
#include <cj/cj_ch_conv.h>

#include <cj/cjfile.h>
#include <cj/cj_log.h>

#include <cj/cj_ymd.h>
#include <cj/cjdatetime.h>


typedef enum cjpair_type_t {

	// key=void*			val=void*
	CJPAIR_TYPE_PTR_PTR = 0,

	// key="key"			val=void*
	CJPAIR_TYPE_STRPTR_PTR,

	
	// key=cjmc[]			val=void*
	CJPAIR_TYPE_STR_PTR = 100,

} cjpair_type;


typedef struct cjpair_t cjpair;

typedef struct cjpair_t {

	cjpair_type					type;
	cjpair*						_next;

	cjpair*						_seq_prev;
	cjpair*						_seq_next;

	void*						key;
	cjsiz						key_siz;

	void*						val;
	cjsiz						val_siz;

} cjpair;



#include <cj/cj_json.h>


void _cjstd_printf(const char* fmt, ...);

#define cjstd_printf(...)				_cjstd_printf(__VA_ARGS__)



#endif   // end of cj.h

