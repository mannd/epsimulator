/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
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

#ifndef EDITSTUDYCONFIGSDIALOG_H
#define EDITSTUDYCONFIGSDIALOG_H

#include "abstractedititemsdialog.h"

class QSqlTableModel;

namespace EpNavigator {

    class EditStudyConfigsDialog: public EpGui::AbstractEditItemsDialog {
    Q_OBJECT

public:
    enum {
        StudyConfiguration_Id = 0,
        StudyConfiguration_Name = 1
    };
    EditStudyConfigsDialog(QWidget *parent = 0);

private:
    virtual void removeItem();
    virtual void copyItem();
    virtual void editItem(EditorType);
    QSqlTableModel* model_;
};

}

#endif // EDITSTUDYCONFIGSDIALOG_H
