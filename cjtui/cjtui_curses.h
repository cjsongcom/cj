
#ifndef _CJTUI_CURSES_H_
#define _CJTUI_CURSES_H_


// https://man.openbsd.org/ncurses.3
// https://www.ibm.com/docs/en/aix/7.2?topic=d-doupdate-refresh-wnoutrefresh-wrefresh-subroutines
// https://doc.bccnsoft.com/docs/php-docs-7-en/book.ncurses.html
// https://blog.naver.com/woorara7/20010805681

/*
 curses does not handle overlapping windows,
 that's done by the panel(3) library. This means that you can either use stdscr or divide the screen into tiled windows and not using stdscr at all.
 Mixing the two will result in unpredictable, and undesired, effects.
*/

/*
Define before inclusion(only those needed) :

    XCURSES         if building / built for X11
    PDC_RGB         if you want to use RGB color definitions
    (Red = 1, Green = 2, Blue = 4) instead of BGR
    PDC_WIDE        if building / built with wide - character support
    PDC_FORCE_UTF8  if forcing use of UTF8
    PDC_DLL_BUILD   if building / built as a Windows DLL
    PDC_NCMOUSE     to use the ncurses mouse API instead
    of PDCurses' traditional mouse API

    PDCDEBUG

    TO USE WIDE(UTF16):
       #define PDC_WIDE

    TO USE UTF8 :
       #define PDC_WIDE
       #define PDC_FORCE_UTF8
*/

#include <curses.h>
//#include <curspriv.h>

/*
> PDCURSES
> https://github.com/wmcbrine/PDCurses/blob/master/wincon/README.md

You can also give the optional parameter "WIDE=Y", to build the library with wide-character (Unicode) support:

  wmake -f Makefile.wcc WIDE=Y
When built this way, the library is not compatible
with Windows 9x, unless you also link with the Microsoft Layer for Unicode (not tested).

Another option, "UTF8=Y",
makes PDCurses ignore the system locale,
and treat all narrow-character strings as UTF-8.

This option has no effect unless WIDE=Y is also set.
Use it to get around the poor support for UTF-8 in the Windows console:

  make -f Makefile.bcc WIDE=Y UTF8=Y
You can also use the optional parameter "DLL=Y" with Visual C++, MinGW or Cygwin, to build the library as a DLL:

  nmake -f Makefile.vc WIDE=Y DLL=Y
When you build the library as a Windows DLL, you must always define PDC_DLL_BUILD
when linking against it. (Or, if you only want to use the DLL, you could add this definition to your curses.h.)

*/


// for PANEL support
#include <panel.h>


// wnoutrefresh(win)
//  > copies the named window to the virtual screen

// doupdate()
// > compares the virtual screen to the  physical screen 
// > and does the actual update


//  wrefresh() : call wnoutrefresh() + call doupdate()
//  > copies the named window to the physical terminal screen,
//  > taking into account what is already there to do optimizations
//
//  > wrefresh() works by first calling >> wnoutrefresh() <<, 
//  > which copies the named window to the virtual screen,
//
//  > and then calling >> doupdate() <<, which compares the virtual screen
//  > to the physical screen and does the actual update.
//  > If the programmer wishes to output several windows at once,
//  > a series of calls to wrefresh results
//  > in alternating calls to wnoutrefresh and doupdate,


// refresh() : same to wrefresh, but use stdscr window

// wnoutrefresh and doupdate routines allow multiple updates
// > with more efficiency than wrefresh alone. 

CJEXTERNC cji cjtui_win_mvprintf_curses(WINDOW* win, cji y, cji x, const cjtui_ch* fmt, ...);

CJEXTERNC cji cjtui_win_printf_curses(WINDOW* win, const cjtui_ch* fmt, ...);
CJEXTERNC cji cjtui_win_nprintf_curses(WINDOW* win, cji str_siz, const cjtui_ch* fmt, ...);



//
// cjtui  -  curses text user interface  
//

#define cjtui_init                      initscr
#define cjtui_finalize                  endwin

// 0: hide cursor,  1:small cursor,  2:big cursor
#define cjtui_cursor(cursor_type)       curs_set(cursor_type)
#define cjtui_echo(use)                 { if(use) echo(); else noecho(); }
#define cjtui_keypad(win,use)           { if(use) keypad(win, 1); else keypad(win, 0); }


#define cjtui_draw_box(win)             box(win, ACS_VLINE, ACS_HLINE)


// cbreak: disable input line buffering, enable one char input
// raw: enter key=> no iterpreted => 13
#define cjtui_getch_noblock(ch)             { nodelay(stdscr, 1); cbreak(); ch=getch(); nodelay(stdscr, 0); }
#define cjtui_getch(ch)                     { nodelay(stdscr, 0); nocbreak(); ch=getch();  nodelay(stdscr, 0); }

#define cjtui_win_getch_noblock(win, ch)    { nodelay(win, 1); cbreak(); ch=wgetch(win); nodelay(win, 0); }
#define cjtui_win_getch(win, ch)            { nodelay(win, 0); nocbreak(); ch=wgetch(win); nodelay(win, 0);}


// Raw mode is similar to cbreak mode, in that characters typed are 
// immediately passed through to the user program.
// The differences are that in raw mode, the interrupt, quit, suspend,
// and flow control characters are all passed through uninterpreted,
// instead of generating a signal.
#define cjtui_disable_signal_key_proc    raw
#define cjtui_enable_signal_key_proc     noraw

#define cjtui_is_color                   has_colors
#define cjtui_use_color                  start_color

#define cjtui_change_scr_siz(h,w)        resize_term(h,w)

#define cjtui_clear()                    wclear(stdscr)

#define cjtui_gettext(buf, buf_siz)     insnstr(buf, buf_siz)
#define cjtui_gettext_ch()              inch()



// screen window= stdscr

// use (wnoutrefresh(stdscr), wnoutrefresh(win), doupdate) pair,
// > instead refresh/wrefresh for efficency draw and avoid flickering
// 1. update current screen structure to reflect changes made 
//    to the standard screen structure
// 2. draw terminal
#define cjtui_update_and_draw_atonce      refresh


// sample
/*
   WINDOW *my_window1, my_window2;
   wnoutrefresh(win1);
   wnoutrefresh(win2);
   doupdate();
*/
// 1. draw terminal only (copy virtual screen to terminal, no update screen struct)
// 2. sends to the terminal the commands to perform any required changes.

// compare virtual screen with terminal window and update terminal
#define cjtui_draw_screen()                     doupdate();


#define cjtui_scr_h                             LINES
#define cjtui_scr_w                             COLS

#define cjtui_set_backgnd_ch_attr(ch_type)      bkgd(ch_type)
#define cjtui_scr_setyx(y, x)                   setsyx(y, x)


//
// window
//

#define cjtui_win_scr                    stdscr
#define cjtui_win                        WINDOW

#define cjtui_win_clear(win)             wclear(win)

#define cjtui_win_create(h,w,y,x)        newwin(h,w,y,x)
#define cjtui_subwin_create(orig_win, h,w,y,x)  subwin(orig_win, h,w,y,x)


#define cjtui_win_destroy(win)          delwin(win)

// determines which parts of the terminal may need updating.
//#define cjtui_win_set_dirty(win)          wnoutrefresh(win)
//#define cjtui_win_set_dirty(win)         touchwin(win)

// The wnoutrefreshand doupdate routines allow multiple updates with more efficiency 
// than wrefresh alone.In addition to all the window structures, 
// curses keeps two data structures representing the terminal screen 
// > : a physical screen, describing what is actually on the screen, 
// > and a virtual screen, describing what the programmer wants to have on the screen.
#define cjtui_win_draw(win)              wnoutrefresh(win)

// copy win buf to virtual screen buf 
// and copy specific win area only to terminal
#define cjtui_win_draw_atonce(win)       wrefresh(win)

#define cjtui_win_redraw_atonce(win)     redrawwin(win)


// refresh window when wgetch is called 
// if there have been changes to itand it is not a pad
// wrefresh() is called unless win is pad
#define cjtui_win_draw_and_input_char(win)  wgetch(win)

#define cjtui_win_draw_border(win)       box(win, ACS_VLINE, ACS_HLINE)

#define cjtui_win_move(win, y, x)        wmove(w, y, x)

#define cjtui_win_cursor_x(win)          getcurx(win)
#define cjtui_win_cursor_y(win)          getcury(win)

#define cjtui_win_x(win)                 getbegx(win)
#define cjtui_win_y(win)                 getbegy(win)

#define cjtui_win_sizx(win)              getmaxx(win)
#define cjtui_win_sizy(win)              getmaxy(win)

#define cjtui_win_mvprintf               cjtui_win_mvprintf_curses
#define cjtui_win_nprintf                cjtui_win_nprintf_curses


//
// pad : offscreen window
//

// wgetch() does not draw pad window, but draw all window, panel

// A pad is a type of window that is not restricted
// > by the terminal's display size or associated 
// > with a particular part of the display. 
// Because a pad is usually larger than the physical display, 
// > only a portion of a pad is visible to the user at a given time.

#define cjtui_pad                    WINDOW


#define cjtui_pad_create(h,w)        newpad(h,w)
#define cjtui_pad_destroy(pad)       delwin(pad)

#define cjtui_pad_clear(pad)         wclear(pad)
#define cjtui_pad_draw_border(pad)   box(pad, ACS_VLINE, ACS_HLINE)

#define cjtui_pad_getyx(pad, y, x)   getyx(pad, y, x)
#define cjtui_pad_setyx(pad, y, x)   wmove(pad, y, x)

#define cjtui_pad_attrset(pad, attr) wattrset(pad, attr)

#define cjtui_pad_mvprintf           cjtui_win_mvprintf_curses
#define cjtui_pad_nprintf            cjtui_win_nprintf_curses

#define cjtui_pad_addstr(pad,str)    waddwstr(pad, str)

// copy pad buf to screen (terminal)
#define cjtui_pad_draw_atonce(pad, src_pady, src_padx, dst_scry, dst_scrx, dst_scry2, dst_scrx2) \
  prefresh(pad, src_pady, src_padx, dst_scry, dst_scrx, dst_scry2, dst_scrx2)

// if you wish to output several pads at once, 
// call pnoutrefresh for each pad and then issue one call to doupdate.
// This updates the physical screen once.
#define cjtui_pad_draw(pad, src_pady, src_padx, dst_scry, dst_scrx, dst_scry2, dst_scrx2) \
 { pnoutrefresh(pad, src_pady, src_padx, dst_scry, dst_scrx, \
        (dst_scry2) < cjtui_scr_h ? (dst_scry2) : cjtui_scr_h-1 , \
        (dst_scrx2) < cjtui_scr_w ? (dst_scrx2) : cjtui_scr_w-1 ); }


// wbkgnd(win, A_REVERSE)
// init_pair(1, COLOR_WHITE, COLOR_BLUE); wbkgnd(win, COLOR_PAIR(1))
#define cjtui_win_set_ch_attr(win, ch_type)   wbkgd(win, ch_type)


//
// Panel (window with depth support extension)
// compile: cc [flags] sourcefiles -lpanel -lncurses
//

#define cjtui_pan                               PANEL

#define cjtui_pan_create(h,w,y,x)               new_panel(newwin(h,w,y,x))
#define cjtui_pan_destroy(pan)                  {  WINDOW* __del_orig_win__=panel_window(pan); \
  del_panel(pan); \
  delwin(__del_orig_win__); }

#define cjtui_pan_set_user_prm(pan, user_prm)   set_panel_userptr(pan, user_prm)
#define cjtui_pan_get_user_prm(pan)             panel_userptr(pan)

#define cjtui_pan_win(pan)                      panel_window(pan)
#define cjtui_pan_change_win(pan, win)          replace_panel(pan, win)

#define cjtui_pan_show(pan)                     show_panel(pan)

// 1. >> removes the given panel from the panel stack <<
// 2. hides it from view.
// 3. The PANEL structure is not lost, merely removed from the stack.
#define cjtui_pan_hide(pan)                     hide_panel(pan)
#define cjtui_pan_is_hidden(pan)                pannel_hidden(pan)

// 1. refreshes the virtual screen to reflect the relations between the panels in the stack 
// 2. this funciton does not call doupdate() to refresh the physical screen
// 3. update_panels() may be called more than once before a call to doupdate(),
//    >but doupdate() is the function responsible for updating the physical screen.

#define cjtui_pan_update                        update_panels

// It does not change the position of the panel in the stack.
// Be sure to use this function, not mvwin(), to move a panel window.
#define cjtui_pan_move(pan, y, x)               move_panel(pan, y, x)

// top_panel() is intended for making an already
// visible panel move to the top of the stack
#define cjtui_pan_set_top_most(pan)             top_panel(pan)
#define cjtui_pan_set_bottom_most(pan)          bottom_panel(pan)

#define cjtui_pan_get_bottom()                  panel_above(0)
#define cjtui_pan_get_top()                     panel_below(0)

#define cjtui_pan_get_above(pan)                panel_above(pan)
#define cjtui_pan_get_below(pan)                panel_below(pan)

// https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html









#endif

