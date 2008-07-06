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

#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QFrame>
#include <QMainWindow>
#include <QScrollArea>
#include <QString>

class QAction;
class QEvent;
class QScrollArea;
class QSettings;

namespace EpRecorder {

class ChannelBar;
class SignalArea;
class SignalWidget;

class DisplayWindow : public QMainWindow {
    Q_OBJECT

public:
    DisplayWindow(const QString& name, int number = 0, 
        QWidget* parent = 0, Qt::WindowFlags fl = 0); 
    ~DisplayWindow() {}

    virtual void writeSettings(QSettings&) = 0;
    virtual void readSettings(QSettings&) = 0;
    virtual QString key() = 0;     // this is the key used to read/write settings

    QString name() const {return name_;}
    int number() const {return number_;}

private:
    const QString name_;
    const int number_;
};



/**
 * Abstract base class for RealTimeWindow and ReviewWindow.  Implements common 
 * funcionality, such as the ChannelBar, but does not implement toolbars and 
 * updating of signal displays.
 *
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class SignalDisplayWindow : public DisplayWindow  {
    Q_OBJECT

public:
    SignalDisplayWindow(const QString& name, int number = 0, 
        QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~SignalDisplayWindow();

    void setCurrentPage(int page);

    int currentPage() const {return currentPage_;}

    void setChannelBar(ChannelBar* cb) {channelBar_ = cb;}

    ChannelBar* channelBar() const {return channelBar_;}
    SignalArea* signalArea() const {return signalArea_;}

    virtual void createCentralWidget();
    virtual void createActions();
    virtual void createToolBars() = 0;


public slots:
    void previousPage();
    void nextPage();
    void updateWindowTitle();

signals:
    void pageChanged(int newPage);

private:
    ChannelBar* channelBar_;
    SignalArea* signalArea_;
    SignalWidget* signalWidget_;
    //QScrollArea* scrollArea_;

    enum {minPage = 1, maxPage = 8};
    int currentPage_;

    QAction* previousPageAction_;
    QAction* nextPageAction_;

};

/*
Here is the logic for below.  For each display window that shows EGMs, 
there is a blue channel bar containing a set of channel labels to the 
right, and a black signal display area to the left.  A ChannelPage 
and a SignalPage are combined in a single QWidget to form a SignalAreaPage.
8 SignalAreaPages are added to a QStackedWidget to form a 
SignalStackedWidget, which then is added to a QScrollBarArea to 
form the final SignalWidget.  The SignalWidget by default has
a vertical scrollbar, implemented to change pages.  
The RealTimeWindow has no system menu (can't be closed by default) 
and only the vertical scrollbar.  ReviewWindows
have a horizontal scrollbar as well, implemented to scroll the SignalPages 
only.

The SignalWidget is the central widget for the SignalDisplayWindows.
*/

/**
 * A ChannelPage is one page of channel labels, a page of the ChannelBar
 * to the right of the SignalArea.
 */
class ChannelPage : public QFrame {
    Q_OBJECT

public:
    ChannelPage(QWidget* parent = 0);
    ~ChannelPage() {};

};

/**
 * A SignalPage is one page of the black (by default) SignalArea, where
 * the signals are drawn.
 */
class SignalPage: public QWidget {
    Q_OBJECT

public:
    SignalPage(QWidget* parent = 0);
    ~SignalPage() {}

};

/**
 * This is the channel label area.    This is an ABC, mostly so that
 * the right click menu can be overriden, as it is different on 
 * the real-time vs the review window.
 */
class ChannelBar : public QFrame {
    Q_OBJECT

public:
    ChannelBar(QWidget* parent = 0);
    ~ChannelBar();

    void setUp();

public slots:
    void changePage(int newPage);

};


/**
 * This is the black signal area to the right of the channel bar.
 */
class SignalArea : public QWidget {
    Q_OBJECT

public:
    SignalArea(QWidget* parent = 0);

    ~SignalArea();

public slots:
    void changePage(int newPage);

};

/**
 * This is the central widget of the SignalDisplayWindow, contains
 * the ChannelBar with the SignalArea and scrollbar(s).
 */
class SignalWidget : public QScrollArea {
    Q_OBJECT

public:
    SignalWidget(QWidget* parent = 0);
    ~SignalWidget() {}

private:
    ChannelBar* channelBar_;
    SignalArea* signalArea_;
};

}

#endif
