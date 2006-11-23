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

#include "epsim.h"
#include "study.h"
#include "heart.h"

#include <cassert> 
#include <qdir.h>

QDataStream& operator<<(QDataStream& out, const Name& name) {
    out << name.first << name.middle << name.last;
    return out;
}

QDataStream& operator>>(QDataStream& in, Name& name) {
    in >> name.first >> name.middle >> name.last;
    return in;
}


QString Name::fullName(bool lastFirst, bool useMiddleName) const {
    QString middleName;
    if (useMiddleName && !middle.isEmpty()) 
	middleName = " " + middle.stripWhiteSpace() + " ";
    else
	middleName = " ";
    if (lastFirst) 
	return (last.stripWhiteSpace() + ", " + first.stripWhiteSpace()
                 + middleName).stripWhiteSpace();
    else
	return first.stripWhiteSpace() + middleName + last.stripWhiteSpace();
}

QDataStream& operator<<(QDataStream& out, const Study& study) {
    out << study.dateTime_ << study.number_ << study.name_
        << study.mrn_ << study.dateOfBirth_ << study.config_
        << (Q_INT32)study.sex_ << study.height_ << study.weight_ << study.heightIn_
        << study.weightLbs_ << study.bsa_ << (Q_INT32)study.bsaManualEdit_
        << (Q_INT32)study.vagalTone_ << (Q_INT32)study.sympatheticTone_ << (Q_INT32)study.ef_
        << (Q_INT32)study.ischemia_ << study.path_ << study.file_;
    ///TODO need to add heart to this
    return out;
}

QDataStream& operator>>(QDataStream& in, Study& study) {
    Q_INT32 sex, bsaManualEdit, vagalTone, sympatheticTone, ef, ischemia;
    in >> study.dateTime_ >> study.number_ >> study.name_
        >> study.mrn_ >> study.dateOfBirth_ >> study.config_
        >> sex >> study.height_ >> study.weight_ >> study.heightIn_
        >> study.weightLbs_ >> study.bsa_ >> bsaManualEdit
        >> vagalTone >> sympatheticTone >> ef
        >> ischemia >> study.path_ >> study.file_;
    ///TODO need to add heart to this
    ///TODO the below may not work, as it has nothing to do with in
    study.sex_ = (sex != 0) ? Female : Male;
    study.bsaManualEdit_ = bsaManualEdit;
    study.vagalTone_ = vagalTone;
    study.sympatheticTone_ = sympatheticTone;
    study.ef_ = ef;
    study.ischemia_ = ischemia;
    return in;
}

Study::Study() : dateTime_(QDateTime::currentDateTime()),
    dateOfBirth_(DEFAULT_BIRTH_DATE),
    sex_(Male), height_(0), weight_(0), 
    heightIn_(0), weightLbs_(0), bsa_(0), 
    bsaManualEdit_(false), vagalTone_(DEFAULT_VAGAL_TONE),
    sympatheticTone_(DEFAULT_SYMPATHETIC_TONE), ef_(DEFAULT_EF), 
    ischemia_(false) {
    ///TODO need to compute path
    config_ = "";
    file_ = "";
    path_ = "";
    heart_ = new Heart;
    testInvariant();
}

void Study::copyStudy(const Study& study) {
    name_ = study.name_;    // default copy constructor works here
    dateTime_ = study.dateTime_;
    dateOfBirth_ = study.dateOfBirth_;
    mrn_ = study.mrn_;
    number_ = study.number_;
    sex_ = study.sex_;
    height_ = study.height_;
    weight_ = study.weight_;
    heightIn_ = study.heightIn_;
    weightLbs_ = study. weightLbs_;
    bsa_ = study.bsa_;
    bsaManualEdit_ = study.bsaManualEdit_;
    vagalTone_ = study.vagalTone_;
    sympatheticTone_ = study.sympatheticTone_;
    ef_ = study.ef_;
    ischemia_ = study.ischemia_;
    config_ = study.config_;
    path_ = study.path_;
    file_ = study.file_;
    // copy the heart pointer
    heart_ = new Heart(*study.heart_);
}

Study::Study(const Study& study) {
    copyStudy(study);
    testInvariant();
}

QString Study::key() const {
    if (name_.last.isNull()) 
        return QString::null;
    return name_.last.stripWhiteSpace() + "_" + name_.first.stripWhiteSpace()
         + "." + dateTime_.toString("dd.MM.yyyy.hh.mm.ss");
}

void Study::setName(const Name& name) {
    name_.last = name.last;
    name_.first = name.first;
    name_.middle = name.middle;
}

AutonomicTone Study::adjustTone(AutonomicTone tone) {
    tone = tone > MAX_TONE ? MAX_TONE : tone;
    tone = tone < MIN_TONE ? MIN_TONE : tone;
    return tone;
}

void Study::setVagalTone(AutonomicTone tone) {
    vagalTone_ = adjustTone(tone);
}

void Study::setSympatheticTone(AutonomicTone tone) {
    sympatheticTone_ = adjustTone(tone);
}

Study& Study::operator =(const Study& rhs) {
    if (this == &rhs)
        return *this;
    copyStudy(rhs);
    testInvariant();
    return *this;
   
}

void Study::setEf(int ef) {
    ef_ = ef > MAX_EF ? MAX_EF : ef;
    ef_ = ef < MIN_EF ? MIN_EF : ef;
}

inline void Study::testInvariant() const {
    assert(vagalTone_ >= MIN_TONE && vagalTone_ <= MAX_TONE);
    assert(sympatheticTone_ >= MIN_TONE && sympatheticTone_ <= MAX_TONE);
    assert (sex_ == Male || sex_ == Female);
    assert (ef_ >= MIN_EF && ef_ <= MAX_EF);
}

///TODO make sure this works under windows.  According to Qt documentation 
/// should.  See QDir::covert()
QString Study::filePath() {
    if (! (path_.endsWith("/") || file_.startsWith("/")))
        path_ += "/";
    return QDir::cleanDirPath(path_ + file_);
}

Study::~Study() {
    delete heart_;
}
