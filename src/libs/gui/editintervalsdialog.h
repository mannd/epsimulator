/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#ifndef EDITINTERVALSDIALOG_H
#define EDITINTERVALSDIALOG_H

#include "abstractedititemsdialog.h"

#include "interval.h"
#include "itemlist.h"

namespace EpGui {

class EditIntervalsDialog : public AbstractEditItemsDialog {
    Q_OBJECT
public:
    EditIntervalsDialog(QWidget* parent = 0);

    EpCore::ItemList<EpCore::Interval> intervals() {return intervals_;}

private slots:
    void insert();
    void edit();
    // no copy slot
    void del();

private:
    void createListWidget();
    void makeEditIntervalTypeDialog();
    EpCore::ItemList<EpCore::Interval> intervals_;
};

}

#endif // EDITINTERVALSDIALOG_H
