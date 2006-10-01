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
#include "studytable.h"

#include <iostream>

StudyTable::StudyTable(QWidget* parent, const char* name)
 : QTable(parent, name) {
    clear();
    ascending_ = true;
    
}

void StudyTable::clear() {
    setNumRows(0);
    setNumCols(0);
    setNumRows(NUM_ROWS);
    setNumCols(NUM_COLS);
    horizontalHeader()->setLabel(0, tr("Patient"));
    horizontalHeader()->setLabel(1, tr("MRN"));
    horizontalHeader()->setLabel(2, tr("Study Date"));
    horizontalHeader()->setLabel(3, tr("Study Number"));
    setReadOnly(true);
    setCurrentCell(0, 0);
}

void StudyTable::refresh(Studies studies) {
    Studies::const_iterator pos;
    for (pos = studies.begin(); pos != studies.end(); ++pos) {
        //populate StudyTable here
    }
}

void StudyTable::columnClicked(int col) {
    QTable::sortColumn( col, ascending_, true );
    ascending_ = !ascending_;   // reverse it for next time
}

StudyTable::~StudyTable() {
}


