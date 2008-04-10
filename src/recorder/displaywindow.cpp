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

#include "displaywindow.h"

#include "actions.h"

#include <QKeySequence>
#include <QHBoxLayout>
#include <QPalette>
#include <QScrollArea>
#include <QSettings>
#include <QSizePolicy>
#include <QSplitter>
#include <QStackedWidget>

SignalDisplayWindow::SignalDisplayWindow(const QString& name, int number,
    QWidget *parent, Qt::WindowFlags fl) 
    : DisplayWindow(name, number, parent, fl),
    currentPage_(minPage) {
    
    // don't keep unused windows in memory
    setAttribute(Qt::WA_DeleteOnClose);
    // probably reasonable initial size for these windows
    setMinimumSize(QSize(400,400));

    createCentralWidget();
    createActions();
    connect(this, SIGNAL(pageChanged(int)), this,
        SLOT(updateWindowTitle()));

}

void SignalDisplayWindow::updateWindowTitle() {
    QString title = (number() == 0) ? tr("%1: Page %2").arg(name()).arg(currentPage()) 
        : tr("%1 %2: Page %3").arg(name()).arg(number()).arg(currentPage());
    setWindowTitle(title);
}

void SignalDisplayWindow::setCurrentPage(int page) {
    if (page < minPage)
        currentPage_ = minPage;
    else if (page > maxPage)
        currentPage_ = maxPage;
    else
        currentPage_ = page;
}

void SignalDisplayWindow::previousPage() {
    if (--currentPage_ < minPage)
        currentPage_ = minPage;
    emit pageChanged(currentPage_);
}

void SignalDisplayWindow::nextPage() {
    if (++currentPage_ > maxPage)
        currentPage_ = maxPage;
    emit pageChanged(currentPage_);
}

void SignalDisplayWindow::createCentralWidget() {

    //QSplitter* splitter = new QSplitter(Qt::Horizontal);
    
    QWidget* centralWidget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;
 
    channelBar_ = new ChannelBar;
    signalArea_ = new SignalArea;
    layout->addWidget(channelBar_);
    layout->addWidget(signalArea_);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    connect(this, SIGNAL(pageChanged(int)), channelBar_,
        SLOT(changePage(int)));
    connect(this, SIGNAL(pageChanged(int)), signalArea_,
        SLOT(changePage(int)));
}

void SignalDisplayWindow::createActions() {
    using EpGui::createAction;
    previousPageAction_ = createAction(this, tr("Previous Page"),
        0, SLOT(previousPage()),
        QKeySequence(QKeySequence::MoveToPreviousPage));
    nextPageAction_ = createAction(this, tr("Next Page"),
        0, SLOT(nextPage()), QKeySequence(QKeySequence::MoveToNextPage));

    addAction(previousPageAction_);
    addAction(nextPageAction_);
}

SignalDisplayWindow::~SignalDisplayWindow() {}

/**
 * ChannelBar is a QStackedWidget of QFrames, signals and slots
 * are used to coordinate channel labels and channel signals
 * in the SignalArea.
 * @param parent 
 */
ChannelBar::ChannelBar(QWidget* parent) : QFrame(parent) {
    setUp();
}

void ChannelBar::setUp() {
    QSizePolicy policy(QSizePolicy::Preferred,
        QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    //policy.setHeightForWidth(false);
    setSizePolicy(policy);
    setFrameShape(QFrame::StyledPanel);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::darkBlue);
    palette.setColor(QPalette::Button, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::white);
    setPalette(palette);
    // necessary to actually apply the Window color to the background
    setAutoFillBackground(true);
    setMinimumWidth(150);
}

void ChannelBar::changePage(int) {}

ChannelBar::~ChannelBar() {}

/**
 * SignalArea is a QScrollArea, wrapping a QStackedWidget of
 * SignalPages.
 * @param parent 
 */
SignalArea::SignalArea(QWidget* parent) : QScrollArea(parent) {
    QWidget* w = new QWidget;
    
    QSizePolicy policy(QSizePolicy::MinimumExpanding,
        QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(1);
    w->setSizePolicy(policy);
    w->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);
    // most if not all SignalAreas have a vertical scroll
    // bar to change pages.  Real-time window does not
    // have a horizontal scroll bar, but review windows do.
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setAutoFillBackground(true);
    setWidget(w);
}

void SignalArea::changePage(int) {}

SignalArea::~SignalArea() {}
