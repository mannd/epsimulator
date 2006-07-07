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


#ifndef _EPSIMULATOR_H_
#define _EPSIMULATOR_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kmainwindow.h>

/**
 * @short Application Main Window
 * @author David Mann <mannd@epstudiossoftware.com>
 * @version 0.1
 */
class Epsimulator : public KMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    Epsimulator();

    /**
     * Default Destructor
     */
    virtual ~Epsimulator();

protected:
    void closeEvent(QCloseEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void newPatient();
    void openPatient();
    void newStudy();
    void openStudy();
    // Patients and studies are saved automatically when created and modified
    // At least I think that is the Pruck behavior
    void about();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void loadPatient();
    void savePatient();
    void loadStudy();
    void saveStudy();
};

#endif // _EPSIMULATOR_H_
