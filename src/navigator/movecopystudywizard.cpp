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
#include "movecopystudywizard.h"

#include "movecopystudydialog.h"

MoveCopyStudyWizard::MoveCopyStudyWizard(QWidget* parent)
    : QStackedWidget(parent) {
//    Q_OBJECT
//protected slots:
//    void exec() {
}


MoveCopyStudyWizard::~MoveCopyStudyWizard()
{
}

CopyStudyWizard::CopyStudyWizard(QWidget* parent)
    : MoveCopyStudyWizard(parent) {
    CopyStudyDialog* d;
    addWidget(d);

}

CopyStudyWizard::~CopyStudyWizard() {
}

MoveStudyWizard::MoveStudyWizard(QWidget* parent)
    : MoveCopyStudyWizard(parent) {
    MoveStudyDialog* d;
    addWidget(d);

}

MoveStudyWizard::~MoveStudyWizard() {
}


