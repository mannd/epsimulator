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
#ifndef STUDYTABLE_H
#define STUDYTABLE_H

#include "study.h"

#include <qtable.h>

/**
	@author David Mann <mannd@epstudiossoftware.com>
*/
class StudyTable : public QTable {
    Q_OBJECT
public:
    StudyTable(QWidget* parent = 0, const char* name = 0);

    void refresh(Studies studies);

    ~StudyTable();

protected slots:
    void columnClicked(int col);

private:
    enum {NUM_ROWS = 40, NUM_COLS = 4};
    void clear();
    bool ascending_;    // column sorting order
    


};

#endif
