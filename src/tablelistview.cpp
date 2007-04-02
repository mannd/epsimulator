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
#include "options.h"

#include <qdatastream.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qheader.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qstringlist.h>

/**
 * Constructor for TableListViewItem subclass of Navigator
 * @param parent 
 * @param study 
 * @param label1  etc.
 */

TableListViewItem::TableListViewItem(TableListView* parent, 
						const Study& study,
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
                    filteredOut_(false) {
}


TableListViewItem::~TableListViewItem() {
}

TableListView::TableListView(QWidget* parent, Options* options) 
    : QListView(parent), filtered_(false), options_(options) {
    bool oldStyle = options_->oldStyleNavigator();
    addColumn(tr("Study Type"));        // col 0
    if (oldStyle) {
        addColumn(tr("Last Name"));     // col 1
        addColumn(tr("First Name"));    // col 2
    }
    else
        addColumn(tr("Patient"));       // col 1
    addColumn(tr("MRN"));               // col 2 or 3
    addColumn(tr("Study Date/Time"));   // col 3 or 4
    addColumn(tr("Study Config"));      // col 4 or 5
    addColumn(tr("Study Number"));      // col 5 or 6
    /// FIXME this needs to be disk label/network location
    addColumn(tr("Location of Study")); // col 6 or 7
    /// FIXME Below can be eliminated, but must also eliminate keycolumn
//    addColumn(tr("Hidden key"));         // col 7

    setAllColumnsShowFocus(true);
    setShowSortIndicator(true);
    setSortColumn(oldStyle ? 4 : 3);    // default sort is date/time
    // hide the hidden key column: make sure it is defined correctly in keyColumn
//     int keyColumn = 7;
//     setColumnWidthMode(keyColumn, QListView::Manual);
//     hideColumn(keyColumn);
//    header()->setResizeEnabled(false, keyColumn);
//    setResizeMode(QListView::AllColumns);
    // above messes up the hidden column
//    connect(this, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)), 
//	    parent->parent(), SLOT(newStudy()));
}

TableListView::~TableListView() {
}

void TableListView::showTable() {
    QListViewItemIterator it(this);
    while (it.current()) {
        TableListViewItem* item = dynamic_cast<TableListViewItem*>(*it);
        bool show = !filtered_ || !item->filteredOut();
        item->setVisible(show);
        ++it;
    }
}

/**
 * Loads studies catalog file
 * @param fileName is name of file to be loaded.
 * @return false if catalog file can't be loaded for any reason
 */
bool TableListView::load(const QString& fileName) {
    // must clear list first
    clear();
    QFile file(fileName);
    // create a studies file if it doesn't exist already
    if (!file.exists()) 
        if (!save(fileName))    // if you can't even do this, things are bad
            return false;
    if (!file.open(IO_ReadOnly)) {
        ioError(file, tr("Cannot open file %1 for reading"));
        return false;
    }
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != MagicNumber) {
        error(file, tr("File %1 is not a EP Study file"));
        return false;
    }
    readFromStream(in);
    if (file.status() != IO_Ok) {
        ioError(file, tr("Error reading from file %1"));
        return false;
    }
    return true;
}

bool TableListView::save(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(IO_WriteOnly)) {
        ioError(file, tr("Cannot open file %1 for writing"));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(5);
    out << (Q_UINT32)MagicNumber;
    writeToStream(out);
    if (file.status() != IO_Ok) {
        ioError(file, tr("Error writing to file %1"));
        return false;
    }
    return true;
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
bool TableListView::save(const QStringList& fileNames) {
    bool success = true;
    for (QStringList::ConstIterator it = fileNames.begin(); it != fileNames.end(); ++it ) {
        success = success && save(*it);
    }
    return success;
}

void TableListView::addStudy(const Study* study) {
    // Need a throwaway variable to do the following if else.
    TableListViewItem* t = 0;
    if (options_->oldStyleNavigator()) {
        t = new TableListViewItem(this, *study,
            study->isPreregisterStudy() ? tr("Pre-Register") : tr("Study"),
            study->name().last,
            study->name().first,
            study->mrn(),
            study->dateTime().toString(),
            study->config(),
            study->number(),
            study->location()); 
    }
    else {
        t = new TableListViewItem(this, *study,
            study->isPreregisterStudy() ? tr("Pre-Register") : tr("Study"),
            study->name().fullName(true, true),
            study->mrn(),
            study->dateTime().toString(),
            study->config(),
            study->number(),
            study->location());
    }
}

void TableListView::deleteStudy() {
    QListViewItem* item = selectedItem();
    delete item;
    showTable();
}



void TableListView::readFromStream(QDataStream& in) {
    clear();
    while (!in.atEnd()) {
        Study study;
        in >> study;
        addStudy(&study);
    }
}

void TableListView::writeToStream(QDataStream& out) {
  QListViewItemIterator it(this);
  while (it.current()) {
    QListViewItem* item = *it;
    out << dynamic_cast<TableListViewItem*>(item)->study();
    ++it;
  }
}

void TableListView::exportCSV(const QString& fileName) {
    QFile file(fileName);
    if (!file.exists()) 
        save(fileName);
    if (!file.open(IO_WriteOnly)) {
        ioError(file, tr("Cannot open file %1 for writing"));
        return;
    }
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
    if (file.status() != IO_Ok) {
        ioError(file, tr("Error writing to file %1"));
        return;
    }
}
    
void TableListView::error(const QFile& file, const QString& message) {
    QMessageBox::warning(0, tr("EP Simulator"), message.arg(file.name()));
}

void TableListView::ioError(const QFile& file, const QString& message) {
    error(file, message + ": " + file.errorString());
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
