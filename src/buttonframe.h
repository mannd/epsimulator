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
#ifndef BUTTONFRAME_H
#define BUTTONFRAME_H

#include <qframe.h>
//#include <qstring.h>

class QGridLayout;
class QLabel;
class QButton;
class QString;

/**
This is the vertical blue button bar on the left of the navigator window.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class ButtonFrame : public QFrame
{
public:
    ButtonFrame(QWidget* parent = 0);


/**
 * Create the "blue bar" to the side of the Navigator window.  Uses
 * setupButton to make each button.  The parent of the buttonFrame_ is
 * the horizontalSplitter_.  This is also the parent of the 
 * tableListView_.
 */

    void addButton(const QString& name, const QString& pixmapName, 
                   const char* slotName, bool lastButton = false);

    ~ButtonFrame();

private:
    static const int buttonHeight = 60;
    static const int buttonWidth = 70;   // size of square buttons in blue panel

    void setupButton(QButton* button, const QPixmap& pixmap,
                     QLabel* label, const char* slotName, 
                     bool lastButton);

    QWidget* parent_;
    QGridLayout* buttonFrameLayout_;

};

#endif
