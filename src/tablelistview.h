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

/** @file
    Contains TableListView and TableListViewItem classes.
*/

#ifndef TABLELISTVIEW_H
#define TABLELISTVIEW_H

#include "study.h"

#include <qlistview.h>
#include <qstring.h>

class Catalog;
class Options;
class QDataStream;
class QDate;
class QFile;
class QRegExp;

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

    void load(Catalog*);
    void save(Catalog*);

    void addStudy(const Study* study);
    void deleteStudy();
    void applyFilter(FilterStudyType filterStudyType,
                    const QRegExp& lastName,
                    const QRegExp& firstName,
                    const QRegExp& mrn,
                    const QRegExp& studyConfig,
                    const QRegExp& studyNumber,
                    const QRegExp& studyLocation,
                    bool anyDate,
                    const QDate& startDate,
                    const QDate& endDate);
    void removeFilter();
    void showTable();
    void adjustColumns(bool oldStyle, bool clearTable = false);

    void exportCSV(const QString& fileName);

private:
    QString location(const QString& studyLocation) const;

    // first bytes of EP Simulator binary files
    enum {MagicNumber = 0x99c798f2};    

    bool filtered_;
    Options* options_;  // copy of Navigator options_

}; // TableListView

class TableListViewItem : public QListViewItem {

public:
    TableListViewItem(TableListView* parent, const Study& study,
                    int dateColumn,
                    QString label1, 
                    QString label2 = QString::null, 
                    QString label3 = QString::null, 
                    QString label4 = QString::null, 
                    QString label5 = QString::null, 
                    QString label6 = QString::null, 
                    QString label7 = QString::null, 
                    QString label8 = QString::null );
    ~TableListViewItem();

// reimplement compare to handle dates correctly.
    int compare(QListViewItem* item, int column, bool ascending) const;

    void setFilteredOut(bool filteredOut) {filteredOut_ = filteredOut;}

    // below can't return reference
    Study study() const {return study_;}
    bool filteredOut() const {return filteredOut_;}

private:
    Study study_;
    int dateColumn_;    // depends on oldStyleNavigator() option
    bool filteredOut_;
}; // TableListViewItem

#endif
