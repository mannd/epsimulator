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

#include "patientdefs.h"
#include "bloodpressure.h"
#include "saturation.h"

#include <QtCore/QCoreApplication>


class QDataStream;

namespace EpPatient {
    class Heart;

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

    void updatePhysiology();

    void setPath(const QString& path) {path_ = path;}

    void setO2Saturation(const Saturation& sat) {o2Saturation_ = sat;}
    void setBp(const BloodPressure& bp) {bp_ = bp;}
    void setHeartRate(const HeartRate rate);
    void setRespRate(const RespRate rate) {respRate_ = rate;}

    Saturation o2Saturation() const {return o2Saturation_;}
    BloodPressure bp() const {return bp_;}
    HeartRate heartRate() const;
    Ep::Interval meanCL() const;
    RespRate respRate() const {return respRate_;}

private:
    enum {MagicNumber = 0x99379335};  // for patient.dat file

    QString filePath();  // get full path to patient file

    static const QString fileName_;

    QString name_;
    QString path_;      // path to patient.dat file
    Saturation o2Saturation_;
    BloodPressure bp_;
    RespRate respRate_;
    Heart* heart_;
    unsigned long int secs_;    // total simulation time

};

}

#endif
