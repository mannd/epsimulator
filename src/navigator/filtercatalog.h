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
#ifndef FILTERCATALOG_H
#define FILTERCATALOG_H

#include "filtercatalogbase.h"
#include "tablelistview.h"  // for FilterStudyType

#include <qdatetime.h>
#include <q3datetimeedit.h>
#include <q3combobox.h>
#include <qlineedit.h>
#include <q3buttongroup.h>
#include <qstring.h>

// public inheritance used here to facilitate testing
class FilterCatalog: public FilterCatalogBase {
Q_OBJECT
public:
    enum DateRange {AnyDate, Today, LastWeek, SpecificDates};
    FilterCatalog(QWidget *parent = 0, const char *name = 0);

    // Functions below return text or * if no text in field
    QString lastNameFilter() const {return makeFilter(lastNameLineEdit->text());}
    QString firstNameFilter() const {return makeFilter(firstNameLineEdit->text());}
    QString mrnFilter() const {return makeFilter(mrnLineEdit->text());}
    QString studyConfigFilter() const {return makeFilter(studyConfigLineEdit->text());}
    QString studyNumberFilter() const {return makeFilter(studyNumberLineEdit->text());}
    QString studyLocationFilter() const {return makeFilter(studyLocationLineEdit->text());}
    DateRange dateFilter() const {return static_cast<DateRange>(studyDateButtonGroup->selectedId());}
    QDate beginDate() const {return beginDateEdit->date();}
    QDate endDate() const {return endDateEdit->date();}
    TableListView::FilterStudyType filterStudyType() const {
        return static_cast<TableListView::FilterStudyType>(studyTypeComboBox->currentItem());}

    ~FilterCatalog() {}
public slots:
    void enableDateRange();
    void clearForm();

private:
    void setDefaultDates();
    QString makeFilter(QString text) const {return text.isEmpty() ? "*" : text;}
};

#endif
