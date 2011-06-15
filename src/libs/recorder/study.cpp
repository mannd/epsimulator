/***************************************************************************
 *   Copyright (C) 2006 - 2011 by EP Studios, Inc.                         *
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

#include "error.h"
#include "fileutilities.h"
#include "studyconfiguration.h"

#include <QDataStream>
#include <QDir>
#include <QUuid>

namespace EpStudy {

using EpPatient::Sex;
using EpPatient::AutonomicTone;
using EpPatient::Name;

const int defaultEf = 55;
const QDate defaultBirthDate = QDate::currentDate().addYears(-50);
const AutonomicTone defaultVagalTone = 70;
const AutonomicTone defaultSympatheticTone = 30;


const int Study::minEf_;
const int Study::maxEf_;
const int Study::minTone_;
const int Study::maxTone_;
const QString Study::fileName_ = "study.dat";
const QString Study::configFileName_ = "config.dat";

Study::Study() 
    : dateTime_(QDateTime::currentDateTime()),
      name_(),
      dateOfBirth_(defaultBirthDate),
      mrn_(), number_(), accountNumber_(),
      sex_(EpPatient::Male), height_(0), weight_(0),
      heightIn_(0), weightLbs_(0), bsa_(0),
      bsaManualEdit_(false),
      vagalTone_(defaultVagalTone),
      sympatheticTone_(defaultSympatheticTone),
      ef_(defaultEf),
      ischemia_(false),
      path_(),
      isPregisterStudy_(true),
      heartName_(QObject::tr("<default>")) {
    studyConfiguration_ = new StudyConfiguration;
    //key_ = QUuid::createUuid().toString();
    testInvariant();
}


Study::Study(const Study& study) {
    copyStudy(study);
    testInvariant();
}

Study::~Study() {
    delete studyConfiguration_;
}

Study& Study::operator =(const Study& rhs) {
    if (this == &rhs)
        return *this;
    copyStudy(rhs);
    testInvariant();
    return *this;
}

void Study::load() {
    EpCore::loadData(filePath(), MagicNumber, *this);
}

void Study::save() {
    EpCore::saveData(filePath(), MagicNumber, *this);
}

void Study::setStudyConfiguration(StudyConfiguration *config) {
    delete studyConfiguration_;
    studyConfiguration_ = config;
}

void Study::loadStudyConfiguration() {
    delete studyConfiguration_;
    studyConfiguration_ = new StudyConfiguration;
    EpCore::loadData(configFilePath(),
                     StudyConfiguration::magicNumber(),
                     *studyConfiguration_);
}

void Study::saveStudyConfiguration() {
    EpCore::saveData(configFilePath(),
                     StudyConfiguration::magicNumber(),
                     *studyConfiguration_);
}

void Study::setName(const Name& name) {
    name_ = name;
}

void Study::setEf(int ef) {
    ef_ = qBound(minEf_, ef, maxEf_);
}

void Study::setVagalTone(AutonomicTone tone) {
    vagalTone_ = adjustTone(tone);
}

void Study::setSympatheticTone(AutonomicTone tone) {
    sympatheticTone_ = adjustTone(tone);
}


// Generates unique key for each study, to be used for 
// searching, etc.  Once generated, will not change,
// even if name, study date, etc. change.
QString Study::key() const {
    if (key_.isEmpty())
        key_ = QUuid::createUuid().toString();
    return key_;
}

QString Study::filePath() {
    Q_ASSERT(!path_.isEmpty());
    return EpCore::joinPaths(path_ , fileName_);
}

QString Study::configFilePath() {
    Q_ASSERT(!path_.isEmpty());
    return EpCore::joinPaths(path_, configFileName_);
}

// private

AutonomicTone Study::adjustTone(AutonomicTone tone) {
    return tone = qBound(minTone_, tone, maxTone_);
}

void Study::copyStudy(const Study& study) {
    name_ = study.name_;    // default copy constructor works here
    dateTime_ = study.dateTime_;
    dateOfBirth_ = study.dateOfBirth_;
    mrn_ = study.mrn_;
    number_ = study.number_;
    accountNumber_ = study.accountNumber_;
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
    path_ = study.path_;
    key_ = study.key_;
    isPregisterStudy_ = study.isPregisterStudy_;
    heartName_ = study.heartName_;
    // copy the heart pointer
    //heart_ = new Heart(*study.heart_);
    studyConfiguration_ = new StudyConfiguration(*study.studyConfiguration_);
}

// friends to Study

QDataStream& operator<<(QDataStream& out, const Study& study) {
    out << study.dateTime_ << study.number_ << study.name_
        << study.mrn_ << study.accountNumber_
        << study.dateOfBirth_
        << (qint32)study.sex_
        << study.height_ << study.weight_
        << study.heightIn_<< study.weightLbs_ << study.bsa_
        << (qint32)study.bsaManualEdit_ << (qint32)study.vagalTone_
        << (qint32)study.sympatheticTone_ << (qint32)study.ef_
        << (qint32)study.ischemia_ << study.path_ << study.key_
        << study.isPregisterStudy_
        << study.heartName_
        << *study.studyConfiguration_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Study& study) {
    // Data below is not natively a 32 bit int.
    qint32 sex, bsaManualEdit, vagalTone, sympatheticTone, ef, ischemia;
    in >> study.dateTime_ >> study.number_ >> study.name_
       >> study.mrn_ >> study.accountNumber_
       >> study.dateOfBirth_
       >> sex >> study.height_
       >> study.weight_ >> study.heightIn_
       >> study.weightLbs_ >> study.bsa_ >> bsaManualEdit
       >> vagalTone >> sympatheticTone >> ef
       >> ischemia >> study.path_ >> study.key_
       >> study.isPregisterStudy_
       >> study.heartName_
       >> *study.studyConfiguration_;
    study.sex_ = (sex != 0) ? EpPatient::Female : EpPatient::Male;
    study.bsaManualEdit_ = bsaManualEdit;
    study.vagalTone_ = vagalTone;
    study.sympatheticTone_ = sympatheticTone;
    study.ef_ = ef;
    study.ischemia_ = ischemia;
    return in;
}
}
