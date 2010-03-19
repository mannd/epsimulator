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

#include <QtCore/QCoreApplication>

class QDataStream;

namespace EpRecorder {

class WindowSetting {
    Q_DECLARE_TR_FUNCTIONS(WindowSetting)

public:
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

    WindowSetting(const QString& name = tr("<default>"));

    static unsigned int magicNumber() {return MagicNumber;}
    static QString fileName() {return fileName_;}
    static QList<WindowSetting> defaultItems();

    void setName(const QString& name) {name_ = name;}
    void setRecorder(Recorder* recorder);

    QString name() const {return name_;}

private:
    enum {MagicNumber = 0x98989f0a};

    static const QString fileName_;

    Recorder* recorder_;
    QString name_;
    MainWindow mainWindow_;
    QStringList subWindowKeys_;
    QList<QMdiSubWindow*> subWindowList_;
    QList<SubWindow> subWindows_;
    QString activeSubWindowKey_;
};

}
#endif // WINDOWSETTING_H
