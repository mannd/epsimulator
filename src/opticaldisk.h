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

class QFile;

/**
 * Base class for real optical disk and optical disk emulation.
 *	@author David Mann <mannd@epstudiossoftware.com>
 */
class OpticalDisk {
public:
    OpticalDisk(const QString& path, bool isTwoSided = false);

    /// This mechanically ejects the disk, but I don't know how to do that yet.
    /// Errors are ignored, and this does nothing for emulated optical disks.
    virtual void eject() {/** TODO make this work. */}
    /// 
//    virtual void relabel(QString& newLabel) {setLabel(newLabel);}

    virtual void setLabel(const QString& label);
//    virtual void writeLabel(const QString& label);
    virtual void setSide(const QString& side) {side_ = side;}
    virtual void setIsTwoSided(bool isTwoSided) {isTwoSided_ = isTwoSided;}

    virtual bool hasLabel();
//    virtual bool isPresent() const {return hasLabel();}
    virtual QString label();
    virtual bool isTwoSided() const {return isTwoSided_;}
    virtual QString side() const {return side_;}
    virtual QString translatedSide() const;
    virtual QString path() const {return path_;}
    virtual QString filePath() const;

    virtual ~OpticalDisk();

protected:
    // first bytes of label file
    enum {MagicNumber = 0x99c798f3};    

    QString load(const QString& fileName);
    void save(const QString& fileName, const QString& label);

    bool isTwoSided_;
    QString side_;  // A or B
    // This is the name of the file specifying the disk label.
    static const QString labelFileName_;
    QString path_;  // optical drive path, all the way to study dir
    
    

};

class EmulatedOpticalDisk : public OpticalDisk {
public:
    EmulatedOpticalDisk(const QString& path, bool isTwoSided = false);

//     virtual void eject() {}
//     virtual void relabel() {}
// // 
// //    virtual QString side() const {}
// // 
// //    virtual void setLabel(const QString& label) {}
// //     virtual void setSide() {}
// // 
// //    virtual QString label() const {}

    virtual QString path() const;
    virtual QString filePath() const;

    QString diskName() const {return diskName_;}


    virtual ~EmulatedOpticalDisk();

private:
	QString diskName_;

};

#endif
