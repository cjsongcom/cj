
#ifndef _CJTUI_WND_H_
#define _CJTUI_WND_H_

#ifdef CJTUI_USE_CURSES

#else
#error "impl not yet"
#endif



//
// cjcurses off screen api mapping
//

#define cjtui_wnd_off_clear                 cjtui_pad_clear
#define cjtui_wnd_off_attrset               cjtui_pad_attrset
#define cjtui_wnd_off_draw_border           cjtui_pad_draw_border

#define cjtui_wnd_off_create                cjtui_pad_create
#define cjtui_wnd_off_destroy               cjtui_pad_destroy

#define cjtui_wnd_off_draw                  cjtui_pad_draw

#define cjtui_wnd_off                       cjtui_pad

#define cjtui_wnd_off_getyx                 cjtui_pad_getyx
#define cjtui_wnd_off_setyx                 cjtui_pad_setyx

#define cjtui_wnd_off_addstr                cjtui_pad_addstr

#define cjtui_wnd_off_nprintf               cjtui_win_nprintf_curses
#define cjtui_wnd_off_printf                cjtui_win_printf_curses


#define CJTUI_WND_CAP_SIZ               128 


CJEXTERNC typedef cjrst (*cjtui_wnd_update_fn)(struct cjtui_wnd* wnd, void* ctx);
CJEXTERNC typedef cjrst (*cjtui_wnd_draw_client_fn)(struct cjtui_wnd* wnd, void* ctx);



//
// cjtui wnd flag
//

#define CJTUI_WND_WF_VISIBLE                0x00000001

//
// cjtui wnd dirty flag
//

#define CJTUI_WND_DF_CHILD_WND              0x00000001
#define CJTUI_WND_DF_PARENT_WND             0x00000002

#define CJTUI_WND_DF_WND_POS                0x00000100
#define CJTUI_WND_DF_WND_SIZ                0x00000200
#define CJTUI_WND_DF_WND_CAPTION            0x00000400
#define CJTUI_WND_DF_WND_SHOW_HIDE          0x00000800

#define CJTUI_WND_DF_WND_FRAME              0x0000ffff


// client area
#define CJTUI_WND_DF_CLIENT                 0x00010000


struct cjtui_wnd {
    cjtui_pad*                      off_frame;
    cjtui_pad*                      off_cli;

    cjtui_ch                        caption[CJTUI_WND_CAP_SIZ];

    void* prm;

    cji x, y;               // window left, top
    cji w, h;

    // client area
    cji cli_x, cli_y;
    cji cli_w, cli_h;

    cjui wnd_flag;
    cjui dirty_flag;

    struct cjtui_wnd*               parent;

    struct cjtui_wnd*               child_next;
    struct cjtui_wnd*               child_prev;

    cjtui_wnd_update_fn             on_update;
    cjtui_wnd_draw_client_fn        on_draw_clent_area;
};

CJEXTERNC struct cjtui_wnd* cjtui_wnd_create(struct cjtui_wnd* parent_wnd, 
    const cjtui_ch* caption, cji y, cji x, cji h, cji w, void* prm);

CJEXTERNC void cjtui_wnd_destroy(struct cjtui_wnd* wnd);

CJEXTERNC cjrst cjtui_wnd_proc(struct cjtui_wnd* wnd, void* prm);

CJEXTERNC void cjtui_wnd_draw(struct cjtui_wnd* wnd, void* prm);

inline  cjtui_wnd_off* cjtui_wnd_get_client_off(struct cjtui_wnd* wnd) {
    return wnd->off_cli;
}

inline void cjtui_wnd_set_df(struct cjtui_wnd* wnd, cjui flag) {
    wnd->dirty_flag |= flag;
}

CJEXTERNC void cjtui_wnd_resize(struct cjtui_wnd* wnd, cji h, cji w);
CJEXTERNC void cjtui_wnd_move(struct cjtui_wnd* wnd, cji y, cji x);


#endif
