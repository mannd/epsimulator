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

#include <QFrame>

class QGridLayout;
class QLabel;
class QAbstractButton;
class QString;


class AbstractButtonFrame : public QFrame {
public:
    virtual void addButton(const QString& name, const QString& pixmapName, 
                   const char* slotName, bool lastButton = false) = 0; 
protected:
    AbstractButtonFrame(QWidget* parent = 0);
    ~AbstractButtonFrame() {}

    void setupButton(QAbstractButton* button, const QPixmap& pixmap,
                     QLabel* label, const char* slotName, 
                     bool lastButton);
private:
    QGridLayout* buttonFrameLayout_;
    static const int buttonHeight = 60;
    static const int buttonWidth = 70;   // size of square buttons in blue panel
};

class OldStyleButtonFrame : public AbstractButtonFrame {
public:
    OldStyleButtonFrame(QWidget* parent = 0);
    ~OldStyleButtonFrame() {}
    virtual void addButton(const QString& name, const QString& pixmapName, 
                   const char* slotName, bool lastButton = false);
}; 

class NewStyleButtonFrame : public AbstractButtonFrame {
public:
    NewStyleButtonFrame(QWidget* parent = 0);
    ~NewStyleButtonFrame() {}
    virtual void addButton(const QString& name, const QString& pixmapName, 
                   const char* slotName, bool lastButton = false);
};

#endif
