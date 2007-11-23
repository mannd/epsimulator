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

#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include "ui_passworddialog.h"

class Options;
class PasswordHandler;
class QString;

class PasswordDialog : public QDialog, 
    private Ui::PasswordDialog {
    Q_OBJECT

public:
    PasswordDialog(Options* options, QWidget* parent = 0);

    /// public function, just for debugging and testing.
    void setPassword(const QString&); 
    bool testPassword();

    ~PasswordDialog();

public slots:
    virtual void accept();
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:
    PasswordHandler* pwHandler_;

};

#endif
