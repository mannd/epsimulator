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

    /// Change the disk.  This is a dialog box for a real optical disk, and the
    /// changing is implemented through hardware.  
    virtual void eject();
    /// 
    virtual void relabel(QString& newLabel) {setLabel(newLabel);}

    virtual void getLabel();
    virtual void setLabel(const QString& label);
//    virtual void writeLabel(const QString& label);
    virtual void setSide(const QString& side);
    virtual void setIsTwoSided(bool isTwoSided) {isTwoSided_ = isTwoSided;}

    virtual bool isPresent() {return false;}
    virtual QString label();
    virtual bool isTwoSided() const {return isTwoSided_;}
    virtual QString side() const {return side_;}
    virtual QString path() const {return path_;}

    virtual ~OpticalDisk();

protected:
    // first bytes of label file
    enum {MagicNumber = 0x99c798f3};    

    bool load(const QString& fileName);
    bool save(const QString& fileName);

    void error(const QFile& file, const QString& message);
    void ioError(const QFile& file, const QString& message);

    void readSettings();

    QString label_;
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


    virtual ~EmulatedOpticalDisk();

};

#endif
