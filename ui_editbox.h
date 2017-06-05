#ifndef _UI_EDITBOX_H_
#define _UI_EDITBOX_H_

enum editbox_state_t {EB_NORMAL, EB_SELECTED};

enum { MAX_EDITOBOX_TEXT_LENGTH = 1024};

struct ui_editbox_t
{
    int x;
    int y;
    unsigned int width;
    unsigned int height;
    enum editbox_state_t state;
    char text[MAX_EDITOBOX_TEXT_LENGTH];
};

void ui_editboxes_init();

struct ui_editbox_t *editbox_create(char *default_text);
char *ui_editbox_gettext(struct ui_editbox_t *editbox);
void ui_editbox_destroy(struct ui_editbox_t *editbox);
void editbox_draw(struct ui_editbox_t* editbox);

#endif
