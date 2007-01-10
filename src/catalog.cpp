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

#include "catalog.h"
#include "options.h"

Catalog::Catalog()
{
}

OpticalCatalog::OpticalCatalog() {
}

SystemCatalog::SystemCatalog() {
}

NetworkCatalog::NetworkCatalog() {
}

Catalogs::Catalogs(CatalogSource defaultCatalog) {
    networkCatalog_ = new NetworkCatalog;
    systemCatalog_ = new SystemCatalog;
    opticalCatalog_ = new OpticalCatalog;
    otherCatalog_ = new Catalog;
    switch (defaultCatalog) {
	case Network:
	    currentCatalog_ = networkCatalog_;
	    break;
	case System:
	    currentCatalog_ = systemCatalog_;
	    break;
	case Optical:
	    currentCatalog_ = opticalCatalog_;
	    break;
	case Other:
	    currentCatalog_ = otherCatalog_;
	    break;
	default:
	    // good old systemCatalog...
	    currentCatalog_ = systemCatalog_;
    }
}



void Catalogs::refresh() {
}

void Catalogs::regenerate() {
}

Catalogs::~ Catalogs() {
    // don't delete currentCatalog_ it is just a copy of
    // one of the below!
    delete opticalCatalog_;
    delete systemCatalog_;
    delete networkCatalog_;
    delete otherCatalog_;
}

