
#include <cj/cj.h>

#include "cjtui.h"
#include "cjtui_menu.h"


// ret: false: no change
static cjrst _sub_input_proc_menu(struct cjtui_menu** rst_item, cji key, 
    struct cjtui_menu* opened_menu, struct cjtui_menu_ctx* menu_ctx)
{
    struct cjtui_menu* desire_item = cjNULL;
    
    if (!opened_menu)
        return cjrstfail;

    cjbool is_changed = cjtrue;

    struct cjtui_menu* sel_item 
        = opened_menu->item_selected;

    switch (key)
    {
    case KEY_EXIT:
    case 27: // escape under cbreak();
    {
        if (opened_menu->on_close_menu)
            opened_menu->on_close_menu(opened_menu, menu_ctx);

        opened_menu->menu_opened = cjfalse;

        return cjrstok;
    }
    break;

    case KEY_ENTER:
    case 13: // 13, enter key in cbreak();
    {
        if (sel_item) {
            if (sel_item->sub) {
                cjtui_menu_open_menu(sel_item->sub, cjfalse, menu_ctx);

                return cjrstok;
            }
            else {
                if (sel_item->on_exec) {
                    if (sel_item->on_exec(sel_item, menu_ctx) == cjrstok)
                        desire_item = sel_item;

                }
                else {
                    desire_item = sel_item;
                }

                if (desire_item) {
                    *rst_item = desire_item;

                    return cjrstok;
                }
            }
        }
    }
    break;

    case KEY_DOWN:
    {
        if (sel_item)
            sel_item = sel_item->item_next;

        if (!sel_item)
            sel_item = opened_menu->item_next;

        opened_menu->item_selected = sel_item;
    }
    break;

    case KEY_UP:
    {
        if (sel_item)
            sel_item = sel_item->item_prev;

        if (!sel_item)
            sel_item = cjtui_menu_get_last_item(opened_menu->item_next);

        opened_menu->item_selected = sel_item;
    }
    break;

    default:
    {
        is_changed = cjfalse;
    }
    break;

    }

    return is_changed ? cjrstok : cjrstfail;
}


CJEXTERNC cjrst cjtui_menu_proc(
    struct cjtui_menu** rst_desired_item,
    struct cjtui_menu* root_menu,
    struct cjtui_menu_ctx* menu_ctx)
{
    struct cjtui_menu* desire_item = cjNULL;

    cjbool is_changed = cjfalse;

    struct cjtui_menu* opened_menu
       = cjtui_menu_get_opened_top_menu(root_menu);

    if (!opened_menu)
        return is_changed ? cjrstok : cjrstfail;

    cji key = 0;

    cjtui_cursor(0);
    cjtui_echo(0);
    cjtui_keypad(root_menu->win, 1);

    cbreak();  // change one char input mode from line input
    raw();     // disable interpret signal key
    nodelay(root_menu->win, 1); // no blocking input
    key = wgetch(root_menu->win); 

    if (cjrstok == _sub_input_proc_menu(&desire_item, key, opened_menu, menu_ctx)) {
        is_changed = cjtrue;

       if (desire_item) {           
            goto exit_menu;
       }
    }

    // compare virtual screen with terminal window and update terminal
    // doupdate();
    
exit_menu:
    cjtui_keypad(root_menu->win, 0);
    noraw();
    nocbreak();
    nodelay(root_menu->win, 0);

    if (rst_desired_item)
        *rst_desired_item = desire_item;

    return is_changed ? cjrstok : cjrstfail;
}


