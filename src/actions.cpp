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
#include "actions.h"

#include "epsimdefs.h"

#include <qmessagebox.h>
#include <qobject.h>

Actions::Actions() {
}

void Actions::about(QWidget* parent) {
    QMessageBox::about(parent, QObject::tr("About EP Simulator"),
		       QObject::tr("<h2>EP Simulator 0.1</h2>"
		          "<p>Copyright &copy; 2006 EP Studios, Inc."
			  "<p>EP Simulator simulates an EP recording "
			  "system."
                          "<p><a href=http://www.epstudiossoftware.com> http://www.epstudiossoftware.com</a>"));
///TODO make this an actual hyperlink that you can click on and go to
}

void Actions::help(QWidget* parent) {
    QMessageBox::information(parent, QObject::tr(
        "%1 Help").arg(PROGRAM_NAME),
        QObject::tr(
        "Help is available from www.epstudiossoftware.com"),
        QMessageBox::Ok);
}


Actions::~Actions() {
}


