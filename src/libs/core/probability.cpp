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

#include "probability.h"

#include <cstdlib>
#include <ctime>

using EpCore::Probability;

Probability::Probability(double value) : delta_(0) {
    setValue(value);
    srand(time(0));     // initialize random number generator
}

Probability::~Probability() {
}

void Probability::setValue(double value) {
    // probabilities are only between 0.0 and 1.0
    value = value > 1.0 ? 1.0 : value;
    value = value < 0.0 ? 0.0 : value;
    value_ = value;
}

bool Probability::result() const {
    int random = rand() % 100;
    return value_ * 100 > random;
}
