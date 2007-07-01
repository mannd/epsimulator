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

#include "catalog.h"

#include <qlistview.h>
#include <qstring.h>

class Options;
class QDataStream;
class QDate;
class QFile;
class QRegExp;

struct StudyData;
/**
	@author David Mann <mannd@epstudiossoftware.com>
        TableListView lists studies from the catalogs in Navigator.
*/
class TableListView : public QListView {
public:
    enum FilterStudyType {AnyStudyType, StudyType, PreregisterType};

    TableListView(QWidget* parent, bool oldStyle);
    ~TableListView();

    void setOldStyle(bool oldStyle) {oldStyle_ = oldStyle;}
    bool filtered() const {return filtered_;}

    void load(Catalog*);

    Study* study() const;     // returns currently selected study or 0 if none selected

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
    void adjustColumns(bool clearTable = false);

    void exportCSV(const QString& fileName);

private:
    class TableListViewItem;
    void addStudy(const Study& study, const QString& location);

    // first bytes of EP Simulator binary files
    enum {MagicNumber = 0x99c798f2};    

    bool filtered_;
    bool oldStyle_;

}; // TableListView

class TableListView::TableListViewItem : public QListViewItem {

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
