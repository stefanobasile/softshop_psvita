#include "utils.h"

int date_format = 0, time_format = 0;

void initSceAppUtil() 
{
	// Init SceAppUtil
	SceAppUtilInitParam init_param;
	SceAppUtilBootParam boot_param;
	sceClibMemset(&init_param, 0, sizeof(SceAppUtilInitParam));
	sceClibMemset(&boot_param, 0, sizeof(SceAppUtilBootParam));
	sceAppUtilInit(&init_param, &boot_param);

	// Mount
	sceAppUtilMusicMount();
	sceAppUtilPhotoMount();
	
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_DATE_FORMAT, &date_format);
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_TIME_FORMAT, &time_format);
	
	// Set common dialog config
	SceCommonDialogConfigParam config;
	sceCommonDialogConfigParamInit(&config);
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, (int *)&config.language);
	sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, (int *)&config.enterButtonAssign);
	sceCommonDialogSetConfigParam(&config);
}

void finishSceAppUtil() 
{
	// Unmount
	sceAppUtilPhotoUmount();
	sceAppUtilMusicUmount();

	// Shutdown AppUtil
	sceAppUtilShutdown();
}

int sysmoduleLoadModule(SceUInt16 id)
{
	if (sceSysmoduleIsLoaded(id) != SCE_SYSMODULE_LOADED)
		return sceSysmoduleLoadModule(id);
	else
		return 0;
}

int sysmoduleUnloadModule(SceUInt16 id)
{
	if (sceSysmoduleIsLoaded(id) == SCE_SYSMODULE_LOADED)
		return sceSysmoduleUnloadModule(id);
	else
		return 0;
}

int controls()
{
	sceClibMemset(&pad, 0, sizeof(SceCtrlData));
	sceCtrlPeekBufferPositive(0, &pad, 1);

	pressed = pad.buttons & ~old_pad.buttons;
	
	old_pad = pad;
	return 0;
}

void* sceMalloc(size_t size)
{
	void* ret = NULL;
	
	SceUID m = sceKernelAllocMemBlock("dummy", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, ALIGN(size,0x40000), NULL);
	
	if (m >= 0) 
		sceKernelGetMemBlockBase(m, &ret);
	
	return ret;
}

void sceFree(void* addr)
{
	SceUID m = sceKernelFindMemBlockByAddr(addr, 1);
	
	if (m >= 0) 
		sceKernelFreeMemBlock(m);
}

void setBilinearFilter(int enabled, vita2d_texture * texture)
{
	if (enabled == 1)
		vita2d_texture_set_filters(texture, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
}

vita2d_texture * loadImg(const char * path, int ext)
{
	vita2d_texture *texture = NULL;
	
	if (ext == 0)
		texture = vita2d_load_JPEG_file(path);
	else if (ext == 1)
		texture = vita2d_load_PNG_file(path);
	
	setBilinearFilter(1, texture);
	
	return texture;
}

vita2d_texture * loadImgBin(const void *buffer, unsigned long buffer_size, int ext)
{
	vita2d_texture *texture = NULL;
	
	if (ext == 0)
		texture = vita2d_load_JPEG_buffer(buffer, buffer_size);
	else if (ext == 1)
		texture = vita2d_load_PNG_buffer(buffer);
	
	setBilinearFilter(1, texture);
	
	return texture;
}

void endDrawing() 
{
	vita2d_end_drawing();
	vita2d_common_dialog_update();
	vita2d_swap_buffers();
	sceDisplayWaitVblankStart();
}

void getSizeString(char *string, uint64_t size) //Thanks TheOfficialFloW
{
	double double_size = (double)size;

	int i = 0;
	static char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
	while (double_size >= 1024.0f) {
		double_size /= 1024.0f;
		i++;
	}

	sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}

void convertUtcToLocalTime(SceDateTime *time_local, SceDateTime *time_utc) 
{
	SceRtcTick tick;
	sceRtcGetTick(time_utc, &tick);
	sceRtcConvertUtcToLocalTime(&tick, &tick);
	sceRtcSetTick(time_local, &tick);	
}

void convertLocalTimeToUtc(SceDateTime *time_utc, SceDateTime *time_local) 
{
	SceRtcTick tick;
	sceRtcGetTick(time_local, &tick);
	sceRtcConvertLocalTimeToUtc(&tick, &tick);
	sceRtcSetTick(time_utc, &tick);	
}

void getTimeString(char string[16], int time_format, SceDateTime time) 
{
	SceDateTime time_local;
	convertUtcToLocalTime(&time_local, &time);

	switch(time_format) 
	{
		case SCE_SYSTEM_PARAM_TIME_FORMAT_12HR:
			snprintf(string, 16, "%02d:%02d %s", (time_local.hour > 12) ? (time_local.hour-12) : ((time_local.hour == 0) ? 12 : time_local.hour), time_local.minute, time_local.hour >= 12 ? "PM" : "AM");
			break;

		case SCE_SYSTEM_PARAM_TIME_FORMAT_24HR:
			snprintf(string, 16, "%02d:%02d", time_local.hour, time_local.minute);
			break;
	}
}

void getDateString(char string[24], int date_format, SceDateTime time) 
{
	SceDateTime time_local;
	convertUtcToLocalTime(&time_local, &time);

	switch (date_format) 
	{
		case SCE_SYSTEM_PARAM_DATE_FORMAT_YYYYMMDD:
			snprintf(string, 24, "%04d/%02d/%02d", time_local.year, time_local.month, time_local.day);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY:
			snprintf(string, 24, "%02d/%02d/%04d", time_local.day, time_local.month, time_local.year);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY:
			snprintf(string, 24, "%02d/%02d/%04d", time_local.month, time_local.day, time_local.year);
			break;
	}
}

char * getStorageInfoString(int type)
{
	uint64_t free_size = 0, max_size = 0;
	sceAppMgrGetDevInfo("ux0:", &max_size, &free_size);
	
	static char free_size_string[16], max_size_string[16];
	getSizeString(free_size_string, free_size);
	getSizeString(max_size_string, max_size);
	
	if (type == 0)
		return max_size_string;
	else 
		return free_size_string;
}

double getStorageInfoDouble(int type)
{
	char * str = getStorageInfoString(type);
	double storage;

	sscanf(str, "%lf", &storage);

	return storage;
}

/*
*	printf with width (95)
*/
void printfw(char * text, int x, int y, unsigned int color)
{
	vita2d_pvf_draw_text(font, (x * 2 * 95) / 100, y * 2, color, 1.2f, text);
}

int is_point_in_rect(int px, int py, int rx, int ry, int rwidth, int rheight)
{
    int ret = 0;
    if ((rx <= px) && (px <= (rx + rwidth)) &&
        (ry <= py) && (py <= (ry + rheight)))
    {
        ret = 1;
    }
    return ret;
}
