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

#include "error.h"
#include "fileutilities.h"
#include "heart.h"
#include "studyconfiguration.h"

#include <QDir>

namespace EpStudy {

using EpPatient::Heart;

Name::Name(const QString& last, 
           const QString& first, 
           const QString& middle) {
    setLastFirstMiddle(last, first, middle);
}

void Name::setLastFirstMiddle(const QString& last, 
                              const QString& first, 
                              const QString& middle) {
    // QString::simplified() removes surround whitespace
    // and shortens internal whitespace to one space.
    last_ = last.simplified();
    first_ = first.simplified();
    middle_ = middle.simplified();
}

QString Name::firstMiddleLast() const {
    return fullName(false, true);
}

QString Name::lastFirstMiddle() const {
    return fullName(true, true);
}

QString Name::lastFirst() const {
    return fullName(true, false);
}

QString Name::fullName(bool lastFirst, bool useMiddleName) const {
    QString middleName;
    if (useMiddleName && !middle_.isEmpty()) 
	middleName = " " + middle_ + " ";
    else
	middleName = " ";
    if (lastFirst) 
	return (last_ + ", " + first_ + middleName).simplified();
    else
	return (first_ + middleName + last_).simplified();
}

QDataStream& operator<<(QDataStream& out, const Name& name) {
    out << name.first_ << name.middle_ << name.last_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Name& name) {
    in >> name.first_ >> name.middle_ >> name.last_;
    return in;
}

const QString Study::fileName_ = "study.dat";
const QString Study::configFileName_ = "config.dat";

Study::Study() : dateTime_(QDateTime::currentDateTime()),
                 name_(),
                 dateOfBirth_(DEFAULT_BIRTH_DATE),
                 mrn_(), number_(), accountNumber_(),
                 sex_(Male), height_(0), weight_(0), 
                 heightIn_(0), weightLbs_(0), bsa_(0), 
                 bsaManualEdit_(false), 
                 vagalTone_(DEFAULT_VAGAL_TONE),
                 sympatheticTone_(DEFAULT_SYMPATHETIC_TONE), 
                 ef_(DEFAULT_EF), 
                 ischemia_(false), 
                 config_(), path_(),
                 studyConfiguration_(0) {
    heart_ = new Heart;
    studyConfiguration_ = new StudyConfiguration;
    testInvariant();
}


Study::Study(const Study& study) {
    copyStudy(study);
    testInvariant();
}

Study::~Study() {
    delete heart_;
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

void Study::loadStudyConfiguration() {
    qDebug() << configFilePath();
    delete studyConfiguration_;
    studyConfiguration_ = new StudyConfiguration;
    EpCore::loadData(configFilePath(), 
		     StudyConfiguration::MagicNumber, 
		     *studyConfiguration_);
    qDebug() << studyConfiguration_->name();
}

void Study::saveStudyConfiguration() {
    EpCore::saveData(configFilePath(), 
		     StudyConfiguration::MagicNumber, 
		     *studyConfiguration_);
}

void Study::setName(const Name& name) {
    name_ = name;
}

void Study::setEf(int ef) {
    int min = MIN_EF;
    int max = MAX_EF;
    ef_ = qBound(min, ef, max);
}

void Study::setVagalTone(AutonomicTone tone) {
    vagalTone_ = adjustTone(tone);
}

void Study::setSympatheticTone(AutonomicTone tone) {
    sympatheticTone_ = adjustTone(tone);
}

/**
 * Generates unique key for each study, to be used for 
 * searching, etc.  Once generated, will not change,
 * even if name, study date, etc. change.
 */
QString Study::key() const {
    // Under normal circumstances PatientDialog won't allow a 
    // blank last name, so shouldn't happen.
    Q_ASSERT(!name_.last().isEmpty());
    if (name_.last().isEmpty())
        return QString();
    if (key_.isEmpty())
        key_ = name_.last().simplified() + "_" 
        + name_.first().simplified()
        + "_" + dateTime_.toString("ddMMyyyyhhmmsszzz");
    return key_;
}

QString Study::filePath() {
    return QDir::cleanPath(path_ + "/" + fileName_);
}

QString Study::configFilePath() {
    return QDir::cleanPath(path_ + "/" + configFileName_);
}

// private

AutonomicTone Study::adjustTone(AutonomicTone tone) {
    tone = tone > MAX_TONE ? MAX_TONE : tone;
    tone = tone < MIN_TONE ? MIN_TONE : tone;
    return tone;
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
    config_ = study.config_;
    path_ = study.path_;
    key_ = study.key_;
    // copy the heart pointer
    heart_ = new Heart(*study.heart_);
    studyConfiguration_ = new StudyConfiguration(*study.studyConfiguration_);
}

// friends to Study

QDataStream& operator<<(QDataStream& out, const Study& study) {
    out << study.dateTime_ << study.number_ << study.name_
        << study.mrn_ << study.accountNumber_ 
        << study.dateOfBirth_ << study.config_
        << (qint32)study.sex_ << study.height_ << study.weight_ 
        << study.heightIn_<< study.weightLbs_ << study.bsa_ 
        << (qint32)study.bsaManualEdit_ << (qint32)study.vagalTone_ 
        << (qint32)study.sympatheticTone_ << (qint32)study.ef_
        << (qint32)study.ischemia_ << study.path_ << study.key_;
    ///TODO need to add heart to this
    return out;
}

QDataStream& operator>>(QDataStream& in, Study& study) {
    // Data below is not natively a 32 bit int.
    qint32 sex, bsaManualEdit, vagalTone, sympatheticTone, ef, ischemia;
    in >> study.dateTime_ >> study.number_ >> study.name_
        >> study.mrn_ >> study.accountNumber_ 
        >> study.dateOfBirth_ >> study.config_
        >> sex >> study.height_ >> study.weight_ >> study.heightIn_
        >> study.weightLbs_ >> study.bsa_ >> bsaManualEdit
        >> vagalTone >> sympatheticTone >> ef
        >> ischemia >> study.path_ >> study.key_;
    ///TODO need to add heart to this
    study.sex_ = (sex != 0) ? Female : Male;
    study.bsaManualEdit_ = bsaManualEdit;
    study.vagalTone_ = vagalTone;
    study.sympatheticTone_ = sympatheticTone;
    study.ef_ = ef;
    study.ischemia_ = ischemia;
    return in;
}

}
