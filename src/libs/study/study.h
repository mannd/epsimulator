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

#include <QtCore/QDateTime>

class QDataStream;

namespace EpPatient { 
    class Heart; }

namespace EpStudy {

    class StudyConfiguration;

    class Name {
    public:
        Name(const QString& last = QString(),
             const QString& first = QString(),
             const QString& middle = QString());

        void setLastFirstMiddle(const QString& last,
                                const QString& first,
                                const QString& middle = QString());

        QString firstMiddleLast() const;
        QString lastFirstMiddle() const;
        QString lastFirst() const;
        QString last() const {return last_;}
        QString first() const {return first_;}
        QString middle() const {return middle_;}

        QString testFullName(bool lastFirst = false,
                             bool useMiddleName = false) const {
            return fullName(lastFirst, useMiddleName);
        }

        friend QDataStream& operator<<(QDataStream&, const Name&);
        friend QDataStream& operator>>(QDataStream&, Name&);

    private:
        QString last_;
        QString first_;
        QString middle_;

        QString fullName(bool lastFirst = false,
                         bool useMiddleName = false) const;
    };

    typedef int AutonomicTone;

    enum Sex {Male, Female};



    /**
 * Encapsulates a specific study.  Patient related members can be copied
 * so that a new study can be created from a previous study, preserving
 * patient data.  Studies are stored in catalogs, but the same information
 * is also stored in the study.dat file, which is located in the
 * root directory of each specific study.  Every study has a unique
 * key, generated from the patient name and the time/date of the study.
 * This key is used to create the name of a unique directory that holds
 * the study data, including the study.dat file.  Catalogs can be 
 * regenerated by reading all the study.dat files on an optical disk.
 * Study holds a pointer to a Heart, which stores all the physiologic data
 * for that specific heart.  There will also be an environment pointer
 * to contain autonomic tone, pharmacologic state, etc.  These latter factors
 * can also change during the study, so these must be the baseline factors
 * for this specific study.
 */
    class Study {

    public:
        friend QDataStream& operator<<(QDataStream&, const Study&);
        friend QDataStream& operator>>(QDataStream&, Study&);

        Study();
        Study(const Study&);
        ~Study();

        Study& operator=(const Study& rhs);

        void resetKey() {key_ = QString();}
        void load();        // load study.dat file
        void save();        // save study.dat file
        void setPreregisterStudy(bool value) {isPregisterStudy_ = value;}

        void loadStudyConfiguration();
        void saveStudyConfiguration();

        void setBsa(double bsa) {bsa_ = bsa;}
        void setName(const Name& name);
        void setMrn(const QString& mrn) {mrn_ = mrn;}
        void setAccountNumber(const QString& num) {accountNumber_ = num;}
        void setDateOfBirth(const QDate& dateOfBirth) {
            dateOfBirth_ = dateOfBirth;}
        void setSex(Sex sex) {sex_ = sex;}
        void setHeight(double height) {height_ = height;}
        void setWeight(double weight) {weight_ = weight;}
        void setHeightIn(double heightIn) {heightIn_ = heightIn;}
        void setWeightLbs(double weightLbs) {weightLbs_ = weightLbs;}
        void setBsaManualEdit(bool bsaManualEdit) {
            bsaManualEdit_ = bsaManualEdit;}
        void setEf(int ef);
        void setDateTime(const QDateTime& dateTime) {dateTime_ = dateTime;}
        void setNumber(const QString& number) {number_ = number;}
        void setIschemia(bool ischemia) {ischemia_ = ischemia;}
        void setVagalTone(AutonomicTone tone);
        void setSympatheticTone(AutonomicTone tone);
        void setPath(const QString& path) {path_ = path;}
        void setStudyConfiguration(const StudyConfiguration&);

        Name name() const {return name_;};
        QString mrn() const {return mrn_;}
        QString number() const {return number_;}
        QString accountNumber() const {return accountNumber_;}
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
        QString key() const;    // Generates key based on name and datetime
        // to identify study uniquely.
        bool isPreregisterStudy() const {return isPregisterStudy_;}
        StudyConfiguration* studyConfiguration() const { 
            return studyConfiguration_;}

        // file related members
        QString path() const {return path_;} // returns path to
        // specific study directory
        QString filePath();     // Returns full path of study.dat file,
        QString configFilePath();   // returns full path of config.dat file
        QString dirName() const {return "/study_" + key();}
        QString fileName() const {return fileName_;}

    private:
        static const int minEf_ = 1;
        static const int maxEf_ = 99;
        static const int minTone_ = 0;
        static const int maxTone_ = 100;
        enum {MagicNumber = 0x99c798f5};  // for study.dat file

        void testInvariant() const;
        void copyStudy(const Study& study);
        AutonomicTone adjustTone(AutonomicTone tone);

        static const QString fileName_; // name of studies data file
        static const QString configFileName_; // name of study config file

        QDateTime dateTime_;
        Name name_;
        QDate dateOfBirth_;
        QString mrn_;	// medical record number
        QString number_;
        QString accountNumber_;     // useless info kept by Prucka
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
        QString path_;      // full path to study directory
        bool isPregisterStudy_;

        mutable QString key_;   // unique key for each study
        EpPatient::Heart* heart_;
        StudyConfiguration* studyConfiguration_;

    };

    inline void Study::testInvariant() const {
        Q_ASSERT(vagalTone_ >= minTone_ && vagalTone_ <= maxTone_);
        Q_ASSERT(sympatheticTone_ >= minTone_ && sympatheticTone_ <= maxTone_);
        Q_ASSERT (sex_ == Male || sex_ == Female);
        Q_ASSERT (ef_ >= minEf_ && ef_ <= maxEf_);
    }

}

#endif
