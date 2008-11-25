/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#ifndef TESTSTIMULATOR_H
#define TESTSTIMULATOR_H

#include "stimulator.h"
#include "ui_teststimulator.h"

#include <QWidget>

namespace EpHardware { namespace EpStimulator {

class TestStimulator : public Stimulator, private Ui::TestStimulator
{
  Q_OBJECT

public:
  TestStimulator(QWidget* parent = 0, Qt::WFlags fl = Qt::Tool);
  ~TestStimulator();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private slots:
    void blink();
    void toggle(bool);
    void changeS1S1();

private:
    bool activated_;

};

}}

#endif
