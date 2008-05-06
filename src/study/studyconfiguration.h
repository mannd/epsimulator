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
#include <QList>
#include <QPoint>
#include <QString>

#include <bitset>

class QDataStream;

namespace EpStudy {

class Channel {

public:
    friend QDataStream& operator<<(QDataStream&, const Channel&);
    friend QDataStream& operator>>(QDataStream&, Channel&);

    enum Clip {NoClip, C1, C2};
    enum Type {Bipolar, UnipolarWCT, UnipolarAuxRef, 
                      AblateRecord, NotUsed};

    Channel() {};
    ~Channel() {};

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

class ColumnFormat {

public:
    friend QDataStream& operator<<(QDataStream&, const ColumnFormat&);
    friend QDataStream& operator>>(QDataStream&, ColumnFormat&);

    ColumnFormat(const QString& name = QString()) {}

private:
    QString name_;

};

class WindowSettings {

public:
    friend QDataStream& operator<<(QDataStream&, const WindowSettings&);
    friend QDataStream& operator>>(QDataStream&, WindowSettings&);

    WindowSettings(const QString& name = QString()) {}
    ~WindowSettings() {}

private:
    QString name_;

};

class MacroList {};



class Protocol {

public:
    friend QDataStream& operator<<(QDataStream&, const Protocol&);
    friend QDataStream& operator>>(QDataStream&, Protocol&);

    Protocol(const QString& name = QString()) {}
    ~Protocol() {}

    void setName(const QString& name) {name_ = name;}

    QString name() const {return name_;}
    
private:
    QString name_;
    Channel senseChannel_;
    ColumnFormat columnFormat_;
    WindowSettings windowSettings_;
    MacroList macroList_;
    bool updateReviewWindow_;
    QPoint focalPoint_;
    int displayPage_;
};

/**
 *Emulates all the study configuration settings seen on the Study Configuration Dialog.  
 *
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class StudyConfiguration {

public:
    friend QDataStream& operator<<(QDataStream&, const StudyConfiguration&);
    friend QDataStream& operator>>(QDataStream&, StudyConfiguration&);

    StudyConfiguration(const QString& name = QString());
    ~StudyConfiguration();

    void setName(const QString& name) {name_ = name;}

    QString name() const {return name_;}

private:
    static const QString configFileName_;

    QString name_;
    QList<Protocol> protocolList_;
    QList<Channel> channelList_;
    

};

}

#endif
