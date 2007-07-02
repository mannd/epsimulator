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



/// TODO study is not needed, need to store study key and location.
/**
 * Constructor for TableListViewItem subclass of Navigator
 * @param parent = TableListView. 
 * @param study = Study associated with this row of the TableListView.
 */
TableListView::TableListViewItem::TableListViewItem(TableListView* parent, 
						    const Study& study) 
    : QListViewItem(parent), study_(study), filteredOut_(false) {
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
    adjustColumns(false);
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
    addColumn(tr("Study Type"));       
    addColumn(tr("Last Name"));     
    addColumn(tr("First Name"));    
    addColumn(tr("Patient"));       
    addColumn(tr("MRN"));               
    addColumn(tr("Study Date/Time"));   
    addColumn(tr("Study Config"));      
    addColumn(tr("Study Number"));      
    addColumn(tr("Location of Study")); 
    header()->setResizeEnabled(true);
    if (oldStyle_) {
        // must set ColumnWidthMode to Manual, or else it is Maximum and the
        // column is expanded no matter what and can't be hidden.
        setColumnWidthMode(FullNameCol, Manual);
        hideColumn(FullNameCol);
        header()->setResizeEnabled(false, FullNameCol);
    }
    else {
        setColumnWidthMode(LastNameCol, Manual);
        setColumnWidthMode(FirstNameCol, Manual);
        hideColumn(LastNameCol);
        hideColumn(FirstNameCol);
        header()->setResizeEnabled(false, LastNameCol);
        header()->setResizeEnabled(false, FirstNameCol);
    }
    setSortColumn(DateTimeCol);    // default sort is date/time
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
        StudyData studyData = (*it).second;
        QString location = catalog->location(studyData);
        addStudy(studyData.study, location);
    }
}

Study* TableListView::study() const {
    Study* study = 0;
    if (QListViewItem* item = selectedItem()) 
        study = new Study(dynamic_cast<TableListViewItem*>(item)->study());
    return study;
}

void TableListView::addStudy(const Study& study, const QString& location) {
        TableListViewItem* t = new TableListViewItem(this, study);
        t->setText(StudyTypeCol, study.isPreregisterStudy() ? tr("Pre-Register") : tr("Study"));
        t->setText(LastNameCol, study.name().last);
        t->setText(FirstNameCol, study.name().first);
        t->setText(FullNameCol, study.name().fullName(true, true));
        t->setText(MRNCol, study.mrn());
        t->setText(DateTimeCol, study.dateTime().toString("yyyy/MM/dd hh:mm:ss"));
        t->setText(ConfigCol, study.config());
        t->setText(NumberCol, study.number());
        t->setText(LocationCol, location); 
}

void TableListView::exportCSV(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(IO_WriteOnly)) 
        throw EpSim::OpenWriteError(file.name());
    QTextStream out(&file);
    for (int i = 0; i < columns(); ++i) 
        if (i != FullNameCol)   // don't need 3 name columns
            out << '"' << columnText(i) << '"' << ',';
    out << '\n';
    QListViewItemIterator it(this);
    while (it.current()) {
        QListViewItem* item = *it;
        for (int i = 0; i < columns(); ++i) 
            if (i != FullNameCol)   // avoid duplicating name columns
                out << '"' << item->text(i) << '"' << ',';
        out << '\n';
        ++it;
    }
    if (file.status() != IO_Ok) 
        throw EpSim::WriteError(file.name());
    file.close();
}


/// FIXME none of these parameters will come from study anymore, but rather from the
/// columns themselves.  Need to adjust for oldStyleNavigator then.
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
/// FIXME it looks like we have to keep track of key and dateTime from study in this class
        QDate studyDate = item->study().dateTime().date();
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
