
#include <cj/cj.h>

#include "cjtui.h"
#include "cjtui_wnd.h"


void cjtui_wnd_draw_caption(struct cjtui_wnd* wnd);



CJEXTERNC struct cjtui_wnd* 
cjtui_wnd_create(struct cjtui_wnd* parent_wnd, const cjtui_ch* caption,
	cji y, cji x, cji h, cji w, void* prm) {

	struct cjtui_wnd* wnd = cjmzalloc(sizeof(struct cjtui_wnd));

	if(caption)
		cjtui_strcpy_s(wnd->caption, CJTUI_WND_CAP_SIZ, caption);

	wnd->y = y;  	wnd->x = x;	
	wnd->h = h;		wnd->w = w;

	// relative
	wnd->cli_y = 1;  
	wnd->cli_x = 1;

	// 2: top border + bottom border
	wnd->cli_h = h - 2;  

	// 2: left border + right border
	wnd->cli_w = w - 2;

	wnd->prm = prm;

	wnd->off_frame = cjtui_wnd_off_create(h, w);
	wnd->off_cli = cjtui_wnd_off_create(wnd->cli_h, wnd->cli_w);

	wnd->wnd_flag = CJTUI_WND_WF_VISIBLE;

	wnd->dirty_flag = CJTUI_WND_DF_WND_CAPTION | CJTUI_WND_DF_WND_SIZ | \
		CJTUI_WND_DF_CLIENT;

	return wnd;
}

CJEXTERNC void cjtui_wnd_resize(struct cjtui_wnd* wnd, cji h, cji w)
{
	wnd->h = h;		
	wnd->w = w;

	// 2: top border + bottom border
	wnd->cli_h = h - 2;
	// 2: left border + right border
	wnd->cli_w = w - 2;

	cjtui_wnd_set_df(wnd, CJTUI_WND_DF_WND_SIZ | CJTUI_WND_DF_CLIENT);
}

CJEXTERNC void cjtui_wnd_move(struct cjtui_wnd* wnd, cji y, cji x)
{
	wnd->y = y;  	wnd->x = x;

	cjtui_wnd_set_df(wnd, CJTUI_WND_DF_WND_POS | CJTUI_WND_DF_CLIENT);
}



CJEXTERNC void cjtui_wnd_destroy(struct cjtui_wnd* wnd) {

	if (!wnd)
		return;

	cjtui_wnd_off_destroy(wnd->off_cli);
	cjtui_wnd_off_destroy(wnd->off_frame);

	cjmfree(wnd);

}

CJEXTERNC cjrst cjtui_wnd_proc(struct cjtui_wnd* wnd, void* prm)
{
	cjbool is_changed = cjfalse;
	
	cjbool frame_changed = cjfalse;
	cjbool client_changed = cjfalse;

	if (!wnd)
		return is_changed ? cjrstok : cjrstfail;
	
	if (wnd->dirty_flag) {

		if (wnd->dirty_flag & CJTUI_WND_DF_WND_FRAME) {
			cjtui_wnd_off_clear(wnd->off_frame);
			cjtui_wnd_off_attrset(wnd->off_frame, A_NORMAL);
			cjtui_wnd_off_draw_border(wnd->off_frame);

			cjtui_wnd_draw_caption(wnd);

			frame_changed = cjtrue;
		}

		if (wnd->dirty_flag & CJTUI_WND_DF_CLIENT) {
			
			if (wnd->on_draw_clent_area) {

				if (0 == wnd->on_draw_clent_area(wnd, prm)) {
					client_changed = cjtrue;
				}
			}
			else {
				;
			}

		}

		wnd->dirty_flag = 0;				
	}

	is_changed = frame_changed | client_changed;

	return is_changed ? cjrstok : cjrstfail;
}

CJEXTERNC void cjtui_wnd_draw(struct cjtui_wnd* wnd, void* prm) {

	if (!wnd)
		return;
	
	cjtui_wnd_off_draw(wnd->off_frame,
		0, 0, 
		wnd->y,          wnd->x, 
		wnd->y + wnd->h, wnd->x + wnd->w);

	cji cli_y = wnd->y + wnd->cli_y;
	cji cli_x = wnd->x + wnd->cli_x;

	cjtui_wnd_off_draw(wnd->off_cli,
		0,0 ,
		cli_y, 
		cli_x,
		cli_y + wnd->cli_h, 
		cli_x + wnd->cli_w);
}

void cjtui_wnd_draw_caption(struct cjtui_wnd* wnd)
{
	if (!wnd)
		return;

	cjtui_wnd_off* pad = wnd->off_frame;

	const cji wnd_border_left = 1;  // ¦£
	const cji wnd_border_right = 1; //  ¦¤

	cji oldy, oldx;
	cjtui_wnd_off_getyx(pad, oldy, oldx);

	// []
	cji wnd_cap_w_half = (wnd->w / 2) - (wnd_border_left+1);

	if (wnd_cap_w_half < 2) {
		cjtui_wnd_off_setyx(pad, oldy, oldx);
		return;
	}

	// [
	cjtui_wnd_off_setyx(pad, 0, wnd_border_left +1);

	cjtui_wnd_off_addstr(pad, CJTUI_CH(" ["));

	cji str_siz;
	str_siz = wnd->w - (wnd_border_left + wnd_border_right + 4);

	if (str_siz > 0) {
		cjtui_wnd_off_nprintf(pad, str_siz, CJTUI_CH("%s"), wnd->caption);
	}

	cjtui_wnd_off_addstr(pad, CJTUI_CH("] "));

	// restore cursor
	cjtui_wnd_off_setyx(pad, oldy, oldx);
}
