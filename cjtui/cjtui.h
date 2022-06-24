
#ifndef _CJTUI_H_
#define _CJTUI_H_

#define CJTUI_USE_CURSES

// use UTF16 (not utf8)
#define CJTUI_USE_WIDE

#ifdef CJTUI_USE_WIDE
// wide(utf16-le)

#define CJTUI_CH(x)     CJWC(x)

typedef cjwc cjtui_ch;

#define cjtui_strlen(x)                             cjwstrlen(x)
#define cjtui_strcpy_s(buf,buf_siz,src)             cjwstrcpy_s(buf,buf_siz,src)
#define cjtui_snprintf                              cjwsnprintf
#define cjtui_vsnprintf(buf, bufsiz, fmt, args)     vswprintf(buf, bufsiz, fmt, args)


#else
// mutlibyte

typedef cjmc cjtui_ch;
#define cjtui_strlen(x)                             cjstrlen(x)
#define cjtui_snprintf                              cjsnprintf
#define cjtui_vsnprintf                             vsprintf(buf, bufsiz, fmt, args)

#endif

#if defined(MOUSE_MOVED)
#undef MOUSE_MOVED
#endif

/*
    change/restore locale

    // preserve current locale info
    char *prev_locale = setlocale(LC_ALL, NULL);

    // dupplicate current locale to prevent furutue modify
    char *dupp_prev_locale = strdup(prev_locale);

    // change locale
    setlocale (LC_ALL, new_locale);

	// restore locale
	setlocale(LC_ALL, dupp_prev_locale);
    free(dupp_prev_locale);

windows
> chcp 437   ;(oem-usa)
> chcp 949   ;(korean)

*/

// https://blog.quasar.ai/2009/04/30/utf-8-strings-in-visual-c

// setlocal(LC_ALL,"C") is executed when program is start,
// locale parameters) : C , en-US, fr-FR

// result: "C"
// after call 'setlocale(LC_ALL, "")' : Korean_Korea.949
#define cjlocale_get_cur()                 setlocale(LC_ALL, cjNULL);

// C/C++ runtime is by default set to the "C" locale
#define cjlocale_set_prog_default()         setlocale(LC_ALL, "C")
#define cjlocale_set_os()                   setlocale(LC_ALL, "")
#define cjlocale_set_utf8()                 setlocale(LC_ALL, ".utf8")


#ifdef CJTUI_USE_CURSES

#define cjtui_win_printf                    cjtui_win_printf_curses

#include <cj/cjtui/cjtui_curses.h>

#else


#endif

#include <cj/cjtui/cjtui_menu.h>
#include <cj/cjtui/cjtui_menu_worker.h>

#include <cj/cjtui/cjtui_wnd.h>


#endif
