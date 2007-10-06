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

#include "epfuns.h"

#include "versioninfo.h"

#include <q3filedialog.h>
#include <qlineedit.h>
#include <qwidget.h>

namespace EpFuns {

bool versionOk(int major, int minor) {
    if (major == BadTestVersion || minor == BadTestVersion)
        return false;
    return (major >= GoodMajorVersion 
        && minor >= GoodMinorVersion);
}

void saveMagicNumber(unsigned int magicNumber, QDataStream& out) {
    out << static_cast<Q_UINT32>(magicNumber);
    VersionInfo* v = VersionInfo::instance();
    out << static_cast<Q_UINT32>(v->versionMajor())
        << static_cast<Q_UINT32>(v->versionMinor());
}

void browseFilePaths(QWidget* parent, QLineEdit* lineEdit) {
    Q3FileDialog *fd = new Q3FileDialog(parent, 0, true);
    fd->setMode(Q3FileDialog::Directory);
    fd->setDir(lineEdit->text());
    if (fd->exec() == QDialog::Accepted) {
        lineEdit->setText(fd->selectedFile());
    }
}


}
