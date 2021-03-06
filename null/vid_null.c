/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.

This file is part of Quake 2 source code.

Quake 2 source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake 2 source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake 2 source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// vid_null.c -- null video loader to aid porting efforts
// this assumes that one of the refs is statically linked to the executable

#include "../client/client.h"

viddef_t	viddef;				// global video state

refexport_t	re;

refexport_t GetRefAPI (refimport_t rimp);

/*
==========================================================================

DIRECT LINK GLUE

==========================================================================
*/

#define	MAXPRINTMSG	8192 // was 4096
void VID_Printf (int print_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr, fmt);
//	vsprintf (msg, fmt, argptr);
	Q_vsnprintf (msg, sizeof(msg), fmt, argptr);
	va_end (argptr);

	if (print_level == PRINT_ALL)
		Com_Printf ("%s", msg);
	else
		Com_DPrintf ("%s", msg);
}

void VID_Error (int err_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr, fmt);
//	vsprintf (msg, fmt, argptr);
	Q_vsnprintf (msg, sizeof(msg), fmt, argptr);
	va_end (argptr);

	Com_Error (err_level, "%s", msg);
}

void VID_NewWindow (int width, int height)
{
	viddef.width = width;
	viddef.height = height;
}

/*
** VID_GetModeInfo
*/
typedef struct vidmode_s
{
    const char *description;
    int         width, height;
    int         mode;
} vidmode_t;

// Knightmare- added 1280x1024, 1400x1050, 856x480, 1024x480 modes
vidmode_t vid_modes[] =
{
	{ "Mode 0: 320x240",	320, 240,   0 },
	{ "Mode 1: 400x300",	400, 300,   1 },
	{ "Mode 2: 512x384",	512, 384,   2 },
	{ "Mode 3: 640x480",	640, 480,   3 },
	{ "Mode 4: 800x600",	800, 600,   4 },
	{ "Mode 5: 960x720",	960, 720,   5 },
	{ "Mode 6: 1024x768",	1024, 768,  6 },
	{ "Mode 7: 1152x864",	1152, 864,  7 },
	{ "Mode 8: 1280x960",	1280, 960,  8 },
	{ "Mode 9: 1280x1024",	1280, 1024, 9 },	// Knightmare added
	{ "Mode 10: 1400x1050", 1400, 1050, 10 },	// Knightmare added
	{ "Mode 11: 1600x1200", 1600, 1200, 11 },
	{ "Mode 12: 1920x1440",	1920, 1440, 12 },	// Knightmare added
	{ "Mode 13: 2048x1536",	2048, 1536, 13 },
	{ "Mode 14: 856x480",   856,  480,	14 },	// Knightmare added
	{ "Mode 15: 1280x720",	1280, 720,  15 },	// Knightmare added
	{ "Mode 16: 1280x768",	1280, 768,  16 },	// Knightmare added
	{ "Mode 17: 1280x800",	1280, 800,  17 },	// Knightmare added
	{ "Mode 18: 1440x900",	1440, 900,	18 },	// Knightmare added
	{ "Mode 19: 1680x1050",	1680, 1050, 19 },	// Knightmare added
	{ "Mode 20: 1920x1080",	1920, 1080, 20 },	// Knightmare added
	{ "Mode 21: 1920x1200",	1920, 1200, 21 },	// Knightmare added
	{ "Mode 22: 2560x1080",	2560, 1080, 22 },	// Knightmare added
	{ "Mode 23: 2560x1440",	2560, 1440, 23 },	// Knightmare added
	{ "Mode 24: 2560x1600",	2560, 1600, 24 }	// Knightmare added
};

#define VID_NUM_MODES ( sizeof( vid_modes ) / sizeof( vid_modes[0] ) )

qboolean VID_GetModeInfo( int *width, int *height, int mode )
{
    if ( mode < 0 || mode >= VID_NUM_MODES )
        return false;

    *width  = vid_modes[mode].width;
    *height = vid_modes[mode].height;

    return true;
}


void	VID_Init (void)
{
    refimport_t	ri;

    viddef.width = 320;
    viddef.height = 240;

    ri.Cmd_AddCommand = Cmd_AddCommand;
    ri.Cmd_RemoveCommand = Cmd_RemoveCommand;
    ri.Cmd_Argc = Cmd_Argc;
    ri.Cmd_Argv = Cmd_Argv;
    ri.Cmd_ExecuteText = Cbuf_ExecuteText;
    ri.Con_Printf = VID_Printf;
    ri.Sys_Error = VID_Error;
    ri.FS_LoadFile = FS_LoadFile;
    ri.FS_FreeFile = FS_FreeFile;
    ri.FS_Gamedir = FS_Gamedir;
	ri.Vid_NewWindow = VID_NewWindow;
    ri.Cvar_Get = Cvar_Get;
    ri.Cvar_Set = Cvar_Set;
    ri.Cvar_SetValue = Cvar_SetValue;
    ri.Vid_GetModeInfo = VID_GetModeInfo;
	ri.TextColor = CL_TextColor; // Knightmare added
	ri.SetParticlePics = CL_SetParticleImages; // Knightmare added

    re = GetRefAPI(ri);

    if (re.api_version != API_VERSION)
        Com_Error (ERR_FATAL, "Re has incompatible api_version");
    
        // call the init function
    if (re.Init (NULL, NULL) == -1)
		Com_Error (ERR_FATAL, "Couldn't start refresh");
}

void	VID_Shutdown (void)
{
    if (re.Shutdown)
	    re.Shutdown ();
}

void	VID_CheckChanges (void)
{
}

void	VID_MenuInit (void)
{
}

void	VID_MenuDraw (void)
{
}

const char *VID_MenuKey( int k)
{
	return NULL;
}
