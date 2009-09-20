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

#ifndef FILTERCATALOGDIALOG_H
#define FILTERCATALOGDIALOG_H

#include "ui_filtercatalogdialog.h"
#include "tablelistview.h"  // for FilterStudyType

#include <QDialog>

namespace EpNavigator {

class FilterCatalogDialog : public QDialog, 
                            private Ui::FilterCatalogDialog {
    Q_OBJECT

public:
    enum DateRange {AnyDate, Today, LastWeek, SpecificDates};

    FilterCatalogDialog(QWidget *parent = 0);
    ~FilterCatalogDialog() {}

    // Functions below return text or * if no text in field
    QString lastNameFilter() const {return makeFilter(lastNameLineEdit->
                                    text());}
    QString firstNameFilter() const {return makeFilter(firstNameLineEdit->
                                     text());}
    QString mrnFilter() const {return makeFilter(mrnLineEdit->text());}
    QString studyConfigFilter() const {return makeFilter(studyConfigLineEdit->
                                       text());}
    QString studyNumberFilter() const {return makeFilter(studyNumberLineEdit->
                                       text());}
    QString studyLocationFilter() const {return makeFilter(
                                         studyLocationLineEdit->text());}
    DateRange dateFilter();
    QDate beginDate() const {return beginDateEdit->date();}
    QDate endDate() const {return endDateEdit->date();}
    TableListView::FilterStudyType filterStudyType() const {
        return static_cast<TableListView::FilterStudyType>
        (studyTypeComboBox->currentIndex());}

public slots:
    void enableDateRange();
    void clearForm();

private:
    void setDefaultDates();
    QString makeFilter(QString text) const {
        return text.isEmpty() ? "*" : text;}

};

}

#endif
