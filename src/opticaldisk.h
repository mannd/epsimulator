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

    virtual bool hasLabel();
    virtual void readLabel();
    virtual void writeLabel() const;
    void setLabelData(const LabelData&);
    void setLabel(const QString& label);
    void setSide(const QString& side);
    virtual QString translatedSide() const;

    LabelData labelData() const;
    QString label() const; 
    QString side() const;

    virtual bool allowSideChange() const {return true;}
    virtual bool singleSideOnly() const {return false;}

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
};

class EmulatedOpticalDisk : public OpticalDisk {
public:
    EmulatedOpticalDisk(const QString& path, bool isTwoSided = false);

    virtual void eject(QWidget*);

    virtual void readLabel();
    virtual void writeLabel() const;
    virtual bool hasLabel();

//    virtual QString path();

    virtual QString filePath() const;

    // allowing relabeling of emulated disks will mess up the path to the emulated disk,
    // so don't allow this...
    virtual bool allowSideChange() const {return false;} 
    virtual bool singleSideOnly() const {return !isTwoSided_;}

    void setIsTwoSided(bool isTwoSided) {isTwoSided_ = isTwoSided;}
    bool isTwoSided() const {return isTwoSided_;}
    QString diskName() const {return diskName_;}

    virtual ~EmulatedOpticalDisk();

private:
    void makeLabel(const QString& fileName, QString& label, QStringList& labelList);

    void makePath() const;    // create subdirectories for the emulated disk

    QString disksPath() const;  // e.g. .../MyStudies/disks
    QString diskPath() const;   // /home/user/MyStudies/disks/disk_xxxxx
    QString sideDir() const;    // A or B
    virtual QString fullPath() const;   // .../MyStudies/disks/disk_xxxxx/B
    virtual QString studiesPath() const;  
                                // /.../MyStudies/disks/disk_xxxxx/B/studies

    void lastDisk();      // loads up last diskName
    void saveLastDisk();   // saves last diskName

    bool isTwoSided_;
    QString diskName_;
};

#endif
