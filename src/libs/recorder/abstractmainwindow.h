/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
 *   mannd@epstudiossoftware.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ABSTRACTMAINWINDOW_H
#define ABSTRACTMAINWINDOW_H

#include <QMainWindow>

class QAction;

namespace EpCore {
class Options;
class User;
}

namespace EpHardware {
namespace EpOpticalDisk {
class OpticalDisk;
}
}

namespace EpGui {

class AbstractMainWindow : public QMainWindow {
    Q_OBJECT
public:
    AbstractMainWindow(EpCore::Options* options,
                       EpCore::User* user,
                       QWidget *parent = 0);
    ~AbstractMainWindow();

    void restore();

public slots:
    virtual void updateAll() = 0;

protected:
    virtual EpHardware::EpOpticalDisk::OpticalDisk* currentDisk() const = 0;
    virtual void updateSimulatorSettings() = 0;
    virtual void updateSystemSettings() = 0;
    virtual void readSettings() = 0;
    void filler();
    EpCore::Options* options() const {return options_;}
    EpCore::User* user() const {return user_;}

    bool showSimulatorSettings();
    void updateWindowTitle(const QString& title = QString());
    bool administrationAllowed();

    QAction* intervalsAction() {return intervalsAction_;}
    QAction* columnFormatsAction() {return columnFormatsAction_;}
    QAction* protocolsAction() {return protocolsAction_;}
    QAction* loginAction() {return loginAction_;}
    QAction* logoutAction() {return logoutAction_;}
    QAction* changePasswordAction() {return changePasswordAction_;}
    QAction* helpAction() {return helpAction_;}
    QAction* aboutAction() {return aboutAction_;}
    QAction* aboutQtAction() {return aboutQtAction_;}
    QAction* simulatorSettingsAction() {return simulatorSettingsAction_;}
    QAction* systemSettingsAction() {return systemSettingsAction_;}

    // protected data members not so bad in this case :)

                       
protected slots:
    void about();
    void help();
    void login();
    void logout();
    void changePassword();
    void simulatorSettings();
    void systemSettings();
    void setIntervals();
    void setProtocols();
    void setColumnFormats();

signals:
    // let Recorder know the disk is changed and what the new disk is
    void opticalDiskChanged(EpHardware::EpOpticalDisk::OpticalDisk*);

private:
    void createActions();
    void changeDatabase();

    EpCore::Options* options_;
    EpCore::User* user_;

    QAction* intervalsAction_;
    QAction* columnFormatsAction_;
    QAction* protocolsAction_;
    QAction* loginAction_;
    QAction* logoutAction_;
    QAction* changePasswordAction_;
    QAction* helpAction_;
    QAction* aboutAction_;
    QAction* aboutQtAction_;
    QAction* simulatorSettingsAction_;
    QAction* systemSettingsAction_;
};

}

#endif
