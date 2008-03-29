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

//#include "recorderdefs.h"

#include <QFrame>
#include <QMainWindow>
#include <QScrollArea>
#include <QString>

class Settings;

class DisplayWindow : public QMainWindow {
    Q_OBJECT
public:
    DisplayWindow(const QString& name, int number = 0, 
        QWidget* parent = 0, Qt::WindowFlags fl = 0) 
        : QMainWindow(parent, fl), name_(name), number_(number) {}

    virtual void writeSettings(Settings&) = 0;
    virtual void readSettings(Settings&) = 0;
    virtual QString key() = 0;     // this is the key used to read/write settings

    //void setName(QString name) {name_ = name;}

    QString name() const {return name_;}
    int number() const {return number_;}

    ~DisplayWindow() {}

private:
    const QString name_;
    const int number_;
};

class ChannelBar;
class QAction;
class QScrollArea;
class SignalArea;

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

    void setCurrentPage(int page);

    int currentPage() const {return currentPage_;}

    void setChannelBar(ChannelBar* cb) {channelBar_ = cb;}

    ChannelBar* channelBar() const {return channelBar_;}
    SignalArea* signalArea() const {return signalArea_;}

    virtual void createCentralWidget();
    virtual void createActions();
    virtual void createToolBars() = 0;

    ~SignalDisplayWindow();

public slots:
    void previousPage();
    void nextPage();
    void updateWindowTitle();

signals:
    void pageChanged(int newPage);

private:
    ChannelBar* channelBar_;
    SignalArea* signalArea_;
    //QScrollArea* scrollArea_;

    enum {minPage = 1, maxPage = 8};
    int currentPage_;

    QAction* previousPageAction_;
    QAction* nextPageAction_;
    
};

/**
 * This is the channel label area.  This is an ABC, mostly so that
 * the right click menu can be overriden, as it is different on 
 * the real-time vs the review window.
 */
class ChannelBar : public QFrame {
    Q_OBJECT
public:
    ChannelBar(QWidget* parent = 0);

    void setUp();

    ~ChannelBar();

public slots:
    void changePage(int newPage);
};

/**
 * This is the black signal area to the right of the channel bar.
 */
class SignalArea : public QScrollArea {
    Q_OBJECT
public:
    SignalArea(QWidget* parent = 0);

    ~SignalArea();

public slots:
    void changePage(int newPage);
};

#endif
