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
#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include "systemdialogbase.h"

#include <qlineedit.h>
#include <qstring.h>

class SystemDialog : public SystemDialogBase {
    Q_OBJECT

public:
    SystemDialog(QWidget *parent = 0, const char *name = 0);
    QString studyPath() const {return localStudyPathLineEdit->text();}
    QString networkStudyPath() const {return networkStudyPathLineEdit->text();}

    void setStudyPath(QString path);
    ~SystemDialog();

public slots:
    virtual void localStudyPathBrowse();
    virtual void networkStudyPathBrowse();

private:
    void browseFilePaths(QLineEdit*);
};

#endif
