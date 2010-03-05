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

#include "windowsetting.h"

#include "displaywindow.h"

using EpRecorder::WindowSetting;


QString const WindowSetting::fileName_ = "windowsettings.dat";

WindowSetting::WindowSetting(const QString& name)
    : recorder_(0), name_(name) {}

void WindowSetting::setRecorder(Recorder* recorder) {
    if ((recorder_ = recorder)) {
        mainWindow_.number_ = recorder_->recorderWindow() == Primary
                              ? 1 : 2;
        mainWindow_.geometry_ = recorder_->saveGeometry();
        mainWindow_.size_ = recorder_->size();
        mainWindow_.pos_ = recorder_->pos();
        mainWindow_.state_ = recorder_->saveState();
        DisplayWindow* dw = 0;
        QMdiSubWindow* activeSubWindow = static_cast<QMdiArea*>(
                recorder_->centralWidget())->currentSubWindow();
        subWindowList_ = static_cast<QMdiArea*>(
                recorder_->centralWidget())->subWindowList();
        for (int i = 0; i < subWindowList_.size(); ++i) {
            dw = static_cast<DisplayWindow*>(subWindowList_[i]->widget());
            subWindowKeys_ << dw->key();
            if (subWindowList_[i] == activeSubWindow)
                activeSubWindowKey_ = dw->key();
            subWindows_[i].key_ = dw->key();
            subWindows_[i].geometry_ = subWindowList_[i]->saveGeometry();
            subWindows_[i].size_ = subWindowList_[i]->size();
            subWindows_[i].pos_ = subWindowList_[i]->pos();
        }
    }
}


QList<WindowSetting> WindowSetting::defaultItems() {
    QList<WindowSetting> windowSettings;
    windowSettings.append(WindowSetting(tr("<default>")));
    return windowSettings;
}

namespace EpRecorder {

     QDataStream& operator<<(QDataStream& out, const WindowSetting& setting) {
        out << setting.name_ << setting.mainWindow_.number_
                << setting.mainWindow_.geometry_
                << setting.mainWindow_.size_ << setting.mainWindow_.pos_
                << setting.mainWindow_.state_ << setting.activeSubWindowKey_
                << setting.subWindowList_;
        for (int i = 0; i < setting.subWindows_.size(); ++i) {
            out << setting.subWindows_[i].key_
                    << setting.subWindows_[i].geometry_
                    << setting.subWindows_[i].size_
                    << setting.subWindows_[i].pos_;
        }
        return out;
    }

    QDataStream& operator>>(QDataStream& in, WindowSetting& setting) {
        in >> setting.name_ >> setting.mainWindow_.number_
                >> setting.mainWindow_.geometry_
                >> setting.mainWindow_.size_ >> setting.mainWindow_.pos_
                >> setting.mainWindow_.state_ >> setting.activeSubWindowKey_
                >> setting.subWindowKeys_;
        // number of subWindowKeys == number of subWindows
        for (int i = 0; i < setting.subWindowKeys_.size(); ++i) {
            in >> setting.subWindows_[i].key_
                    >> setting.subWindows_[i].geometry_
                    >> setting.subWindows_[i].size_
                    >> setting.subWindows_[i].pos_;
        }
        return in;
    }

}
