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

#include "listselector.h"

#include <QListView>
#include <QStringListModel>

using EpGui::ListSelector;

ListSelector::ListSelector(QListView* unselectedListView,
                           QListView* selectedListView)
                               : unselectedListView_(unselectedListView),
                               selectedListView_(selectedListView),
                               unselectedModel_(0),
                               selectedModel_(0) {}

ListSelector::ListSelector(const QStringList& unselected,
                           const QStringList& selected,
                           QListView* unselectedListView,
                           QListView* selectedListView)
                               : unselectedListView_(unselectedListView),
                               selectedListView_(selectedListView) {
    initialize(unselected, selected);
}

ListSelector::~ListSelector() {
    delete unselectedModel_;
    delete selectedModel_;
}

void ListSelector::initialize(const QStringList& unselected,
                              const QStringList& selected) {
    unselectedModel_ = new QStringListModel();
    unselectedModel_->setStringList(unselected);
    unselectedListView_->setModel(unselectedModel_);
    unselectedListView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    selectedModel_ = new QStringListModel();
    selectedModel_->setStringList(selected);
    selectedListView_->setModel(selectedModel_);
    selectedListView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ListSelector::select() {
    move(unselectedListView_, selectedListView_,
         unselectedModel_, selectedModel_);
}

void ListSelector::unselect() {
    move(selectedListView_, unselectedListView_,
         selectedModel_, unselectedModel_);
}

void ListSelector::selectAll() {
    QStringList allItems = all();
    unselectedModel_->setStringList(QStringList());
    selectedModel_->setStringList(allItems);
}

void ListSelector::unselectAll() {
    QStringList allItems = all();
    selectedModel_->setStringList(QStringList());
    unselectedModel_->setStringList(allItems);
}

QStringList ListSelector::all() {
    return selectedModel_->stringList() + unselectedModel_->stringList();
}

void ListSelector::move(QListView* sourceView,
                        QListView* destView,
                        QStringListModel* sourceModel,
                        QStringListModel* destModel) {
    int row = sourceView->currentIndex().row();
    QModelIndex index = sourceModel->index(row);
    QVariant name = index.data();
    sourceModel->removeRows(row, 1);
    row = destView->currentIndex().row();
    destModel->insertRows(++row, 1);
    index = destModel->index(row);
    destView->setCurrentIndex(index);
    destModel->setData(index, name);
}

QStringList ListSelector::selected() {
    return selectedModel_->stringList();
}
