#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>

#include <vita2d.h>

#include "ui_button.h"
#include "definitions.h"
#include "fornitori_screen.h"
#include "fornitori_screen.h"
#include "definitions.h"
#include "ime_dialog.h"
#include "ui_editbox.h"

extern unsigned char _binary_background_png_start;
vita2d_texture *background;

static struct ui_editbox_t *ui_hello_editbox = NULL;

static void fornitori_screen_handle_events(int x, int y, enum touch_t touchtype, enum screen_t *current_screen);
static void fornitori_screen_draw();

static void load_buttons()
{
}

static void draw_buttons()
{
}

static void draw_editboxes()
{
    editbox_draw(ui_hello_editbox);
}

static void set_buttons_positions()
{
}

static void set_editboxes_positions()
{
}

static void fornitori_screen_handle_events(int x, int y, enum touch_t touchtype, enum screen_t *current_screen)
{
    if (touchtype == TOUCHUP)
    {
    }
    else if (touchtype == TOUCHDOWN)
    {
    }
    else
    {
    }
}

void fornitori_screen_init()
{
	background = vita2d_load_PNG_buffer(&_binary_background_png_start);
    load_buttons(); 
    set_buttons_positions();
    set_editboxes_positions();

    ui_hello_editbox = editbox_create("Hello World");
}

static void fornitori_screen_draw()
{
    vita2d_draw_texture(background, 0, 0);
    draw_buttons();
    draw_editboxes();
}

void fornitori_screen_activate(enum screen_t *current_screen)
{
	int port = 0;
	SceTouchData touch_old[SCE_TOUCH_PORT_MAX_NUM];
	SceTouchData touch[SCE_TOUCH_PORT_MAX_NUM];
	SceTouchPanelInfo 	pPanelInfo;	
	sceTouchGetPanelInfo(0, &pPanelInfo);
    int touch_max_x = pPanelInfo.maxDispX;
    int touch_max_y = pPanelInfo.maxDispY;
    int touch_x = 0;
    int touch_y = 0;
    int touch_last_num = 0;
    int touch_current_num = 0;
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));
	while (1)
    {
		memcpy(touch_old, touch, sizeof(touch_old));
        touch_last_num = touch_current_num;
		sceTouchPeek(port, &touch[port], 1);
        touch_current_num = touch[0].reportNum;
        if (touch[0].report[0].x != 0)
        {
            touch_x = (int)(((double)touch[0].report[0].x / touch_max_x) * SCREEN_WIDTH);
        }
        else
        {
            touch_x = 0;
        }
        if (touch[0].report[0].y != 0)
        {
            touch_y = (int)(((double)touch[0].report[0].y / touch_max_y) * SCREEN_HEIGHT);
        }
        else
        {
            touch_y = 0;
        }

        if (touch_current_num == 1)
        {
            fornitori_screen_handle_events(touch_x, touch_y, TOUCHDOWN, current_screen);
        }
        if ((touch_current_num ==0) && (touch_last_num == 1))
        {
            fornitori_screen_handle_events(touch_x, touch_y, TOUCHUP, current_screen);
        }
        if ((touch_current_num == 0) && (touch_last_num == 0))
        {
            fornitori_screen_handle_events(touch_x, touch_y, TOUCHNO, current_screen);
        }

		sceCtrlPeekBufferPositive(0, &pad, 1);

		if (pad.buttons & SCE_CTRL_START)
			break;

		vita2d_start_drawing();
		vita2d_clear_screen();


        fornitori_screen_draw();

/*
		vita2d_draw_fill_circle(touch[0].report[0].x,
					touch[0].report[0].y,
					100, RGBA8(0, 255, 0 ,255));
		vita2d_draw_fill_circle(touch_x,
					touch_y,
					100, RGBA8(0, 255, 255 ,255));

//		vita2d_draw_line(500, 30, 800, 300, RGBA8(255, 0, 255, 255));

        char temp_str[30];
        snprintf(temp_str, sizeof(temp_str), "x: %d, y: %d", touch_x, touch_y);
        vita2d_pgf_draw_text(pgf, 0, 100, RGBA8(0, 255, 0, 255), 1.0f,
                            temp_str);
        snprintf(temp_str, sizeof(temp_str), "tx: %d, ty: %d",
                touch[0].report[0].x, touch[0].report[0].y);
        vita2d_pgf_draw_text(pgf, 0, 150, RGBA8(0, 255, 0, 255), 1.0f,
                            temp_str);
        snprintf(temp_str, sizeof(temp_str), "maxX: %d, maxY: %d",
                touch_max_x, touch_max_y);
        vita2d_pgf_draw_text(pgf, 0, 200, RGBA8(0, 255, 0, 255), 1.0f,
                            temp_str);
        snprintf(temp_str, sizeof(temp_str), "reportnum: %d",
                touch[0].reportNum);
        vita2d_pgf_draw_text(pgf, 0, 250, RGBA8(0, 255, 0, 255), 1.0f,
                            temp_str);
*/
//        vita2d_pvf_draw_text(pgf, 100, 200, RGBA8(0, 255, 0, 255), 1.0f, "hello");

//		vita2d_pgf_draw_text(pgf, 700, 30, RGBA8(0,255,0,255), 1.0f, "PGF Font sample!");

//		vita2d_pvf_draw_text(pvf, 700, 80, RGBA8(0,255,0,255), 1.0f, "PVF Font sample!");

        char str[100];
        keyboard_vita_get(str, 100);
        break;

		vita2d_end_drawing();
		vita2d_swap_buffers();

//		rad += 0.1f;
	}
	vita2d_free_texture(background);

}
