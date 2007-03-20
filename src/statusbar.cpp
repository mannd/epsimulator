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

#include <qlabel.h>
#include <qstring.h>

StatusBar::StatusBar(const QString& catalogPath, QWidget *parent, const char *name)
 : QStatusBar(parent, name) {
    messageLabel_ = new QLabel(tr("For Help, press F1"), this);

    /// Apparently getenv works in Windows too.
    /// TODO If logged in as administrator, show this on status bar
    /// Difference from Prucka: Only shows "cluser" for general user or "administrator"
    userLabel_ = new QLabel(tr(" User: %1 ").arg(std::getenv("USER")), this);
    userLabel_->setAlignment(AlignHCenter);
    userLabel_->setMinimumSize(userLabel_->sizeHint());

    sourceLabel_ = new QLabel(tr(" Source: %1 ")
        .arg(catalogPath), this);
    sourceLabel_->setAlignment(AlignHCenter);
    sourceLabel_->setMinimumSize(sourceLabel_->sizeHint());

    filterLabel_ = new QLabel(tr(" Unfiltered "), this);
    filterLabel_->setAlignment(AlignHCenter);
    filterLabel_->setMinimumSize(filterLabel_->sizeHint());

    addWidget(messageLabel_, 1);
    addWidget(userLabel_);
    addWidget(sourceLabel_);
    addWidget(filterLabel_);
}

/// Updates status bar to show source of current catalog.
void StatusBar::updateSourceLabel(const QString& label) {
        sourceLabel_->setText(tr(" Source: %1 ")
            .arg(label));
        sourceLabel_->setMinimumSize(sourceLabel_->sizeHint());
        update();
}

void StatusBar::updateFilterLabel(bool filtered) {
    filterLabel_->setText(filtered ? tr(" Filtered ") : tr(" Unfiltered "));
    update();
}

StatusBar::~StatusBar()
{
}


