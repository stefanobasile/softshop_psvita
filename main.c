/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>

#include <vita2d.h>

#include "ui_button.h"
#include "main_screen.h"
#include "fornitori_screen.h"
#include "ui_helper.h"
#include <stdlib.h>

/*
 * Symbol of the image.png file
 */
extern unsigned char _binary_image_png_start;

int printbanner(void)
{
   int i;
   for(i=979;i<990;i++)
   {
       fprintf(stdout,"%s",lngtxt(i));
       if(i==984)
           fprintf(stdout,"%s", "123");
   }
   if(getuid()==0)
       fprintf(stdout,lngtxt(990));
   fflush(stdout);
   return 0x0;
}

int main()
{
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

    ui_helper_init();
    ui_buttons_init();

    int aa = printbanner();
    printf("aa: %d\n", aa);

	/*
	 * Load the statically compiled image.png file.
	 */
    main_screen_init();
    fornitori_screen_init();


	// touch
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_BACK);

	// end of touch

    enum screen_t current_screen = MAIN_SCREEN;
	while (1)
    {
        if (current_screen == MAIN_SCREEN)
        {
            main_screen_activate(&current_screen);
        }
	}

	/*
	 * vita2d_fini() waits until the GPU has finished rendering,
	 * then we can free the assets freely.
	 */
	vita2d_fini();
    ui_helper_destroy();

	sceKernelExitProcess(0);


    long r = random() % 10;
    printf("r: %d", r);
	return 0;
}
