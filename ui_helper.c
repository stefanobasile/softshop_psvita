#include "ui_helper.h"
#include <stdlib.h>

static struct ui_helper_t *ui_helper = NULL;

void ui_helper_init()
{
    if (ui_helper != NULL)
    {
        free(ui_helper);
        ui_helper = NULL;
    }
    ui_helper = malloc(sizeof(struct ui_helper_t));
	ui_helper->pgf = vita2d_load_default_pgf();
	ui_helper->pvf = vita2d_load_default_pvf();
}


vita2d_pgf* ui_helper_get_pgf()
{
    vita2d_pgf *ret = NULL;
    if (ui_helper != NULL)
    {
        ret = ui_helper->pgf;
    }
    return ret;
}

vita2d_pvf* ui_helper_get_pvf()
{
    vita2d_pvf *ret = NULL;
    if (ui_helper != NULL)
    {
        ret = ui_helper->pvf;
    }
    return ret;
}

void ui_helper_destroy()
{
    if (ui_helper != NULL)
    {
        if (ui_helper->pgf != NULL)
        {
            vita2d_free_pgf(ui_helper->pgf);
            ui_helper->pgf = NULL;
        }
        if (ui_helper->pvf != NULL)
        {
            vita2d_free_pvf(ui_helper->pvf);
            ui_helper->pvf = NULL;
        }
    free(ui_helper);
    ui_helper = NULL;
    }
}
