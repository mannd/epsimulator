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

/** \file
    Contains TableListView and TableListViewItem classes.
*/

#ifndef TABLELISTVIEW_H
#define TABLELISTVIEW_H

#include "study.h"

#include <qlistview.h>
#include <qstring.h>

class Options;
class QDataStream;
class QDate;
class QFile;
class QRegExp;
class QStringList;

/**
	@author David Mann <mannd@epstudiossoftware.com>
        TableListView lists studies from the catalogs in Navigator.
*/
    class TableListView : public QListView {
    public:
        enum FilterStudyType {AnyStudyType, StudyType, PreregisterType};

        TableListView(QWidget* parent, Options* options);
        ~TableListView();

        bool filtered() const {return filtered_;}

        bool load(const QString& fileName);
        bool save(const QString& fileName);
        bool save(const QStringList& fileNames);
        void addStudy(const Study& study);
        void applyFilter(FilterStudyType filterStudyType,
                         const QRegExp& lastName,
                         const QRegExp& firstName,
                         const QRegExp& mrn,
                         const QRegExp& studyConfig,
                         const QRegExp& studyNumber,
                         const QRegExp& studyFile,
                         bool anyDate,
                         const QDate& startDate,
                         const QDate& endDate);
        void removeFilter();
        void showTable();
        void exportCSV(const QString& fileName);
    
    private:
        // first bytes of EP Simulator binary files
        enum {MagicNumber = 0x99c798f2};    

        void readFromStream(QDataStream& in);
        void writeToStream(QDataStream& out);
        void error(const QFile& file, const QString& message);
        void ioError(const QFile& file, const QString& message);
        
        bool filtered_;
        Options* options_;  // copy of Navigator options_

    }; // TableListView

    class TableListViewItem : public QListViewItem {

    public:
        TableListViewItem(TableListView* parent, const Study& study,
                          QString label1, 
                          QString label2 = QString::null, 
                          QString label3 = QString::null, 
                          QString label4 = QString::null, 
                          QString label5 = QString::null, 
                          QString label6 = QString::null, 
                          QString label7 = QString::null, 
                          QString label8 = QString::null );
        ~TableListViewItem();

        void setFilteredOut(bool filteredOut) {filteredOut_ = filteredOut;}

        // below can't return reference
        Study study() {return study_;}
        bool filteredOut() const {return filteredOut_;}

    private:
        Study study_;
        bool filteredOut_;
    }; // TableListViewItem

#endif
