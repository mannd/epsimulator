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

#include "logwindow.h"

#include "actions.h"

#include <QAction>
#include <QComboBox>
#include <QToolBar>

//
LogWindow::LogWindow(int number, QWidget *parent)
 : DisplayWindow(tr("Log"), number, parent) {

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(name());
    //setMinimumSize(QSize(200,100));
    createActions();
    createToolBars();
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    //QSizePolicy policy(QSizePolicy::MinimumExpanding,
    //    QSizePolicy::Minimum);
    //policy.setHorizontalStretch(1);
    //centralWidget->setSizePolicy(policy);
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    centralWidget->setPalette(palette);

}


LogWindow::~LogWindow() {}

void LogWindow::createActions() {
    using EpGui::createAction;

    updateLogWindowAct_ = createAction(this, tr("Update Log Window On/Off"),
        tr("Toggle log window updating"), 0, 0, "hi32-updatewindowtoggle.png");
    firstLogEntryAct_ = createAction(this, tr("First Log Entry"),
        tr("Go to first log entry"), 0, 0, "hi32-firstlogentry.png");
    lastLogEntryAct_ = createAction(this, tr("Last Log Entry"),
        tr("Go to last log entry"), 0, 0, "hi32-lastlogentry.png");
    undoAct_ = createAction(this, tr("Undo"), tr("Undo the last action"),
        0, 0, "hi32-undo.png");
    redoAct_ = createAction(this, tr("Redo"), tr("Redo the last action"),
        0, 0, "hi32-redo.png");
    createEventAct_ = createAction(this, tr("Create Event"),
        tr("Insert an event into the log window"), 0, 0,
        "hi32-createlogevent.png");
    printAct_ = createAction(this, tr("Print"), tr("Print the log"),
        0, 0, "hi32-printreview.png");

 

}

void LogWindow::createToolBars() {
    QToolBar* toolBar = new QToolBar(tr("Log Window ToolBar"));
    toolBar->setObjectName("LogWindowToolBar");
    toolBar->setAutoFillBackground(true);
    logFilterComboBox_ = new QComboBox(this);
    logFilterComboBox_->addItem(tr("All                                  "));
    toolBar->addWidget(logFilterComboBox_);
    toolBar->addSeparator();
    toolBar->addAction(updateLogWindowAct_);
    toolBar->addSeparator();
    toolBar->addAction(firstLogEntryAct_);
    toolBar->addAction(lastLogEntryAct_);
    toolBar->addSeparator();
    toolBar->addAction(undoAct_);
    toolBar->addAction(redoAct_);
    toolBar->addSeparator();
    toolBar->addAction(createEventAct_);
    toolBar->addSeparator();
    toolBar->addAction(printAct_);

    addToolBar(toolBar); 
}



