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


#include "teststimulator.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSound>
#include <QTimer>

namespace EpRecorder {

TestStimulator::TestStimulator(QWidget* parent, Qt::WFlags fl)
: Stimulator( parent, fl ), Ui::TestStimulator(), activated_(false) {
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(stimulus(ChannelSet)), this, SLOT(blink()));
    connect(onOffPushButton, SIGNAL(clicked(bool)), this,
        SLOT(toggle(bool)));
    connect(s1S1SpinBox, SIGNAL(editingFinished()), this, SLOT(changeS1S1()));
    stimLight->setChecked(false);
    changeS1S1();
    if (!QSound::isAvailable())
        QMessageBox::information(this, tr("No Sound"),
            tr("Sound is not available for this computer."));
}

void TestStimulator::toggle(bool enable) {
    if (enable)
        activate();
    else {
        inactivate();
        stimLight->setChecked(false);
    }
    activated_ = enable;
}

void TestStimulator::blink() {
    QSound::play("/opt/kde3/share/sounds/KDE_Beep_ShortBeep.wav");
    stimLight->setChecked(true);
    QTimer::singleShot(100, stimLight, SLOT(toggle()));
}

void TestStimulator::changeS1S1() {
    inactivate();
    stimLight->setChecked(false);
    setS1S1(s1S1SpinBox->value());
    if (activated_)
        activate();
}
    

TestStimulator::~TestStimulator() {
}

/*$SPECIALIZATION$*/

}


