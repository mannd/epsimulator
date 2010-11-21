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

#ifndef EDITPROTOCOLSDIALOG_H
#define EDITPROTOCOLSDIALOG_H

#include "abstractedititemsdialog.h"

#include "studyconfiguration.h"

class QSqlRelationalTableModel;

namespace EpGui {

class EditProtocolsDialog : public AbstractEditItemsDialog {
    Q_OBJECT
public:
    enum {
        Protocol_Id = 0,
        Protocol_Name = 1,
        Protocol_SenseChannelLabelID = 2,
        Protocol_ColumnFormatID = 3,
        Protocol_WindowSettingID = 4,
        Protocol_MacroCategoryID = 5,
        Protocol_UpdateReviewWindowID = 6,
        Protocol_FocalPointID = 7,
        Protocol_DisplayPageID = 8
    };
    EditProtocolsDialog(QWidget* parent = 0);

private:
    virtual void removeItem();
    virtual void editItem(EditorType);
    virtual void copyItem();

    QSqlRelationalTableModel* model_; 
};

} // namespace EpGuid

#endif // EDITPROTOCOLSDIALOG_H
