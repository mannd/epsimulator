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
#ifndef STUDYCONFIGURATION_H
#define STUDYCONFIGURATION_H

#include <QColor>
#include <QString>

#include <bitset>

/**
 *Emulates all the study configuration settings seen on the Study Configuration Dialog.  
 *
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class StudyConfiguration {

public:
    StudyConfiguration();

    ~StudyConfiguration();

};

class Channel {

public:
    enum Clip {NoClip, C1, C2};
    enum Type {Bipolar, UnipolarWCT, UnipolarAuxRef, 
                      AblateRecord, NotUsed};

    Channel();
    ~Channel();

private:
    int number_;
    QString label_;
    Clip clip_;
    QColor color_; 
    std::bitset<8> displayPages_;
    bool allwaysSave_;
    Type type_;
    int posInput_;
    int negInput_;
    int gain_;
    double highPassFilter_;
    double lowPassFilter_;
    bool notchFilter_;
    
};

#endif
