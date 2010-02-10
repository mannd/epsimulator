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
#ifndef HEART_H
#define HEART_H

#include "epdefs.h"
#include "patientdefs.h"

#include <QtCore/QCoreApplication>

class QDataStream;

namespace EpPatient {

/**
 * Heart simulation.  For now essentially an empty class.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Heart {
    Q_DECLARE_TR_FUNCTIONS(Heart)

public:
    friend QDataStream& operator<<(QDataStream&, const Heart&);
    friend QDataStream& operator>>(QDataStream&, Heart&);

    Heart(const QString& name = tr("<default>"));
    Heart (const Heart& heart);
    ~Heart();

    QString name() {return name_;}

    // do nothing below for now
    void setHeartRate(HeartRate) {}

    // these functions are used to update Patient physiology numbers
    HeartRate meanHeartRate() const;
    Ep::CL meanCL() const;

private:
    QString name_;
};

}

#endif
