
#include <cj/cj.h>

#include "cjtui.h"
#include "cjtui_curses.h"


CJEXTERNC cji cjtui_win_mvprintf_curses(WINDOW* win, cji y, cji x, const cjtui_ch* fmt, ...)
{
    cjstr_siz n = 0;
    cjtui_ch buf[2048] = { 0 };

    if (!win)
        return 0;

    va_list ap;
    va_start(ap, fmt);

    n = cjtui_vsnprintf(buf, sizeof(buf) / sizeof(buf[0]), fmt, ap);

    va_end(ap);

#ifdef CJTUI_USE_WIDE
    mvwaddwstr(win, y, x, buf);
    //waddwstr(win, buf);
#else
    mvwaddstr(win, y, x, buf);
#endif

    return n;
}

CJEXTERNC cji cjtui_win_nprintf_curses(WINDOW* win, cji str_siz, const cjtui_ch* fmt, ...)
{
    cjstr_siz n = 0;
    cjtui_ch buf[2048] = { 0 };

    if (!win)
        return 0;

    va_list ap;
    va_start(ap, fmt);

    if (str_siz > 2047)
        str_siz = 2048;

    n = cjtui_vsnprintf(buf, str_siz, fmt, ap);

    va_end(ap);

#ifdef CJTUI_USE_WIDE
    waddwstr(win, buf);
//    mvwaddwstr(win, y, x, buf);
#else
    waaddstr(win, buf);
#endif

    return n;
}


CJEXTERNC cji cjtui_win_printf_curses(WINDOW* win, const cjtui_ch* fmt, ...)
{
    cjstr_siz n = 0;
    cjtui_ch buf[2048] = { 0 };

    if (!win)
        return 0;

    va_list ap;
    va_start(ap, fmt);

    n = cjtui_vsnprintf(buf, sizeof(buf) / sizeof(buf[0]), fmt, ap);

    va_end(ap);

#ifdef CJTUI_USE_WIDE
    waddwstr(win, buf);
    //    mvwaddwstr(win, y, x, buf);
#else
    waaddstr(win, buf);
#endif

    return n;
}
