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

#include <qobject.h>
#include <qstring.h>

/**
Base class for real optical disk and optical disk emulation.
TODO At this point (and probably never) we don't want to try to replicate the
streaming data storage to an actual optical disk, a la Prucka.  It is
not clear if it can even work with traditional optical media (as opposed to
what the Prucka system uses, proprietary disks I suppose.  However, we
do want to have an OpticalDisk base class, whose functions can, if we
wish, someday be fleshed out to provide true optical disk storage.
Similar comments obviously apply to the OpticalDiskDrive class too.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class OpticalDisk {
//    Q_OBJECT
public:
    OpticalDisk();

    virtual void eject() {}
    virtual void relabel() {}
    virtual QString side() const {return side_;}

    virtual void setLabel(const QString& label) {label_ = label;}
//    virtual void setSide(const QString& side);

    virtual QString label() const {return label_;}

    virtual ~OpticalDisk();

protected:
    QString label_;
    bool twoSided_;
    QString side_;  // A or B
    static const char* labelFileName_;    
    

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


#endif
