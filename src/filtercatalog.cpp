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
#include "filtercatalog.h"

#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <qlabel.h>
#include <qradiobutton.h>


FilterCatalog::FilterCatalog(QWidget *parent, const char *name)
    : FilterCatalogBase(parent, name) {
    todayLabel->setText(QDate::currentDate().toString());
    thisWeekLabel->setText(tr("%1 to %2")
			    .arg(QDate::currentDate()
				 .addDays(-7).toString())
			    .arg(QDate::currentDate().toString()));
    // Set default begin and end dates to a "month" (30 days).
    beginDateEdit->setDate(QDate::currentDate().addDays(-30));
    endDateEdit->setDate(QDate::currentDate());
    
}

void FilterCatalog::enableDateRange() {
    bool checked = datesBetweenRadioButton->isChecked();
    beginDateEdit->setEnabled(checked);
    endDateEdit->setEnabled(checked);
}
