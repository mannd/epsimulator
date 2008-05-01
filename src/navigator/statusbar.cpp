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

#include <QKeySequence>
#include <QLabel>
#include <QString>

using EpNavigator::StatusBar;

StatusBar::StatusBar(const QString& catalogPath, QWidget *parent) 
    : QStatusBar(parent) {
    messageLabel_ = new QLabel(tr("For Help, press %1")
        .arg(QKeySequence(QKeySequence::HelpContents).toString()), this);

    userLabel_ = new QLabel(this);
    userLabel_->setAlignment(Qt::AlignHCenter);

    sourceLabel_ = new QLabel(this);
    sourceLabel_->setAlignment(Qt::AlignHCenter);
    updateSourceLabel(catalogPath);

    filterLabel_ = new QLabel(this);
    // calculate the proper size and set initial unfiltered indication
    filterLabel_->setText(tr(" Unfiltered "));
    filterLabel_->setAlignment(Qt::AlignHCenter);
    filterLabel_->setMinimumSize(filterLabel_->sizeHint());   

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
