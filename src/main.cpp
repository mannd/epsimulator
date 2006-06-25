/***************************************************************************
 *   Copyright (C) 2006 by David Mann   *
 *   mannd@epstudiossoftware.com   *
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


#include "epsimulator.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char version[] = "0.1";

static KCmdLineOptions options[] = { KCmdLineLastOption };

int main(int argc, char **argv)
{
    KAboutData about("epsimulator", 
                        I18N_NOOP("Epsimulator"), 
                        version,
                        I18N_NOOP("Simulates an EP recording system"),
		        KAboutData::License_GPL, "(C) 2006 David Mann",
                        0,  //any text 
                        "http://www.epstudiossoftware.com",
                        "mannd@epstudiossoftware.com");
    about.addAuthor( "David Mann", I18N_NOOP("Author"), "mannd@epstudiossoftware.com" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    Epsimulator *mainWin = 0;

    // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    /// @todo do something with the command line args here

    mainWin = new Epsimulator();
    app.setMainWidget( mainWin );
    mainWin->show();

    args->clear();

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

