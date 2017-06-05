#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#include <vita2d.h>

enum  button_state_t {BT_NORMAL, BT_PRESSED, BT_SELECTED};

typedef void (*callback_func_t)(void);

struct ui_button_t
{
    int x;
    int y;
    unsigned int width;
    unsigned int height;
    vita2d_texture *img_normal;
    enum button_state_t state;
    callback_func_t handler;
};

void ui_buttons_init();
struct ui_button_t *button_create(unsigned char* normal,
                                  callback_func_t click_handler);

void button_draw(struct ui_button_t* button);
int button_is_point_inside(struct ui_button_t *button, int x, int y);
int button_handle_touch_down(struct ui_button_t *button, int x, int y);
int button_handle_touch_up(struct ui_button_t *button, int x, int y);
void button_reset(struct ui_button_t *button);

#endif
