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

#include "study.h"

#include "epfuns.h"
#include "error.h"
#include "heart.h"

#include <qdir.h>

#ifndef NDEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

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
	middleName = " " + middle.simplified() + " ";
    else
	middleName = " ";
    if (lastFirst) 
	return (last.simplified() + ", " + first.simplified()
                 + middleName).simplified();
    else
	return first.simplified() + middleName + last.simplified();
}

QDataStream& operator<<(QDataStream& out, const Study& study) {
    out << study.dateTime_ << study.number_ << study.name_
        << study.mrn_ << study.dateOfBirth_ << study.config_
        << (qint32)study.sex_ << study.height_ << study.weight_ 
        << study.heightIn_<< study.weightLbs_ << study.bsa_ 
        << (qint32)study.bsaManualEdit_ << (qint32)study.vagalTone_ 
        << (qint32)study.sympatheticTone_ << (qint32)study.ef_
        << (qint32)study.ischemia_ << study.path_;
    ///TODO need to add heart to this
    return out;
}

QDataStream& operator>>(QDataStream& in, Study& study) {
    // Data below is not natively a 32 bit int.
    qint32 sex, bsaManualEdit, vagalTone, sympatheticTone, ef, ischemia;
    in >> study.dateTime_ >> study.number_ >> study.name_
        >> study.mrn_ >> study.dateOfBirth_ >> study.config_
        >> sex >> study.height_ >> study.weight_ >> study.heightIn_
        >> study.weightLbs_ >> study.bsa_ >> bsaManualEdit
        >> vagalTone >> sympatheticTone >> ef
        >> ischemia >> study.path_;
    ///TODO need to add heart to this
    study.sex_ = (sex != 0) ? Female : Male;
    study.bsaManualEdit_ = bsaManualEdit;
    study.vagalTone_ = vagalTone;
    study.sympatheticTone_ = sympatheticTone;
    study.ef_ = ef;
    study.ischemia_ = ischemia;
    return in;
}

const QString Study::studyFileName_ = "study.dat";

/**
 *  Study ctor.  New Study is initialized to certain defaults.  
 */
Study::Study() : dateTime_(QDateTime::currentDateTime()),
                 dateOfBirth_(DEFAULT_BIRTH_DATE),
                 sex_(Male), height_(0), weight_(0), 
                 heightIn_(0), weightLbs_(0), bsa_(0), 
                 bsaManualEdit_(false), 
                 vagalTone_(DEFAULT_VAGAL_TONE),
                 sympatheticTone_(DEFAULT_SYMPATHETIC_TONE), 
                 ef_(DEFAULT_EF), 
                 ischemia_(false) {
    ///TODO need to compute path
    config_ = "";
    path_ = "";
    key_ = "";
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
    key_ = study.key_;
    // copy the heart pointer
    heart_ = new Heart(*study.heart_);
}

Study::Study(const Study& study) {
    copyStudy(study);
    testInvariant();
}

/**
 * Generates unique key for each study, to be used for 
 * searching, etc.  Once generated, will not change,
 * even if name, study date, etc. change.
 */
QString Study::key() const {
    // Under normal circumstances PatientDialog won't allow a 
    // blank last name, so shouldn't happen.
    assert(!name_.last.isNull());
    if (name_.last.isNull())
        return QString();
    if (key_.isEmpty())
        key_ = name_.last.simplified() + "_" 
        + name_.first.simplified()
        + "_" + dateTime_.toString("ddMMyyyyhhmmsszzz");
    return key_;
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


QString Study::filePath() {
    return QDir::cleanPath(path_ + "/" + studyFileName_);
}

void Study::save() {
 EpCore::saveData(filePath(), MagicNumber, *this);
}

void Study::load() {
 EpCore::loadData(filePath(), MagicNumber, *this);
}

Study::~Study() {
    delete heart_;
}