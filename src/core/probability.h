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
#ifndef PROBABILITY_H
#define PROBABILITY_H

/**
 * This class contains a probability that an event will happen.  
 * The value ranges from 0.0 to 1.0.  Probabilities must be set
 * up keeping the time between probability checks in mind.  For
 * fully independent probabilities (e.g. coin tosses) this doesn't
 * matter.  For continuous probabilities over time, you must know
 * how frequently the probability function will be accessed to 
 * calculate the probability adequately.  E.g. the patient simulator
 * will use a 1 sec timer, so Probability:: value() must be the
 * chance of an event per each sec.
 *
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Probability {
public:
    Probability(double probability);
    // compiler generated copy constructor and operator= are ok

    void setValue(double);
    void setDelta(double);

    double value() const {return value_;}
    double delta() const {return delta_;}

    bool result() const;

    friend Probability operator+(const Probability&, const Probability&);
    friend Probability operator-(const Probability&, const Probability&);

    Probability& operator+=(const Probability&);
    Probability& operator-=(const Probability&);

    // conveniently increase and decrease probability
    Probability& operator++();
    Probability operator++(int);
    Probability& operator--();
    Probability operator--(int);

    

    ~Probability();

private:
    double value_;
    double delta_; // change in value_ with increment/decrement

};

#endif
