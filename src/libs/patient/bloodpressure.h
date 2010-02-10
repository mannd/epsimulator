/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#ifndef BLOODPRESSURE_H
#define BLOODPRESSURE_H

#include <QtCore/QString>

namespace EpPatient {

class BloodPressure {
public:
    friend QDataStream& operator<<(QDataStream&, const BloodPressure&);
    friend QDataStream& operator>>(QDataStream&, BloodPressure&);

    BloodPressure(const int systolic, const int diastolic);

    QString bp() const;
    int systolicBp() const {return systolic_;}
    int diastolicBp() const {return diastolic_;}
    int meanBp() const;
    int meanBp(int heartRate) const;

    void setSystolic(const int systolic);
    void setDiastolic(const int diastlic);
    void setBp(const int systolic, const int diastolic);

    static const int rateCutoff; // heart rate at which mean BP formula changes

private:
    void normalizeBp();

    int systolic_;
    int diastolic_;
};

}

#endif // BLOODPRESSURE_H
