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

/*
 * This SettingsManager class holds the getters and setters needed to deal with the database
 * in which all OctoPkg configuration is saved and retrieved (~/.config/octoxbps/)
 */

#include "settingsmanager.h"
#include "unixcommand.h"
#include "uihelper.h"

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSettings>
#include <QDir>

//Initialization of Singleton pointer...
SettingsManager* SettingsManager::m_pinstance = 0;

SettingsManager::SettingsManager(){
  m_SYSsettings = new QSettings(QSettings::UserScope, ctn_ORGANIZATION, ctn_APPLICATION);
}

SettingsManager::~SettingsManager(){  
  delete m_SYSsettings;
}

// Class singleton
SettingsManager* SettingsManager::instance(){
  if (m_pinstance == 0)
  {
    m_pinstance = new SettingsManager();
  }

  return m_pinstance;
}

//CacheCleaner related --------------------------------------------------------------
int SettingsManager::getKeepNumInstalledPackages() {
  return instance()->getSYSsettings()->value(ctn_KEEP_NUM_INSTALLED, 3).toInt();
}

int SettingsManager::getKeepNumUninstalledPackages() {
  return instance()->getSYSsettings()->value(ctn_KEEP_NUM_UNINSTALLED, 1).toInt();
}
//CacheCleaner related --------------------------------------------------------------


//Notifier related ------------------------------------------------------------------

int SettingsManager::getSyncDbHour()
{
  SettingsManager p_instance;
  int h = p_instance.getSYSsettings()->value(ctn_KEY_SYNC_DB_HOUR, -1).toInt();

  if (h != -1)
  {
    if (h < 0)
    {
      h = 0;
      p_instance.getSYSsettings()->setValue(ctn_KEY_SYNC_DB_HOUR, h);
      p_instance.getSYSsettings()->sync();
    }
    else if (h > 23)
    {
      h = 23;
      p_instance.getSYSsettings()->setValue(ctn_KEY_SYNC_DB_HOUR, h);
      p_instance.getSYSsettings()->sync();
    }
  }

  return h;
}

//The syncDb interval is in MINUTES and it cannot be less than 5!
int SettingsManager::getSyncDbInterval()
{
  SettingsManager p_instance;
  int n = p_instance.getSYSsettings()->value(ctn_KEY_SYNC_DB_INTERVAL, -1).toInt();

  if ((n != -1) && (n < 5))
  {
    n = 5;
    p_instance.getSYSsettings()->setValue(ctn_KEY_SYNC_DB_INTERVAL, n);
    p_instance.getSYSsettings()->sync();
  }
  else if (n > 44640)
  {
    n = 44640; //This is 1 month, the maximum allowed!
    p_instance.getSYSsettings()->setValue(ctn_KEY_SYNC_DB_INTERVAL, n);
    p_instance.getSYSsettings()->sync();
  }

  return n;
}

QDateTime SettingsManager::getLastSyncDbTime()
{
  if (!instance()->getSYSsettings()->contains(ctn_KEY_LAST_SYNC_DB_TIME))
  {
    return QDateTime();
  }
  else
  {
    SettingsManager p_instance;
    return (p_instance.getSYSsettings()->value( ctn_KEY_LAST_SYNC_DB_TIME, 0)).toDateTime();
  }
}

void SettingsManager::setSyncDbHour(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEY_SYNC_DB_HOUR, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setSyncDbInterval(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEY_SYNC_DB_INTERVAL, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setLastSyncDbTime(QDateTime newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEY_LAST_SYNC_DB_TIME, newValue);
  instance()->getSYSsettings()->sync();
}

//Notifier related ------------------------------------------------------------------


//OctoPkg related --------------------------------------------------------------------
int SettingsManager::getCurrentTabIndex(){
  return instance()->getSYSsettings()->value(
        ctn_KEY_CURRENT_TAB_INDEX, 0).toInt();
}

int SettingsManager::getPanelOrganizing(){
  return instance()->getSYSsettings()->value( ctn_KEY_PANEL_ORGANIZING, ectn_NORMAL ).toInt();
}

int SettingsManager::getPackageListOrderedCol(){
  return instance()->getSYSsettings()->value( ctn_KEY_PACKAGE_LIST_ORDERED_COL, 1 ).toInt();
}

int SettingsManager::getPackageListSortOrder(){
  return instance()->getSYSsettings()->value(
        ctn_KEY_PACKAGE_LIST_SORT_ORDER, Qt::AscendingOrder ).toInt();
}

int SettingsManager::getPackageIconColumnWidth()
{
  return instance()->getSYSsettings()->value(
        ctn_KEY_PACKAGE_ICON_COLUMN_WIDTH, 40).toInt();
}

int SettingsManager::getPackageNameColumnWidth()
{
  return instance()->getSYSsettings()->value(
        ctn_KEY_PACKAGE_NAME_COLUMN_WIDTH, 450).toInt();
}

int SettingsManager::getPackageVersionColumnWidth()
{
  return instance()->getSYSsettings()->value(
        ctn_KEY_PACKAGE_VERSION_COLUMN_WIDTH, 250).toInt();
}

int SettingsManager::getPackageInstalledSizeColumnWidth()
{
  return instance()->getSYSsettings()->value(
        ctn_KEY_PACKAGE_INSTALLED_SIZE_COLUMN_WIDTH, 100).toInt();
}

int SettingsManager::getPackageViewMode()
{
  return instance()->getSYSsettings()->value(
        ctn_KEY_PACKAGE_VIEW_MODE, 0).toInt();
}

QString SettingsManager::getVoid_RSS_URL()
{
  if (!instance()->getSYSsettings()->contains(ctn_KEY_VOID_RSS_URL)){
    instance()->getSYSsettings()->setValue(ctn_KEY_VOID_RSS_URL, ctn_DEFAULT_VOID_RSS_URL);
  }

  return instance()->getSYSsettings()->value(
        ctn_KEY_VOID_RSS_URL, ctn_DEFAULT_VOID_RSS_URL).toString();
}

bool SettingsManager::getSkipMirrorCheckAtStartup(){
  if (!instance()->getSYSsettings()->contains(ctn_KEY_SKIP_MIRRORCHECK_ON_STARTUP)){
    instance()->getSYSsettings()->setValue(ctn_KEY_SKIP_MIRRORCHECK_ON_STARTUP, 0);
  }

  return (instance()->getSYSsettings()->value( ctn_KEY_SKIP_MIRRORCHECK_ON_STARTUP, false).toInt() == 1);
}

bool SettingsManager::getShowGroupsPanel()
{
  if (!instance()->getSYSsettings()->contains(ctn_KEY_SHOW_GROUPS_PANEL)){
    instance()->getSYSsettings()->setValue(ctn_KEY_SHOW_GROUPS_PANEL, 1);
  }

  return (instance()->getSYSsettings()->value( ctn_KEY_SHOW_GROUPS_PANEL, false).toInt() == 1);
}

QString SettingsManager::getTerminal(){
  if (!instance()->getSYSsettings()->contains(ctn_KEY_TERMINAL))
  {
    instance()->getSYSsettings()->setValue(ctn_KEY_TERMINAL, ctn_AUTOMATIC);
    return ctn_AUTOMATIC;
  }
  else
  {
    SettingsManager p_instance;
    return (p_instance.getSYSsettings()->value( ctn_KEY_TERMINAL, ctn_AUTOMATIC)).toString();
  }
}

QByteArray SettingsManager::getWindowSize(){
  return (instance()->getSYSsettings()->value( ctn_KEY_WINDOW_SIZE, 0).toByteArray());
}

QByteArray SettingsManager::getTransactionWindowSize()
{
  return (instance()->getSYSsettings()->value( ctn_KEY_TRANSACTION_WINDOW_SIZE, 0).toByteArray());
}

QByteArray SettingsManager::getSplitterHorizontalState(){
  return (instance()->getSYSsettings()->value( ctn_KEY_SPLITTER_HORIZONTAL_STATE, 0).toByteArray());
}

void SettingsManager::setCurrentTabIndex(int newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_CURRENT_TAB_INDEX, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPackageListOrderedCol(int newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_PACKAGE_LIST_ORDERED_COL, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPackageListSortOrder(int newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_PACKAGE_LIST_SORT_ORDER, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setShowGroupsPanel(int newValue)
{
  instance()->getSYSsettings()->setValue( ctn_KEY_SHOW_GROUPS_PANEL, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPanelOrganizing(int newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_PANEL_ORGANIZING, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setWindowSize(QByteArray newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_WINDOW_SIZE, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setTransactionWindowSize(QByteArray newValue)
{
  instance()->getSYSsettings()->setValue( ctn_KEY_TRANSACTION_WINDOW_SIZE, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setSplitterHorizontalState(QByteArray newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_SPLITTER_HORIZONTAL_STATE, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setTerminal(QString newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_TERMINAL, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setKeepNumInstalledPackages(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEEP_NUM_INSTALLED, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setKeepNumUninstalledPackages(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEEP_NUM_UNINSTALLED, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPackageIconColumnWidth(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEY_PACKAGE_ICON_COLUMN_WIDTH, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPackageNameColumnWidth(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEY_PACKAGE_NAME_COLUMN_WIDTH, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPackageVersionColumnWidth(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEY_PACKAGE_VERSION_COLUMN_WIDTH, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPackageInstalledSizeColumnWidth(int newValue)
{
  instance()->getSYSsettings()->setValue(ctn_KEY_PACKAGE_INSTALLED_SIZE_COLUMN_WIDTH, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setPackageViewMode(int newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_PACKAGE_VIEW_MODE, newValue);
  instance()->getSYSsettings()->sync();
}

void SettingsManager::setVoid_RSS_URL(QString newValue){
  instance()->getSYSsettings()->setValue( ctn_KEY_VOID_RSS_URL, newValue);
  instance()->getSYSsettings()->sync();
}

/*
 * Search all supported terminals to see if the selected one is valid
 */
bool SettingsManager::isValidTerminalSelected()
{
  QString userTerminal = getTerminal();
  if (userTerminal == ctn_AUTOMATIC)
    return true;

  if (userTerminal == ctn_XFCE_TERMINAL ||
      userTerminal == ctn_LXDE_TERMINAL ||
      userTerminal == ctn_LXQT_TERMINAL ||
      userTerminal == ctn_KDE_TERMINAL ||
      userTerminal == ctn_TDE_TERMINAL ||
      userTerminal == ctn_CINNAMON_TERMINAL ||
      userTerminal == ctn_MATE_TERMINAL ||
      userTerminal == ctn_RXVT_TERMINAL ||
      userTerminal == ctn_XTERM)
  {
    if (UnixCommand::hasTheExecutable(userTerminal))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

//OctoPkg related --------------------------------------------------------------------
