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

#include <QDialog>

class QSqlTableModel;

namespace EpGui {

class AbstractEditItemsDialog : public QDialog,
                                protected Ui::AbstractEditItemsDialog {
    Q_OBJECT
public:
    enum EditorType {NewItem, EditItem, CopyItem};
    enum {Column_Id, Column_Name};
    AbstractEditItemsDialog(const QString& title,
                            QWidget* parent = 0);

protected:
    void showCopyButton(bool);
    bool selectionIsEmpty();
    void selectionIsEmptyWarning();
    void removeItem(QSqlTableModel*);

private slots:
    void insert();
    void edit();
    void copy();
    void del();

private:
    virtual void removeItem() = 0;
    virtual void editItem(EditorType) = 0;
    // copyItem is not abstract, since not implemented for all item types
    virtual void copyItem() {}
};

}  // namespace EpGui

#endif // ABSTRACTEDITITEMSDIALOG_H
