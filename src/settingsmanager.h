/*
* This file is part of OctoXBPS, an open-source GUI for XBPS.
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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QVariant>
#include <QSettings>
#include <QDir>

class SettingsManager
{

  private:

    static SettingsManager *m_pinstance;
    QSettings *m_SYSsettings;

    SettingsManager(const SettingsManager&);
    SettingsManager& operator= (const SettingsManager&);
    SettingsManager();
    virtual ~SettingsManager();
    QSettings* getSYSsettings() { return m_SYSsettings; }

  public:

    static SettingsManager* instance();
    static QString getTerminal();

    static QString getOctoXBPSConfPath()
    {
      return QDir::homePath() +
          QDir::separator() + ".config/octoxbps/octoxbps.conf";
    }

    static int getCurrentTabIndex();
    static int getPanelOrganizing();
    static int getPackageListOrderedCol();
    static int getPackageListSortOrder();
    static int getPackageIconColumnWidth();
    static int getPackageNameColumnWidth();
    static int getPackageVersionColumnWidth();
    static int getPackageInstalledSizeColumnWidth();
    static int getPackageViewMode();
    static QString getVoid_RSS_URL();

    //Notifier related
    static int getSyncDbHour();
    static int getSyncDbInterval();
    static QDateTime getLastSyncDbTime();

    static bool getSkipMirrorCheckAtStartup();
    static bool getShowGroupsPanel();
    static QByteArray getWindowSize();
    static QByteArray getTransactionWindowSize();
    static QByteArray getSplitterHorizontalState();
    static bool isValidTerminalSelected();

    //CacheCleaner related
    static int getKeepNumInstalledPackages();
    static int getKeepNumUninstalledPackages();

    static void setCurrentTabIndex(int newValue);
    static void setPanelOrganizing(int newValue);
    static void setPackageListOrderedCol(int newValue);
    static void setPackageListSortOrder(int newValue);
    static void setShowGroupsPanel(int newValue);
    static void setWindowSize(QByteArray newValue);
    static void setTransactionWindowSize(QByteArray newValue);
    static void setSplitterHorizontalState(QByteArray newValue);

    static void setSyncDbHour(int newValue);
    static void setSyncDbInterval(int newValue);
    static void setLastSyncDbTime(QDateTime newValue);

    static void setTerminal(QString newValue);
    static void setKeepNumInstalledPackages(int newValue);
    static void setKeepNumUninstalledPackages(int newValue);

    static void setPackageIconColumnWidth(int newValue);
    static void setPackageNameColumnWidth(int newValue);
    static void setPackageVersionColumnWidth(int newValue);
    static void setPackageInstalledSizeColumnWidth(int newValue);
    static void setPackageViewMode(int newValue);
    static void setVoid_RSS_URL(QString newValue);
};

#endif // SETTINGSMANAGER_H
