/***************************************************************************
 *   Copyright (C) 2009 by EP Studios, Inc.                                *
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

#include "faketitlebar.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPalette>
#include <QPushButton>
#include <QToolButton>

using EpGui::FakeTitleBar;

FakeTitleBar::FakeTitleBar(QWidget* parent) : QWidget(parent) {
    // general background is dark blue
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::darkBlue);
    pal.setColor(QPalette::WindowText, Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);
    setMinimumHeight(35);
    setMaximumHeight(35);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    QLabel* title = new QLabel;
    title->setText("Test");
    QPushButton* button = new QPushButton;
    button->setText("X");
    button->setFixedSize(25, 25);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(title);
    layout->addStretch(1);
    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(button);
    layout->addLayout(vLayout);
    setLayout(layout);
}
