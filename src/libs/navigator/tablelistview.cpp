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

#include "tablelistview.h"

#include "catalog.h"
#include "error.h"
#include "study.h"
#include "studyconfiguration.h"

#include <QFile>
#include <QHeaderView>
#include <QRegExp>
#include <QStringList>
#include <QTextStream>

// uncomment below to show a key column in the table
// #define DEBUGKEYS 1

using EpNavigator::TableListView;
using EpStudy::Study;
using EpStudy::StudyConfiguration;

/**
 * Constructor for TableListViewItem subclass of Navigator
 * @param parent = TableListView.
 * @param key = unique index key for each study.
 * @param dateTime = keeping track of study dateTime allows easier filtering.
 * @param isPreregisterStudy = ditto above.
 */
TableListView::TableListViewItem::TableListViewItem(TableListView* parent, 
    const QString& key, const QDateTime& dateTime, bool isPreregisterStudy)
    : QTreeWidgetItem(parent), key_(key), dateTime_(dateTime),
      isPreregisterStudy_(isPreregisterStudy), filteredOut_(false) {}

TableListView::TableListViewItem::~TableListViewItem() {}

/**
 * Constructor for TableListView.
 * @param parent = owned by horizontalSplitter_ in Navigator.
 * @param oldStyle = sets up the columns according to OldStyleNavigator. 
 * @param catalog_ = keeps a reference to its underlying Catalog to look up studies.
 * Ctor does not populate the table, each catalog must be loaded with load(). 
*/
TableListView::TableListView(QWidget* parent, bool oldStyle) 
    : QTreeWidget(parent),filtered_(false), oldStyle_(oldStyle),
      catalog_(0) {
    adjustColumns();
    setAllColumnsShowFocus(true);
    setSortingEnabled(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setRootIsDecorated(false);
}

TableListView::~TableListView() {}

/**
 * Sets up the columns according to oldStyle.
 */
void TableListView::adjustColumns() {
    setColumnCount(0);  // clear all columns
    // Note that after clearing the table, the table has to be reloaded.  This
    // is entrusted to the calling procedure and not done here.    
    QStringList headers;
    headers << tr("Study Type")
            << tr("Last Name")
            << tr("First Name")
            << tr("Patient")
            << tr("Patient MRN")
            << tr("Study Date/Time")
            << tr("Study Config")
            << tr("Study Number")
            << tr("Location of Study");
#ifdef DEBUGKEYS
    headers << tr("Study Key");
#endif    
    setColumnCount(headers.count());
    setHeaderLabels(headers);
    header()->setSortIndicatorShown(true);
    header()->setMovable(false);
    header()->setResizeMode(QHeaderView::ResizeToContents);
    //header()->setStretchLastSection(false);
    if (oldStyle_) {
        hideColumn(FullNameCol);
    }
    else {
        hideColumn(LastNameCol);
        hideColumn(FirstNameCol);
    }
    sortItems(DateTimeCol, Qt::AscendingOrder);
}

/**
 * Runs through the table and shows or hides each row depending on the 
 * filtering in place.  Does not reload table from catalog.
 */
void TableListView::showTable() {
    QTreeWidgetItemIterator it(this);
    while (*it) {
        TableListViewItem* item = static_cast<TableListViewItem*>(*it);
        setItemHidden(item, filtered_ && item->filteredOut());
        ++it;
    }
}

/**
 * Clears the table and then fills it in with from the catalog.
 * @param catalog = Catalog* that is the source for the table. Note
 * that TableListView does not take possession of the Catalog pointer.
 * Does not apply filter (use showTable() to do that).
 */
void TableListView::load(Catalog* catalog) {
    clear();
    catalog_ = catalog;
    if (!catalog_) 
        return;  // don't know why it would be 0, but just in case
    for (Catalog::CatalogMap::const_iterator it = catalog->begin(); 
        it != catalog->end(); ++it)
        addStudy(it.value().study, catalog->location(it.value()));
}

/**
 * Returns a pointer to a copy of the Study at selected TableListView row.
 * Note that TableListView owns its pointers, and so this returns a
 * new pointer to a copy of the study from the underlying catalog.
 * Catalogs keep their study members on the stack.  Sooo... the Study
 * pointer returned by this function is owned by the calling function,
 * which needs to delete it.
 * @return Study pointer, 0 if no row selected.
 */
Study* TableListView::study() const {
    if (selectedItems().isEmpty())
        return 0;
    if (TableListViewItem* item = 
        static_cast<TableListViewItem*>(selectedItems()[0]))
        return new Study((*catalog_)[item->key()].study);
    else
        return 0;
}

void TableListView::addStudy(const Study& study, const QString& location) {
        TableListViewItem* t = new TableListViewItem(this, study.key(), 
            study.dateTime(), study.isPreregisterStudy());
        t->setText(StudyTypeCol, study.isPreregisterStudy() 
            ? tr("Pre-Register") : tr("Study"));
        t->setText(LastNameCol, study.name().last());
        t->setText(FirstNameCol, study.name().first());
        t->setText(FullNameCol, study.name().lastFirstMiddle());
        t->setText(MRNCol, study.mrn());
        t->setText(DateTimeCol, 
            study.dateTime().toString("yyyy/MM/dd hh:mm:ss"));
        t->setText(ConfigCol,
                   study.isPreregisterStudy() ?
                   QString() : study.studyConfiguration()->name());
        t->setText(NumberCol, study.number());
        t->setText(LocationCol, location); 
        // for debugging show study key 
#ifdef DEBUGKEYS
        t->setText(LocationCol+1, study.key());
#endif
}

void TableListView::exportCSV(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) 
        throw EpCore::OpenWriteError(file.fileName());
    QTextStream out(&file);
    for (int i = 0; i < columnCount(); ++i)
        if (i != FullNameCol)   // don't need 3 name columns
            out << '"' << headerItem()->text(i) << '"' << ',';
    out << '\n';
    QTreeWidgetItemIterator it(this);
    while (*it) {
        QTreeWidgetItem* item = *it;
        for (int i = 0; i < columnCount(); ++i)
            if (i != FullNameCol)   // avoid duplicating name columns
                out << '"' << item->text(i) << '"' << ',';
        out << '\n';
        ++it;
    }
    if (file.error()) 
        throw EpCore::WriteError(file.fileName());
    file.close();
}


void TableListView::applyFilter( FilterStudyType filterStudyType,
                                            const QRegExp& lastName,
                                            const QRegExp& firstName,
                                            const QRegExp& mrn,
                                            const QRegExp& studyConfig,
                                            const QRegExp& studyNumber,
                                            const QRegExp& studyLocation,
                                            bool anyDate,
                                            const QDate& startDate,
                                            const QDate& endDate) {
    bool match = false;
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if (TableListViewItem* item = static_cast<TableListViewItem*>(*it)) {
            QDate studyDate = item->dateTime().date();
            match = lastName.exactMatch(item->text(LastNameCol)) &&
                firstName.exactMatch(item->text(FirstNameCol)) &&
                mrn.exactMatch(item->text(MRNCol)) &&
                studyConfig.exactMatch(item->text(ConfigCol)) &&
                studyNumber.exactMatch(item->text(NumberCol)) &&
                studyLocation.exactMatch(item->text(LocationCol)) &&
                (anyDate ? true : (startDate <= studyDate)) &&
                (studyDate <= endDate);
            switch (filterStudyType) {
                case AnyStudyType :
                    break;
                case StudyType :
                    match = match && !item->isPreregisterStudy();
                    break;
                case PreregisterType :
                    match = match && item->isPreregisterStudy();
                    break;
            }
            item->setFilteredOut(!match);
        }
        ++it;
    }
    filtered_ = true;
    showTable();
}

void TableListView::removeFilter() {
    QTreeWidgetItemIterator it(this);
    while (*it) {
        TableListViewItem* item = static_cast<TableListViewItem*>(*it);
        item->setFilteredOut(false);
        ++it;
    }
    filtered_ = false;
    showTable();
}
