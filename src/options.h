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

/**
    \file 
    options.h contains all system options.
*/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <qstring.h>

/**
Singleton class providing one-stop shopping for all program options.

	@author David Mann <mannd@epstudiossoftware.com>
    Explanation of the various study/catalog paths:
    
    opticalStudyPath is the path to the optical drive.  If emulateOpticalDrive is
true, this is a directory on the hard drive, by default ($HOME)/EPStudies.  The
actual disks are emulated as subdirectories in this directory, each one named for the
disk label.  Example: ($HOME)/EPStudies/Disk32A/.  In each of these subdirectories there
is a catalog.eps file, and the actual data files for each study.  Emulation will ensure that
each directory is limited in size, to emulate switching disks.  The systemCatalogPath is
fixed at ($EPSIMULATOR)/System/.  It cannot be changed.  This path just contains a
catalog.eps file, but no data files.  (Note that the real Prucka uses studies.dat 
files for this purpose.  The networkStudyPath is a path to a server directory.
This directory can hold a catalog.eps file, and optionally studies that are exported to it.
Export can be manual, or automatic when the study is closed.  Finally there is a tempCatalogPath,
which can be provided on the command line, or can be browsed to.  If not specified on 
the command line, it defaults to the systemCatalogPath.
    opticalStudyPath and networkStudyPath can be specified in the systemDialog.  Export
*/
class Options{
public:
    static Options* instance();

    void setOpticalStudyPath(const QString& opticalStudyPath) {opticalStudyPath_ = opticalStudyPath;}
    void setNetworkStudyPath(const QString& networkStudyPath) {networkStudyPath_ = networkStudyPath;}
    void setExportFilePath(const QString& exportFilePath) {exportFilePath_ = exportFilePath;}
    void setTempStudyPath(const QString& tempStudyPath) {tempStudyPath_ = tempStudyPath;}

    void setEnableAcquisition(bool enable) {enableAcquisition_ = enable;}
    void setEmulateOpticalDrive(bool emulate) {emulateOpticalDrive_ = emulate;}
    void setEnableFileExport(bool enable) {enableFileExport_ = enable;}
    void setEnableNetworkStorage(bool enable) { enableNetworkStorage_ = enable;}


    QString opticalStudyPath() const {return opticalStudyPath_;}
    QString networkStudyPath() const {return networkStudyPath_;}
    QString exportFilePath() const {return exportFilePath_;}
    QString tempStudyPath() const {return tempStudyPath_;}
    QString systemCatalogPath() const {return systemCatalogPath_;}
    QString catalogFileName() const {return catalogFileName_;}

    bool enableAcquisition() const {return enableAcquisition_;}
    bool emulateOpticalDrive() const {return emulateOpticalDrive_;}
    bool enableFileExport() const {return enableFileExport_;}
    bool enableNetworkStorage() const {return enableNetworkStorage_;}

    void readSettings();
    void writeSettings();

    // call ONLY at end or program
    // see www.informit.com/guides/content.asp?g=cplusplus&seqNum=148&rl=1
    void destroy() {delete instance_; instance_ = 0;}


protected:
    Options();
    ~Options();
    static Options* instance_;

private:
    // name of catalog files is hardwired, and defined in options.cpp
    static const char* catalogFileName_;
    QString opticalStudyPath_;
    QString networkStudyPath_;
    QString tempStudyPath_;
    QString exportFilePath_;
    QString systemCatalogPath_;
    bool emulateOpticalDrive_;
    bool enableAcquisition_;
    bool enableFileExport_;
    bool enableNetworkStorage_;

};

#endif
