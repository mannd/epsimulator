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

#ifndef OPTICALDISK_H
#define OPTICALDISK_H

#include "fileutilities.h"
#include "options.h"

#include <QCoreApplication>
#include <QString>

#include <vector>

class QDataStream;
class QFile;
class QStringList;
class QWidget;

namespace EpHardware { 
namespace EpOpticalDisk {

struct LabelData {
    QString label;
    QString side;
    friend QDataStream& operator<<(QDataStream&, const LabelData&);
    friend QDataStream& operator>>(QDataStream&, LabelData&);
    bool operator==(const LabelData&) const;
    bool operator!=(const LabelData&) const;
};

// OpticalDisk is a directory that behind the scenes manages 
// caching, as well as burning back to the disk if it really is
// an optical disk.  
class OpticalDisk {
    Q_DECLARE_TR_FUNCTIONS(OpticalDisk)
public:
    OpticalDisk(const QString& path, const QString& cachePath);
    virtual ~OpticalDisk();

    // returns the QString::null or the local translation of "A" and "B"
    // note: returns translation of "B" if side is anything other than 
    // empty or "A"
    static QString translateSide(const QString& side);
    static QString studiesDirName() {return studiesDirName_;}
    static QString makeStudiesPath(const QString& path);

    virtual void init();
    virtual void eject(QWidget*);
    virtual void burn();
    virtual void burn(const QStringList& files, 
                      const QString& source);
    void close();


    virtual void readLabel();
    virtual void writeLabel() const;

 
    virtual void saveLastDisk() {}  // this function only implemented
                                    // for emulated optical disks

    // returns local translation of current side or 
    // QString::null if single sided
    virtual QString translatedSide() const;

    void setLabelData(const LabelData&);
    void setLabel(const QString& label);
    void setSide(const QString& side);
    void setLabelSide(const QString& label, const QString& side) {
        setLabel(label);
        setSide(side);
    }
    virtual void setCacheControl(EpCore::Options::CacheControl);

    bool isInitialized() const {return initialized_;}
    bool isLabeled() const {return !label().isEmpty();}
    bool isRemovable() const;

    LabelData labelData() const;
    QString label() const; 
    QString side() const;

    virtual bool allowSideChange() const {return true;}
    virtual bool showAllSideButtons() const {return true;}
    // below might check to see if the drive actually is two sided.
    // actually it is the disks that are two sided, not the drive
    virtual bool isTwoSided() const {return true;}

    QString path() const {return path_;}    // root path of this optical disk
    // labelPath() == path() in this class, but see EmulatedOpticalDisk.
    // The top level writable path for a "disk" is always equal to
    // the labelPath().
    virtual QString labelPath() const {return path_;}  // path to label
    virtual QString labelFilePath() const; // full path to label.dat
    virtual QString studiesPath() const;    // full path of studies dir
    bool isOpticalDiskPath(const QString& path) const {
        return path == path_;}


protected:
    // first bytes of label file
    enum {MagicNumber = 0x99c798f3};    
    static const QString labelFileName_;

    void load(const QString& fileName);
    void save(const QString& fileName) const;

    void setUseCache(bool enable) {
        useCache_ = enable;
        enable ? workingPath_ = cachePath_ : workingPath_ = path_;
    }
    bool useCache() const {return useCache_;}
    void clearCache();

    QString workingPath() const {return workingPath_;}
private:
    OpticalDisk(OpticalDisk&);
    bool workingCatalogFileExists();
    bool createWorkingCatalogFile();
    
    static const QString studiesDirName_;

    QString path_;  // optical drive path, all the way to study dir
                    // e.g. /home/user/MyStudies
    QString cachePath_; // path to optical disk cache
    QString workingPath_; // either path_ or cachePath_, depending on useCache_
    LabelData labelData_;
    EpCore::Options::CacheControl cacheControl_;
    bool initialized_;
    bool useCache_;
};

class EmulatedOpticalDisk : public OpticalDisk {

public:
    EmulatedOpticalDisk(const QString& path, const QString& cachePath,
                        bool isTwoSided = false);
    ~EmulatedOpticalDisk();

    // returns last disk or 0 if none
    static EmulatedOpticalDisk* getLastDisk(const QString& path);    

    void eject(QWidget*);
    void burn();

    void readLabel();
    void writeLabel() const;
    virtual void saveLastDisk();   // saves last diskName

    virtual void setCacheControl(EpCore::Options::CacheControl);

    // allowing relabeling of emulated disks will mess up 
    // the path to the emulated disk,
    // so don't allow this...
    bool allowSideChange() const {return !isLabeled() && isTwoSided_;} 
    bool showAllSideButtons() const {return false;}

    void setIsTwoSided(bool isTwoSided) {isTwoSided_ = isTwoSided;}

    bool isTwoSided() const {return isTwoSided_;}
    QString diskName() const {return diskName_;}

    QString labelFilePath() const;
    QString labelPath() const;   // .../MyStudies/disks/disk_xxxxx/B
    QString studiesPath() const;  
                                // /.../MyStudies/disks/disk_xxxxx/B/studies

private:
    EmulatedOpticalDisk(const QString& path, const QString& cachePath,
                        const QString& diskName);
    EmulatedOpticalDisk(EmulatedOpticalDisk&);
    struct DiskInfo {
        QString name;
        LabelData labelData;
    };

    typedef std::vector<DiskInfo*> DiskInfoList;

    int makeLabel(const QString& diskName, QStringList& labelList,
                   DiskInfoList& diskInfoList, int& row);

    void makePath() const;    // create subdirectories for the emulated disk
                                               //  i.e. has label.dat in it

    QString disksPath() const;  // e.g. .../MyStudies/disks
    QString diskPath() const;   // /home/user/MyStudies/disks/disk_xxxxx
    QString sideDir() const;    // A or B


    void lastDisk();      // loads up last diskName

    bool isTwoSided_;
    QString diskName_;
};

class HardDrive : public OpticalDisk {
public:
    HardDrive(const QString& path, const QString& cachePath);
    
    virtual bool allowSideChange() const {return false;}
    virtual void setCacheControl(EpCore::Options::CacheControl);
};

} // namespace EpOpticalDisk
} // namespace EpHardware

#endif
