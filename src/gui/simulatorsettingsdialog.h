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

#ifndef SIMULATORSETTINGSDIALOG_H
#define SIMULATORSETTINGSDIALOG_H

#include "ui_simulatorsettingsdialog.h"

#include <QCheckBox>
#include <QDialog>

class Options;

class SimulatorSettingsDialog : public QDialog, 
                                private Ui::SimulatorSettingsDialog {
    Q_OBJECT

public:
    SimulatorSettingsDialog(Options* options, QWidget* parent = 0);
    ~SimulatorSettingsDialog();

    void setOptions();
    void removeNavigatorTab();

    
public slots:
    virtual void enableDriveEmulation();

private:
    bool emulateOpticalDrive() const {
        return emulateOpticalDriveCheckBox->isChecked();}

    void setEmulateOpticalDrive(bool);
    void setDualSidedDrive(bool);
    void setEmulatedOpticalDriveCapacity(int);

    bool dualSidedDrive() const;
    int emulatedOpticalDriveCapacity() const;

    Options* options_;

};

#endif
