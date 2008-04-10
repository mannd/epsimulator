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

#include <QString>

/**
 * Singleton class providing one-stop shopping for all program options.
 * These are set both in SystemDialog and SimulatorSettingsDialog.
 * Options are stored on disk as QSettings.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Options {

public:
    static Options* instance();

    // read and write options to disk
    void readSettings();
    void writeSettings();

    // call ONLY at end or program
    // see www.informit.com/guides/content.asp?g=cplusplus&seqNum=148&rl=1
    void destroy() {delete instance_; instance_ = 0;}    

    // Navigator and general options
    // paths to the main system catalogs
    void setOpticalStudyPath(const QString& opticalStudyPath) {
        opticalStudyPath_ = opticalStudyPath;}
    void setNetworkStudyPath(const QString& networkStudyPath) {
        networkStudyPath_ = networkStudyPath;}
    void setExportFilePath(const QString& exportFilePath) {
        exportFilePath_ = exportFilePath;}
    void setTempStudyPath(const QString& tempStudyPath) {
        tempStudyPath_ = tempStudyPath;}

    void setLabName(const QString& labName) {labName_ = labName;}
    void setUseLabName(bool useLabName) {useLabName_ = useLabName;}

    void setEnableAcquisition(bool enable) {enableAcquisition_ = enable;}
    void setEnableFileExport(bool enable) {enableFileExport_ = enable;}
    void setEnableNetworkStorage(bool enable) { enableNetworkStorage_ = enable;}
    void setEmulateOpticalDrive(bool emulate) {emulateOpticalDrive_ = emulate;}
    void setDualSidedDrive(bool emulate) {dualSidedDrive_ = emulate;}
    void setEmulatedOpticalDriveCapacity(int capacity) {
        emulatedOpticalDriveCapacity_ = capacity;}
    void setOldStyleNavigator(bool isOldStyle) {
        oldStyleNavigator_ = isOldStyle;}
    void setNewStyleBlueBar(bool isNewStyle) {newStyleBlueBar_ = isNewStyle;}
    void setAdministratorAccountRequired(bool isRequired) {
        administratorAccountRequired_ = isRequired;}
    void setHideSimulatorMenu(bool hideMenu) {hideSimulatorMenu_ = hideMenu;}
    void setPermanentDelete(bool permanentDelete) {
        permanentDelete_ = permanentDelete;}

    void setPasswordHash(QString hash) {passwordHash_ = hash;}

    // Recorder options
    void setAutoSaveDiskIcon(bool enable) {autoSaveDiskIcon_ = enable;}
    void setImmovablePatientStatusBar(bool enable) {
        immovablePatientStatusBar_ = enable;}
    void setPatientStatusBarHasTitle(bool enable) {
        patientStatusBarHasTitle_ = enable;}

    QString opticalStudyPath() const {return opticalStudyPath_;}
    QString networkStudyPath() const {return networkStudyPath_;}
    QString exportFilePath() const {return exportFilePath_;}
    QString tempStudyPath() const {return tempStudyPath_;}
    QString systemCatalogPath() const {return systemCatalogPath_;}

    QString labName() const {return labName_;}
    bool useLabName() const {return useLabName_;}

    bool enableAcquisition() const {return enableAcquisition_;}
    bool enableFileExport() const {return enableFileExport_;}
    bool enableNetworkStorage() const {return enableNetworkStorage_;}
    bool emulateOpticalDrive() const {return emulateOpticalDrive_;}
    bool dualSidedDrive() const {return dualSidedDrive_;}
    int emulatedOpticalDriveCapacity() const {
        return emulatedOpticalDriveCapacity_;} 
    bool oldStyleNavigator() const {return oldStyleNavigator_;}
    bool newStyleBlueBar() const {return newStyleBlueBar_;}
    bool administratorAccountRequired() const {
        return administratorAccountRequired_;}
    bool hideSimulatorMenu() const {return hideSimulatorMenu_;}
    bool permanentDelete() const {return permanentDelete_;}
    
    QString passwordHash() const {return passwordHash_;}

    bool autoSaveDiskIcon() const {return autoSaveDiskIcon_;}
    bool immovablePatientStatusBar() const {return immovablePatientStatusBar_;}
    bool patientStatusBarHasTitle() const {return patientStatusBarHasTitle_;}

protected:
    Options();
    ~Options();

    static Options* instance_;

private:
    QString opticalStudyPath_;
    QString networkStudyPath_;
    QString tempStudyPath_;
    QString exportFilePath_;
    QString systemCatalogPath_;
    QString labName_;   // name of ep lab study for this computer
    bool enableAcquisition_;
    bool enableFileExport_;
    bool enableNetworkStorage_;
    
    // some simulator specific options
    // related to Optical drive emulation
    bool emulateOpticalDrive_;
    bool dualSidedDrive_;

    // emulated optical drive capacity (per side) in megabytes
    // if = 0 then no limit.  Default is 0 (unlimited)
    int emulatedOpticalDriveCapacity_;

    // determines if last name and first name are displayed separately
    bool oldStyleNavigator_;
    bool newStyleBlueBar_;  // flat buttons in Navigator Blue Bar
    
    bool useLabName_;   // use lab name in location column of TableListView 
                        // for for Network, if false or if lab name is empty,
                        // use machine name.

    bool administratorAccountRequired_;
    bool hideSimulatorMenu_;

    bool permanentDelete_;
    
    // not a real option, but convenient to have the password hash here
    QString passwordHash_;

    // Recorder options
    bool autoSaveDiskIcon_;
    bool immovablePatientStatusBar_;
    bool patientStatusBarHasTitle_;

};

#endif
