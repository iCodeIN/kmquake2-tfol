#include <stdio.h>
#include "../renderer/r_local.h"
#include "glw_unix.h"

glwstate_t glw_state;

int mx, my;

// FIXME -flibit

int GLimp_Init(void *hinstance, void *wndproc)
{
}

void GLimp_Shutdown(void)
{
}

void GLimp_BeginFrame(float camera_seperation)
{
}

void GLimp_EndFrame(void)
{
}

int GLimp_SetMode(int *pwidth, int *pheight, int mode, dispType_t fullscreen)
{
	return 0;
}

void UpdateGammaRamp(void)
{
}

char *Sys_GetClipboardData(void)
{
	return NULL;
}

void IN_Activate(qboolean active)
{
}

void HandleEvents(void)
{
}
