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

#ifndef STUDYINFORMATIONDIALOG_H
#define STUDYINFORMATIONDIALOG_H

#include "ui_studyinformationdialog.h"

#include <QDialog>

namespace Ui {
    class StudyInformationDialog;
}

namespace EpStudy {

class StudyInformationDialog : public QDialog,
    private Ui::StudyInformationDialog {
    Q_OBJECT
public:
    StudyInformationDialog(QWidget *parent = 0);
    ~StudyInformationDialog() {}

    enum List {ConsciousSedation, Procedures, Staff, Supplies,
               Medications, Radiology, Orders, Complications, Contrast};

    void openList(List list);

protected:
    //void changeEvent(QEvent *e);

private:
    Ui::StudyInformationDialog *ui;
};

}

#endif
