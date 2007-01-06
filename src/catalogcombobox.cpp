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
#include "catalogcombobox.h"

CatalogComboBox::CatalogComboBox(QWidget *parent, const char *name)
 : QComboBox(parent, name), browse_(true)
{
    options_ = Options::instance();
    setup();
}

void CatalogComboBox::setSource(CatalogSource source) {
    source_ = source;
    /// TODO 
}

void CatalogComboBox::setup() {
    clear();
    source_ = System;
    if (options_->enableNetworkStorage()) {
        insertItem(tr("Network"));
        source_ = Network;
    }
    insertItem(tr("System        "));
    insertItem(tr("Optical"));
    if (browse_) {
        insertItem("");
        source_ = Other;
    }
}

CatalogComboBox::~CatalogComboBox()
{
    // DO NOT delete options_!!
}


