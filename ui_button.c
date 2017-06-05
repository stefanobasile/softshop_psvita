#include "ui_button.h"
#include <stdlib.h>
#include <string.h>
#include "definitions.h"
#include "utils.h"


extern unsigned char _binary_pressed_png_start;
extern unsigned char _binary_selected_png_start;

static vita2d_texture *button_pressed;
static vita2d_texture *button_selected;

void ui_buttons_init()
{
	button_pressed = vita2d_load_PNG_buffer(&_binary_pressed_png_start);
	button_selected = vita2d_load_PNG_buffer(&_binary_selected_png_start);
}

struct ui_button_t *button_create(unsigned char* normal,
                                  callback_func_t click_handler)
{
    struct ui_button_t *new_button;
    new_button = malloc(sizeof(struct ui_button_t));
    memset(new_button, 0, sizeof(struct ui_button_t));

	new_button->img_normal = vita2d_load_PNG_buffer(normal);

    new_button->width = BUTTON_WIDTH;
    new_button->height = BUTTON_HEIGHT;
    new_button->state = BT_NORMAL;
    new_button->handler = click_handler;
    return new_button;
}

void button_draw(struct ui_button_t* button)
{
    if (button == NULL)
    {
        return;
    }
    vita2d_draw_texture(button->img_normal, button->x, button->y);
    if (button->state == BT_PRESSED)
    {
        vita2d_draw_texture(button_pressed, button->x, button->y);
    }
    else if (button->state == BT_SELECTED)
    {
        vita2d_draw_texture(button_selected, button->x, button->y);
    }
}

int button_is_point_inside(struct ui_button_t *button, int x, int y)
{
    int ret;
    ret = is_point_in_rect(x, y,
                           button->x, button->y,
                           button->width, button->height);
    return ret;
}

int button_handle_touch_up(struct ui_button_t *button, int x, int y)
{
    int ret = 0;
    if (button_is_point_inside(button, x, y))
    {
        button->state = BT_PRESSED;
        ret = 1;
        if (button->handler != NULL)
        {
            button->handler();
        }
    }
    else
    {
        button->state = BT_NORMAL;
    }
    return ret;
}

int button_handle_touch_down(struct ui_button_t *button, int x, int y)
{
    int ret = 0;
    if (button_is_point_inside(button, x, y))
    {
        button->state = BT_SELECTED;
        ret = 1;
    }
    else
    {
        button->state = BT_NORMAL;
    }
    return ret;
}

void button_reset(struct ui_button_t *button)
{
    button->state = BT_NORMAL;
}
