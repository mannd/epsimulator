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

#ifndef DISKLABELDIALOG_H
#define DISKLABELDIALOG_H

#include "ui_disklabeldialog.h"

#include <qstring.h>

class DiskLabelDialog : public QDialog, private Ui::DiskLabelDialog
{
  Q_OBJECT

public:
    DiskLabelDialog(QWidget* parent = 0);

    QString label() const;
    QString side() const;

    void setLabel(const QString& label);
    // sideA is the local translation for side A of the disk.
    void setSide(const QString& side);
    void enableSideButtons(bool);
    void enableNoneButton(bool);

    ~DiskLabelDialog();

public slots:
  /*$PUBLIC_SLOTS$*/
    void accept();
    void enableCancelButton();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

