#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>

#include <vita2d.h>

#include "ui_button.h"
#include "definitions.h"
#include "main_screen.h"
#include "main_screen.h"
#include "definitions.h"
#include "ime_dialog.h"
#include "fornitori_screen.h"

extern unsigned char _binary_b1_normal_png_start;
extern unsigned char _binary_b2_normal_png_start;
extern unsigned char _binary_b3_normal_png_start;
extern unsigned char _binary_b4_normal_png_start;
extern unsigned char _binary_b5_normal_png_start;
extern unsigned char _binary_b6_normal_png_start;
extern unsigned char _binary_b7_normal_png_start;
extern unsigned char _binary_b8_normal_png_start;
extern unsigned char _binary_background_png_start;

struct ui_button_t *button1;
struct ui_button_t *button2;
struct ui_button_t *button3;
struct ui_button_t *button4;
struct ui_button_t *button5;
struct ui_button_t *button6;
struct ui_button_t *button7;
struct ui_button_t *button8;

void main_screen_handle_events(int x, int y, enum touch_t touchtype, enum screen_t *current_screen);
void main_screen_draw();

void fornitori_button_handler()
{
    enum screen_t temp_solution = MAIN_SCREEN;
    fornitori_screen_activate(&temp_solution);
}

void load_buttons()
{
    button1 = button_create(&_binary_b1_normal_png_start,
                            NULL);
    button2 = button_create(&_binary_b2_normal_png_start,
                            NULL);
    button3 = button_create(&_binary_b3_normal_png_start,
                            NULL);
    button4 = button_create(&_binary_b4_normal_png_start,
                            &fornitori_button_handler);
    button5 = button_create(&_binary_b5_normal_png_start,
                            NULL);
    button6 = button_create(&_binary_b6_normal_png_start,
                            NULL);
    button7 = button_create(&_binary_b7_normal_png_start,
                            NULL);
    button8 = button_create(&_binary_b8_normal_png_start,
                            NULL);
}

void draw_buttons()
{
        button_draw(button1);
        button_draw(button2);
        button_draw(button3);
        button_draw(button4);
        button_draw(button5);
        button_draw(button6);
        button_draw(button7);
        button_draw(button8);
}

void set_buttons_positions()
{
    int margin_x = 10;
    int gap_x = 50;
    int row1_y = 100;
    int row2_y = 300;
    int step_x = BUTTON_WIDTH + gap_x;
    int offset_x = -100;

    // row 1
    button1->x = margin_x + 1 * step_x + offset_x;
    button1->y = row1_y;
    button2->x = margin_x + 2 * step_x + offset_x;
    button2->y = row1_y;
    button3->x = margin_x + 3 * step_x + offset_x;
    button3->y = row1_y;
    button4->x = margin_x + 4 * step_x + offset_x;
    button4->y = row1_y;
    // row 2
    button5->x = margin_x + 1 * step_x + offset_x;
    button5->y = row2_y;
    button6->x = margin_x + 2 * step_x + offset_x;
    button6->y = row2_y;
    button7->x = margin_x + 3 * step_x + offset_x;
    button7->y = row2_y;
    button8->x = margin_x + 4 * step_x + offset_x;
    button8->y = row2_y;
}

void main_screen_handle_events(int x, int y, enum touch_t touchtype, enum screen_t *current_screen)
{
    if (touchtype == TOUCHUP)
    {
        button_handle_touch_up(button1, x, y);
        button_handle_touch_up(button2, x, y);
        button_handle_touch_up(button3, x, y);
        button_handle_touch_up(button4, x, y);
        button_handle_touch_up(button5, x, y);
        button_handle_touch_up(button6, x, y);
        button_handle_touch_up(button7, x, y);
        button_handle_touch_up(button8, x, y);
    }
    else if (touchtype == TOUCHDOWN)
    {
        button_handle_touch_down(button1, x, y);
        button_handle_touch_down(button2, x, y);
        button_handle_touch_down(button3, x, y);
        button_handle_touch_down(button4, x, y);
        button_handle_touch_down(button5, x, y);
        button_handle_touch_down(button6, x, y);
        button_handle_touch_down(button7, x, y);
        button_handle_touch_down(button8, x, y);
    }
    else
    {
        button_reset(button1);
        button_reset(button2);
        button_reset(button3);
        button_reset(button4);
        button_reset(button5);
        button_reset(button6);
        button_reset(button7);
        button_reset(button8);
    }
}

void main_screen_init()
{
    load_buttons(); 
    set_buttons_positions();
}

void main_screen_draw()
{
    draw_buttons();
}

void main_screen_activate(enum screen_t *current_screen)
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
	vita2d_texture *background;
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));
	background = vita2d_load_PNG_buffer(&_binary_background_png_start);
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
            main_screen_handle_events(touch_x, touch_y, TOUCHDOWN, current_screen);
        }
        if ((touch_current_num ==0) && (touch_last_num == 1))
        {
            main_screen_handle_events(touch_x, touch_y, TOUCHUP, current_screen);
        }
        if ((touch_current_num == 0) && (touch_last_num == 0))
        {
            main_screen_handle_events(touch_x, touch_y, TOUCHNO, current_screen);
        }

		sceCtrlPeekBufferPositive(0, &pad, 1);

		if (pad.buttons & SCE_CTRL_START)
			break;

		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background, 0, 0);

        main_screen_draw();

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

     //   char str[100];
   //     keyboard_vita_get(str, 100);
    //    fornitori_screen_activate(current_screen);

		vita2d_end_drawing();
		vita2d_swap_buffers();

//		rad += 0.1f;
	}
	vita2d_free_texture(background);

}
