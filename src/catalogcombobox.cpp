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

#include <algorithm>

using namespace std;

CatalogComboBox::CatalogComboBox(QWidget *parent, const char *name)
 : QComboBox(parent, name), browse_(false), network_(false) {
    options_ = Options::instance();
    setup();
}

void CatalogComboBox::setSource(CatalogSource source) {
    // Add the blank Other field to the combobox if not already there
    setBrowse(source == Other && !browse_);
    if (source == Network && !network_) 
        setup();  // this will only work if options_->enableNetworkStorage enabled
    setCurrentItem(sourceMap_[source]);
}

void CatalogComboBox::setBrowse(bool browse) {
    // Don't bother changing setup if browse unchanged.
    if (browse != browse_) {
        browse_ = browse;
        setup();
    }
}

CatalogSource CatalogComboBox::source() {
    int item = currentItem();
    CatalogMap::iterator pos;
    for (pos = sourceMap_.begin(); pos != sourceMap_.end(); ++pos) {
        if (pos->second == item) 
            return pos->first;
    }
    // If above doesn't produce (it should), return safe default.
    setSource(System);
    return System;  // safe default
}
        
void CatalogComboBox::setup() {
    clear();
    sourceMap_.clear();
    int index = 0;
    if (options_->enableNetworkStorage()) {
        insertItem(tr("Network"));
        sourceMap_[Network] = index++;
        network_ = true;
    }
    insertItem(tr("System        "));
    sourceMap_[System] = index++;
    insertItem(tr("Optical"));
    sourceMap_[Optical] = index;
    if (browse_) {
        insertItem("");
        sourceMap_[Other] = ++index;
    }
    setCurrentItem(0);
}

CatalogComboBox::~CatalogComboBox()
{
    // DO NOT delete options_!!
}


