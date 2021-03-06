/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#ifndef MOVECOPYSTUDYDIALOG_H
#define MOVECOPYSTUDYDIALOG_H

#include "ui_movecopystudydialog.h"

#include <QDialog>
#include <QString>

namespace EpHardware {
    namespace EpOpticalDisk {
        class OpticalDisk;
    }
}

namespace EpNavigator {

using EpHardware::EpOpticalDisk::OpticalDisk;

class StudyListWidgetItem : public QListWidgetItem {

public:
    StudyListWidgetItem(const QString& studyName, 
        const QString& studyPath, QListWidget* parent = 0);
    QString studyPath() const {return studyPath_;}
private:
    QString studyPath_;

};

class MoveCopyStudyDialog : public QDialog, private Ui::MoveCopyStudyDialog {
    Q_OBJECT

public:
    MoveCopyStudyDialog(QWidget* parent, 
                        EpHardware::EpOpticalDisk::OpticalDisk*);
    QString sourcePath();
    QString destinationPath();
    QList<QString> selectedItems();
    ~MoveCopyStudyDialog();

protected:
    void setUpLabels(const QString& label);
    QString addOpticalToPath(const QString& path);
    QString removeOpticalFromPath(const QString& path);

private slots:
    void sourcePathBrowse();
    void destinationPathBrowse();
    void activateFinishButton();
    void fillStudiesListView();
    void selectAll();

private:
    EpHardware::EpOpticalDisk::OpticalDisk* opticalDisk_;

};

class CopyStudyDialog : public MoveCopyStudyDialog {

public:
    CopyStudyDialog(QWidget* parent, OpticalDisk*);
    ~CopyStudyDialog();

};

class MoveStudyDialog : public MoveCopyStudyDialog {

public:
    MoveStudyDialog(QWidget* parent, OpticalDisk*);
    ~MoveStudyDialog();
};

}

#endif
