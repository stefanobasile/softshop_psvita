#ifndef _UI_HELPER_H_
#define _UI_HELPER_H_

#include <vita2d.h>

struct ui_helper_t
{
    vita2d_pgf *pgf;
    vita2d_pvf *pvf;
};

void ui_helper_init();
vita2d_pgf* ui_helper_get_pgf();
vita2d_pvf* ui_helper_get_pvf();
void ui_helper_destroy();

#endif
