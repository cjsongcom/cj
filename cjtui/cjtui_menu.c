
#include <cj/cj.h>

#include "cjtui.h"
#include "cjtui_menu.h"


CJEXTERNC struct cjtui_menu*
cjtui_menu_create(struct cjtui_menu* par_item, cji y, cji x, cji h, cji w,
    cjtui_ch* caption, void* prm)
{
    if (par_item && !CJTUI_MENU_IS_ITEM(par_item))
        return cjNULL;

    struct cjtui_menu* menu;

    menu = (struct cjtui_menu*)cjmzalloc(sizeof(struct cjtui_menu));

    menu->parent = par_item;

    if (par_item)
        par_item->sub = menu;

    menu->type_flag |= CJTUI_MENU_TYPE_MENU;
    menu->type_flag |= par_item ? CJTUI_MENU_TYPE_SUB_MENU : CJTUI_MENU_TYPE_ROOT_MENU;

    menu->x = x;
    menu->y = y;

    menu->w = w;
    menu->h = h;

    cjwstrcpy_s(menu->caption, CJTUI_MENU_CAP_SIZ, caption);
    menu->prm = prm;

    if (par_item)
        par_item->sub = menu;

    return menu;
}

static void _recur_cjtui_menu_calc_xy(struct cjtui_menu* menu, cji* y, cji* x)
{
    if (!menu)
        return;

    cji wx = 0;
    cji wy = 0;

    _recur_cjtui_menu_calc_xy(menu->parent, &wy, &wx);

    *y += menu->y + wy;
    *x += menu->x + wx;
}

CJEXTERNC void cjtui_menu_calc_yx(struct cjtui_menu* menu, cji* y, cji* x)
{
    if (!menu)
        return;

    if (!CJTUI_MENU_IS_MENU(menu))
        return;

    cjassert(menu->win);

    _recur_cjtui_menu_calc_xy(menu->parent, y, x);

    *y += menu->y;
    *x += menu->x;
}

CJEXTERNC void cjtui_menu_calc_menu_hw(struct cjtui_menu* menu, cjbool do_resiz_win, 
    cji* h, cji* w)
{
    if (!menu)
        return;

    if (!CJTUI_MENU_IS_MENU(menu))
        return;

    struct cjtui_menu* cur_item = menu->item_next;

    const int menu_margin_w = 6; // L(3)+R(3)
    const int menu_margin_h = 2; // T(1)+B(1)

    cji cw = menu_margin_w;
    cji ch = menu_margin_h;

    while (cur_item) {

        cji len = menu_margin_w + cjtui_strlen(cur_item->caption);

        if (cur_item->sub)
            len += cjtui_strlen(CJTUI_MENU_SUB_MENU_RENDER_STR);

        if (cw < len)
            cw = len;

        ch += 1;

        cur_item = cur_item->item_next;
    }

    if (h) *h = ch;
    if (w) *w = cw;

    if (do_resiz_win)
        cjtui_menu_fit_menu_hw(menu, ch, cw);
}

CJEXTERNC cjbool cjtui_menu_fit_menu_hw(struct cjtui_menu* menu, cji h, cji w)
{
    if (!menu || !menu->win)
        return cjfalse;

    return wresize(menu->win, h, w) == OK ? cjtrue : cjfalse;
}

//
//void cjtui_menu_destroy(struct cjtui_menu* menu) {
//
//    struct cjtui_menu* menu;
//
//    struct cjtui_menu_item* cur_item = 
//        menu->item_next;
//
//    while (cur_item) {
//        cur_item = cjtui_menu_destroy_item(cur_item);
//    }
//
//    cjmfree(menu);
//}
//

CJEXTERN struct cjtui_menu* cjtui_menu_get_opened_top_menu(
    struct cjtui_menu* root_menu)
{
    if (!root_menu)
        return cjNULL;

    struct cjtui_menu_worker_find_opened_top_menu_ctx worker_ctx = { 0 };

    if (!cjtui_menu_recur_iterate_first_work(root_menu,
        cjtui_menu_worker_find_opened_top_menu, &worker_ctx)) {
        return cjNULL;
    }

    return worker_ctx.opened_top_menu;
}

CJEXTERN void cjtui_menu_draw(struct cjtui_menu* menu, struct cjtui_menu_ctx* menu_ctx,
    cji scr_y, cji scr_x)
{
    if (!menu)
        return;

    struct cjtui_menu_worker_draw_pad_ctx worker_ctx = {
        .menu_ctx = menu_ctx,
        .draw_x = scr_x,
        .draw_y = scr_y
    };

    if (menu_ctx->on_erase_bkgnd) {
        menu_ctx->on_erase_bkgnd(menu_ctx, 0);

    } 
    
    //else {
    //    wclear(stdscr);
    //    // copy stdscr to virtual screen
    //    wnoutrefresh(stdscr);
    //}

    cjtui_menu_recur_iterate_first_work(menu, cjtui_menu_worker_draw_pad, &worker_ctx);

    if (menu_ctx->on_after_draw_menu)
        menu_ctx->on_after_draw_menu(menu_ctx, 0);
}

static void cjtui_menu_recur_get_yx(struct cjtui_menu* menu,
    cji* y, cji* x, cji margin_y, cji margin_x)
{
    if (!menu)
        return;

    cji cy = 0, cx = 0;

    if (CJTUI_MENU_IS_MENU(menu)) {
        // menu
        // parent: meun-item
        if (menu->parent) {
            cjtui_menu_recur_get_yx(menu->parent, &cy, &cx, margin_y, margin_x);
        }

        cy += menu->y;
        cx += menu->x;

        if (menu->parent) {
            cy += margin_y;
            cx += margin_x;
        }

    }
    else if (CJTUI_MENU_IS_ITEM(menu)) {
        // menu-item
        // parent: menu

        cji item_row = cjtui_menu_get_item_row(menu);

        if (item_row == -1)
        {
            cjassert(0 && "invalid menu item");
            return;
        }

        cjtui_menu_recur_get_yx(menu->parent, &cy, &cx, margin_y, margin_x);

        cy += menu->y + item_row + margin_y; // menu border
        cx += menu->x + margin_x; // 1, left space

    }
    else {
        cjassert(0 && "invalid menu attr");
    }

    *y = cy;
    *x = cx;
}

CJEXTERN void cjtui_menu_get_yx(struct cjtui_menu* menu, cji* y, cji* x) {

    if (!menu) {
        if (x) *x = 0;
        if (y) *y = 0;

        return;
    }

    cjtui_menu_recur_get_yx(menu, y, x, CJTUI_MENU_BORDER_SIZ, CJTUI_MENU_BORDER_SIZ);
}



CJEXTERN cjbool cjtui_menu_open_menu(struct cjtui_menu* menu, cjbool is_force,
    struct cjtui_menu_ctx* menu_ctx)
{
    if (!menu)
        return cjfalse;

    if (!CJTUI_MENU_IS_MENU(menu))
        return cjfalse;

    if (!is_force && menu->menu_opened)
        return cjtrue;

    if (menu->on_open_menu)
        menu->on_open_menu(menu, menu_ctx);

    menu->menu_opened = cjtrue;

    cjtui_menu_calc_menu_hw(menu, cjtrue, cjNULL, cjNULL);

    menu->item_selected = menu->item_next;

    return cjtrue;
}

CJEXTERNC cjbool cjtui_menu_del_menu(struct cjtui_menu* menu,
    struct cjtui_menu_ctx* menu_ctx) {

    if (!menu || !CJTUI_MENU_IS_MENU(menu))
        return cjfalse;

    cjassert(menu->sub == cjNULL);

    if (menu->on_destroy)
        menu->on_destroy(menu, menu_ctx);

    struct cjtui_menu* cur = menu->item_next;

    while (cur) {
        cjassert(CJTUI_MENU_IS_ITEM(cur));
        cur = cjtui_menu_del_item(cur, menu_ctx);

        if (cur)
            cur = cur->item_next;
    }

    cjmfree(menu);

    return cjtrue;
}
