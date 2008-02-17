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

#include "reviewwindow.h"

#include "settings.h"

#include <QSplitter>

ReviewWindow::ReviewWindow(int number, QWidget *parent)
 : SignalDisplayWindow(parent), number_(number) {
    setWindowTitle(tr("Review %1").arg(QString::number(number_)));
}

void ReviewWindow::saveSettings() {
    Settings settings;
    QString prefix = "/reviewWindow" + QString::number(number_);
    settings.setValue(prefix + "Size", size());
    settings.setValue(prefix + "Pos", pos()); 
    settings.setValue(prefix + "State", saveState());
    settings.setValue(prefix + "Splitter", 
        static_cast<QSplitter*>(centralWidget())->saveState());   
}

void ReviewWindow::readSettings() {
    Settings settings;
    QString prefix = "/reviewWindow" + QString::number(number_);
    restoreState(settings.value(prefix + "State").toByteArray());
    QVariant size = settings.value(prefix + "Size");
    if (size.isNull()) {
        showMaximized();
        return;
    }
    resize(size.toSize());
    move(settings.value(prefix + "Pos").toPoint());
    static_cast<QSplitter*>(centralWidget())->restoreState(settings.value(
        prefix + "Splitter").toByteArray());
}


ReviewWindow::~ReviewWindow() {}


