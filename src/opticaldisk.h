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
};

/**
 * Base class for real optical disk and optical disk emulation.
 *	@author David Mann <mannd@epstudiossoftware.com>
 */
class OpticalDisk {
public:
    OpticalDisk(const QString& path);

    virtual void eject(QWidget*);

    bool hasLabel();

    bool isLabeled() const {return isLabeled_;}
    void setIsLabeled(bool isLabeled) {isLabeled_ = isLabeled;}

    virtual void readLabel();
    virtual void writeLabel() const;
    void setLabelData(const LabelData&);
    void setLabel(const QString& label);
    void setSide(const QString& side);
    virtual QString translatedSide() const;

    static QString translateSide(const QString& side);

    LabelData labelData() const;
    QString label() const; 
    QString side() const;

    virtual bool allowSideChange() const {return true;}
    virtual bool showAllSideButtons() const {return true;}
    virtual bool singleSideOnly() const {return false;}
    // below might check to see if the drive actually is two sided.
    virtual bool isTwoSided() const {return true;}

    QString path() const {return path_;}
    // fullPath() == path() in this class, but see EmulatedOpticalDisk.
    virtual QString fullPath() const {return path_;}  
    virtual QString filePath() const; // full path to label.dat
    virtual QString studiesPath() const; 

    virtual ~OpticalDisk();

protected:
    // first bytes of label file
    enum {MagicNumber = 0x99c798f3};    
    static const QString labelFileName_;
    void load(const QString& fileName);
    void save(const QString& fileName) const;

private:

    QString path_;  // optical drive path, all the way to study dir
                    // e.g. /home/user/MyStudies
    LabelData labelData_;
    bool isLabeled_;
};

class EmulatedOpticalDisk : public OpticalDisk {
public:
    EmulatedOpticalDisk(const QString& path, bool isTwoSided = false);
    EmulatedOpticalDisk(const QString& path, const QString& diskName,
                        const QString& side);

    virtual void eject(QWidget*);

    virtual void readLabel();
    virtual void writeLabel() const;
    bool hasLabel();

//    virtual QString path();

    virtual QString filePath() const;

    // allowing relabeling of emulated disks will mess up the path to the emulated disk,
    // so don't allow this...
    virtual bool allowSideChange() const {return !isLabeled() && isTwoSided_;} 
    virtual bool singleSideOnly() const {return !isTwoSided_;}
    virtual bool showAllSideButtons() const {return false;}


    void setIsTwoSided(bool isTwoSided) {isTwoSided_ = isTwoSided;}
    bool isTwoSided() const {return isTwoSided_;}
    QString diskName() const {return diskName_;}

    virtual ~EmulatedOpticalDisk();

private:
    struct DiskInfo {
        QString name;
        LabelData labelData;
    };
    typedef std::vector<DiskInfo*> DiskInfoList;

    bool makeLabel(const QString& diskName, QStringList& labelList,
                   DiskInfoList& diskInfoList, int& row);

    void makePath() const;    // create subdirectories for the emulated disk
                                               //  i.e. has label.dat in it

    QString disksPath() const;  // e.g. .../MyStudies/disks
    QString diskPath() const;   // /home/user/MyStudies/disks/disk_xxxxx
    QString sideDir() const;    // A or B
    virtual QString fullPath() const;   // .../MyStudies/disks/disk_xxxxx/B
    virtual QString studiesPath() const;  
                                // /.../MyStudies/disks/disk_xxxxx/B/studies

    void lastDisk();      // loads up last diskName
    void saveLastDisk();   // saves last diskName

    bool isTwoSided_;
//    bool allowSideChange_;
    QString diskName_;
};

#endif
