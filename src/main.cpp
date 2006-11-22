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

// Note that below is Froglogics GetOpt class, but header file is
// renamed from getopt.h to getopts.h because of conflict with 
// unix getopt.h.
#include "getopts.h"
#include "options.h"
#include "navigator.h"

#include <qapplication.h>
#include <qstring.h>

#include <iostream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    GetOpt opts;
    QString path;
    opts.addOption('p', "path", &path);
    bool help;
    opts.addSwitch("help", &help);
    if (!opts.parse()) {
        std::cerr << "Usage: " << opts.appName().ascii()
        << " [--path=filename]" << std::endl;
        return 1;
    }
    if (help) {
        std::cerr << "Help message here..." << std::endl; 
        return 1;
    }
    Options* options = Options::instance();
    if (! path.isEmpty())
        options->setStudyPath(path);
    Navigator *mainWin = new Navigator();
    app.setMainWidget(mainWin);
    mainWin->showMaximized();
    return app.exec();
}
