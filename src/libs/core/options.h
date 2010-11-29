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

// convenient evil macro
/// DEPRECATED!!!!!
#define epOptions (EpCore::Options::instance())

namespace EpCore {

/**
 * NOT a Singleton class providing one-stop shopping for all program options.
 * At present program invokes only a single Options* pointer in the
 * Navigator class and passes it around.  This allows for testing with
 * mock Options.  Note that loading and saving Options are separate from
 * construction and destruction.
 * These are set both in SystemDialog and SimulatorSettingsDialog.
 * Options are stored on disk as QSettings.
 * As there seems little point to "protect" option data members
 * behind setter and getter functions, we will break an OOP rule
 * and make the data members public.  Options is basically a POD
 * class with the means to read and write itself to disk, so this
 * should be ok.  To emphasize this, we have changed the 
 * data type from class to struct.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
struct Options {

public:
   ~Options();

   enum ScreenFlag {
        NoScreenEmulation       = 0x000000,
        // Puts everything on the left-hand screen, even if dual monitors,
        // no effect if only one monitor.
        EmulateOneScreen        = 0x000001,
        // Emulates dual monitors on a single monitor,
        // no effect if dual monitors.  If both EmulateOneScreen 
        // and EmulateTwoScreens selected and dual monitors,
        // ignore the 2nd monitor and act like EmulateTwoScreens
        // on a single monitor.  If single monitor,
        // same as EmulateTwoScreens alone.
        EmulateTwoScreens       = 0x000002,
        // This emulates the Prucka windows manager, i.e. automatic layout of
        // windows.  If not set, traditional tile and cascade menu items appear.
        // However, note next option too.
        // Right now this retiles Recorder subwindows when Recorder resized.
        EmulateWindowsManager   = 0x000004,
        // Tiling uses Prucka layout if set, otherwise tradition MDA tiling.
        // Ignored if EmulateWindowsManager set.
        EmulatePruckaTiling     = 0x000008,
        TwoRecorderWindows      = 0x000010,
        // Simpler to just set a flag for 2 recorder windows, have Recorder
        // constructor figure out what to do.  When there are 2 screens,
        // positioning acts like there is only one large screen.
        DefaultScreenFlags      = EmulateWindowsManager | EmulatePruckaTiling
    };
    Q_DECLARE_FLAGS(ScreenFlags, ScreenFlag)
    ScreenFlags screenFlags;

    enum RecorderFlag {
        ImmovablePatientStatusBar   = 0x000001,
        PatientStatusBarHasTitle    = 0x000002,
        RecorderHasStatusBar        = 0x000004,
        DefaultRecorderFlags        = ImmovablePatientStatusBar 
                                      | RecorderHasStatusBar  
    };
    Q_DECLARE_FLAGS(RecorderFlags, RecorderFlag)
    RecorderFlags recorderFlags;

    enum BluePanelStyle {
        OpaqueButtons       = 0,
        TransparentButtons  = 1
    } bluePanelStyle;

    // will use this to correct flat blue panel button problems on some systems
    bool bluePanelTweak;

    // determines if last name and first name are displayed separately
    // and whether actual user name is displayed in status bar
    bool oldStyleNavigator;

    enum OpticalDiskFlag {
        NoOpticalDiskFlags      = 0x000000,
        Emulation               = 0x000001,
        DualSided               = 0x000002
    };
    Q_DECLARE_FLAGS(OpticalDiskFlags, OpticalDiskFlag)
    OpticalDiskFlags opticalDiskFlags;

    // emulated optical drive capacity (per side) in megabytes
    // if = 0 then no limit.  Default is 0 (unlimited)
    int emulatedOpticalDiskCapacity;

    // paths and cataloging
    QString opticalStudyPath;
    QString networkStudyPath;
    QString exportFilePath;
    QString tempStudyPath;
    QString systemCatalogPath;
    QString labName;    // name of ep lab study for this computer
    // use lab name in location column of TableListView 
    // for for Network, if false or if lab name is empty,
    // use machine name.
    bool useLabName;

    enum FilePathFlag {
        NoFilePathFlag          = 0x000000,
        EnableAcquisition       = 0x000001,
        EnableFileExport        = 0x000002,
        EnableNetworkStorage    = 0x000004
    };
    Q_DECLARE_FLAGS(FilePathFlags, FilePathFlag)
    FilePathFlags filePathFlags;

    // utility function, since this is a popular request
    inline bool includeNetworkCatalog() {
        return filePathFlags.testFlag(Options::EnableNetworkStorage);
    }

    bool administratorAccountRequired;
    bool hideSimulatorMenu;
    bool permanentDelete;
    
    // control tweakable aspects of the simulation
    enum SimulationControlFlag {
	NoUserControls = 0x000000,
	UserPhysiologyControl = 0x000001,
	UserHeartControl = 0x000002,
	UserPatientControl = UserPhysiologyControl | UserHeartControl,
	DefaultUserControl = UserPatientControl
    };
    Q_DECLARE_FLAGS(SimulationControlFlags, SimulationControlFlag)
    SimulationControlFlags simulationControlFlags;

    // not a real option, but convenient to have the password hash here
    QString passwordHash;

    enum DiskCache {
        NoCache, ForceCache, AutoCache
    } diskCache;

    int numChannels;

    static Options* instance() {return new Options;}

    // read and write options to disk
    void readSettings();
    void writeSettings();

    void load() {readSettings();}
    void save() {writeSettings();}

protected:
    Options();
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

QString systemPath();

} // EpCore

#endif
