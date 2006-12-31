/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
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
#ifndef OPTIONS_H
#define OPTIONS_H

#include <qstring.h>

/**
Singleton class providing one-stop shopping for all program options.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class Options{
public:
    static Options* instance();

//    void setSystemStudyPath(const QString& systemStudyPath) {systemStudyPath_ = systemStudyPath;}
    void setOpticalStudyPath(const QString& opticalStudyPath) {opticalStudyPath_ = opticalStudyPath;}
//    void setOtherStudyPath(const QString& otherStudyPath) {otherStudyPath_ = otherStudyPath;}
    void setNetworkStudyPath(const QString& networkStudyPath) {networkStudyPath_ = networkStudyPath;}
    void setExportFilePath(const QString& exportFilePath) {exportFilePath_ = exportFilePath;}
    void setTempStudyPath(const QString& tempStudyPath) {tempStudyPath_ = tempStudyPath;}

    void setEnableAcquisition(bool enable) {enableAcquisition_ = enable;}
    void setEmulateOpticalDrive(bool emulate) {emulateOpticalDrive_ = emulate;}


//    QString systemStudyPath() const {return systemStudyPath_;}
    QString opticalStudyPath() const {return opticalStudyPath_;}
//    QString otherStudyPath() const {return otherStudyPath_;}
    QString networkStudyPath() const {return networkStudyPath_;}
    QString exportFilePath() const {return exportFilePath_;}
    QString tempStudyPath() const {return tempStudyPath_;}

    bool enableAcquisition() const {return enableAcquisition_;}
    bool emulateOpticalDrive() const {return emulateOpticalDrive_;}

    void readSettings();
    void writeSettings();

    ~Options();

protected:
    Options();
    static Options* instance_;

private:
//    QString systemStudyPath_;
    QString opticalStudyPath_;
    QString networkStudyPath_;
    QString tempStudyPath_;
    QString exportFilePath_;
//    QString otherStudyPath_;
//  otherStudyPath just defaults to systemStudyPath for now.
    bool emulateOpticalDrive_;
    bool enableAcquisition_;

};

#endif
