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

#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include "ui_changepassworddialog.h"

#include <QDialog>

class QWidget;

namespace EpCore {
    class PasswordHandler;
}

namespace EpGui {

using EpCore::PasswordHandler;

class ChangePasswordDialog: public QDialog, private Ui::ChangePasswordDialog {
    Q_OBJECT

public:
    ChangePasswordDialog(QWidget *parent = 0);
    ~ChangePasswordDialog();

    void changePassword() const;
    void clear();

public slots:
    void accept();

private:
    bool testPasswordsEqual() const;
    PasswordHandler* pwHandler_;
};

}

#endif
