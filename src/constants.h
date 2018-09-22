/*
* This file is part of OctoXBPS, an open-source GUI for pacman.
* Copyright (C) 2015 Alexandre Albuquerque Arnt
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QDir>

/*
 * Collection of OctoXBPS constants and enums
 */

//MainWindow related
enum SystemUpgradeOptions { ectn_NO_OPT, ectn_SYNC_DATABASE_OPT, ectn_NOCONFIRM_OPT };

//UnixCommand related
const QString ctn_MIRROR_CHECK_APP("mirror-check");

enum CommandExecuting { ectn_NONE, ectn_MIRROR_CHECK, ectn_SYNC_DATABASE,
                        ectn_SYSTEM_UPGRADE, ectn_INSTALL, ectn_REMOVE,
                        ectn_REMOVE_INSTALL, ectn_REMOVE_KCP_PKG, ectn_CLEAN_CACHE,
                        ectn_RUN_SYSTEM_UPGRADE_IN_TERMINAL, ectn_RUN_IN_TERMINAL, ectn_LOCAL_PKG_REFRESH };

enum LinuxDistro { ectn_VOID, ectn_UNKNOWN };

enum Language { ectn_LANG_ENGLISH, ectn_LANG_USER_DEFINED };


//PacmanExec - QTextBrowser related
enum TreatString { ectn_TREAT_STRING, ectn_DONT_TREAT_STRING };
enum TreatURLLinks { ectn_TREAT_URL_LINK, ectn_DONT_TREAT_URL_LINK };

//SettingsManager related
enum SaveSettingsReason { ectn_PackageList, ectn_CurrentTabIndex, ectn_NORMAL=30,
                          ectn_MAXIMIZE_PACKAGES=40, ectn_MAXIMIZE_PROPERTIES=50, ectn_GROUPS=5 };

const QString ctn_ORGANIZATION("octoxbps");
const QString ctn_APPLICATION("octoxbps");
const QString ctn_KEY_CURRENT_TAB_INDEX("Current_Tab_Index");
const QString ctn_KEY_WINDOW_SIZE("Window_Size");
const QString ctn_KEY_TRANSACTION_WINDOW_SIZE("Transaction_Window_Size");
const QString ctn_KEY_PANEL_ORGANIZING("Panel_Organizing");
const QString ctn_KEY_PACKAGE_LIST_ORDERED_COL("PackageList_Ordered_Col");
const QString ctn_KEY_PACKAGE_LIST_SORT_ORDER("PackageList_Sort_Order");
const QString ctn_KEY_AUR_PACKAGE_LIST_ORDERED_COL("Aur_PackageList_Ordered_Col");
const QString ctn_KEY_AUR_PACKAGE_LIST_SORT_ORDER("Aur_PackageList_Sort_Order");
const QString ctn_KEY_SKIP_MIRRORCHECK_ON_STARTUP("Skip_Mirror_Check_At_Startup");
const QString ctn_KEY_SPLITTER_HORIZONTAL_STATE("Splitter_Horizontal_State");
const QString ctn_KEY_SHOW_GROUPS_PANEL("Show_Groups_Panel");
const QString ctn_KEY_PACKAGE_ICON_COLUMN_WIDTH("Package_Icon_Column_Width");
const QString ctn_KEY_PACKAGE_NAME_COLUMN_WIDTH("Package_Name_Column_Width");
const QString ctn_KEY_PACKAGE_VERSION_COLUMN_WIDTH("Package_Version_Column_Width");
const QString ctn_KEY_PACKAGE_INSTALLED_SIZE_COLUMN_WIDTH("Package_Installed_Size_Column_Width");
const QString ctn_KEY_PACKAGE_VIEW_MODE("Package_View_Mode");
const QString ctn_KEY_TERMINAL("Terminal");
const QString ctn_AUTOMATIC("automatic");
const QString ctn_KEY_VOID_RSS_URL("Void_RSS_URL");
const QString ctn_DEFAULT_VOID_RSS_URL("https://voidlinux.org/atom.xml");
//const QString ctn_DEFAULT_VOID_RSS_URL("https://github.com/void-linux/void-packages/commits/master.atom");

//Notifier related
const QString ctn_KEY_LAST_SYNC_DB_TIME("LastSyncDbTime");
const QString ctn_KEY_SYNC_DB_INTERVAL("SyncDbInterval");
const QString ctn_KEY_SYNC_DB_HOUR("SyncDbHour");

//CacheCleaner related
const QString ctn_KEEP_NUM_INSTALLED("Keep_Num_Installed");
const QString ctn_KEEP_NUM_UNINSTALLED("Keep_Num_Uninstalled");

//Package related
const QString ctn_TEMP_ACTIONS_FILE ( QDir::tempPath() + QDir::separator() + ".qt_temp_" );
const QString ctn_XBPS_DATABASE_DIR = "/var/db/xbps";
const QString ctn_PACMAN_CORE_DB_FILE = "/var/lib/pacman/sync/core.db";

enum PackageStatus { ectn_INSTALLED, ectn_NON_INSTALLED, ectn_OUTDATED, ectn_NEWER,
                     ectn_FOREIGN, ectn_FOREIGN_OUTDATED };

enum ViewOptions { ectn_ALL_PKGS, ectn_INSTALLED_PKGS, ectn_NON_INSTALLED_PKGS };

//TransactionDialog related
const int ctn_RUN_IN_TERMINAL(328);

//WMHelper related
const QString ctn_NO_SU_COMMAND("none");
const QString ctn_ROOT_SH("/bin/sh -c ");

const QString ctn_LXQTSU("lxqt-sudo");
const QString ctn_QSUDO("qsudo");
const QString ctn_KDESU("kdesu");
const QString ctn_KDE_DESKTOP("kwin");
const QString ctn_KDE_X11_DESKTOP("kwin_x11");
const QString ctn_KDE_WAYLAND_DESKTOP("kwin_wayland");
const QString ctn_KDE_EDITOR("kwrite");
const QString ctn_KDE_FILE_MANAGER("kfmclient");
const QString ctn_KDE_TERMINAL("konsole");
const QString ctn_KDE4_OPEN("kde-open");
const QString ctn_KDE5_OPEN("kde-open5");
const QString ctn_KDE4_FILE_MANAGER("dolphin");
const QString ctn_KDE4_EDITOR("kate");

const QString ctn_TDESU("tdesu");
const QString ctn_TDE_DESKTOP("twin");
const QString ctn_TDE_EDITOR("kwrite");
const QString ctn_TDE_FILE_MANAGER("kfmclient");
const QString ctn_TDE_TERMINAL("konsole");

const QString ctn_GKSU_1("/usr/libexec/gksu");
const QString ctn_GKSU_2("gksu");

const QString ctn_ANTERGOS_FILE_MANAGER("nautilus");

const QString ctn_ARCHBANG_EDITOR("medit");
const QString ctn_ARCHBANG_FILE_MANAGER("spacefm");

const QString ctn_MOOOS_EDITOR("subl3");
const QString ctn_RXVT_TERMINAL("urxvt");

const QString ctn_XFCE_DESKTOP("xfdesktop");
const QString ctn_XFCE_EDITOR("mousepad");
const QString ctn_XFCE_EDITOR_ALT("leafpad");
const QString ctn_XFCE_FILE_MANAGER("thunar");
const QString ctn_XFCE_TERMINAL("xfce4-terminal");

const QString ctn_OPENBOX_DESKTOP("openbox");
const QString ctn_LXDE_DESKTOP("lxsession");
const QString ctn_LXDE_TERMINAL("lxterminal");
const QString ctn_LXDE_FILE_MANAGER("pcmanfm");

const QString ctn_LXQT_DESKTOP("lxqt-session");
const QString ctn_LXQT_TERMINAL("qterminal");
const QString ctn_LXQT_FILE_MANAGER("pcmanfm-qt");
const QString ctn_LXQT_EDITOR("juffed");

const QString ctn_MATE_DESKTOP("mate-session");
const QString ctn_MATE_EDITOR("mate-open");
const QString ctn_MATE_FILE_MANAGER("caja");
const QString ctn_MATE_TERMINAL("mate-terminal");

const QString ctn_CINNAMON_DESKTOP("cinnamon-session");
const QString ctn_CINNAMON_EDITOR("gedit");
const QString ctn_CINNAMON_FILE_MANAGER("nemo");
const QString ctn_CINNAMON_TERMINAL("gnome-terminal");

const QString ctn_RAZORQT_DESKTOP("razor-session");
const QString ctn_PEK_TERMINAL("sakura");
const QString ctn_XTERM("xterm");

enum EditOptions { ectn_EDIT_AS_ROOT, ectn_EDIT_AS_NORMAL_USER };


//OctoXBPS-notifier related  -------------------------------------------------------------------------------

const QString ctn_PKEXEC_BINARY = "/usr/bin/pkexec";

const QString ctn_PACMANHELPER_BINARY = "/usr/lib/octoxbps/pacmanhelper";

const QString ctn_DBUS_PACMANHELPER_SERVICE = "/usr/share/dbus-1/system-services/org.octoxbps.pacmanhelper.service";

enum ExecOpt { ectn_NORMAL_EXEC_OPT, ectn_SYSUPGRADE_EXEC_OPT,
               ectn_SYSUPGRADE_NOCONFIRM_EXEC_OPT };

#endif // CONSTANTS
