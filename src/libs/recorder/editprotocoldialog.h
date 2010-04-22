/***************************************************************************
 *   Copyright (C) %YEAR% by EP Studios, Inc.                                *
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

#ifndef EDITPROTOCOLDIALOG_H
#define EDITPROTOCOLDIALOG_H

#include "ui_editprotocoldialog.h"

#include "abstractedititemsdialog.h"
#include "itemlist.h"
#include "studyconfiguration.h"
#include "windowsetting.h"

namespace EpGui {

class EditProtocolDialog : public QDialog, private Ui::EditProtocolDialog {
    Q_OBJECT
public:
    EditProtocolDialog(AbstractEditItemsDialog::EditorType,
                       QWidget *parent = 0);
    void setProtocol(const EpStudy::Protocol&);
    EpStudy::Protocol protocol() const;

private slots:
    void enableOkButton(const QString&);

private:
    EpCore::ItemList<EpCore::ColumnFormat> columnFormats_;
    EpCore::ItemList<EpRecorder::WindowSetting> windowSettings_;
};

}

#endif // EDITPROTOCOLDIALOG_H
