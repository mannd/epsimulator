/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#ifndef PATIENT_H
#define PATIENT_H

#include "saturation.h"

#include <QtCore/QCoreApplication>

class QDataStream;

namespace EpPatient {

/**
 * real-time patient simulation
 *
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Patient{
    Q_DECLARE_TR_FUNCTIONS(Patient)

public:
    friend QDataStream& operator<<(QDataStream&, const Patient&);
    friend QDataStream& operator>>(QDataStream&, Patient&);

    Patient(const QString& name = tr("<default>"));
    ~Patient();

    void load();        
    void save();        

    void setPath(const QString& path) {path_ = path;}

    Saturation o2Saturation();

private:
    enum {MagicNumber = 0x99379335};  // for patient.dat file

    QString filePath();  // get full path to patient file

    static const QString fileName_;

    QString name_;
    QString path_;      // path to patient.dat file
    Saturation o2Saturation_;

    // patient physiology
};

}

#endif
