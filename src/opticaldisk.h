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

class Options;

/**
Base class for real optical disk and optical disk emulation.
TODO At this point (and probably never) we don't want to try to replicate the
streaming data storage to an actual optical disk, a la Prucka.  It is
not clear if it can even work with traditional optical media (as opposed to
what the Prucka system uses, proprietary disks I suppose.  However, we
do want to have an OpticalDisk base class, whose functions can, if we
wish, someday be fleshed out to provide true optical disk storage.
Similar comments obviously apply to the OpticalDiskDrive class too.

Probably should eliminate OpticalDiskDrive class -- we are either modeling something
that already exists as hardware, or something that can be easily assimilated into
the OpticalDisk class, e.g. whether a disk is present, ejecting a disk, relabeling
a disk -- these are all actions on the disk, and the optical drive itself is
not that important.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class OpticalDisk {
public:
    OpticalDisk();

    /// Change the disk.  This is a dialog box for a real optical disk, and the
    /// changing is implemented through hardware.  
    virtual void eject();
    /// 
    virtual void relabel(QString& newLabel) {}

    virtual QString getLabel();
    virtual void setLabel(const QString& label) {label_ = label;}
    virtual void writeLabel(const QString& label) {}
    virtual void setSide(const QString& side);
    virtual void setIsTwoSided(bool isTwoSided) {isTwoSided_ = isTwoSided;}

    virtual bool isPresent() {return false;}
    virtual QString label() const {return label_;}
    virtual bool isTwoSided() const {return isTwoSided_;}
    virtual QString side() const {return side_;}
    virtual QString path() const {return path_;}

    virtual ~OpticalDisk();

protected:
    QString label_;
    bool isTwoSided_;
    QString side_;  // A or B
    // This is the name of the file specifying the disk label.
    static const QString labelFileName_;
    Options* options_;
    QString path_;  // optical drive path, all the way to study dir
    
    

};

class EmulatedOpticalDisk : public OpticalDisk {
public:
    EmulatedOpticalDisk();

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

class OpticalDiskFactory {
public:
    static OpticalDisk* instance();
private:
    OpticalDiskFactory();
    OpticalDiskFactory(OpticalDiskFactory&);
};


#endif
