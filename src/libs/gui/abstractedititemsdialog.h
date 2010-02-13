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

namespace EpGui {

class AbstractEditItemsDialog : public QDialog,
    protected Ui::AbstractEditItemsDialog {
    Q_OBJECT
public:
    AbstractEditItemsDialog(const QString& title,
                            QWidget* parent = 0);

protected:
    void showCopyButton(bool);

private slots:
    virtual void insert() = 0;
    virtual void edit() = 0;
    virtual void copy();
    virtual void del() = 0;
};


}

#endif // ABSTRACTEDITITEMSDIALOG_H
