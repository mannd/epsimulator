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

#include <qdatastream.h>
#include <qdatetime.h>
#include <qstring.h>

class Heart;

struct Name {
    QString first;
    QString last;
    QString middle;
    QString fullName(bool lastFirst = false,
                     bool useMiddleName = false) const;
    friend QDataStream& operator<<(QDataStream&, const Name&);
    friend QDataStream& operator>>(QDataStream&, Name&);
};

typedef int AutonomicTone;

enum Sex {Male, Female};

// change these to programmable options
#define DEFAULT_EF 55
#define DEFAULT_VAGAL_TONE 70
#define DEFAULT_SYMPATHETIC_TONE 30
#define DEFAULT_BIRTH_DATE (QDate(1950,1,1))

class Study {
    friend QDataStream& operator<<(QDataStream&, const Study&);
    friend QDataStream& operator>>(QDataStream&, Study&);

public:
    Study();
    Study(const Study&);

    Name name() const {return name_;};
    QString mrn() const {return mrn_;}
    QDate dateOfBirth() const {return dateOfBirth_;}
    Sex sex() const {return sex_;}
    double height() const {return height_;}
    double weight() const {return weight_;}
    double heightIn() const {return heightIn_;}
    double weightLbs() const {return weightLbs_;}
    double bsa() const {return bsa_;}
    bool bsaManualEdit() const {return bsaManualEdit_;}
    int ef() const {return ef_;}
    bool ischemia() const {return ischemia_;}
    AutonomicTone vagalTone() const {return vagalTone_;}
    AutonomicTone sympatheticTone() const {return sympatheticTone_;}
    QDateTime dateTime() const {return dateTime_;}
    QString number() const {return number_;}
    QString path() const {return path_;}
    QString file() const {return file_;}
    QString filePath();   // returns full path of study, fixes path if doesn't end in '/'
    QString config() const {return config_;}
    QString key() const;    // generates key based on name and datetime to identify study uniquely

    void setBsa(double bsa) {bsa_ = bsa;}
    void setName(const Name& name);
    void setMrn(QString mrn) {mrn_ = mrn;}
    void setDateOfBirth(QDate dateOfBirth) {dateOfBirth_ = dateOfBirth;}
    void setSex(Sex sex) {sex_ = sex;}
    void setHeight(double height) {height_ = height;}
    void setWeight(double weight) {weight_ = weight;}
    void setHeightIn(double heightIn) {heightIn_ = heightIn;}
    void setWeightLbs(double weightLbs) {weightLbs_ = weightLbs;}
    void setBsaManualEdit(bool bsaManualEdit) {
	bsaManualEdit_ = bsaManualEdit;}
    void setEf(int ef);
    void setDateTime(QDateTime dateTime) {dateTime_ = dateTime;}
    void setNumber(QString number) {number_ = number;}
    void setIschemia(bool ischemia) {ischemia_ = ischemia;}
    void setVagalTone(AutonomicTone tone);
    void setSympatheticTone(AutonomicTone tone);
    void setPath(QString path) {path_ = path;}
    void setFile(QString file) {file_ = file;}
    void setConfig(QString config) {config_ = config;}

    Study& operator=(const Study& rhs);

    ~Study();

private:
    enum {MIN_TONE = 0, MAX_TONE = 100};  // min and max autonomic tone
    enum {MIN_EF = 1, MAX_EF = 99};

    void testInvariant() const;
    void copyStudy(const Study& study);
    AutonomicTone adjustTone(AutonomicTone tone);

    QDateTime dateTime_;
    QString number_;
    Name name_;
    QString mrn_;	// medical record number
    QDate dateOfBirth_;
    QString config_;    // this will eventually be a class probably
    Sex sex_;
    double height_;	// use metric units: cm for height,
    double weight_;	// in kg
    double heightIn_;   // English versions
    double weightLbs_;
    double bsa_;  // BSA either manually edited or calculated
    bool bsaManualEdit_;  // should BSA be manually edited or calculated?
    AutonomicTone vagalTone_;
    AutonomicTone sympatheticTone_;
    int ef_;
    bool ischemia_;
    Heart *heart_;
    QString path_;  // location (path) of this study
    QString file_;  //  name of study file
};

#endif
