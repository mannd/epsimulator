/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#ifndef WINDOWSETTING_H
#define WINDOWSETTING_H

#include "recorder.h"

class QDataStream;

namespace EpRecorder {

class WindowSetting {
    friend QDataStream& operator<<(QDataStream&, const WindowSetting&);
    friend QDataStream& operator>>(QDataStream&, WindowSetting&);

    // basic window data for any window
    struct Window {
        QByteArray geometry_;
        QSize size_;
        QPoint pos_;
    };

    struct MainWindow : public Window {
        int number_;
        QByteArray state_;
    };

    struct SubWindow : public Window {
        QString key_;
    };

public:
    WindowSetting(Recorder* recorder);

private:
    Recorder* recorder_;
    MainWindow mainWindow_;
    QStringList subWindowKeys_;
    QList<QMdiSubWindow*> subWindowList_;
};

class WindowSettings {
public:
    friend QDataStream& operator<<(QDataStream&, const WindowSettings&);
    friend QDataStream& operator>>(QDataStream&, WindowSettings&);

    WindowSettings(Recorder* recorder1, Recorder* recorder2 = 0)
        : win1_(recorder1), win2_(recorder2) {}
private:
    WindowSetting win1_;
    WindowSetting win2_;
};

}
#endif // WINDOWSETTING_H
