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

#include "studytable.h"

#include "catalog.h"
#include "coreconstants.h"
#include "error.h"
#include "study.h"
#include "studywriter.h"
#include "studyconfiguration.h"

#include <QFile>
#include <QHeaderView>
#include <QRegExp>
#include <QSortFilterProxyModel>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStringList>
#include <QTextStream>

// uncomment below to show a key column in the table
// #define DEBUGKEYS 1

namespace EpConstants = EpCore::Constants;
using EpNavigator::StudyTable;
using EpStudy::Study;
using EpStudy::StudyConfiguration;
using EpStudy::StudyWriter;

StudyTable::StudyTable(bool oldStyleNavigator, 
                       StudyWriter* studyWriter,
                       QWidget* parent)
    : QTableView(parent), oldStyle_(oldStyleNavigator),
      studyWriter_(studyWriter) {
    Q_ASSERT(studyWriter);
    Q_ASSERT(QSqlDatabase::database(EpConstants::EPSIM_OPTICAL_DB).isOpen());
    systemModel_ = new QSqlTableModel(this, 
                                QSqlDatabase::database(EpConstants::EPSIM_SYSTEM_DB));
    opticalModel_ = new QSqlTableModel(this,
                        QSqlDatabase::database(EpConstants::EPSIM_OPTICAL_DB));
    useNetwork_ = false;        // TODO don't need this 
    networkModel_ = 0; /*new QSqlTableModel(this, 
                         QSqlDatabase::database(EpConstants::EPSIM_NETWORK_DB));*/
    model_ = systemModel_;
    source_ = Catalog::System;
    adjustColumns();
}

void StudyTable::initModel() {
    model_->setTable(EpConstants::EPSIM_CATALOG_TABLENAME);
    model_->setSort(CatalogEntry_StudyDateTime, Qt::DescendingOrder);
    model_->select();
}

void StudyTable::createHeader() {
    const int defaultSectionSize = 140; // so the header isn't squooshed
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setColumnHidden(CatalogEntry_Id, true);
    setColumnHidden(CatalogEntry_StudyKey, true);
    setColumnHidden(CatalogEntry_FullName, oldStyle_);
    setColumnHidden(CatalogEntry_LastName, !oldStyle_);
    setColumnHidden(CatalogEntry_FirstName, !oldStyle_);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    resizeColumnsToContents();
    setShowGrid(false);
    setSortingEnabled(true);
    verticalHeader()->hide();
    QHeaderView* header = horizontalHeader();
    header->setDefaultSectionSize(defaultSectionSize);
    header->setStretchLastSection(true);
    header->setSortIndicator(CatalogEntry_StudyDateTime, Qt::DescendingOrder);
    header->setSortIndicatorShown(true);
    header->setMovable(false);
}

void StudyTable::setSource(Catalog::Source source) {
    source_ = source;
    switch (source) {
    case Catalog::Network :
        model_ = networkModel_;
        break;
    case Catalog::Optical :
        model_ = opticalModel_;
        break;
    case Catalog::System :
    default :
        model_ = systemModel_;
    }
    initModel();
    setHeaderLabels(model_);
    setModel(model_);
}
    

void StudyTable::setHeaderLabels(QSqlTableModel* model) {
    model->setHeaderData(CatalogEntry_StudyType,
                         Qt::Horizontal, tr("Study Type"));
    model->setHeaderData(CatalogEntry_LastName,
                         Qt::Horizontal, tr("Last Name"));
    model->setHeaderData(CatalogEntry_FirstName,
                         Qt::Horizontal, tr("First Name"));
    model->setHeaderData(CatalogEntry_FullName,
                         Qt::Horizontal, tr("Patient Name"));
    model->setHeaderData(CatalogEntry_PatientMrn,
                         Qt::Horizontal, tr("Patient MRN"));
    model->setHeaderData(CatalogEntry_StudyDateTime,
                         Qt::Horizontal, tr("Study Date/Time"));
    model->setHeaderData(CatalogEntry_StudyConfig,
                         Qt::Horizontal, tr("Study Config"));
    model->setHeaderData(CatalogEntry_StudyNumber,
                         Qt::Horizontal, tr("Study Number"));
    model->setHeaderData(CatalogEntry_StudyLocation,
                         Qt::Horizontal, tr("Location of Study Files"));
}

void StudyTable::adjustColumns() {
    initModel();
    setHeaderLabels(model_);
    setModel(model_);
    createHeader();
}


// /**
//  * Sets up the columns according to oldStyle.
//  */
// void StudyTable::adjustColumns() {
//     setColumnCount(0);  // clear all columns
//     // Note that after clearing the table, the table has to be reloaded.  This
//     // is entrusted to the calling procedure and not done here.    
//     QStringList headers;
//     headers << tr("Study Type")
//             << tr("Last Name")
//             << tr("First Name")
//             << tr("Patient")
//             << tr("Patient MRN")
//             << tr("Study Date/Time")
//             << tr("Study Config")
//             << tr("Study Number")
//             << tr("Location of Study");
// #ifdef DEBUGKEYS
//     headers << tr("Study Key");
// #endif    
//     setColumnCount(headers.count());
//     setHeaderLabels(headers);
//     header()->setSortIndicatorShown(true);
//     header()->setMovable(false);
//     header()->setResizeMode(QHeaderView::ResizeToContents);
//     //header()->setStretchLastSection(false);
//     if (oldStyle_) {
//         hideColumn(FullNameCol);
//     }
//     else {
//         hideColumn(LastNameCol);
//         hideColumn(FirstNameCol);
//     }
//     sortItems(DateTimeCol, Qt::AscendingOrder);
// }

// /**
//  * Runs through the table and shows or hides each row depending on the 
//  * filtering in place.  Does not reload table from catalog.
//  */
// void StudyTable::showTable() {
//     QTreeWidgetItemIterator it(this);
//     while (*it) {
//         StudyTableItem* item = static_cast<StudyTableItem*>(*it);
//         setItemHidden(item, filtered_ && item->filteredOut());
//         ++it;
//     }
// }

// /**
//  * Clears the table and then fills it in with from the catalog.
//  * @param catalog = Catalog* that is the source for the table. Note
//  * that StudyTable does not take possession of the Catalog pointer.
//  * Does not apply filter (use showTable() to do that).
//  */
// void StudyTable::load(Catalog* catalog) {
//     clear();
//     catalog_ = catalog;
//     if (!catalog_) 
//         return;  // don't know why it would be 0, but just in case
//     for (Catalog::CatalogMap::const_iterator it = catalog->begin(); 
//         it != catalog->end(); ++it)
//         addStudy(it.value().study, catalog->location(it.value()));
// }


// Returns a pointer to a copy of the Study at selected StudyTable row.
// Note that StudyTable owns its pointers, and so this returns a
// new pointer to a copy of the study from the underlying catalog.
// Catalogs keep their study members on the stack.  Sooo... the Study
// pointer returned by this function is owned by the calling function,
// which needs to delete it.
Study* StudyTable::study() const {
    QModelIndexList indexList = selectedIndexes();
    //QModelIndex index = currentIndex();
    if (indexList.count() < 1)
        return 0;
    QModelIndex index = indexList[0];
    QSqlRecord record = model_->record(index.row());
    Study study;
    study.setKey(record.value(CatalogEntry_StudyKey).toString());
    study.setName(EpPatient::Name(record.value(CatalogEntry_LastName).toString(),
                       record.value(CatalogEntry_FirstName).toString()));

    return new Study(study);
}




// Study* StudyTable::study() const {
//     if (selectedIndexes().isEmpty())
//         return 0;
// //    if (StudyTableItem* item =
// //        static_cast<StudyTableItem*>(selectedItems()[0]))
// //        return new Study((*catalog_)[item->key()].study);
// //    else
//         return 0;
// }

QString StudyTable::key() const {
    QModelIndex index = currentIndex();
    QString key;
    if (index.isValid()) {
        QSqlRecord record = model_->record(index.row());
        key = record.value(CatalogEntry_StudyKey).toString();
    }
    return key;
}

bool StudyTable::isPreregisterStudy() const {
    QModelIndex index = currentIndex();
    bool result = false;
    if (index.isValid()) {
        QSqlRecord record = model_->record(index.row());
        result = record.value(CatalogEntry_StudyType).toBool();
    }
    return result;
}

void StudyTable::addStudy(Study* study, const QString& location) {
    StudyWriter::WriteLocations writeLocations = studyWriter_->study(study);
    // if (writeLocations & StudyWriter::WriteOptical)
    //     updateModel(opticalModel_, study, location);
    // if (writeLocations & StudyWriter::WriteSystem)
    //    updateModel(systemModel_, study, location);
     // ((writeLocations & StudyWriter::WriteNetwork) && useNetwork_)
     //    updateModel(networkModel_, study, location);
    updateModel(model_, study, location);
}

void StudyTable::updateModel(QSqlTableModel* model, 
                             Study* study, 
                             const QString& location) {
    int row = model->rowCount();
    model->insertRow(row);
    model->setData(model->index(row, CatalogEntry_StudyKey), study->key());
    model->setData(model->index(row, CatalogEntry_StudyType),
                     study->isPreregisterStudy() ? tr("Pre-Register")
              : tr("Study"));
    model->setData(model->index(row, CatalogEntry_LastName),
                    study->name().last());
    model->setData(model->index(row, CatalogEntry_FirstName),
                    study->name().first());
    model->setData(model->index(row, CatalogEntry_FullName),
                    study->name().lastFirstMiddle());
    model->setData(model->index(row, CatalogEntry_PatientMrn),
                    study->mrn());
    model->setData(model->index(row, CatalogEntry_StudyDateTime ), 
                    study->dateTime().toString("yyyy/MM/dd hh:mm:ss"));
    model->setData(model->index(row, CatalogEntry_StudyConfig),
                    study->isPreregisterStudy() ?
                    QString() : study->studyConfiguration()->name());
    model->setData(model->index(row, CatalogEntry_StudyNumber),
                    study->number());
    model->setData(model->index(row, CatalogEntry_StudyLocation),
                    location);
    model->submitAll();
    setCurrentIndex(model_->index(row, 0));
}


// void StudyTable::addStudy(const Study& study, const QString& location) {
//         StudyTableItem* t = new StudyTableItem(this, study.key(), 
//             study.dateTime(), study.isPreregisterStudy());
//         t->setText(StudyTypeCol, study.isPreregisterStudy() 
//             ? tr("Pre-Register") : tr("Study"));
//         t->setText(LastNameCol, study.name().last());
//         t->setText(FirstNameCol, study.name().first());
//         t->setText(FullNameCol, study.name().lastFirstMiddle());
//         t->setText(MRNCol, study.mrn());
//         t->setText(DateTimeCol, 
//             study.dateTime().toString("yyyy/MM/dd hh:mm:ss"));
//         t->setText(ConfigCol,
//                    study.isPreregisterStudy() ?
//                    QString() : study.studyConfiguration()->name());
//         t->setText(NumberCol, study.number());
//         t->setText(LocationCol, location); 
//         // for debugging show study key 
// #ifdef DEBUGKEYS
//         t->setText(LocationCol+1, study.key());
// #endif
// }

// void StudyTable::exportCSV(const QString& fileName) {
//     QFile file(fileName);
//     if (!file.open(QIODevice::WriteOnly)) 
//         throw EpCore::OpenWriteError(file.fileName());
//     QTextStream out(&file);
//     for (int i = 0; i < columnCount(); ++i)
//         if (i != FullNameCol)   // don't need 3 name columns
//             out << '"' << headerItem()->text(i) << '"' << ',';
//     out << '\n';
//     QTreeWidgetItemIterator it(this);
//     while (*it) {
//         QTreeWidgetItem* item = *it;
//         for (int i = 0; i < columnCount(); ++i)
//             if (i != FullNameCol)   // avoid duplicating name columns
//                 out << '"' << item->text(i) << '"' << ',';
//         out << '\n';
//         ++it;
//     }
//     if (file.error()) 
//         throw EpCore::WriteError(file.fileName());
//     file.close();
// }


void StudyTable::applyFilter( FilterStudyType filterStudyType,
                                            const QRegExp& lastName,
                                            const QRegExp& firstName,
                                            const QRegExp& mrn,
                                            const QRegExp& studyConfig,
                                            const QRegExp& studyNumber,
                                            const QRegExp& studyLocation,
                                            bool anyDate,
                                            const QDate& startDate,
                                            const QDate& endDate) {
    QString filter = "LastName='lastName'";
    model_->setFilter(filter);
    // bool match = false;
    // QTreeWidgetItemIterator it(this);
    // while (*it) {
    //     if (StudyTableItem* item = static_cast<StudyTableItem*>(*it)) {
    //         QDate studyDate = item->dateTime().date();
    //         match = lastName.exactMatch(item->text(LastNameCol)) &&
    //             firstName.exactMatch(item->text(FirstNameCol)) &&
    //             mrn.exactMatch(item->text(MRNCol)) &&
    //             studyConfig.exactMatch(item->text(ConfigCol)) &&
    //             studyNumber.exactMatch(item->text(NumberCol)) &&
    //             studyLocation.exactMatch(item->text(LocationCol)) &&
    //             (anyDate ? true : (startDate <= studyDate)) &&
    //             (studyDate <= endDate);
    //         switch (filterStudyType) {
    //             case AnyStudyType :
    //                 break;
    //             case StudyType :
    //                 match = match && !item->isPreregisterStudy();
    //                 break;
    //             case PreregisterType :
    //                 match = match && item->isPreregisterStudy();
    //                 break;
    //         }
    //         item->setFilteredOut(!match);
    //     }
    //     ++it;
    // }
    // filtered_ = true;
    // showTable();
}

void StudyTable::removeFilter() {
    model_->setTable(EpConstants::EPSIM_CATALOG_TABLENAME);
    model_->select();
    // QTreeWidgetItemIterator it(this);
    // while (*it) {
    //     StudyTableItem* item = static_cast<StudyTableItem*>(*it);
    //     item->setFilteredOut(false);
    //     ++it;
    // }
    // filtered_ = false;
    // showTable();
}
