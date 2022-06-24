
#ifndef _CJ_STRING_H_
#define _CJ_STRING_H_

//
// multibyte
//
CJEXTERNC cji cjatoi(const cjmc* s);

CJEXTERNC cjbool cj_atoi(cji* out, const cjmc* s);
CJEXTERNC cjbool cj_atoi64(cji64* out, const cjmc* s);


CJEXTERNC cjmc* cjitoa(cjmc* buf, const cji n);

// allow_sign: +,-,.
CJEXTERNC cjbool cj_is_number(const cjmc* src, cjbool allow_sign, cjbool allow_dot);


CJEXTERNC cjstr_siz cjstrlen(const cjmc* s);

// 0:success, n:fail
CJEXTERNC cji cjstrcpy_s(cjmc* d, cjstr_siz siz, const cjmc* s);


// 0:success, STRUNCATE: string is truncated, n:fail
CJEXTERNC cji cjstrncpy_s(cjmc* d, cjstr_siz siz, const cjmc* s, cjstr_siz cnt);

// 0:equal , >0: a is greater, <0 : b is greater,  
CJEXTERNC cji cjstrcmp(const cjmc* a, const cjmc* b);
CJEXTERNC cji cjstricmp(const cjmc* a, const cjmc* b);
CJEXTERNC cji cjstrncmp(const cjmc* a, const cjmc* b, cjstr_siz len);
CJEXTERNC cji cjstrnicmp(const cjmc* a, const cjmc* b, cjstr_siz len);

CJEXTERNC cjstr_siz cjsnprintf(cjmc* str, cjstr_siz siz, const cjmc* fmt, ...);

CJEXTERNC const cjmc* cjstrchr(const cjmc* src, cjmc c);
CJEXTERNC const cjmc* cjstrrchr(const cjmc* src, cjmc c);

// find null terminator pos
CJEXTERNC const cjmc* cjstrfind_null_term(const cjmc* src);

CJEXTERNC cjbool cj_parse_int(cji* out_num, const cjmc* src);

// 0
// sec: 1s 10s 30s 60s 
// minute: 1m 10m 30m 60m
// hour: 1h 10h 24h 48h
// day: 1d 10d 30d ..
CJEXTERNC cjbool cj_parse_duration_str(cji64* out_num, const cjmc* src);
#define cj_parse_interval_str			cj_parse_duration_str


//
// unicode-wide (UTF16 le)
//

CJEXTERNC cjstr_siz cjwstrlen(const cjwc* s);

CJEXTERNC cji cjwstrcpy_s(cjwc* d, cjstr_siz siz, const cjwc* s);
CJEXTERNC cji cjwstrncpy_s(cjwc* d, cjstr_siz siz, const cjwc* s, cjstr_siz cnt);

CJEXTERNC cji cjwstrcmp(const cjwc* a, const cjwc* b);
CJEXTERNC cji cjwstricmp(const cjwc* a, const cjwc* b);
CJEXTERNC cji cjwstrncmp(const cjwc* a, const cjwc* b, cjstr_siz len);
CJEXTERNC cji cjwstrnicmp(const cjwc* a, const cjwc* b, cjstr_siz len);

CJEXTERNC cjstr_siz cjwsnprintf(cjwc* str, cjstr_siz siz, const cjwc* fmt, ...);

CJEXTERNC const cjwc* cjwstrchr(const cjwc* src, cjwc c);
CJEXTERNC const cjwc* cjwstrrchr(const cjwc* src, cjwc c);



#endif
