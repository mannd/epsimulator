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
#ifndef OPTIONS_H
#define OPTIONS_H

#include <qstring.h>

/**
Singleton class providing one-stop shopping for all program options.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class Options{
public:
    static Options* instance();

    void setLocalStudyPath(const QString& localStudyPath) {localStudyPath_ = localStudyPath;}
    void setOpticalStudyPath(const QString& opticalStudyPath) {opticalStudyPath_ = opticalStudyPath;}
    void setOtherStudyPath(const QString& otherStudyPath) {otherStudyPath_ = otherStudyPath;}
    void setNetworkStudyPath(const QString& networkStudyPath) {networkStudyPath_ = networkStudyPath;}

    QString localStudyPath() const {return localStudyPath_;}
    QString opticalStudyPath() const {return opticalStudyPath_;}
    QString otherStudyPath() const {return otherStudyPath_;}
    QString networkStudyPath() const {return networkStudyPath_;}

    void readSettings();
    void writeSettings();

    ~Options();

protected:
    Options();
    static Options* instance_;

private:
    QString localStudyPath_;
    QString opticalStudyPath_;
    QString networkStudyPath_;
    QString otherStudyPath_;

};

#endif
