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
#ifndef CATALOGCOMBOBOX_H
#define CATALOGCOMBOBOX_H

#include "epsim.h"
#include "options.h"

#include <qcombobox.h>

using namespace epsim;

/**
Encapsulates specific behavior of the catalog combo box in Navigator.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class CatalogComboBox : public QComboBox
{
Q_OBJECT
public:
    CatalogComboBox(QWidget *parent = 0, const char *name = 0);
    
    void setBrowse(bool browse) {browse_ = browse;}
    void setSource(CatalogSource source);
    
    CatalogSource source();
    bool browse() const {return browse_;}

    ~CatalogComboBox();

private:
    void setup();

    bool browse_;   // puts in blank line in combobox if true
    Options* options_;
    CatalogSource source_;

};

#endif
