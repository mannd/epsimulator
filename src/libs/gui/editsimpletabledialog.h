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

#ifndef EDITSIMPLETABLEDIALOG_H
#define EDITSIMPLETABLEDIALOG_H

#include <QDialog>

class QSqlTableModel;

namespace EpGui {

class EditSimpleTableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditSimpleTableDialog(QWidget *parent = 0);

signals:

public slots:
    void commit();

private:
    QSqlTableModel* model_;

};

} // namespace EpGui

#endif // EDITSIMPLETABLEDIALOG_H
