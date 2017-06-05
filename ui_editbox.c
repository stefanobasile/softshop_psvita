#include "ui_editbox.h"
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "ime_dialog.h"
#include "ui_helper.h"

static const int EDITBOX_DEFAULT_WIDTH = 100;
static const int EDITBOX_DEFAULT_HEIGHT = 30;

void ui_editboxes_init()
{
}

struct ui_editbox_t *editbox_create(char *default_text)
{
    struct ui_editbox_t *new_editbox;
    new_editbox = malloc(sizeof(struct ui_editbox_t));
    new_editbox->x = 0;
    new_editbox->y = 0;
    new_editbox->width = EDITBOX_DEFAULT_WIDTH;
    new_editbox->height = EDITBOX_DEFAULT_HEIGHT;
    new_editbox->state = EB_NORMAL;
    if (default_text != NULL)
    {
        strcpy(new_editbox->text, default_text);
    }
    else
    {
        strcpy(new_editbox->text, "");
    }
    return new_editbox;
}

char *ui_editbox_gettext(struct ui_editbox_t *editbox)
{
    static char return_str[MAX_EDITOBOX_TEXT_LENGTH];
    if (editbox != NULL)
    {
        strcpy(return_str, editbox->text);
    }
    else
    {
        strcpy(return_str, "");
    }
    return return_str;
}

void ui_editbox_destroy(struct ui_editbox_t *editbox)
{
    if (editbox != NULL)
    {
        free(editbox);
        editbox = NULL;
    }
}

int editbox_is_point_inside(struct ui_editbox_t *editbox, int x, int y)
{
    int ret;
    ret =  is_point_in_rect(x, y,
                           editbox->x, editbox->y,
                           editbox->width, editbox->height);
    return ret;
}

int editbox_handle_touch_up(struct ui_editbox_t *editbox, int x, int y)
{
    int ret = 0;
    if (editbox_is_point_inside(editbox, x, y))
    {
        editbox->state = EB_SELECTED;
        ret = 1;
        // call the keyboard
        keyboard_vita_get(editbox->text, MAX_EDITOBOX_TEXT_LENGTH);
    }
    else
    {
        editbox->state = EB_NORMAL;
    }
    return ret;
}

int editbox_handle_touch_down(struct ui_editbox_t *editbox, int x, int y)
{
    int ret = 0;
    if (editbox_is_point_inside(editbox, x, y))
    {
        editbox->state = EB_NORMAL;
        ret = 1;
    }
    else
    {
        editbox->state = EB_NORMAL;
    }
    return ret;
}

void editbox_draw(struct ui_editbox_t* editbox)
{
    if (editbox == NULL)
    {
        return;
    }
    if (editbox->state == EB_NORMAL)
    {
    vita2d_draw_rectangle(editbox->x, editbox->y,
                          editbox->width, editbox->height,
                          RGBA8(255, 255, 255, 255));
    }
    else if (editbox->state == EB_SELECTED)
    {
    vita2d_draw_rectangle(editbox->x, editbox->y,
                          editbox->width, editbox->height,
                          RGBA8(255, 100, 0, 255));
    }
    vita2d_pgf *pgf = ui_helper_get_pgf();
    if (pgf != NULL)
    {
        vita2d_pgf_draw_text(pgf, editbox->x, editbox->y, RGBA8(0, 0, 0, 255),
                             1.0f,
                             editbox->text);
    }
}
