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

#include "signaldisplaywindow.h"

#include <QPalette>
#include <QSizePolicy>
#include <QSplitter>

SignalDisplayWindow::SignalDisplayWindow(QWidget *parent)
 : QMainWindow(parent) {}



void SignalDisplayWindow::createCentralWidget() {
    QSplitter* centralWidget = new QSplitter(Qt::Horizontal, this);
 
    ChannelBar* channelBar = new ChannelBar;
    SignalArea* signalArea = new SignalArea;
    centralWidget->addWidget(channelBar);
    centralWidget->addWidget(signalArea);
    setCentralWidget(centralWidget);
}


SignalDisplayWindow::~SignalDisplayWindow() {}


ChannelBar::ChannelBar(QWidget* parent) : QFrame(parent) {
    setUp();
}

void ChannelBar::setUp() {
    QSizePolicy policy(QSizePolicy::Minimum,
        QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    //policy.setHeightForWidth(false);
    setSizePolicy(policy);
    setFrameShape(QFrame::StyledPanel);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::darkBlue);
    palette.setColor(QPalette::Button, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::white);
    setPalette(palette);
    // necessary to actually apply the Window color to the background
    setAutoFillBackground(true);
    setMaximumWidth(200);
    setMinimumWidth(100);
}

ChannelBar::~ChannelBar() {}

SignalArea::SignalArea(QWidget* parent) : QWidget(parent) {
    QSizePolicy policy(QSizePolicy::Minimum,
        QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(1);
    setSizePolicy(policy);
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);
}

SignalArea::~SignalArea() {}


