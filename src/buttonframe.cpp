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

#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>


ButtonFrame::ButtonFrame(QWidget* parent)
 : QFrame(parent), parent_(parent) {
    setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, 
                              (QSizePolicy::SizeType)5, 0, 0,
                              sizePolicy().hasHeightForWidth()));
    setFrameShape(QFrame::StyledPanel);
    setPaletteBackgroundColor("blue");
    setMaximumWidth(200);
    buttonFrameLayout_ = new QGridLayout(this, 1, 1, 11, 6, "");
}

void ButtonFrame::addButton(const QString& name, const QString& pixmap, 
                            const char* slotName, bool lastButton) {
    QPushButton* button = new QPushButton(this);
    QLabel* label = new QLabel(tr(name), this);
    setupButton(button, pixmap, label, slotName, lastButton);
}

/**
 * Sets up each square button along the side of the Navigator window.
 * @param button The button to be set up.
 * @param pixmapName The pixmap on the button.
 * @param label Label under each button
 * @param slotName The slot associated with the button.
 * @param lastButton The last button is handled differently. 
 */
void ButtonFrame::setupButton(QPushButton* button, const QString& pixmapName,
                            QLabel* label, const char* slotName, 
                            bool lastButton) {
    button->setFixedSize(buttonWidth, buttonHeight);
    button->setPixmap(QPixmap::fromMimeSource(pixmapName));
    static int row = 0;   // allows adding widgets in correct row
    // last parameter centers the buttons and labels horizontally
    if (row == 0) {
        // insert blank row at top -- looks better with this!
        QLabel* topLabel = new QLabel("", this);  
        topLabel->setAlignment(int(QLabel::AlignCenter));
        buttonFrameLayout_->addWidget(topLabel, row++, 0);
    }
    buttonFrameLayout_->addWidget(button, row++, 0, Qt::AlignHCenter);
    // Notice that a SLOT is passed as a function parameter as a const char*.
    if (slotName)
        connect(button, SIGNAL(clicked()), parent_->parent(), slotName); 
    label->setPaletteForegroundColor("white");
    label->setAlignment(int(QLabel::AlignCenter));
    buttonFrameLayout_->addWidget(label, row++, 0, Qt::AlignHCenter);
   // insert line between button/label groups
    QLabel* spaceLabel = new QLabel("", this);
    spaceLabel->setAlignment(int(QLabel::AlignCenter));
    buttonFrameLayout_->addWidget(spaceLabel, row++, 0);
    if (lastButton) {
        QSpacerItem* spacer = new QSpacerItem( 20, 40, 
            QSizePolicy::Minimum, QSizePolicy::Expanding );
        buttonFrameLayout_->addItem( spacer, row, 0 );
    }
}


ButtonFrame::~ButtonFrame() {
}


