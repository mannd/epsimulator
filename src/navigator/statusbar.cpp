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
#include "statusbar.h"

#include <QLabel>
#include <QString>

StatusBar::StatusBar(const QString& catalogPath, QWidget *parent) 
    : QStatusBar(parent) {
    messageLabel_ = new QLabel(tr("For Help, press F1"), this);
    userLabel_ = new QLabel(this);
    sourceLabel_ = new QLabel(this);
    updateSourceLabel(catalogPath);
    filterLabel_ = new QLabel(this);
    updateFilterLabel(false);   // filter always off at start of program

    addWidget(messageLabel_, 1);
    addPermanentWidget(userLabel_);
    addPermanentWidget(sourceLabel_);
    addPermanentWidget(filterLabel_);
}

/// Updates status bar to show source of current catalog.
void StatusBar::updateSourceLabel(const QString& label) {
    sourceLabel_->setText(tr(" Source: %1 ")
        .arg(label));
    update();
}

void StatusBar::updateFilterLabel(bool filtered) {
    filterLabel_->setText(filtered ? tr(" Filtered ") : tr(" Unfiltered "));
    update();
}

void StatusBar::updateUserLabel(const QString& userName) {
    userLabel_->setText(tr(" User: %1 ").arg(userName));
    update();
}

StatusBar::~StatusBar() {
}
