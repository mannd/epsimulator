/***************************************************************************
 *   Copyright (C) 2006-2011 by EP Studios, Inc.                           *
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

#ifndef STUDYTABLE_H
#define STUDYTABLE_H

#include "catalog.h"

#include <QDateTime>
#include <QTableView>

class QRegExp;
class QSortFilterProxyModel;
class QSqlTableModel;

namespace EpStudy {
    class Study;
}

namespace EpNavigator {

struct StudyData;

class StudyTable : public QTableView {
public:
    enum {
        CatalogEntry_Id = 0,
        CatalogEntry_StudyKey = 1,
        CatalogEntry_StudyType = 2,
        CatalogEntry_LastName = 3,
        CatalogEntry_FirstName = 4,
        CatalogEntry_PatientMrn = 5,
        CatalogEntry_StudyDateTime = 6,
        CatalogEntry_StudyConfig = 7,
        CatalogEntry_StudyNumber = 8,
        CatalogEntry_StudyLocation = 9 
    };
    enum FilterStudyType {AnyStudyType, 
                          StudyType, 
                          PreregisterType
    };
    enum UpdateFlag {
        NoUpdate        = 0x00,
        OpticalUpdate   = 0x01,
        SystemUpdate    = 0x02,
        NetworkUpdate   = 0x04,
        AllButNetworkUpdate = OpticalUpdate | SystemUpdate,
        AllUpdate       = AllButNetworkUpdate | NetworkUpdate
    };


    StudyTable(QWidget* parent = 0);

    void setSource(Catalog::Source);

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
    // void showTable();
    // void adjustColumns();
    // void exportCSV(const QString& fileName);

    // void setOldStyle(bool oldStyle) {oldStyle_ = oldStyle;}
    // bool filtered() const {return filtered_;}
    EpStudy::Study* study() const; // returns currently selected study
                                   // or 0 if none selected
    QString key() const;           // return selected study key
                                   // or empty string
    Catalog::Source source() const {return source_;}
    void addStudy(const EpStudy::Study& study, const QString& location);
private:
    void initModel();
    void createHeader();
    void setHeaderLabels(QSqlTableModel*);

    QSqlTableModel* systemModel_;
    QSqlTableModel* opticalModel_;
    QSqlTableModel* networkModel_;
    //QSortFilterProxyModel* proxyModel_;
    QSqlTableModel* model_;
    // class TableListViewItem;

    Catalog::Source source_;

    // bool filtered_;
    // bool oldStyle_;

    // Catalog* catalog_;      // the catalog underlying this TableListView
    
}; // TableListView

// class TableListView::TableListViewItem : public QTreeWidgetItem {

// public:
//     TableListViewItem(TableListView* parent, const QString& key, 
//                       const QDateTime& dateTime, bool isPreregisterStudy);
//     ~TableListViewItem();

//     void setFilteredOut(bool filteredOut) {filteredOut_ = filteredOut;}

//     bool filteredOut() const {return filteredOut_;}
//     QString key() const {return key_;}
//     QDateTime dateTime() const {return dateTime_;}
//     bool isPreregisterStudy() const {return isPreregisterStudy_;}

// private:
//     QString key_;
//     QDateTime dateTime_;
//     bool isPreregisterStudy_;
//     bool filteredOut_;


// }; // TableListViewItem

}

#endif
