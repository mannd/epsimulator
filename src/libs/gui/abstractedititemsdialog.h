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

#ifndef ABSTRACTEDITITEMSDIALOG_H
#define ABSTRACTEDITITEMSDIALOG_H

#include "ui_abstractedititemsdialog.h"

#include "itemlist.h"

#include <QDialog>

namespace EpGui {

class AbstractEditItemsDialog : public QDialog,
    protected Ui::AbstractEditItemsDialog {
    Q_OBJECT
public:
    enum EditorType {NewItem, EditItem, CopyItem};
    AbstractEditItemsDialog(const QString& title,
                            QWidget* parent = 0);

protected:
    void showCopyButton(bool);
    bool selectionIsEmpty() const;
    void selectionIsEmptyWarning();
    void duplicateItemWarning(const QString& name);
    template<typename T>
    void removeItemFromList(T& items);
    template<typename T>
    void createListWidgetItems(T&);
    void editCopiedItem(const QString& name);
    template<typename T, typename K>
    bool itemIsDuplicated(const EditorType type,
                          const QString& originalName,
                          const T& item,
                          const K& items);

private slots:
    virtual void insert();
    virtual void edit();
    virtual void copy();
    void del();

private:
    virtual void createListWidget() = 0;
    virtual void removeItem() = 0;
    virtual void editItem(EditorType) = 0;
    // copyItem is not abstract, since not implemented for all item types
    virtual void copyItem(const QList<QListWidgetItem*>&) {}
};


template<typename T>
void AbstractEditItemsDialog::removeItemFromList(T& items) {
    items.remove(listWidget->currentItem()->text());
}

template<typename T>
void AbstractEditItemsDialog::createListWidgetItems(T& items) {
    listWidget->clear();
    listWidget->setSortingEnabled(true);
    for (int i = 0; i < items.size(); ++i) {
        new QListWidgetItem(items[i].name(), listWidget);
    }
}

template<typename T, typename K>
bool AbstractEditItemsDialog::itemIsDuplicated(const EditorType type,
                                               const QString& originalName,
                                               const T& item,
                                               const K& items) {
    bool duplicate = ((type == NewItem && items.duplicate(item)) ||
                      (type == EditItem && item.name() != originalName &&
                       items.duplicate(item)));
    if (duplicate)
        duplicateItemWarning(item.name());
    return duplicate;
}

}

#endif // ABSTRACTEDITITEMSDIALOG_H
