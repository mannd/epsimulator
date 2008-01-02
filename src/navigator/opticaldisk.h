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

#include <qstring.h>

#include <vector>

class QDataStream;
class QFile;
class QStringList;
class QWidget;

struct LabelData {
    QString label;
    QString side;
    friend QDataStream& operator<<(QDataStream&, const LabelData&);
    friend QDataStream& operator>>(QDataStream&, LabelData&);
    bool operator==(const LabelData&) const;
    bool operator!=(const LabelData&) const;
};

/**
 * Base class for real optical disk and optical disk emulation.
 *	@author David Mann <mannd@epstudiossoftware.com>
 */
class OpticalDisk {
public:
    OpticalDisk(const QString& path);

    virtual void eject(QWidget*);

    bool isLabeled() const {return isLabeled_;}
    void setIsLabeled(bool isLabeled) {isLabeled_ = isLabeled;}

    virtual void readLabel();
    virtual void writeLabel() const;
    void setLabelData(const LabelData&);
    void setLabel(const QString& label);
    void setSide(const QString& side);
    void setLabelSide(const QString& label, const QString& side) {
        setLabel(label);
        setSide(side);
    }
    virtual void saveLastDisk() {}  // this function only implemented
                                    // for emulated optical disks

    // returns local translation of current side or 
    // QString::null if single sided
    virtual QString translatedSide() const;

    // returns the QString::null or the local translation of "A" and "B"
    // note: returns translation of "B" if side is anything other than 
    // empty or "A"
    static QString translateSide(const QString& side);
    static QString studiesDirName() {return studiesDirName_;}
    static QString makeStudiesPath(const QString& path);

    LabelData labelData() const;
    QString label() const; 
    QString side() const;

    virtual bool allowSideChange() const {return true;}
    virtual bool showAllSideButtons() const {return true;}
    // below might check to see if the drive actually is two sided.
    virtual bool isTwoSided() const {return true;}

    QString path() const {return path_;}    // root path of this optical disk
    // labelPath() == path() in this class, but see EmulatedOpticalDisk.
    virtual QString labelPath() const {return path_;}  // path to label
    virtual QString labelFilePath() const; // full path to label.dat
    virtual QString studiesPath() const;    // full path of studies dir
    bool isOpticalDiskPath(const QString& path) const {
        return path == path_;}

    virtual ~OpticalDisk();

protected:
    // first bytes of label file
    enum {MagicNumber = 0x99c798f3};    
    static const QString labelFileName_;
    void load(const QString& fileName);
    void save(const QString& fileName) const;

private:
    OpticalDisk(OpticalDisk&);
    
    static const QString studiesDirName_;
    QString path_;  // optical drive path, all the way to study dir
                    // e.g. /home/user/MyStudies
    LabelData labelData_;
    bool isLabeled_;
};

class EmulatedOpticalDisk : public OpticalDisk {
public:
    EmulatedOpticalDisk(const QString& path, bool isTwoSided = false);

    static EmulatedOpticalDisk* getLastDisk(const QString& path);    // returns last disk or 0 if none

    void eject(QWidget*);

    void readLabel();
    void writeLabel() const;
    virtual void saveLastDisk();   // saves last diskName


    // allowing relabeling of emulated disks will mess up the path to the emulated disk,
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

    ~EmulatedOpticalDisk();

private:
    EmulatedOpticalDisk(const QString& path, const QString& diskName);
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

#endif
