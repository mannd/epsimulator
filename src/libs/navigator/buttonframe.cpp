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

#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QToolButton>
#include <QSizePolicy>
#include <QPixmap>
#include <QGridLayout>

using EpNavigator::AbstractButtonFrame;
using EpNavigator::OldStyleButtonFrame;
using EpNavigator::NewStyleButtonFrame;

const int AbstractButtonFrame::buttonHeight;
const int AbstractButtonFrame::buttonWidth;

/**
 * Constructs blue bar on left side of Navigator window
 * @param parent the QSplitter that owns this widget 
 */
AbstractButtonFrame::AbstractButtonFrame(QWidget* parent) 
    : QFrame(parent) {
    QSizePolicy policy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    policy.setVerticalStretch(0);
    policy.setHeightForWidth(false);
    setSizePolicy(policy);
    setFrameShape(QFrame::StyledPanel);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::darkBlue);
    palette.setColor(QPalette::Button, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::white);
    setPalette(palette);
    // necessary to actually apply the Window color to the background
    setAutoFillBackground(true);
    setMaximumWidth(200);
    buttonFrameLayout_ = new QGridLayout(this);
    buttonFrameLayout_->setVerticalSpacing(6);  // ? this is right



    //buttonFrameLayout_ = new QGridLayout(this, 1, 1, 11, 6, "");
}

/**
 * Sets up each square button along the side of the Navigator window.
 * @param button The button to be set up.
 * @param pixmap The pixmap on the button.
 * @param label Label under each button
 * @param slotName The slot associated with the button.
 * @param lastButton The last button is handled differently. 
 */
void AbstractButtonFrame::setupButton(QAbstractButton* button, 
                                      const QPixmap& pixmap, 
                                      QLabel* label, 
                                      const char* slotName, 
                                      bool lastButton) {
    button->setFixedSize(buttonWidth, buttonHeight);
    button->setIcon(QIcon(pixmap));
    button->setIconSize(QSize(buttonWidth - 10, buttonHeight - 10));
    static int row = 0;   // allows adding widgets in correct row
    // last parameter centers the buttons and labels horizontally
    if (row == 0) {
        // insert blank row at top -- looks better with this!
        QLabel* topLabel = new QLabel("", this);  
        topLabel->setAlignment(Qt::AlignCenter);
        buttonFrameLayout_->addWidget(topLabel, row++, 0);
    }
    buttonFrameLayout_->addWidget(button, row++, 0, Qt::AlignHCenter);
    // Notice that a SLOT is passed as a function parameter as a const char*.
    if (slotName)
        connect(button, SIGNAL(clicked()), parent()->parent(), slotName); 
    label->setAlignment(Qt::AlignCenter);
    buttonFrameLayout_->addWidget(label, row++, 0, Qt::AlignHCenter);
    // insert line between button/label groups
    QLabel* spaceLabel = new QLabel("", this);
    spaceLabel->setAlignment(Qt::AlignCenter);
    buttonFrameLayout_->addWidget(spaceLabel, row++, 0);
    if (lastButton) {
        QSpacerItem* spacer = new QSpacerItem( 20, 40, 
            QSizePolicy::Minimum, QSizePolicy::Expanding );
        buttonFrameLayout_->addItem( spacer, row, 0 );
        // set row back to 0 in case we recreate the blue bar
        row = 0;
    }
}


// OldStyleButtonFrame

OldStyleButtonFrame::OldStyleButtonFrame(QWidget* parent) 
    : AbstractButtonFrame(parent) {}

/**
 * Adds a button along with the corresponding pixmap and slot
 * @param name name of the label under the button
 * @param pixmapName name of pixmap image WITHOUT trailing ".png"
 * @param slotName name of slot attached to button
 * @param lastButton the last button is handled differently
 */
void OldStyleButtonFrame::addButton(const QString& name, 
                                    const QString& pixmapName, 
                                    const char* slotName, 
                                    bool lastButton) {
    QPixmap pixmap(":/images/" + pixmapName + ".png");
    QLabel* label = new QLabel(name, this);
    QPushButton* button = new QPushButton(this);
    setupButton(button, pixmap, label, slotName, lastButton);
}

// NewStyleButtonFrame

NewStyleButtonFrame::NewStyleButtonFrame(QWidget* parent)
    : AbstractButtonFrame(parent) {
    // we give the buttons a darkBlue color so they don't get whited out.
    // Unfortunately doesn't seem to work on WinXP, only on Linux.
    QPalette framePalette = palette();
    framePalette.setColor(QPalette::Button, Qt::darkBlue);
    /// TODO autoRaise frame too dark on Sluggo, white on SuperSluggo.  ? Why
    setPalette(framePalette);
}

/**
 * Adds a button along with the corresponding pixmap and slot
 * @param name name of the label under the button
 * @param pixmapName name of pixmap image WITHOUT trailing "white.png"
 * @param slotName name of slot attached to button
 * @param lastButton the last button is handled differently
 */
void NewStyleButtonFrame::addButton(const QString& name, 
                                    const QString& pixmapName, 
                                    const char* slotName, 
                                    bool lastButton) {

    QPixmap pixmap(":/images/" + pixmapName + "white.png");
    QLabel* label = new QLabel(name, this);
    QToolButton* button = new QToolButton(this);
    button->setAutoRaise(true);
    setupButton(button, pixmap, label, slotName, lastButton);
}
