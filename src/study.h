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

#include <qdatetime.h>
#include <qstring.h>

#include <vector>

class Heart;

struct Name {
    QString first;
    QString last;
    QString middle;
};

typedef int AutonomicTone;

enum Sex {MALE, FEMALE};

class Study {
    friend void studyCopy(Study* lhs, Study* rhs);
public:
    Study();

    Name name() const {return name_;};
    QString fullName(bool lastFirst = false,
                     bool useMiddleName = false) const;
    QString mrn() const {return mrn_;}
    QDate dateOfBirth() const {return dateOfBirth_;}
    Sex sex() const {return sex_;}
    double height() const {return height_;}
    double weight() const {return weight_;}
    double heightIn() const {return heightIn_;}
    double weightLbs() const {return weightLbs_;}
    bool bsaManualEdit() const {return bsaManualEdit_;}
    int ef() const {return ef_;}
    bool hasIschemia() const {return ischemia_;}
    AutonomicTone vagalTone() const {return vagalTone_;}
    AutonomicTone sympatheticTone() const {return sympatheticTone_;}
    QDate date() const;
    QTime time() const;
    QString number() const;

    void setBsa(const int bsa) {bsa_ = bsa;}
    void setName(const Name name);
    void setMrn(const QString mrn) {mrn_ = mrn;}
    void setHeight(const double height) {height_ = height;}
    void setWeight(const double weight) {weight_ = weight;}
    void setHeightIn(const double heightIn) {heightIn_ = heightIn;}
    void setWeightLbs(const double weightLbs) {weightLbs_ = weightLbs;}
    void setBsaManualEdit(const bool bsaManualEdit) {
	bsaManualEdit_ = bsaManualEdit;}
    void setDate(QDate date);
    void setTime(QTime time);
    void setNumber(QString number);

    ~Study();

private:
    QDate date_;
    QTime time_;
    QString number_;
    Name name_;
    QString mrn_;	// medical record number
    QDate dateOfBirth_;
    Sex sex_;
    double height_;	// use metric units: cm for height,
    double weight_;	// in kg
    double heightIn_;   // English versions
    double weightLbs_;
    double bsa_;  // BSA either manually edited or calculated
    bool bsaManualEdit_;  // should BSA be manually edited or calculated?
    Heart *heart_;
    AutonomicTone vagalTone_;
    AutonomicTone sympatheticTone_;
    int ef_;
    bool ischemia_;
    // other factors?
};

typedef std::vector<Study*> Studies;

#endif
