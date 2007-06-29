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
#include "options.h"

#include <qdatastream.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qheader.h>
#include <qmessagebox.h>
#include <qregexp.h>

#ifndef NDEBUG
#include <iostream>
#endif

/**
 * Constructor for TableListViewItem subclass of Navigator
 * @param parent = TableListView. 
 * @param study = Study associated with this row of the TableListView.
 * @param label1  etc. = Study fields, depending on OldStyleNavigator option.
 */
TableListView::TableListViewItem::TableListViewItem(TableListView* parent, 
						const Study& study,
                                                int dateColumn,
						QString label1, 
						QString label2, 
						QString label3,
						QString label4, 
						QString label5, 
						QString label6, 
						QString label7, 
						QString label8 ) 
    : QListViewItem(parent, label1, label2, label3, label4, 
		    label5, label6, label7, label8), study_(study),
                    dateColumn_(dateColumn), 
                    filteredOut_(false) {
}

TableListView::TableListViewItem::~TableListViewItem() {
}


/**
 * Constructor for TableListView.
 * @param parent = owned by horizontalSplitter_ in Navigator.
 * @param oldStyle = sets up the columns according to OldStyleNavigator. 
 * Ctor does not populate the table, each catalog must be loaded with load(). 
*/
TableListView::TableListView(QWidget* parent, bool oldStyle) 
    : QListView(parent), 
      filtered_(false), oldStyle_(oldStyle) {
    adjustColumns(oldStyle_);
    setAllColumnsShowFocus(true);
    setShowSortIndicator(true);
}

TableListView::~TableListView() {
}



/**
 * Sets up the columns according to oldStyle.
 * @param clearTable = true if you need to change a prexisting table.
 *                     This is always true except in the constructor.
 */
void TableListView::adjustColumns(bool clearTable) {
    if (clearTable) {
        int numCols = columns();
        for (int i = numCols - 1; i >= 0; --i) 
            removeColumn(i);
    // Note that after clearing the table, the table has to be reloaded.  This
    // is entrusted to the calling procedure and not done here.    
    }

    addColumn(tr("Study Type"));        // col 0
    if (oldStyle_) {
        addColumn(tr("Last Name"));     // col 1
        addColumn(tr("First Name"));    // col 2
    }
    else
        addColumn(tr("Patient"));       // col 1
    addColumn(tr("MRN"));               // col 2 or 3
    addColumn(tr("Study Date/Time"));   // col 3 or 4
    addColumn(tr("Study Config"));      // col 4 or 5
    addColumn(tr("Study Number"));      // col 5 or 6
    addColumn(tr("Location of Study")); // col 6 or 7
    setSortColumn(oldStyle_ ? 4 : 3);    // default sort is date/time
    setSortOrder(Qt::Ascending);       // most recent study last
}

/**
 * Runs through the table and shows or hides each row depending on the 
 * filtering in place.  Does not reload table from catalog.
 */
void TableListView::showTable() {
    QListViewItemIterator it(this);
    while (it.current()) {
        TableListViewItem* item = dynamic_cast<TableListViewItem*>(*it);
        item->setVisible(!filtered_ || !item->filteredOut());
        ++it;
    }
}

/**
 * Clears the table and then fills it in with from the catalog.
 * @param catalog = Catalog that is the source for the table.
 * Does not apply filter (use showTable() to do that).
 */
void TableListView::load(Catalog* catalog) {
    clear();
    for (Catalog::Iterator it = catalog->begin(); 
        it != catalog->end(); ++it) {
        Study study = (*it).second;
        QString location = catalog->location(study);
        addStudy(study, location);
    }
}

/**
 * Saves the table to the catalog, replacing any items that have been
 * modified with the modified item, and adding new items.  
 * NB: does not remove any items.  Removed items from the TableListView
 * must be specificaly deleted from the catalog, or will show up again.
 *
 * This function is called for each catalog when the TableListView is saved.
 * @param catalog = Catalog to save to.
 */
void TableListView::save(Catalog* catalog) {
    QListViewItemIterator it(this);
    while (it.current()) {
        QListViewItem* item = *it;
        Study s = dynamic_cast<TableListViewItem*>(item)->study();
        QString key = s.key();
        (*catalog)[key] = s;
        ++it;
    }
}

Study* TableListView::study() const {
    Study* study = 0;
    if (QListViewItem* item = selectedItem()) 
        study = new Study(dynamic_cast<TableListViewItem*>(item)->study());
    return study;
}


/// FIXME This is naive.  What happens when the catalog is saved?  Reading is easy:
/// whatever Catalog is being viewed, just load that catalog.dat file.  What happens
/// when a study is added, or edited?  The different catalog.dat files must be updated
/// immediately, or else you could change catalog source and not have it work right.
/// You can't just write the whole TableListView to each catalog.dat, as the catalogs
/// are different.  So, we have these situations:
///     System and Optical only:
///         Study added/moved/deleted: Optical and System always updated; however
///             it is necessary to "insert" optical disk if not already inserted
///             when studies, moved or deleted.
///     Network:
///         Network is a super System catalog.  When study added or exported to Network,
///             Network is updated.  Also deletions, moving recorded in Network.  Exported
///             study data to a network folder overrides the Optical data.  Thus, system
///             and Network can be out of sync.
/// NOTE This function has been moved out of tablelistview to catalog.h


void TableListView::addStudy(const Study& study, const QString& location) {
    if (oldStyle_) {
        (void) new TableListViewItem(this, study, 4,
        study.isPreregisterStudy() ? tr("Pre-Register") : tr("Study"),
        study.name().last,
        study.name().first,
        study.mrn(),
        study.dateTime().toString(Qt::LocalDate),
        study.config(),
        study.number(),
        location); 
    }
    else {
        (void) new TableListViewItem(this, study, 3,
        study.isPreregisterStudy() ? tr("Pre-Register") : tr("Study"),
        study.name().fullName(true, true),
        study.mrn(),
        study.dateTime().toString(Qt::LocalDate),
        study.config(),
        study.number(),
        location);
    }
}

void TableListView::exportCSV(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(IO_WriteOnly)) 
        throw EpSim::OpenWriteError(file.name());
    QTextStream out(&file);
    for (int i = 0; i < columns(); ++i) {
        out << '"' << columnText(i) << '"' << ',';
    }
    out << '\n';
    QListViewItemIterator it(this);
    while (it.current()) {
        QListViewItem* item = *it;
        for (int i = 0; i < columns(); ++i) 
            out << '"' << item->text(i) << '"' << ',';
        out << '\n';
        ++it;
    }
    if (file.status() != IO_Ok) 
        throw EpSim::WriteError(file.name());
    file.close();
}

/// BUG or feature? Matches location without considering disk side 
/// or machine location.  Not sure how Prucka handles this. 
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
    QListViewItemIterator it(this);
    while (it.current()) {
        TableListViewItem* item = dynamic_cast<TableListViewItem*>(*it);
        QDate studyDate = item->study().dateTime().date();
        match = lastName.exactMatch(item->study().name().last) &&
            firstName.exactMatch(item->study().name().first) &&
            mrn.exactMatch(item->study().mrn()) &&
            studyConfig.exactMatch(item->study().config()) &&
            studyNumber.exactMatch(item->study().number()) &&
            studyLocation.exactMatch(item->study().location()) &&
            (anyDate ? true : (startDate <= studyDate) &&
            (studyDate <= endDate));
        switch (filterStudyType) {
            case AnyStudyType :
                break;
            case StudyType :
                match = match && !item->study().isPreregisterStudy();
                break;
            case PreregisterType :
                match = match && item->study().isPreregisterStudy();
                break;
        }
        item->setFilteredOut(!match);
        ++it;
    }
    filtered_ = true;
    showTable();

}

void TableListView::removeFilter() {
    QListViewItemIterator it(this);
    while (it.current()) {
        QListViewItem* item = *it;
        dynamic_cast<TableListViewItem*>(item)->setFilteredOut(false);
        ++it;
    }
    filtered_ = false;
    showTable();
}
