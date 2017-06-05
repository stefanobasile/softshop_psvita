#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include <vita2d.h>
#include <vitasdk.h>

#define ALIGN(x, a)	(((x) + ((a) - 1)) & ~((a) - 1))

SceCtrlData pad, old_pad;
unsigned int pressed;

vita2d_pvf *font;

extern int date_format, time_format;

void initSceAppUtil();
void finishSceAppUtil();
int sysmoduleLoadModule(SceUInt16 id);
int sysmoduleUnloadModule(SceUInt16 id);
int controls();
void* sceMalloc(size_t size);
void sceFree(void* addr);
void setBilinearFilter(int enabled, vita2d_texture * texture);
vita2d_texture * loadImg(const char * path, int ext);
vita2d_texture * loadImgBin(const void *buffer, unsigned long buffer_size, int ext);
void endDrawing();
void getTimeString(char string[16], int time_format, SceDateTime time);
void getDateString(char string[24], int date_format, SceDateTime time);
void getSizeString(char string[16], uint64_t size);
double getStorageInfoDouble(int type);

/*
*	printf with width (95)
*/
void printfw(char * text, int x, int y, unsigned int color);

int is_point_in_rect(int px, int py, int rx, int ry, int rwidth, int rheight);

#endif
