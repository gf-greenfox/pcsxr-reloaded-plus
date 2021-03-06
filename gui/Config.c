/*  Pcsx - Pc Psx Emulator
 *  Copyright (C) 1999-2002  Pcsx Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Linux.h"

/* TODO escaping/unescaping would be nice, as would maxchars */
static void GetValue(char *src, char *name, char *outvar) {
	char *tmp;

	*outvar = 0;
	tmp = strstr(src, name);
	if (tmp == NULL) return;

	tmp += strlen(name); 
	while ((*tmp == ' ') || (*tmp == '=')) tmp++;

	while (*tmp != '\n' && *tmp != 0)
		*outvar++ = *tmp++;

	*outvar = 0;
	return;
}

static long GetValuel(char *src, char *name) {
	char *tmp = strstr(src, name);
	if (tmp != NULL) {
		tmp += strlen(name);
		while ((*tmp == ' ') || (*tmp == '=')) tmp++;
		if (*tmp != '\n') return atol(tmp);
	}
	return 0;
}

static boolean GetValueb(char *src, char *name) {
	char *tmp = strstr(src, name);
	if (tmp != NULL) {
		tmp += strlen(name);
		while ((*tmp == ' ') || (*tmp == '=')) tmp++;
		if (*tmp != '\n') return (atoi(tmp) != 0);
	}
	return FALSE;
}

#define SetValue(name, var) \
	fprintf(f, "%s = %s\n", name, var);

#define SetValuel(name, var) \
	fprintf(f, "%s = %li\n", name, var);

#define SetValueb(name, var) \
	fprintf(f, "%s = %d\n", name, (var) ? 1 : 0);

int LoadConfig(PcsxConfig *Conf) {
	struct stat buf;
	FILE *f;
	int size;
	char *data;
	gchar *tmp;
	
	if (stat(cfgfile, &buf) == -1) {
		// the config file doesn't exist!
		// Try again, appending cfgfile_basename 
		tmp = g_build_filename( cfgfile_basename , cfgfile , NULL);
		if (stat(tmp , &buf) == -1){
			printf("Configuration file %s couldn't be found\n", cfgfile);
			g_free( tmp );
			return -1;
		}
		// The asked cfg file was in short form, under home/PCSXR_DOT_DIR
		g_free( cfgfile );
		cfgfile = g_strdup( tmp );
		g_free( tmp );
		// cfgfile will be freed at main();
		printf("Using configuration file %s\n", cfgfile);
		
	}

	size = buf.st_size;

	/* TODO Error checking for the next two lines, and at least log failures */
	f = fopen(cfgfile, "r");
	if (f == NULL) return -1;

	data = (char *)malloc(size + 1);
	if (data == NULL) return -1;

	fread(data, 1, buf.st_size, f);
	fclose(f);

	data[size] = '\0';

	GetValue(data, "Bios", Config.Bios);
	GetValue(data, "Gpu",  Config.Gpu);
	GetValue(data, "Spu",  Config.Spu);
	GetValue(data, "Cdr",  Config.Cdr);
#ifdef ENABLE_SIO1API
	GetValue(data, "Sio1", Config.Sio1);
#endif
	GetValue(data, "Pad1", Config.Pad1);
	GetValue(data, "Pad2", Config.Pad2);
	GetValue(data, "Net",  Config.Net);
	GetValue(data, "Mcd1", Config.Mcd1);
	GetValue(data, "Mcd2", Config.Mcd2);
	GetValue(data, "BiosDir",    Config.BiosDir);
	GetValue(data, "PluginsDir",    Config.PluginsDir);
	GetValue(data, "IsoImgDir",  Config.IsoImgDir);

	Config.Xa      = GetValueb(data, "Xa");
	Config.SioIrq  = GetValueb(data, "SioIrq");
	Config.Mdec    = GetValueb(data, "Mdec");
	Config.PsxAuto = GetValueb(data, "PsxAuto");
	Config.Cdda    = GetValuel(data, "Cdda");
	Config.SlowBoot= GetValueb(data, "SlowBoot");
	Config.Debug   = GetValueb(data, "Dbg");
	Config.PsxOut  = (Config.PsxOut || GetValueb(data, "PsxOut"));
	Config.SpuIrq  = GetValueb(data, "SpuIrq");
	Config.RCntFix = GetValueb(data, "RCntFix");
	Config.VSyncWA = GetValueb(data, "VSyncWA");
	Config.NoMemcard = GetValueb(data, "NoMemcard");
	Config.Widescreen = GetValueb(data, "Widescreen");

	Config.Cpu     = GetValuel(data, "Cpu");
	Config.PsxType = GetValuel(data, "PsxType");
	Config.RewindCount = GetValuel(data, "RewindCount");
	Config.RewindInterval = GetValuel(data, "RewindInterval");

	free(data);

	return 0;
}

void SaveConfig() {
	FILE *f;

	/* TODO Error checking for the next two lines, and at least log
	   failures - suggest a file dialog to specify a new file or
	   create a new file */
	f = fopen(cfgfile, "w");
	if (f == NULL) return;

	SetValue("Bios", Config.Bios);
	SetValue("Gpu",  Config.Gpu);
	SetValue("Spu",  Config.Spu);
	SetValue("Cdr",  Config.Cdr);
#ifdef ENABLE_SIO1API
	SetValue("Sio1", Config.Sio1);
#endif
	SetValue("Net",  Config.Net);
	SetValue("Pad1", Config.Pad1);
	SetValue("Pad2", Config.Pad2);
	SetValue("Mcd1", Config.Mcd1);
	SetValue("Mcd2", Config.Mcd2);
	SetValue("BiosDir",    Config.BiosDir);
	SetValue("PluginsDir",    Config.PluginsDir);
	SetValue("IsoImgDir",  Config.IsoImgDir);

	SetValueb("Xa",      Config.Xa);
	SetValueb("SioIrq",  Config.SioIrq);
	SetValueb("Mdec",    Config.Mdec);
	SetValueb("PsxAuto", Config.PsxAuto);
	SetValuel("Cdda",    Config.Cdda);
	SetValueb("SlowBoot",Config.SlowBoot);
	SetValueb("Dbg",     Config.Debug);
	SetValueb("PsxOut",  Config.PsxOut);
	SetValueb("SpuIrq",  Config.SpuIrq);
	SetValueb("RCntFix", Config.RCntFix);
	SetValueb("VSyncWA", Config.VSyncWA);
	SetValueb("NoMemcard", Config.NoMemcard);
	SetValueb("Widescreen", Config.Widescreen);

	SetValuel("Cpu",     Config.Cpu);
	SetValuel("PsxType", Config.PsxType);
	SetValuel("RewindCount", Config.RewindCount);
	SetValuel("RewindInterval", Config.RewindInterval);

	fclose(f);
}
