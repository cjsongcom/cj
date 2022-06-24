
#ifndef _CJTUI_MENU_WORKER_H_
#define _CJTUI_MENU_WORKER_H_

//
// cjtui menu worker
//

// allocate curses win, calc menu width and height
CJEXTERNC cjbool cjtui_menu_worker_init_curses_win(struct cjtui_menu* menu, void* ctx);

struct cjtui_menu_worker_draw_pad_ctx {
    struct cjtui_menu_ctx* menu_ctx;
    cji draw_x;
    cji draw_y;
};

CJEXTERNC cjbool cjtui_menu_worker_draw_pad(struct cjtui_menu* menu, void* _ctx);

struct cjtui_menu_worker_find_opened_top_menu_ctx {
    struct cjtui_menu* opened_top_menu;
};

CJEXTERNC cjbool cjtui_menu_worker_find_opened_top_menu(struct cjtui_menu* menu, void* _ctx);



#endif
