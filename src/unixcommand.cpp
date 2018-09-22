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

#include "unixcommand.h"
#include "strconstants.h"
#include "wmhelper.h"
#include "terminal.h"
#include <iostream>

#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QTextStream>
#include <QtNetwork/QNetworkInterface>
#include <QRegularExpression>
#include <QDebug>

/*
 * Collection of methods to execute many Unix commands
 */

QFile *UnixCommand::m_temporaryFile = 0;

/*
 * Executes given command and returns the StandardError Output.
 */
QString UnixCommand::runCommand(const QString& commandToRun)
{
  QProcess proc;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.remove("LANG");
  env.remove("LC_MESSAGES");
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  proc.setProcessEnvironment(env);
  proc.start(commandToRun);
  proc.waitForStarted();
  proc.waitForFinished(-1);

  QString res = proc.readAllStandardError();
  proc.close();

  return res;
}

/*
 * Executes the CURL command and returns the StandardError Output, if result code <> 0.
 */
QString UnixCommand::runCurlCommand(const QString& commandToRun){
  QProcess proc;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  proc.setProcessEnvironment(env);
  proc.start(commandToRun);
  proc.waitForStarted();
  proc.waitForFinished(-1);

  QString res("");

  if (proc.exitCode() != 0)
  {
    res = proc.readAllStandardError();
  }

  proc.close();
  return res;
}

/*
 * Returns the path of given executable
 */
QString UnixCommand::discoverBinaryPath(const QString& binary){
  QProcess proc;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  proc.setProcessEnvironment(env);
  proc.start("/usr/bin/sh -c \"which " + binary + "\"");
  proc.waitForFinished();
  QString res = proc.readAllStandardOutput();

  proc.close();
  res = res.remove('\n');

  //If it still didn't find it, try "/sbin" dir...
  if (res.isEmpty()){
    QFile fbin("/sbin/" + binary);
    if (fbin.exists()){
      res = "/sbin/" + binary;
    }
  }

  return res;
}

/*
 * Cleans Pacman's package cache.
 * Returns true if finished OK
 */
bool UnixCommand::cleanPacmanCache()
{
  QProcess pacman;
  QString commandStr = "\"xbps-remove -O\"";

  QString command = WMHelper::getSUCommand() + " " + commandStr;
  pacman.start(command);
  pacman.waitForFinished();

  return (pacman.exitCode() == 0);
}

/*
 * Performs a pacman query
 */
QByteArray UnixCommand::performQuery(const QStringList args)
{
  QByteArray result("");
  QProcess pacman;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  env.insert("LC_ALL", "C");
  pacman.setProcessEnvironment(env);

  pacman.start("pkg", args);
  pacman.waitForFinished();
  result = pacman.readAllStandardOutput();
  pacman.close();

  return result;
}

/*
 * Performs a pacman query
 * Overloaded with QString parameter
 */
QByteArray UnixCommand::performQuery(const QString &args)
{
  QByteArray result("");
  QProcess pacman;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.remove("COLUMNS");
  env.insert("COLUMNS", "170");
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  env.insert("LC_ALL", "C");
  pacman.setProcessEnvironment(env);

  pacman.start("xbps-" + args);
  pacman.waitForFinished();
  result = pacman.readAllStandardOutput();
  pacman.close();
  return result;
}

QByteArray UnixCommand::performCustomQuery()
{
  QByteArray result("");
  QProcess pacman;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.remove("COLUMNS");
  env.insert("COLUMNS", "170");
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  env.insert("LC_ALL", "C");
  pacman.setProcessEnvironment(env);

  pacman.start("xbps-query-custom");
  pacman.waitForFinished();
  result = pacman.readAllStandardOutput();
  pacman.close();
  return result;
}

/*
 * Performs a yourt command
 */
QByteArray UnixCommand::performAURCommand(const QString &args)
{
  QByteArray result("");
  QProcess aur;

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  aur.setProcessEnvironment(env);

  aur.start(StrConstants::getForeignRepositoryToolName() + " " + args);
  aur.waitForFinished(-1);
  result = aur.readAllStandardOutput();

  aur.close();
  return result;
}

/*
 * Returns a string containing all AUR packages given a searchString parameter
 */
QByteArray UnixCommand::getRemotePackageList(const QString &searchString, bool useCommentSearch)
{
  QByteArray result("");

  if (useCommentSearch)
    result = performQuery("query -Rs " + searchString);
  else
    result = performQuery("query -Rs " + searchString);

  return result;
}

/*
 * Returns a string containing all packages no one depends on
 */
QByteArray UnixCommand::getUnrequiredPackageList()
{
  QByteArray result = performQuery("query -m");
  return result;
}

/*
 * Returns a string containing all packages that are outdated since last DB sync
 */
QByteArray UnixCommand::getOutdatedPackageList()
{
  //QByteArray result = "qt5-x11extras-5.5.0_2 update x86_64 http://repo.voidlinux.eu/current\nqtchooser-52_1 update x86_64 http://repo.voidlinux.eu/current\nrtkit-0.11_12 update x86_64 http://repo.voidlinux.eu/current\nsudo-1.8.14p3_1 update x86_64 http://repo.voidlinux.eu/current";
  QByteArray result = performQuery("install -un");
  return result;
}

/*
 * Returns a string containing all AUR outdated packages
 */
QByteArray UnixCommand::getOutdatedAURPackageList()
{
  QByteArray result;

  if (StrConstants::getForeignRepositoryToolName() == "kcp")
  {
    result = performAURCommand("-lO");
  }
  else if (StrConstants::getForeignRepositoryToolName() != "kcp")
  {
    result = performAURCommand("-Qua");
  }

  return result;
}

/*
 * Returns a string containing all packages that are not contained in any repository
 * (probably the ones installed by a tool such as yaourt)
 */
QByteArray UnixCommand::getForeignPackageList()
{
  QByteArray result = performQuery(QStringList("-Qm"));
  return result;
}

/*
 * Retrieves the dependencies pkg list
 */
QByteArray UnixCommand::getDependenciesList(const QString &pkgName)
{
  QByteArray result = performQuery("query -x " + pkgName);
  return result;
}

/*
 * Retrieves the remote dependencies pkg list
 */
QByteArray UnixCommand::getRemoteDependenciesList(const QString &pkgName)
{
  QByteArray result = performQuery("query -Rx " + pkgName);
  return result;
}

/*
 * Returns a string with the list of all packages available in all repositories
 * (installed + not installed)
 *
 * @param pkgName Used while the user is searching for the pkg that provides a certain file
 */
QByteArray UnixCommand::getPackageList(const QString &pkgName)
{
  QByteArray result;

  if (pkgName.isEmpty())
  {
#ifdef UNIFIED_SEARCH
    result = performCustomQuery();
#else
    result = performQuery("query -l");
#endif
  }
  else
  {
  }

  return result;
}

/*
 * Given a package name and if it is default to the official repositories,
 * returns a string containing all of its information fields
 * (ex: name, description, version, dependsOn...)
 */
QByteArray UnixCommand::getPackageInformation(const QString &pkgName, bool foreignPackage = false)
{
  QString args;

  if(foreignPackage)
  {
  }
  else
  {
    args = "query " + pkgName;
  }

  //if (pkgName.isEmpty() == false) // enables get for all ("")
  //  args << pkgName;

  QByteArray result = performQuery(args);
  return result;
}

/*
 * Given an AUR package name, returns a string containing all of its information fields
 * (ex: name, description, version, dependsOn...)
 */
QByteArray UnixCommand::getAURPackageVersionInformation()
{
  QByteArray result;

  if (StrConstants::getForeignRepositoryToolName() == "kcp")
  {
    result = performAURCommand("-lO");
  }
  else if (StrConstants::getForeignRepositoryToolName() != "kcp")
  {
    result = performAURCommand("-Qua");
  }

  return result;
}

/*
 * Given a package name, returns a string containing all the files inside it
 */
QByteArray UnixCommand::getPackageContentsUsingPacman(const QString& pkgName)
{
  QByteArray res = performQuery("query -f " + pkgName);
  return res;
}

/*
 * Check if pkgfile is installed on the system
 */
bool UnixCommand::isPkgfileInstalled()
{
  QProcess pkgfile;

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  pkgfile.setProcessEnvironment(env);

  pkgfile.start("pkgfile -V");
  pkgfile.waitForFinished();

  return pkgfile.exitStatus() == QProcess::NormalExit;
}

/*
 * Given a package name, which can be installed or uninstalled on system
 * returns a string containing all the files inside it, the file list is
 * obtained using pkgfile
 */
QByteArray UnixCommand::getPackageContentsUsingPkgfile(const QString &pkgName)
{
  QByteArray result("");
  QProcess pkgfile;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  pkgfile.setProcessEnvironment(env);

  pkgfile.start("pkgfile -l " + pkgName);
  pkgfile.waitForFinished();
  result = pkgfile.readAllStandardOutput();

  return result;
}

/*
 * Given a complete file path, returns the package that provides that file
 */
QString UnixCommand::getPackageByFilePath(const QString &filePath)
{
  QString pkgName="";
  QString out = performQuery("query -o " + filePath);

  if (!out.isEmpty())
  {
    int pos = out.indexOf(":");
    if (pos != -1)
    {      
      pkgName = out.left(pos);
      //Now we have to remove the pkg version...
      int dash = pkgName.lastIndexOf("-");
      if (dash != -1)
      {
        pkgName = pkgName.left(dash);
      }
    }
  }

  return pkgName;
}

/*
 * Based on the given file, we use 'slocate' to suggest complete paths
 */
QStringList UnixCommand::getFilePathSuggestions(const QString &file)
{
  QProcess slocate;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  slocate.setProcessEnvironment(env);
  slocate.start("slocate -l 8 " + file);
  slocate.waitForFinished();

  QString ba = slocate.readAllStandardOutput();
  return ba.split("\n", QString::SkipEmptyParts);
}

/*
 * Retrives the list of package groups
 */
QByteArray UnixCommand::getPackageGroups()
{
  QByteArray res = performQuery(QStringList("-Sg"));
  return res;
}

/*
 * Given a group name, returns a string containing all packages from it
 */
QByteArray UnixCommand::getPackagesFromGroup(const QString &groupName)
{
  QByteArray res =
      performQuery(QString("--print-format \"%r %n\" -Spg " ) + groupName);

  return res;
}

/*
 * Retrieves the list of installed packages in a special format for TargetList
 */
QByteArray UnixCommand::getInstalledPackages()
{
  QString args = "query '%n-%v %n#%v";
  QByteArray res = performQuery(args);
  return res;
}

/*
 * Retrieves the list of targets needed to update the entire system or a given package
 */
QByteArray UnixCommand::getTargetUpgradeList(const QString &pkgName)
{
  QString args;
  QByteArray res = "";

  if(!pkgName.isEmpty())
  {
    args = "install -n -f -Rs " + pkgName;
    res = performQuery(args);
  }
  else //pkg upgrade
  {
    args = "install -un";
    res = performQuery(args);
  }

  return res;
}

/*
 * Given a package name, retrieves the list of all targets needed for its removal
 */
QByteArray UnixCommand::getTargetRemovalList(const QString &pkgName)
{
  QString args;
  QByteArray res = "";

  if(!pkgName.isEmpty())
  {
    args = "remove -R -n " + pkgName;
    res = performQuery(args);
  }

  return res;
}

/*
 * Retrieves the given field for a local package search
 */
QByteArray UnixCommand::getFieldFromLocalPackage(const QString &field, const QString &pkgName)
{
  QByteArray res = performQuery("query -p " + field + " " + pkgName);
  return res;
}

/*
 * Retrieves the given field for a remote package search
 */
QByteArray UnixCommand::getFieldFromRemotePackage(const QString &field, const QString &pkgName)
{
  QByteArray res = performQuery("query -R -p " + field + " " + pkgName);
  return res;
}

/*
 * Retrieves the system arch
 */
QString UnixCommand::getSystemArchitecture()
{
  QStringList slParam;
  QProcess proc;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  proc.setProcessEnvironment(env);

  slParam << "-m";
  proc.start("uname", slParam);
  proc.waitForFinished();

  QString out = proc.readAllStandardOutput();
  proc.close();

  return out;
}

/*
 * Checks if we have internet access!
 */
bool UnixCommand::hasInternetConnection()
{
  QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
  bool result = false;

  for (int i = 0; i < ifaces.count(); i++){
    QNetworkInterface iface = ifaces.at(i);

    if ( iface.flags().testFlag(QNetworkInterface::IsUp)
         && !iface.flags().testFlag(QNetworkInterface::IsLoopBack) ){
      for (int j=0; j<iface.addressEntries().count(); j++){
        /*
         We have an interface that is up, and has an ip address
         therefore the link is present.

         We will only enable this check on first positive,
         all later results are incorrect
        */
        if (result == false)
          result = true;
      }
    }
  }

  //It seems to be alright, but let's make a ping to see the result
  /*if (result == true)
  {
    result = UnixCommand::doInternetPingTest();
  }*/

  return result;
}

/*
 * Pings google site, to make sure internet is OK
 */
bool UnixCommand::doInternetPingTest()
{
  QProcess ping;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  ping.setProcessEnvironment(env);
  ping.start("ping -c 1 -W 3 www.google.com");

  ping.waitForFinished();

  int res = ping.exitCode();
  ping.close();

  return (res == 0);
}

/*
 * Checks if the given executable is available somewhere in the system
 */
bool UnixCommand::hasTheExecutable( const QString& exeName )
{
  //std::cout << "Searching for the executable: " << exeName.toLatin1().data() << std::endl;

  QProcess proc;
  proc.setProcessChannelMode(QProcess::MergedChannels);
  QString sParam = "\"which " + exeName + "\"";

  proc.start("/bin/sh -c " + sParam);
  proc.waitForFinished();

  QString out = proc.readAllStandardOutput();
  proc.close();

  if (out.isEmpty() || out.count("which") > 0) return false;
  else return true;
}

/*
 * Does some garbage collection, removing uneeded files
 */
void UnixCommand::removeTemporaryFiles()
{
  QDir tempDir(QDir::tempPath());
  QStringList nameFilters;
  nameFilters << "qtsingleapp*" << "gpg*" << ".qt_temp_*";
  QFileInfoList list = tempDir.entryInfoList(nameFilters, QDir::Dirs | QDir::Files | QDir::System | QDir::Hidden);

  foreach(QFileInfo file, list){
    QFile fileAux(file.filePath());

    if (!file.isDir()){
      fileAux.remove();
    }
    else{
      QDir dir(file.filePath());
      QFileInfoList listd = dir.entryInfoList(QDir::Files | QDir::System);

      foreach(QFileInfo filed, listd){
        QFile fileAuxd(filed.filePath());
        fileAuxd.remove();
      }

      dir.rmdir(file.filePath());
    }
  }
}

/*
 * Runs a command AS NORMAL USER externaly with QProcess!
 */
void UnixCommand::execCommandAsNormalUser(const QString &pCommand)
{
  QProcess::startDetached(pCommand);
}

/*
 * Runs a command with a QProcess blocking object!
 */
void UnixCommand::execCommand(const QString &pCommand)
{
  QProcess p;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  p.setProcessEnvironment(env);

  p.start(WMHelper::getSUCommand() + "\"" + pCommand + "\"");
  p.waitForFinished(-1);
  p.close();
}

/*
 * Runs a command with a QProcess blocking object and returns its output!
 */
QByteArray UnixCommand::getCommandOutput(const QString &pCommand)
{
  QProcess p;
  /*QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  p.setProcessEnvironment(env);*/

  p.start(pCommand);
  p.waitForFinished(-1);
  return p.readAllStandardOutput();
}

/*
 * Given a filename, checks if it is a text file
 */
bool UnixCommand::isTextFile(const QString& fileName)
{
  QProcess *p = new QProcess();
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  p->setProcessEnvironment(env);

  QStringList s(fileName);
  p->start( "file", s );
  p->waitForFinished();

  QByteArray output = p->readAllStandardOutput();
  p->close();
  delete p;

  int from = output.indexOf(":", 0)+1;

  return (((output.indexOf( "ASCII", from ) != -1) ||
          (output.indexOf( "text", from ) != -1) ||
          (output.indexOf( "empty", from ) != -1)) &&
          (output.indexOf( "executable", from) == -1));
}

/*
 * Retrieves pkgNG version.
 */
QString UnixCommand::getXBPSVersion()
{
  QString v = performQuery("query -V");
  return v;
}

/*
 * Opens a root terminal
 */
void UnixCommand::openRootTerminal(){
  m_terminal->openRootTerminal();
}

/*
 * Executes given commandToRun inside a terminal, so the user can interact
 */
void UnixCommand::runCommandInTerminal(const QStringList& commandList){
  m_terminal->runCommandInTerminal(commandList);
}

/*
 * Executes given commandToRun inside a terminal, as the current user!
 */
void UnixCommand::runCommandInTerminalAsNormalUser(const QStringList &commandList)
{
  m_terminal->runCommandInTerminalAsNormalUser(commandList);
}

/*
 * Executes the given command using QProcess async technology with ROOT credentials
 */
void UnixCommand::executeCommand(const QString &pCommand, Language lang)
{
  QString command;

  if (lang == ectn_LANG_USER_DEFINED)
  {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("LANG");
    env.remove("LC_MESSAGES");
    env.insert("LANG", QLocale::system().name() + ".UTF-8");
    env.insert("LC_MESSAGES", QLocale::system().name() + ".UTF-8");
    m_process->setProcessEnvironment(env);
  }

  if(isRootRunning())
  {
    command += "dbus-launch " + pCommand;
  }
  else
  {
    if (WMHelper::getSUCommand() == ctn_KDESU)
    {
      command = WMHelper::getSUCommand() + pCommand;
    }
    else
    {
      command = WMHelper::getSUCommand() + "\"" + pCommand + "\"";
    }
  }

  m_process->start(command);
}

/*
 * Executes the given command using QProcess async technology as a normal user
 */
void UnixCommand::executeCommandAsNormalUser(const QString &pCommand)
{
  m_process->start(pCommand);
}

/*
 * Puts all Standard output of the member process into a member string
 */
void UnixCommand::processReadyReadStandardOutput()
{
  if (m_process->isOpen())
    m_readAllStandardOutput = m_process->readAllStandardOutput();
}

/*
 * Puts all StandardError output of the member process into a member string
 */
void UnixCommand::processReadyReadStandardError()
{
  if (m_process->isOpen())
  {
    m_readAllStandardError = m_process->readAllStandardError();
    m_errorString = m_process->errorString();
  }
}

/*
 * Retrieves Standard output of member process
 */
QString UnixCommand::readAllStandardOutput()
{
  return m_readAllStandardOutput;
}

/*
 * Retrieves StandardError output of member process
 */
QString UnixCommand::readAllStandardError()
{
  return m_readAllStandardError;
}

/*
 * Retrieves ErrorString of member process
 */
QString UnixCommand::errorString()
{
  return m_errorString;
}

/*
 * UnixCommand's constructor: the relevant environment english setting and the connectors
 */
UnixCommand::UnixCommand(QObject *parent): QObject()
{
  m_process = new QProcess(parent);
  m_terminal = new Terminal(parent, SettingsManager::getTerminal());

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("LANG", "C");
  env.insert("LC_MESSAGES", "C");
  m_process->setProcessEnvironment(env);

  QObject::connect(m_process, SIGNAL( started() ), this,
                   SIGNAL( started() ));
  QObject::connect(this, SIGNAL( started() ), this,
                   SLOT( processReadyReadStandardOutput() ));

  QObject::connect(m_process, SIGNAL( readyReadStandardOutput() ), this,
                   SIGNAL( readyReadStandardOutput() ));
  QObject::connect(this, SIGNAL( readyReadStandardOutput() ), this,
                   SLOT( processReadyReadStandardOutput() ));

  QObject::connect(m_process, SIGNAL( finished ( int, QProcess::ExitStatus )), this,
                   SIGNAL( finished ( int, QProcess::ExitStatus )) );
  QObject::connect(this, SIGNAL( finished ( int, QProcess::ExitStatus )), this,
                   SLOT( processReadyReadStandardOutput() ));

  QObject::connect(m_process, SIGNAL( readyReadStandardError() ), this,
                   SIGNAL( readyReadStandardError() ));
  QObject::connect(this, SIGNAL( readyReadStandardError() ), this,
                   SLOT( processReadyReadStandardError() ));

  //Terminal signals
  QObject::connect(m_terminal, SIGNAL( started()), this,
                   SIGNAL( started()));
  QObject::connect(m_terminal, SIGNAL( finished ( int, QProcess::ExitStatus )), this,
                   SIGNAL( finished ( int, QProcess::ExitStatus )) );

  QObject::connect(m_terminal, SIGNAL( startedTerminal()), this,
                   SIGNAL( startedTerminal()));
  QObject::connect(m_terminal, SIGNAL( finishedTerminal(int,QProcess::ExitStatus)), this,
                   SIGNAL( finishedTerminal(int,QProcess::ExitStatus)));
}

/*
 * If justOneInstance = false (default), returns TRUE if one instance of the app is ALREADY running
 * Otherwise, it returns TRUE if the given app is running.
 */
bool UnixCommand::isAppRunning(const QString &appName, bool justOneInstance)
{
  QStringList slParam;
  QProcess proc;

  slParam << "-C";
  //ps only works with 15 char process names
  QString app = appName.left(15);
  slParam << app;
  proc.start("ps", slParam);
  proc.waitForFinished();

  QString out = proc.readAll();
  proc.close();

  if (justOneInstance)
  {
    if (out.count(app)>0)
      return true;
    else
      return false;
  }
  else
  {
    if (out.count(app)>1)
      return true;
    else
      return false;
  }
}

/*
 * Given a 'pkgName' package name, checks if that one is installed in the system
 */
bool UnixCommand::isPackageInstalled(const QString &pkgName)
{
  QProcess pacman;
  QString command = "xbps-query -S " + pkgName;
  pacman.start(command);
  pacman.waitForFinished();
  return (pacman.exitCode() == 0);
}

/*
 * Searches "/etc/pacman.conf" to see if ILoveCandy is enabled
 */
bool UnixCommand::isILoveCandyEnabled()
{
  bool res=false;
  QFile file("/etc/pacman.conf");

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QString contents = file.readAll();
  int end = contents.indexOf("ILoveCandy");
  int start=0;

  if (end != -1)
  {
    //Does it contains a # before it???
    start = end;
    do{
      start--;
    }while (contents.at(start) != '\n');

    QString str = contents.mid(start+1, (end-start-1)).trimmed();

    if (str.isEmpty()) res = true;
    else res = false;
  }

  file.close();

  return res;
}

/*
 * Returns the list of strings after "fieldName" in Pacman.conf;
 */
QStringList UnixCommand::getFieldFromPacmanConf(const QString &fieldName)
{
  QStringList result;
  QFile file("/etc/pacman.conf");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return result;

  QString contents = file.readAll();
  int from = 0;
  const int ctn_FIELD_LENGTH = fieldName.length();

  do
  {
    int end = contents.indexOf(fieldName, from, Qt::CaseInsensitive);
    int start=0;

    if (end != -1)
    {
      //Does it contains a # before it???
      start = end;
      do{
        start--;
      }while (contents.at(start) != '\n');

      QString str = contents.mid(start+1, (end-start-1)).trimmed();

      if (str.isEmpty())
      {
        QString ignorePkg = contents.mid(end);
        int equal = ignorePkg.indexOf("=");
        int newLine = ignorePkg.indexOf("\n");

        ignorePkg = ignorePkg.mid(equal+1, newLine-(equal+1)).trimmed();
        result = ignorePkg.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        break;
      }
      else if (str != "#")
        from += end + ctn_FIELD_LENGTH;
      else
        from += ctn_FIELD_LENGTH;
    }
    else break;
  }
  while(true);

  file.close();
  return result;
}

/*
 * Searches "/etc/pacman.conf" to retrive IgnorePkg items (if any)
 */
QStringList UnixCommand::getIgnorePkgsFromPacmanConf()
{
  QStringList resPkgs;
  QStringList resGroups;

  resPkgs = getFieldFromPacmanConf("IgnorePkg");
  resGroups = getFieldFromPacmanConf("IgnoreGroup");

  if (!resGroups.isEmpty())
  {
    //Let's retrieve all pkgs that live inside each group
    foreach (QString group, resGroups)
    {
      QStringList *packagesOfGroup = Package::getPackagesOfGroup(group);
      if (!packagesOfGroup->isEmpty())
      {
        foreach (QString pkg, *packagesOfGroup)
        {
          resPkgs.append(pkg);
        }
      }
    }
  }

  resPkgs.removeDuplicates();
  return resPkgs;
}

/*
 * Retrieves the BSDFlavour where OctoPkg is running on!
 * Reads file "/etc/os-release" and searchs for compatible OctoPkg BSDs
 */
LinuxDistro UnixCommand::getLinuxDistro()
{
  static LinuxDistro ret;
  static bool firstTime = true;

  if (firstTime)
  {
    if (QFile::exists("/etc/os-release"))
    {
      QFile file("/etc/os-release");

      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        ret = ectn_UNKNOWN;

      QString contents = file.readAll();

      if (contents.contains("PRETTY_NAME=\"void\""))
      {
        ret = ectn_VOID;
      }
      else
      {
        ret = ectn_UNKNOWN;
      }
    }

    firstTime = false;
  }

  return ret;
}
