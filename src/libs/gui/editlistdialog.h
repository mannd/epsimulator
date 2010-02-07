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

#ifndef EDITLISTDIALOG_H
#define EDITLISTDIALOG_H

#include "ui_editlistdialog.h"

#include <QDialog>

class QStringListModel;

namespace EpGui {

class EditListDialog: public QDialog, private Ui::EditListDialog {
    Q_OBJECT

public:
    EditListDialog(const QStringList& items,
                   const QString& title,
                   const QString& label,
                   QWidget* parent = 0);

    QStringList items();

private slots:
    void editItem();
    void newItem();
    void deleteItem();

private:
    QStringListModel* model_;
};

}

#endif // EDITLISTDIALOG_H

