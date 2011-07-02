/***************************************************************************
 *   Copyright (C) 2011 by EP Studios, Inc.                                *
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

#ifndef STUDYWRITER_H
#define STUDYWRITER_H

namespace EpStudy {

class Study;

// StudyWriter extracts the location writing logic common to both
// StudyTable and StudyManager.
class StudyWriter {
public:
    enum WriteLocations {
        WriteNowhere        = 0x00,
        WriteOptical        = 0x01,
        WriteSystem         = 0x02,
        WriteNetwork        = 0x04,
        WriteSystemOptical  = WriteSystem | WriteOptical,
        WriteNetworkSystem  = WriteNetwork | WriteSystem,
        WriteAll            = WriteNetwork | WriteSystemOptical
    };
    explicit StudyWriter(bool useNetwork = false);
    bool useNetwork() const {return useNetwork_;}
    void setUseNetwork(bool value) {useNetwork_ = value;}

    WriteLocations preRegisterStudy() const;
    WriteLocations fullStudy() const;
    WriteLocations study(Study*) const;
private:
    bool useNetwork_;
};

} // namespace EpStudy

#endif // STUDYWRITER_H
