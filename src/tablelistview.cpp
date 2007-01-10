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

#include <qfile.h>
#include <qmessagebox.h>

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
/// TODO Figure out if item is a Local, Optical
//        if (study_.path() == options->localStudyPath())
}


TableListViewItem::~TableListViewItem() {
}

TableListView::TableListView(QWidget* parent, Options* options) 
    : QListView(parent), filtered_(false), options_(options) {
    connect(this, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)), 
	    parent->parent(), SLOT(newStudy()));
/// FIXME non of the below is needed
//    catalogSource_ = options->enableNetworkStorage() 
//	? Network : System;
//    catalogs_ = new Catalogs(catalogSource_);
}

TableListView::~TableListView() {
//    delete catalogs_;
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
    QFile file(fileName);
    // create a studies file if it doesn't exist already
    if (!file.exists()) 
        save(fileName);
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

void TableListView::addStudy(const Study& study) {
    (void) new TableListViewItem(this, study,
            study.isPreregisterStudy() ? tr("Pre-Register") : tr("Study"),
            study.name().fullName(true, true),
            study.mrn(),
            study.dateTime().toString(),
            study.config(),
            study.number(),
            //study.path(),
            ///FIXME below is temporary
            study.file(),
            study.key());
}



void TableListView::readFromStream(QDataStream& in) {
    clear();
    while (!in.atEnd()) {
        Study study;
        in >> study;
        addStudy(study);
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
    
void TableListView::error(const QFile& file, const QString& message) {
    QMessageBox::warning(0, tr("EP Simulator"), message.arg(file.name()));
}

void TableListView::ioError(const QFile& file, const QString& message) {
    error(file, message + ": " + file.errorString());
}

void TableListView::applyFilter( FilterStudyType filterStudyType,
                                            QRegExp lastName,
                                            QRegExp firstName,
                                            QRegExp mrn,
                                            QRegExp studyConfig,
                                            QRegExp studyNumber,
                                            QRegExp studyFile,
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
            studyFile.exactMatch(item->study().file()) &&
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
