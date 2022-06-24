
#include <cj/cj.h>

#include "cjtui.h"
#include "cjtui_menu.h"


static struct cjtui_menu* _cjtui_menu_del_item_from_link(struct cjtui_menu* del_item) {

    if (!del_item || !CJTUI_MENU_IS_ITEM(del_item))
        return cjNULL;

    struct cjtui_menu* item_next = del_item->item_next;

    if (del_item->item_prev)
        del_item->item_prev->item_next = item_next;

    if (item_next)
        item_next->item_prev = del_item->item_prev;

    return item_next;
}

static void _cjtui_menu_add_item_to_link(struct cjtui_menu* item,
    struct cjtui_menu* new_item) {

    if (!item || !CJTUI_MENU_IS_ITEM(item))
        return;

    while (item) {
        if (!item->item_next)
            break;

        item = item->item_next;
    }

    item->item_next = new_item;
    new_item->item_prev = item;
}

// return next item
CJEXTERNC struct cjtui_menu* cjtui_menu_del_item(struct cjtui_menu* item,
    struct cjtui_menu_ctx* menu_ctx) {

    if (!item)
        return cjNULL;

    if (item->sub) {

        if (CJTUI_MENU_IS_MENU(item->sub)) {

            if (!cjtui_menu_del_menu(item->sub, menu_ctx))
                return cjNULL;
        }
        else {            
            cjtui_menu_del_item(item->sub, menu_ctx);  // single item
        }

        item->sub = cjNULL;
    }

    cjassert(CJTUI_MENU_IS_ITEM(item));

    if (item->on_destroy)
        item->on_destroy(item, menu_ctx);

    struct cjtui_menu* item_next
        = _cjtui_menu_del_item_from_link(item);

    cjmfree(item);

    return item_next;
}

CJEXTERNC struct cjtui_menu*
cjtui_menu_add_item(struct cjtui_menu* menu, cjtui_ch* caption, void* prm) {

    if (!menu)
        return cjNULL;

    struct cjtui_menu* item
        = cjmzalloc(sizeof(struct cjtui_menu));

    item->type_flag |= CJTUI_MENU_TYPE_ITEM;
    item->parent = menu;

    cjwstrcpy_s(item->caption, CJTUI_MENU_CAP_SIZ, caption);

    item->prm = prm;

    if (!menu->item_next) {
        menu->item_next = item;
    }
    else {
        _cjtui_menu_add_item_to_link(menu->item_next, item);
    }

    return item;
}

CJEXTERN struct cjtui_menu* cjtui_menu_get_last_item(struct cjtui_menu* item)
{
    if (!item)
        return cjNULL;

    if (!CJTUI_MENU_IS_ITEM(item))
        return cjNULL;

    struct cjtui_menu* cur = item;

    cur = item->item_next;

    while (cur) {

        if (!cur->item_next)
            return cur;

        cur = cur->item_next;
    }

    return cjNULL;
}

CJEXTERN cji cjtui_menu_get_item_row(struct cjtui_menu* item)
{
    if (!item)
        return -1;

    if (!CJTUI_MENU_IS_ITEM(item))
        return -1;

    cji row = 0;
    struct cjtui_menu* cur = item->parent->item_next;

    while (cur) {

        if (cur == item)
            return row;

        cur = cur->item_next;
        row++;
    }

    return -1;
}

