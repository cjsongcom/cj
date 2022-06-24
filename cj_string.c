
#include "cj.h"
#include "cj_string.h"


#ifdef _MSC_VER
#pragma warning(disable:4996)
// C4996 This function or variable may be unsafe. Consider using strcpy_s instead.
//       To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details
#endif

#if defined(WIN32)

//
// win32
//

#define _cjstrcmp(a,b) strcmp(a,b)
#define _cjwcscmp(a,b) wcscmp(a,b)
#define _cjstricmp(a,b) _stricmp(a,b)
#define _cjwcsicmp(a,b) _wcsicmp(a,b)
#define _cjstrncmp(a,b,n) strncmp(a,b,n)
#define _cjwcsncmp(a,b,n) wcsncmp(a,b,n)
#define _cjstrnicmp(a,b,n) strnicmp(a,b,n)
#define _cjwcsnicmp(a,b,n) _wcsnicmp(a,b,n)

#define _cjstrcpy(d,s) strcpy(d,s)
#define __cjstrcpy(d,siz,s) strcpy_s(d,siz,s)
#define _cjwcscpy(d,s) wcscpy(d,s)
#define __cjwcscpy(d,siz,s) wcscpy_s(d,siz,s)
#define _cjstrncpy(d,s,scnt) strncpy(d,s,scnt)
#define __cjstrncpy(d,siz,s,scnt) strncpy_s(d,siz,s,scnt)
#define _cjwcsncpy(d,s,scnt) wcsncpy(d,s,scnt)
#define __cjwcsncpy(d,siz,s,scnt) wcsncpy_s(d,siz,s,scnt)

#define _cjstrlwr(d) strlwr(d)
#define _cjwcslwr(d) wcslwr(d)
#define _cjstrupr(d) strupr(d)
#define _cjwcsupr(d) wcsupr(d)

#define _cjstrcat(d,s) strcat(d,s)
#define __cjstrcat(d,siz,s) strcat_s(d,siz,s)
#define _cjwcscat(d,s) wcscat(d,s)
#define __cjwcscat(d,siz,s) wcscat_s(d,siz,s)

#define _cjstrlen(s) strlen(s)
#define _cjwcslen(s) wcslen(s)

#define _cjstrchr(s,c) strchr(s,c)
#define _cjwcschr(s,c) wcschr(s,c)

#define _cjstrrchr(s,c) strrchr(s,c)
#define _cjwcsrchr(s,c) wcsrchr(s,c)

#define _cjstrstr(s,f) strstr(s,f)
#define _cjwcsstr(s,f) wcsstr(s,f)

// str to unsigned long
#define _cjstrtoul(s,e,radix) strtoul(s,e,radix)
#define _cjwcstoul(s,e,radix) wcstoul(s,e,radix)

#define _cjatoi(s) atoi(s)
#define _cjwtoi(s) _wtoi(s)
#define _cjatof(s) atof(s)
#define _cjwtof(s) _wtof(s)

#define _cjWCS2MBS(d,s,bufsiz)  ::WideCharToMultiByte(CP_ACP, 0, s, -1, d, bufsiz, NULL, FALSE)
#define _cjMBS2WCS(d,s,bufsiz)  ::MultiByteToWideChar(CP_ACP, 0, s, -1, d, bufsiz)

#define _cjwcs_vsnprintf(buf, bufsiz, fmt, args)    vswprintf(buf, bufsiz, fmt, args)


////////////////////////////////////////////////////////////////
#else
////////////////////////////////////////////////////////////////

//
// apple
//

//
// listing of non ansi functions
//
static cjChar* _cj_strlower(cjChar* str)
{
    for (cjChar* p = str; *p; p++)
        *p = (cjChar)tolower((cjINT32)*p);

    return(str);
}
static cjUni* _cj_wcslower(cjUni* str)
{
    for (cjUni* p = str; *p; p++)
        *p = (cjUni)tolower((cjINT32)*p);

    return(str);
}
static cjChar* _cj_strupper(cjChar* str)
{
    for (cjChar* p = str; *p; p++)
        *p = (cjChar)toupper((cjINT32)*p);

    return(str);
}
static cjUni* _cj_wcsupper(cjUni* str)
{
    for (cjUni* p = str; *p; p++)
        *p = (cjUni)toupper((cjINT32)*p);

    return(str);
}

#define _cjstrcmp(a,b) strcmp(a,b)
#define _cjwcscmp(a,b) wcscmp(a,b)

// strcasecmp, strncasecmp - compare two strings ignoring case
#define _cjstricmp(a,b) strcasecmp(a,b)
#define _cjwcsicmp(a,b) wcscasecmp(a,b)

#define _cjstrncmp(a,b,n) strncmp(a,b,n)
#define _cjwcsncmp(a,b,n) wcsncmp(a,b,n)

#define _cjstrnicmp(a,b,n) strncasecmp(a,b,n)
#define _cjwcsnicmp(a,b,n) wcsncasecmp(a,b,n)

#define _cjstrcpy(d,s) strcpy(d,s)
#define __cjstrcpy(d,siz,s) strcpy(d,s)
#define _cjwcscpy(d,s) wcscpy(d,s)
#define __cjwcscpy(d,siz,s) wcscpy(d,s)

#define _cjstrncpy(d,s,siz) strncpy(d,s,siz)
#define _cjwcsncpy(d,s,siz) wcsncpy(d,s,siz)

// strlwr , strupr is not ansi functions
#define _cjstrlwr(d) _cj_strlower(d)
#define __cjstrlwr(d,siz) _cj_strlower(d)
#define _cjwcslwr(d) _cj_wcslower(d)
#define __cjwcslwr(d,siz) _cj_wcslower(d)

#define _cjstrupr(d) _cj_strupper(d)
#define __cjstrupr(d,siz) _cj_strupper(d)
#define _cjwcsupr(d) _cj_wcsupper(d)
#define __cjwcsupr(d,siz) _cj_wcsupper(d)

#define _cjstrcat(d,s) strcat(d,s)
#define __cjstrcat(d,siz,s) strcat(d,s)
#define _cjwcscat(d,s) wcscat(d,s)
#define __cjwcscat(d,siz,s) wcscat(d,s)

#define _cjstrlen(s) strlen(s)
#define _cjwcslen(s) wcslen(s)
#define _cjstrchr(s,c) strchr(s,c)
#define _cjwcschr(s,c) wcschr(s,c)

#define _cjstrrchr(s,c) strrchr(s,c)
#define _cjwcsrchr(s,c) wcsrchr(s,c)
#define _cjstrstr(s,f) strstr(s,f)
#define _cjwcsstr(s,f) wcsstr(s,f)

// str to unsigned long
#define _cjstrtoul(s,e,radix) strtoul(s,e,radix)
#define _cjwcstoul(s,e,radix) wcstoul(s,e,radix)

#define _cjatoi(s) atoi(s)
#define _cjwtoi(s) _cjwcstoul(s,0,10)
#define _cjatof(s) atof(s)
#define _cjwtof(s) wcstof(s,0)

/*
 (void)setlocale(LC_ALL, "");
 * Calculate the maximum number of bytes needed to
 * store the wide character buffer in multibyte form in the
 * current code page and malloc() the appropriate storage,
 * including the terminating null.

  s = (char *) malloc( wcslen(pwcs) * MB_CUR_MAX + 1 );
  retval= wcstombs( s, pwcs, n);
  if( retval == -1) {
    * Error handle
    * s points to the multibyte character string.
*/

// The behavior of this function depends on the LC_CTYPE category of the selected C locale.

// return size of converted from wcstombs in mac is only for string, not added for null-terminator
// so adding +1 for null-terminator
#define _cjWCS2MBS(d,s,bufsiz)  wcstombs(d,s,bufsiz)+1
#define _cjMBS2WCS(d,s,bufsiz)  mbstowcs(d,s,bufsiz)+1

#endif

CJEXTERNC cji cjatoi(const cjmc* s) {
    return atoi(s);
}

CJEXTERNC cjbool cj_atoi(cji* out, const cjmc* s) {
    
    cji v = atoi(s);

    if (errno != 0)
        return cjfalse;

    *out = v;

    return cjtrue;
}

CJEXTERNC cjbool cj_atoi64(cji64* out, const cjmc* s) {

    cji64 v = _atoi64(s);

    if (errno != 0)
        return cjfalse;

    *out = v;

    return cjtrue;
}

CJEXTERNC cjmc* cjitoa(cjmc* buf, const cji n) {
    return itoa(n, buf, 10);
}

CJEXTERNC cjstr_siz cjstrlen(const cjc* s)
{
    if (!s)
        return 0;

    return strlen(s);
}

CJEXTERNC cji cjstrcpy_s(cjc* d, cjstr_siz siz, const cjc* s)
{
    return __cjstrcpy(d,siz, s);
}

CJEXTERNC cji cjstrncpy_s(cjc* d, cjstr_siz siz, const cjc* s, cji cnt)
{
    return __cjstrncpy(d, siz, s, cnt);
}

CJEXTERNC cji cjstrcmp(const cjc* a, const cjc* b)
{
    return _cjstrcmp(a, b);
}
CJEXTERNC cji cjstricmp(const cjc* a, const cjc* b)
{
    return _cjstricmp(a, b);
}
CJEXTERNC cji cjstrncmp(const cjc* a, const cjc* b, cjstr_siz len)
{
    return _cjstrncmp(a, b, len);
}
CJEXTERNC cji cjstrnicmp(const cjc* a, const cjc* b, cjstr_siz len)
{
    return _cjstrnicmp(a, b, len);
}

CJEXTERNC
cjstr_siz cjsnprintf(cjc* str, cjstr_siz siz, const cjc* fmt, ...)
{
    va_list ap;
    cjstr_siz n;

    if (!siz)
        return 0;

    va_start(ap, fmt);
    n = vsnprintf(str, siz, fmt, ap);
    va_end(ap);

    if (n >= (cjstr_siz)siz)
        return (cjstr_siz)siz;

    return n;
}

CJEXTERNC const cjmc* cjstrchr(const cjmc* src, cjmc c) {
    return _cjstrchr(src, c);
}

CJEXTERNC const cjmc* cjstrrchr(const cjmc* src, cjmc c) {
    return _cjstrrchr(src, c);
}

CJEXTERNC const cjmc* cjstrfind_null_term(const cjmc* src) {
    if (!src)
        return cjNULL;

    while (*src != cjNULL)
        src++;

    return src;
}

CJEXTERNC cjbool cj_is_number(const cjmc* src, cjbool allow_sign, cjbool allow_dot) {

    if (!src || !*src)
        return cjfalse;

    cjbool dot_found = cjfalse;

    cjbool chk_sign = cjfalse;
    cjbool chk_dot = cjfalse;
    
    cjmc c = *src;

    if (allow_sign && (c == '-' || c == '+'))
        src++;
   
    while ( (c=*src++) ) {
        if (chk_dot && allow_dot) {
            if (c == '.') {
                chk_dot = cjfalse;
                src++;

                continue;
            }
        }

        if (c >= '0' && c <= '9')
            continue;

        return cjfalse;         
    }

    return cjtrue;
}

CJEXTERNC cjbool cj_parse_int(cji* out_num, const cjmc* src) {

    if (!src || *src == 0)
        return cjfalse;

    if (!cj_is_number(src, cjtrue, cjfalse))
        return cjfalse;

    if (out_num)
        *out_num = cjatoi(src);

    return cjtrue;
}

CJEXTERNC cjbool cj_parse_duration_str(cji64* out_num, const cjmc* src)     {

    if (!src || *src == 0)
        return cjfalse;

    const cjmc* s = src;

    // skip space
    while (*s) {
        if (*s != ' ')
            break;

        s++;
    }

    cjmc buf[CJ_MAX_NUM_STR] = { 0 };
    cji64 num;

    cjidx i=0;
    cjmc c;

    cji bias = 1;
    cjbool bias_found = cjfalse;

    while ((c = *s++)) {
        if (i > (CJ_MAX_NUM_STR - 1))
            return cjfalse;

        if (c >= '0' && c <= '9') {
            buf[i] = c;
            i++;

            continue;
        }

        bias_found = cjtrue;
        s++;
        break;
    }

    if (*(s-1) != cjNULL)
        return cjfalse;

    if (bias_found) {
        // possible identifier
        // s=sec, m=minute, h=hour d=1day
        switch (c) {
        case 's': {
            bias = 1;
        }
        break;

        case 'm': {
            bias = 60;
        }
        break;

        case 'h': {
            bias = 60 * 60;
        }
        break;

        case 'd': {
            bias = 60 * 60 * 24;
        }
        break;

        default:
            return cjfalse;
        }
    }

    // must start with numeric
    if (!cj_is_number(buf, cjfalse, cjfalse))
        return cjfalse;

    if (!cj_atoi64(&num, buf))
        return cjfalse;

    if (out_num) {
        if (bias_found)
            *out_num = num * bias;
        else
            *out_num = num;
    }

    return cjtrue;
}


//
// unicode-wide (UTF16 LE)
//

CJEXTERNC cjstr_siz cjwstrlen(const cjwc* s)
{
    if (!s)
        return 0;

    return _cjwcslen(s);
}

CJEXTERNC cji cjwstrcpy_s(cjwc* d, cjstr_siz siz, const cjwc* s)
{
    return __cjwcscpy(d, siz, s);
}

CJEXTERNC cji cjwstrncpy_s(cjwc* d, cjstr_siz siz, const cjwc* s, cji cnt)
{
    return __cjwcsncpy(d, siz, s, cnt);
}

CJEXTERNC cji cjwstrcmp(const cjwc* a, const cjwc* b)
{
    return _cjwcscmp(a, b);
}
CJEXTERNC cji cjwstricmp(const cjwc* a, const cjwc* b)
{
    return _cjwcsicmp(a, b);
}
CJEXTERNC cji cjwstrncmp(const cjwc* a, const cjwc* b, cjstr_siz len)
{
    return _cjwcsncmp(a, b, len);
}
CJEXTERNC cji cjwstrnicmp(const cjwc* a, const cjwc* b, cjstr_siz len)
{
    return _cjwcsnicmp(a, b, len);
}

CJEXTERNC
cjstr_siz cjwsnprintf(cjwc* str, cjstr_siz siz, const cjwc* fmt, ...)
{
    va_list ap;
    cjstr_siz n;

    if (!siz)
        return 0;

    va_start(ap, fmt);
    n = _cjwcs_vsnprintf(str, siz, fmt, ap);
    va_end(ap);

    if (n >= (cjstr_siz)siz)
        return (cjstr_siz)siz;

    return n;
}

CJEXTERNC const cjwc* cjwstrchr(const cjwc* src, cjwc c) {
    return _cjwcschr(src, c);
}

CJEXTERNC const cjwc* cjwstrrchr(const cjwc* src, cjwc c) {
    return _cjwcsrchr(src, c);
}

