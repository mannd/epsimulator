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

/**
 *  \file systemdialog.h
 *  Sets file paths, etc.
 */

#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include "systemdialogbase.h"

#include <qlineedit.h>
#include <qstring.h>

class SystemDialog : public SystemDialogBase {
    Q_OBJECT

public:
    SystemDialog(QWidget *parent = 0, const char *name = 0);
//     QString opticalStudyPath() const {return opticalStudyPathLineEdit->text();}
//     QString networkStudyPath() const {return networkStudyPathLineEdit->text();}
//     QString exportFilePath() const {return exportFilePathLineEdit->text();}
// 
//     void setOpticalStudyPath(QString path);
//     void setNetworkStudyPath(QString path);
//     void setExportFilePath(QString path);
    
/// TODO make the set check boxes functions, so that the line edits and
/// browse buttons will be enabled correctly
    void setEnableNetworkStorageCheckBox(bool checked);
    void setEnableFileExportCheckBox(bool checked);

    ~SystemDialog();

public slots:
    void opticalStudyPathBrowse();
    void networkStudyPathBrowse();
    void exportFilePathBrowse();
    void enableExportFilePathLineEdit();
    void enableNetworkStudyPathLineEdit();

private:
    void browseFilePaths(QLineEdit*);
};

#endif
