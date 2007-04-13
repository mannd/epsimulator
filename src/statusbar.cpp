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

StatusBar::StatusBar(const QString& catalogPath, QWidget *parent, 
    bool oldStyle, const char *name) : QStatusBar(parent, name) {
    messageLabel_ = new QLabel(tr("For Help, press F1"), this);

    /// Apparently getenv works in Windows too.
    /// TODO If logged in as administrator, show this on status bar
    /// Difference from Prucka: Only shows "cluser" for general user or "administrator"
//     QString user = oldStyle ? tr(" User: eplabuser ") :
//         tr(" User: %1 ").arg(std::getenv("USER"));
    userLabel_ = new QLabel(this);
//    userLabel_->setAlignment(AlignHCenter);
    updateUserLabel(false, oldStyle);

    sourceLabel_ = new QLabel(this);
//    sourceLabel_->setAlignment(AlignHCenter);
    updateSourceLabel(catalogPath);

    filterLabel_ = new QLabel(this);
//    filterLabel_->setAlignment(AlignHCenter);
    updateFilterLabel(false);   // filter always off at start of program

    // 1 or 0 determine how much space the label takes up.  0 minimizes space.
    // true or false (default) determine if label is permanently displayed, or covered by
    // messages.
    addWidget(messageLabel_, 1);
    addWidget(userLabel_, 0, true);
    addWidget(sourceLabel_, 0, true);
    addWidget(filterLabel_, 0, true);
}

/// Updates status bar to show source of current catalog.
void StatusBar::updateSourceLabel(const QString& label) {
    sourceLabel_->setText(tr(" Source: %1 ")
        .arg(label));
//    sourceLabel_->setMinimumSize(sourceLabel_->sizeHint());
    update();
}

void StatusBar::updateFilterLabel(bool filtered) {
    filterLabel_->setText(filtered ? tr(" Filtered ") : tr(" Unfiltered "));
//    filterLabel_->setMinimumSize(filterLabel_->sizeHint());
    update();
}

void StatusBar::updateUserLabel(bool userIsAdministrator, bool oldStyle) {
    QString user = oldStyle ? tr("EPSIMUSER") : std::getenv("USER");
    user = userIsAdministrator ? tr("ADMINISTRATOR") : user;
    userLabel_->setText(tr(" User: %1 ").arg(user));
//    userLabel_->setMinimumSize(userLabel_->sizeHint());
    update();

}

StatusBar::~StatusBar()
{
}


