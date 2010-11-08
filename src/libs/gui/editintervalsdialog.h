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

#ifndef EDITINTERVALSDIALOG_H
#define EDITINTERVALSDIALOG_H

#include "abstractedititemsdialog.h"

class QComboBox;
class QDataWidgetMapper;
class QDialogButtonBox;
class QLineEdit;
class QSpinBox;
class QSqlRelationalTableModel;

namespace EpGui {

class EditIntervalsDialog : public AbstractEditItemsDialog {
    Q_OBJECT
public:
    enum {
        Interval_Id = 0,
        Interval_Name = 1,
        Interval_Mark1 = 2,
        Interval_Mark2 = 3,
        Interval_Width = 4
    };
    EditIntervalsDialog(QWidget* parent = 0);

private:
    virtual void removeItem();
    virtual void editItem(EditorType);
    QSqlRelationalTableModel* model_;
};



class EditIntervalTypeDialog : public QDialog {
    Q_OBJECT
public:
    EditIntervalTypeDialog(AbstractEditItemsDialog::EditorType,
                           QSqlRelationalTableModel* model,
                           int id,
                           QWidget* parent = 0);

public slots:
    virtual void accept();

private slots:
    void enableOkButton(const QString&);

private:
    QDataWidgetMapper* mapper_;
    QLineEdit* nameLineEdit_;
    QComboBox* mark1ComboBox_;
    QComboBox* mark2ComboBox_;
    QSpinBox* widthSpinBox_;
    QDialogButtonBox* buttonBox_;
};

} // namespace EpGui

#endif // EDITINTERVALSDIALOG_H
