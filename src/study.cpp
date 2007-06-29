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

#include "epsimdefs.h"
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
        << (Q_INT32)study.sex_ << study.height_ << study.weight_ 
        << study.heightIn_<< study.weightLbs_ << study.bsa_ 
        << (Q_INT32)study.bsaManualEdit_ << (Q_INT32)study.vagalTone_ 
        << (Q_INT32)study.sympatheticTone_ << (Q_INT32)study.ef_
        << (Q_INT32)study.ischemia_ << study.path_ << study.file_ 
        << study.location_ << study.side_ 
        << study.machineName_ << study.labName_;
    ///TODO need to add heart to this
    return out;
}

QDataStream& operator>>(QDataStream& in, Study& study) {
    // Data below is not natively a 32 bit int.
    Q_INT32 sex, bsaManualEdit, vagalTone, sympatheticTone, ef, ischemia;
    in >> study.dateTime_ >> study.number_ >> study.name_
        >> study.mrn_ >> study.dateOfBirth_ >> study.config_
        >> sex >> study.height_ >> study.weight_ >> study.heightIn_
        >> study.weightLbs_ >> study.bsa_ >> bsaManualEdit
        >> vagalTone >> sympatheticTone >> ef
        >> ischemia >> study.path_ >> study.file_ >> study.location_
        >> study.side_ >> study.machineName_ >> study.labName_;
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
    file_ = "";
    path_ = "";
    location_ = "";
    labName_ = "";
    machineName_ = "";
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
    file_ = study.file_;
    location_ = study.location_;
    side_ = study.side_;
    machineName_ = study.machineName_;
    labName_ = study.labName_;
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
        return QString::null;
    if (key_.isEmpty())
        key_ = name_.last.stripWhiteSpace() + "_" 
        + name_.first.stripWhiteSpace()
        + "_" + dateTime_.toString("ddMMyyyyhhmmsszzz");
    return key_;
}

/**
 * Returns name of file containing study data, without full path.
 * file_ is data member that stores this name.  It is originally 
 * empty (i.e. = ""), initialized by Navigator ctor.  It stays empty
 * when a Pre-register study is created.  When a real study is created,
 * the original patient name and key generated by key() are used to
 * generate the file name.  This is then the permanent file name, even
 * if key or patient name later change.  
 * @return data file name
 */
///TODO allow user to rename study files to more meaningful names?
QString Study::fileName() {
    if (!isPreregisterStudy())
        return key() + ".eps";
    else
        return "";
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
    if (!(path_.endsWith("/") ))
        path_ += "/";
    return QDir::cleanDirPath(path_ + studyFileName_);
}

Study::~Study() {
    delete heart_;
}
