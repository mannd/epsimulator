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

#ifndef STUDY_H
#define STUDY_H

#include "patient.h"

#include <qdatetime.h>
#include <qstring.h>

class Study {
   
public:
    Study();
    void setPatient(Patient *patient);
    void setDate(QDateTime& date);
    void setTime(QDateTime& time);
    void setNumber(QString& number);

    Patient* patient() const;
    QDateTime date() const;
    QDateTime time() const;
    QString number() const;
    
    ~Study();

private:
    Patient *patient_;
    QDateTime date_;
    QDateTime time_;
    QString number_;
    
};

#endif
