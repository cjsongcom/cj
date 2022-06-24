
#include <cj/cj.h>

#include "cjtui.h"
#include "cjtui_menu.h"


CJEXTERNC cjbool cjtui_menu_recur_iterate_first_work(struct cjtui_menu* menu,
    cjtui_menu_iter_worker_fn worker, void* worker_ctx)
{
    if (!menu)
        return cjfalse;

    if (!worker(menu, worker_ctx))
        return cjfalse;

    if (menu->sub) {

        if (!worker(menu->sub, worker_ctx))
            return cjfalse;

        if (!cjtui_menu_recur_iterate_first_work(menu->sub, worker, worker_ctx))
            return cjfalse;
    }

    struct cjtui_menu* cur_item = menu->item_next;

    while (cur_item)
    {
        if (cur_item->sub) {
            if (!worker(cur_item->sub, worker_ctx))
                return cjfalse;

            if (!cjtui_menu_recur_iterate_first_work(cur_item->sub, worker, worker_ctx))
                return cjfalse;
        }

        if (!worker(cur_item, worker_ctx))
            return cjfalse;

        cur_item = cur_item->item_next;
    }

    return cjtrue;
}


//
// cjtui menu worker
//

CJEXTERNC cjbool cjtui_menu_worker_init_curses_win(struct cjtui_menu* menu, void* _worker_ctx)
{
    //    wprintf(L"%s\n", item->caption);        
    if (CJTUI_MENU_IS_MENU(menu)) {
        if (!menu->win) {
            cji h, w;

            cjtui_menu_calc_menu_hw(menu, cjfalse, &h, &w);
            menu->win = cjtui_pad_create(h, w);

            menu->h = h;
            menu->w = w;
        }
    }
    else if (CJTUI_MENU_IS_ITEM(menu)) {
        cjassert(!menu->win);
    }

    return cjtrue;
}

CJEXTERNC cjbool cjtui_menu_worker_find_opened_top_menu(struct cjtui_menu* menu, void* _worker_ctx)
{
    struct cjtui_menu_worker_find_opened_top_menu_ctx* worker_ctx =
        (struct cjtui_menu_worker_find_opened_top_menu_ctx*)_worker_ctx;

    if (CJTUI_MENU_IS_MENU(menu)) {

        if (menu->menu_opened)
            worker_ctx->opened_top_menu = menu;

    }

    return cjtrue;

}

CJEXTERNC cjbool cjtui_menu_worker_draw_pad(struct cjtui_menu* menu, void* _worker_ctx)
{
    if (!menu)
        return cjtrue; // continue work

    struct cjtui_menu_worker_draw_pad_ctx* worker_ctx =
        (struct cjtui_menu_worker_draw_pad_ctx*)_worker_ctx;

    if (CJTUI_MENU_IS_MENU(menu)) {

        if (!menu->menu_opened)
            return cjtrue; // continue next

        struct cjtui_menu* cur_item = menu->item_next;
        cji y = 1;

        cji par_x = 0;
        cji par_y = 0;

        if (menu->parent) {
            if (CJTUI_MENU_IS_ITEM(menu->parent)) {
                cjtui_menu_get_yx(menu->parent, &par_y, &par_x);
                par_y++;
                par_x++;
            }
        }

        cjtui_win_clear(menu->win);

        wattrset(menu->win, A_NORMAL);

        cjtui_win_draw_border(menu->win);

        while (cur_item) {

            if (menu->item_selected == cur_item)
                wattrset(menu->win, A_REVERSE);
            else
                wattrset(menu->win, A_NORMAL);

            if (cur_item->sub) {
                cjtui_win_mvprintf_curses(menu->win, y, 2, CJTUI_CH("%s") CJTUI_MENU_SUB_MENU_RENDER_STR, cur_item->caption);
            }
            else {
                cjtui_win_mvprintf_curses(menu->win, y, 2, CJTUI_CH("%s"), cur_item->caption);
            }
            y++;

            cur_item = cur_item->item_next;
        }

        wattrset(menu->win, A_NORMAL);

        // do not call wnoutrefresh(menu->win)
        cjtui_pad_draw(
            menu->win,
            0, 0,
            worker_ctx->draw_y + par_y + menu->y,
            worker_ctx->draw_x + par_x + menu->x,
            worker_ctx->draw_y + par_y + menu->y + menu->h,
            worker_ctx->draw_x + par_x + menu->x + menu->w);

    }
    else if (CJTUI_MENU_IS_ITEM(menu)) {
        cjassert(!menu->win);
    }

    return cjtrue;
}


