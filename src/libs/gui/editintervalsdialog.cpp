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

#include "editintervalsdialog.h"

#include <QMessageBox>
#include <QStringList>

using EpCore::Interval;
using EpCore::ItemList;
using EpGui::EditIntervalsDialog;

EditIntervalsDialog::EditIntervalsDialog(QWidget* parent)
        : AbstractEditItemsDialog(tr("Intervals"), parent) {
    showCopyButton(false);
    createListWidget();
}

void EditIntervalsDialog::createListWidget() {
    listWidget->setSortingEnabled(true);
    listWidget->clear();
    for (int i = 0; i < intervals_.size(); ++i)
        new QListWidgetItem(intervals_[i].name(), listWidget);
}

void EditIntervalsDialog::insert() {
//    bool ok = false;
//    QString name;
//    name
}

void EditIntervalsDialog::edit() {
   // makeEditIntervalTypeDialog();
}

void EditIntervalsDialog::del() {
    if (listWidget->selectedItems().size() == 0) {
        return;
    }
    int result = QMessageBox::warning(this, tr("Delete Inteval?"),
                         tr("Really delete this Interval?"
                            " Deletion will be permanent."),
                            QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        intervals_.removeAt(listWidget->currentRow());
        createListWidget();
    }
}

void EditIntervalsDialog::makeEditIntervalTypeDialog() {
//   /* QDialog d(this);
//    d.setWindowTitle(tr("Edit Interval Type"));
//    d.exec(*/);

}
