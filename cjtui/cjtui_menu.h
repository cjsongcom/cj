
#ifndef _CJTUI_MENU_H_
#define _CJTUI_MENU_H_


//
// cjtui menu ctx
//

CJEXTERNC typedef cjrst (*cjtui_menu_ctx_fn)(struct cjtui_menu_ctx* ctx, void* prm);

struct cjtui_menu_ctx {
    cjtui_menu_ctx_fn        on_erase_bkgnd;
    cjtui_menu_ctx_fn        on_after_draw_menu;
};


//
// cjtui menu
//

#define CJTUI_MENU_CAP_SIZ               64

CJEXTERNC typedef cjbool(*cjtui_menu_iter_worker_fn)(struct cjtui_menu* item, void* ctx);
CJEXTERNC typedef cjrst(*cjtui_menu_item_fn)(struct cjtui_menu* item, struct cjtui_menu_ctx* ctx);


#define CJTUI_MENU_TYPE_MENU              0x0001
#define CJTUI_MENU_TYPE_ITEM              0x0002

#define CJTUI_MENU_TYPE_ROOT_MENU         0x1000
#define CJTUI_MENU_TYPE_SUB_MENU          0x2000

#define CJTUI_MENU_FL_SHOW                0x0001

#define CJTUI_MENU_IS_MENU(m)             ((m->type_flag & CJTUI_MENU_TYPE_MENU) ? cjtrue : cjfalse)
#define CJTUI_MENU_IS_ITEM(m)             ((m->type_flag & CJTUI_MENU_TYPE_ITEM) ? cjtrue : cjfalse)


#define CJTUI_MENU_BORDER_SIZ             1

#define CJTUI_MENU_SUB_MENU_RENDER_STR    CJTUI_CH(" ->")


struct cjtui_menu {
    cjtui_win* win;  // off screen pad

    cjuw                    type_flag;
    cjuw                    wnd_flag;

    cjtui_ch                caption[CJTUI_MENU_CAP_SIZ];
    void* prm;

    cji x, y;  // window left, top
    cji w, h;

    struct cjtui_menu* item_prev; // always null,  if fl_type is menu_type
    struct cjtui_menu* item_next;

    // can be submenu or subitem
    struct cjtui_menu* sub;

    // parent menu which contains item
    // parent item for menu, cjNULL: root menu
    struct cjtui_menu* parent;

    cjtui_menu_item_fn on_exec;
    cjtui_menu_item_fn on_destroy;

    cjtui_menu_item_fn on_open_menu;
    cjtui_menu_item_fn on_close_menu;

    // menu_oper
    cjbool             menu_opened;
    struct cjtui_menu* item_selected;

};


CJEXTERNC struct cjtui_menu*
cjtui_menu_create(struct cjtui_menu* par_item, cji y, cji x, cji h, cji w,
    cjtui_ch* caption, void* prm);

//void cjtui_menu_destroy(struct cjtui_menu* menu);

CJEXTERNC cjbool cjtui_menu_recur_iterate_first_work(struct cjtui_menu* menu,
    cjtui_menu_iter_worker_fn worker, void* worker_ctx);

CJEXTERN struct cjtui_menu* cjtui_menu_get_opened_top_menu(struct cjtui_menu* root_menu);

CJEXTERN void cjtui_menu_get_yx(struct cjtui_menu* item, cji* y, cji* x);
CJEXTERNC void cjtui_menu_calc_yx(struct cjtui_menu* menu, cji* y, cji* x);
CJEXTERNC void cjtui_menu_calc_menu_hw(struct cjtui_menu* menu, cjbool do_resiz_win, cji* h, cji* w);
CJEXTERNC cjbool cjtui_menu_fit_menu_hw(struct cjtui_menu* menu, cji h, cji w);


CJEXTERN cjrst cjtui_menu_proc(struct cjtui_menu** rst_desired_item,
    struct cjtui_menu* root_menu, struct cjtui_menu_ctx* menu_ctx);

CJEXTERN void cjtui_menu_draw(struct cjtui_menu* menu, 
    struct cjtui_menu_ctx* menu_ctx,  cji scr_y, cji scr_x);



//
// item
//

CJEXTERNC struct cjtui_menu*
cjtui_menu_add_item(struct cjtui_menu* menu, cjtui_ch* caption, void* prm);

// return next menu item
CJEXTERN struct cjtui_menu* cjtui_menu_get_last_item(struct cjtui_menu* item);
CJEXTERN cji cjtui_menu_get_item_row(struct cjtui_menu* item);


//
// menu/item handler
//

CJEXTERN cjbool cjtui_menu_open_menu(struct cjtui_menu* menu, cjbool is_force,
    struct cjtui_menu_ctx* menu_ctx);
CJEXTERNC cjbool cjtui_menu_del_menu(struct cjtui_menu* menu, 
    struct cjtui_menu_ctx* menu_ctx);

CJEXTERNC struct cjtui_menu* cjtui_menu_del_item(struct cjtui_menu* item, 
    struct cjtui_menu_ctx* menu_ctx);


#endif

