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
#ifndef MOVECOPYSTUDYWIZARD_H
#define MOVECOPYSTUDYWIZARD_H

#include <QStackedWidget>

/**
	@author David Mann <mannd@epstudiossoftware.com>
*/
class MoveCopyStudyWizard : public QStackedWidget {
public:
    MoveCopyStudyWizard(QWidget* parent = 0);

    ~MoveCopyStudyWizard();

};

class CopyStudyWizard : public MoveCopyStudyWizard {
    Q_OBJECT
public:
    CopyStudyWizard(QWidget* parent = 0);

    ~CopyStudyWizard();

};

class MoveStudyWizard : public MoveCopyStudyWizard {
    Q_OBJECT
public:
    MoveStudyWizard(QWidget* parent = 0);

    ~MoveStudyWizard();

};


#endif
