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

#ifndef __LINUX_H__
#define __LINUX_H__

#include "config.h"

#include "../libpcsxcore/psxcommon.h"
#include <gtk/gtk.h>

#include "Cheat.h"

#define DEFAULT_MEM_CARD_1 "/.pcsxr/memcards/card1.mcd"
#define DEFAULT_MEM_CARD_2 "/.pcsxr/memcards/card2.mcd"
#define MEMCARD_DIR "/.pcsxr/memcards/"
#define PLUGINS_DIR "/.pcsxr/plugins/"
#define PLUGINS_CFG_DIR "/.pcsxr/plugins/cfg/"
#define PCSXR_DOT_DIR "/.pcsxr/"
#define BIOS_DIR "/.pcsxr/bios/"
#define STATES_DIR "/.pcsxr/sstates/"
#define CHEATS_DIR "/.pcsxr/cheats/"
#define PATCHES_DIR "/.pcsxr/patches/"

extern gboolean UseGui;
extern int StatesC;
gchar *cfgfile;
gchar *cfgfile_basename;
// Note that we really should have this handled in the main(). no need to be global, if LoadConfig()/SaveConvig() is fixed.

int LoadConfig();
void SaveConfig();

void StartGui();

void autoloadCheats(); // try load cheat file from default path and filename

void PADhandleKey(int key);

int UpdateMenuSlots();

gchar* get_state_filename(int i);
gchar* get_cdrom_label_id(const gchar* suffix); // get cdrom label and id and append suffix to string
gchar* get_cdrom_label_trim(); // trim cdrom label out of whitespaces

void state_save(gchar *state_filename);
void state_load(gchar *state_filename);

int match(const char* string, char* pattern);
int plugins_configured();

void UpdatePluginsBIOS();

void SysErrorMessage(gchar *primary, gchar *secondary);
void SysInfoMessage(gchar *primary, gchar *secondary);

#endif /* __LINUX_H__ */
