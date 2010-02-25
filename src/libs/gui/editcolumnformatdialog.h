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

#ifndef EDITCOLUMNFORMATDIALOG_H
#define EDITCOLUMNFORMATDIALOG_H

#include "ui_editcolumnformatdialog.h"

#include "abstractedititemsdialog.h"
#include "columnformat.h"

namespace EpGui {

class EditColumnFormatDialog : public QDialog,
        private Ui::EditColumnFormatDialog {
    Q_OBJECT
public:
    EditColumnFormatDialog(AbstractEditItemsDialog::EditorType,
                           QWidget *parent = 0);
    void setColumnFormat(const EpCore::ColumnFormat&);

    EpCore::ColumnFormat columnFormat() const;

private slots:
    void enableSelectButtons(const QString&);
};

}
#endif // EDITCOLUMNFORMATDIALOG_H
