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
#include "buttonframe.h"

#include "../epsim/options.h"

#include <qlabel.h>
#include <qlayout.h>
#include <QPushButton>
#include <QToolButton>
#include <qsizepolicy.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3GridLayout>
#include <Q3Frame>


ButtonFrame::ButtonFrame(QWidget* parent)
 : Q3Frame(parent), parent_(parent) {
    setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, 
                              (QSizePolicy::SizeType)5, 0, 0,
                              sizePolicy().hasHeightForWidth()));
    setFrameShape(Q3Frame::StyledPanel);
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::darkBlue);
    setPalette(palette);
    // old Qt3 style
    //setPaletteBackgroundColor("darkBlue");
    setMaximumWidth(200);
    buttonFrameLayout_ = new Q3GridLayout(this, 1, 1, 11, 6, "");
}

void ButtonFrame::addButton(const QString& name, const QString& pixmapName, 
                            const char* slotName, bool lastButton) {
    QPixmap pixmap(":/images/" + pixmapName);
    QLabel* label = new QLabel(tr(name), this);
    if (Options::instance()->newStyleBlueBar()) {   // set up flat buttons
        QToolButton* button = new QToolButton(this);
        button->setUsesBigPixmap(true);
        button->setPaletteBackgroundColor("blue");
        button->setAutoRaise(true);
        pixmap.fill();  // sets to white by default
        setupButton(button, pixmap, label, slotName, lastButton);
    }
    else {  // using original style raised pushbuttons
        QPushButton* button = new QPushButton(this);
        setupButton(button, pixmap, label, slotName, lastButton);
    }
}

/**
 * Sets up each square button along the side of the Navigator window.
 * @param button The button to be set up.
 * @param pixmap The pixmap on the button.
 * @param label Label under each button
 * @param slotName The slot associated with the button.
 * @param lastButton The last button is handled differently. 
 */
void ButtonFrame::setupButton(QAbstractButton* button, const QPixmap& pixmap,
                              QLabel* label, const char* slotName, 
                              bool lastButton) {
    button->setFixedSize(buttonWidth, buttonHeight);
    button->setIcon(QIcon(pixmap));
    static int row = 0;   // allows adding widgets in correct row
    // last parameter centers the buttons and labels horizontally
    if (row == 0) {
        // insert blank row at top -- looks better with this!
        QLabel* topLabel = new QLabel("", this);  
        topLabel->setAlignment(int(Qt::AlignCenter));
        buttonFrameLayout_->addWidget(topLabel, row++, 0);
    }
    buttonFrameLayout_->addWidget(button, row++, 0, Qt::AlignHCenter);
    // Notice that a SLOT is passed as a function parameter as a const char*.
    if (slotName)
        connect(button, SIGNAL(clicked()), parent_->parent(), slotName); 
    label->setPaletteForegroundColor("white");
    label->setAlignment(int(Qt::AlignCenter));
    buttonFrameLayout_->addWidget(label, row++, 0, Qt::AlignHCenter);
    // insert line between button/label groups
    QLabel* spaceLabel = new QLabel("", this);
    spaceLabel->setAlignment(int(Qt::AlignCenter));
    buttonFrameLayout_->addWidget(spaceLabel, row++, 0);
    if (lastButton) {
        QSpacerItem* spacer = new QSpacerItem( 20, 40, 
            QSizePolicy::Minimum, QSizePolicy::Expanding );
        buttonFrameLayout_->addItem( spacer, row, 0 );
    }
}


ButtonFrame::~ButtonFrame() {
}


