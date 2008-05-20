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

#include <QFlags>
#include <QSettings>
#include <QString>

namespace EpCore {

/**
 * Singleton class providing one-stop shopping for all program options.
 * These are set both in SystemDialog and SimulatorSettingsDialog.
 * Options are stored on disk as QSettings.
 * As there seems little point to "protect" option data members
 * behind setter and getter functions, we will break an OOP rule
 * and make the data members private.  Options is basically a POD
 * class with the means to read and write itself to disk, so this
 * should be ok.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Options {

public:
    enum ScreenFlag {
        NoScreenEmulation       = 0x000000,
        EmulateOneScreen        = 0x000001,
        EmulateTwoScreens       = 0x000002,
        EmulateWindowsManager   = 0x000004
    };
    Q_DECLARE_FLAGS(ScreenFlags, ScreenFlag)
    ScreenFlags screenFlags;

    enum BluePanelStyle {
        OpaqueButtons       = 0,
        TransparentButtons  = 1
    } bluePanelStyle;

    enum OpticalDiskFlag {
        NoOpticalDiskFlags      = 0x000000,
        Emulation               = 0x000001,
        DualSided               = 0x000002,
    };
    Q_DECLARE_FLAGS(OpticalDiskFlags, OpticalDiskFlag)
    OpticalDiskFlags opticalDiskFlags;

    int emulatedOpticalDiskCapacity;    // in megabytes

    // paths

    QString opticalStudyPath;
    QString networkStudyPath;
    QString exportFilePath;
    QString tempStudyPath;
    QString systemCatalogPath;

    enum FilePathFlag {
        EnableAcquisition       = 0x000001,
        EnableFileExport        = 0x000002,
        EnableNetworkStorage    = 0x000004
    };
    Q_DECLARE_FLAGS(FilePathFlags, FilePathFlag)
    FilePathFlags filePathFlags;

    static Options* instance();

    // read and write options to disk
    void readSettings();
    void writeSettings();
    // call ONLY at end or program
    // see www.informit.com/guides/content.asp?g=cplusplus&seqNum=148&rl=1
    void destroy() {delete instance_; instance_ = 0;}    


    void setLabName(const QString& labName) {labName_ = labName;}
    void setUseLabName(bool useLabName) {useLabName_ = useLabName;}

    void setEmulateOpticalDrive(bool emulate) {emulateOpticalDrive_ = emulate;}
    void setDualSidedDrive(bool emulate) {dualSidedDrive_ = emulate;}
    void setEmulatedOpticalDriveCapacity(int capacity) {
        emulatedOpticalDriveCapacity_ = capacity;}
    void setOldStyleNavigator(bool isOldStyle) {
        oldStyleNavigator_ = isOldStyle;}
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
    void setEmulateTwoScreens(bool enable) {
        emulateTwoScreens_ = enable;}
    void setEmulateWindowsManager(bool enable) {
        emulateWindowsManager_ = enable;}
    void setHasStatusBar(bool enable) {
        recorderStatusBar_ = enable;}

    // Amplifier
    void setNumChannels(unsigned int n) {numChannels_ = n;}

    //QString systemCatalogPath() const {return systemCatalogPath_;}

    QString labName() const {return labName_;}
    bool useLabName() const {return useLabName_;}

//    bool enableAcquisition() const {return enableAcquisition_;}
//    bool enableFileExport() const {return enableFileExport_;}
//    bool enableNetworkStorage() const {return enableNetworkStorage_;}
    bool emulateOpticalDrive() const {return emulateOpticalDrive_;}
    bool dualSidedDrive() const {return dualSidedDrive_;}
    int emulatedOpticalDriveCapacity() const {
        return emulatedOpticalDriveCapacity_;} 
    bool oldStyleNavigator() const {return oldStyleNavigator_;}
    bool administratorAccountRequired() const {
        return administratorAccountRequired_;}
    bool hideSimulatorMenu() const {return hideSimulatorMenu_;}
    bool permanentDelete() const {return permanentDelete_;}
    
    QString passwordHash() const {return passwordHash_;}

    bool autoSaveDiskIcon() const {return autoSaveDiskIcon_;}
    bool immovablePatientStatusBar() const {return immovablePatientStatusBar_;}
    bool patientStatusBarHasTitle() const {return patientStatusBarHasTitle_;}
    bool emulateTwoScreens() const {return emulateTwoScreens_;}
    bool emulateWindowsManager() const {return emulateWindowsManager_;}
    bool recorderStatusBar() const {return recorderStatusBar_;}

    unsigned int numChannels() const {return numChannels_;}

protected:
    Options();
    ~Options();

    static Options* instance_;

private:
    //QString systemCatalogPath_;
    QString labName_;   // name of ep lab study for this computer

    // some simulator specific options
    // related to Optical drive emulation
    bool emulateOpticalDrive_;
    bool dualSidedDrive_;

    // emulated optical drive capacity (per side) in megabytes
    // if = 0 then no limit.  Default is 0 (unlimited)
    int emulatedOpticalDriveCapacity_;

    // determines if last name and first name are displayed separately
    bool oldStyleNavigator_;
    
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
    bool emulateTwoScreens_;
    bool emulateWindowsManager_;    // emulate the CardioLab windows manager
    bool recorderStatusBar_;

    // Amplifier
    unsigned int numChannels_;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Options::ScreenFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Options::OpticalDiskFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Options::FilePathFlags)



template<typename T, typename K>
inline T readFlags(const QString& name, const K& defaultValue, 
            const QSettings& settings) {
    return static_cast<T>(settings.value(name, int(defaultValue)).toInt());
}

template<typename T, typename K>
inline void setFlag(T& flags, const K& flag, bool enable = true) {
    if (enable)
        flags |= flag;
    else
        flags &= ~flag;
}

template<typename T, typename K>
inline void clearFlag(T& flags, const K& flag) {
    setFlag(flags, flag, false);
}

}

#endif
