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

#include "editcolumnformatdialog.h"

#include "itemlist.h"
#include "listselector.h"

#include <QStringListModel>

using EpCore::ColumnFormat;
using EpCore::Interval;
using EpCore::ItemList;
using EpGui::EditColumnFormatDialog;

EditColumnFormatDialog::EditColumnFormatDialog(
        AbstractEditItemsDialog::EditorType type,
        QWidget *parent)
            :  QDialog(parent), listSelector_(0) {
    setupUi(this);
    listSelector_ = new ListSelector(availableListView, selectedListView);
    QString title = type == AbstractEditItemsDialog::NewItem
                        ? tr("New") : tr("Edit");
    title = tr("%1 Column Format").arg(title);
    setWindowTitle(title);
    connect(selectPushButton, SIGNAL(clicked()), this, SLOT(select()));
    connect(unselectPushButton, SIGNAL(clicked()), this, SLOT(unselect()));
    connect(selectAllPushButton, SIGNAL(clicked()),
            this, SLOT(selectAll()));
    connect(unselectAllPushButton, SIGNAL(clicked()),
            this, SLOT(unselectAll()));
    connect(availableListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enableSelectButtons()));
    connect(selectedListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enableSelectButtons()));
    connect(availableListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select()));
    connect(selectedListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(unselect()));
    connect(nameLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(enableOkButton(const QString&)));
    enableSelectButtons();
    enableOkButton(nameLineEdit->text());
}

EditColumnFormatDialog::~EditColumnFormatDialog() {
    delete listSelector_;
}

void EditColumnFormatDialog::select() {
    listSelector_->select();
    enableSelectButtons();
}

void EditColumnFormatDialog::unselect() {
    listSelector_->unselect();
    enableSelectButtons();
}

void EditColumnFormatDialog::selectAll() {
    listSelector_->selectAll();
    enableSelectButtons();
}

void EditColumnFormatDialog::unselectAll() {
    listSelector_->unselectAll();
    enableSelectButtons();
}

void EditColumnFormatDialog::setColumnFormat(const EpCore::ColumnFormat& cf) {
    nameLineEdit->setText(cf.name());
    listSelector_->
            initialize(ColumnFormat::intervalNames(
                        cf.unselectedIntervals()),
                    ColumnFormat::intervalNames(
                        cf.selectedIntervals()));
}

EpCore::ColumnFormat EditColumnFormatDialog::columnFormat() const {
    QStringListIterator iter(listSelector_->selected());
    ItemList<Interval> allIntervals;
    QList<Interval> intervals;
    while (iter.hasNext())
        intervals.append(allIntervals[iter.next()]);
    EpCore::ColumnFormat cf(nameLineEdit->text(), intervals);
    return cf;
}

void EditColumnFormatDialog::enableSelectButtons() {
    bool availableItemIsSelected =
            availableListView->currentIndex().row() > -1;
    bool selectedItemIsSelected =
            selectedListView->currentIndex().row() > -1;
    selectPushButton->setEnabled(availableItemIsSelected);
    unselectPushButton->setEnabled(selectedItemIsSelected);
}

void EditColumnFormatDialog::enableOkButton(const QString& text) {
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
}
