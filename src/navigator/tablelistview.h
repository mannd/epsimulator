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

#include <qdatetime.h>
#include <q3listview.h>
#include <qstring.h>

class Catalog;
class Options;
class Study;

class QRegExp;

struct StudyData;
/**
	@author David Mann <mannd@epstudiossoftware.com>
        TableListView lists studies from the catalogs in Navigator.
*/
class TableListView : public Q3ListView {
public:
    enum ColumnName {FirstCol = 0, StudyTypeCol = FirstCol, LastNameCol,
                     FirstNameCol, FullNameCol, MRNCol, DateTimeCol, 
                     ConfigCol, NumberCol, LocationCol, 
                     LastCol = LocationCol};
    enum FilterStudyType {AnyStudyType, StudyType, PreregisterType};

    TableListView(QWidget* parent, bool oldStyle);
    ~TableListView();

    void setOldStyle(bool oldStyle) {oldStyle_ = oldStyle;}
    bool filtered() const {return filtered_;}

    void load(Catalog*);

    Study* study() const; // returns currently selected study 
                          // or 0 if none selected

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

    bool filtered_;
    bool oldStyle_;

    Catalog* catalog_;      // the catalog underlying this TableListView

}; // TableListView

class TableListView::TableListViewItem : public Q3ListViewItem {

public:
    TableListViewItem(TableListView* parent, const QString& key, 
                      const QDateTime& dateTime, bool isPreregisterStudy);
    ~TableListViewItem();

    QString key() const {return key_;}
    QDateTime dateTime() const {return dateTime_;}
    bool isPreregisterStudy() const {return isPreregisterStudy_;}

    void setFilteredOut(bool filteredOut) {filteredOut_ = filteredOut;}

    bool filteredOut() const {return filteredOut_;}

private:
    QString key_;
    QDateTime dateTime_;
    bool isPreregisterStudy_;
    bool filteredOut_;
}; // TableListViewItem

#endif
