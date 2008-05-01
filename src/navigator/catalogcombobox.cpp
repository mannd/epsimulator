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

#include "options.h"

using EpNavigator::CatalogComboBox;
using EpNavigator::Catalog;

/** 
 * Ctor.  Gets the options_ instance, sets up the combobox.  
 * Default item is 1st, which is either System or Network depending on options.
 * Other selection is not present unless Browse...
 * is selected from the menu, and goes away as soon as possible.  
 * Default is no Network
*/
CatalogComboBox::CatalogComboBox(QWidget *parent) : 
                                 QComboBox(parent), 
                                 browse_(false), 
                                 includeNetwork_(false),
                                 options_(Options::instance()) {
    setup();
    connect(this, SIGNAL(activated(int)), this, SLOT(resetOther()));
    setCurrentItem(0);  // will be Network or System depending on options
}

CatalogComboBox::~CatalogComboBox() {}

/**
 * When the combobox changes, this slot is called.  
 * It will eliminate the the blank browse row if
 * it is not selected.
 */
void CatalogComboBox::resetOther() {
    if (source() != Catalog::Other && browse_) {
        Catalog::Source newSource = source();
        setBrowse(false);
        setSource(newSource);
    }
}

/**
 * Sets the combobox to the selected source.
 * Handles situations when browsing the "Other" catalog
 * and if Network is inappropriately selected, though this
 * last should also be handled by the menu system by 
 * inactivating the Network menu item.
 * @param source 
 */
void CatalogComboBox::setSource(Catalog::Source source) {
    // Add the blank Other field to the combobox if not already there;
    // also take away the blank field if not browsing, 
    // and set browse_ correctly.
    if (source == Catalog::Other && !browse_)
        setBrowse(true);
    if (source != Catalog::Other && browse_)
        setBrowse(false);
    if (source == Catalog::Network && !includeNetwork_) 
        source = Catalog::System;  // setSource assumes sourceMap has been 
                                   // setup correctly and 
                                   // this assignment is wrong.
    setCurrentItem(sourceMap_[source]);
}

/**
 * Used after system options changed.
 * Sets source back to first item, either Network or System.
 */
void CatalogComboBox::refresh() {
    Catalog::Source oldSource = source();
    setup();
    // must account for Network disappearing
    if (source() == Catalog::Network && !includeNetwork_)
        setSource(Catalog::System);
    else
        setSource(oldSource);
}

/**
 * Will change the browse status if it has changed and
 * reconstruct the combobox.
 * @param browse whether or not browsing is enabled.
 */
void CatalogComboBox::setBrowse(bool browse) {
    // Don't bother changing setup if browse unchanged.
    if (browse != browse_) {
        browse_ = browse;
        setup();
    }
}

Catalog::Source CatalogComboBox::source() {
    int item = currentItem();
    for (CatalogMap::const_iterator pos = sourceMap_.begin(); 
        pos != sourceMap_.end(); ++pos) {
        if (pos->second == item) 
            return pos->first;
    }
    // If above doesn't produce (it should), return safe default.
    setSource(Catalog::System);
    return Catalog::System;  // safe default
}

/**
 * Constructs combobox depending on whether network storage is 
 * enabled.  Also if browse_ is true, inserts the funny blank
 * choice corresponding with browsing to a foreign catalog.
 */
void CatalogComboBox::setup() {
    clear();
    sourceMap_.clear();
    int index = 0;
    includeNetwork_ = options_->enableNetworkStorage();
    if (includeNetwork_) {
        insertItem(tr("Network"));
        sourceMap_[Catalog::Network] = index++;
    }
    insertItem(tr("System        "));
    sourceMap_[Catalog::System] = index++;
    insertItem(tr("Optical"));
    sourceMap_[Catalog::Optical] = index;
    if (browse_) {
        insertItem("");
        sourceMap_[Catalog::Other] = ++index;
    }
}
