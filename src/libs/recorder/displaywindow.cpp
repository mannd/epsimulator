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
#include "options.h"
#include "signalwidget.h"
#include "study.h"
#include "studyconfigurationdialog.h"

#include <QKeySequence>
#include <QSettings>

#include <QtDebug>

using namespace EpRecorder;
using EpCore::Options;
using EpStudy::Study;

DisplayWindow::DisplayWindow(const QString& name,
                             Study* study, 
                             const Options* const options,
			     int number, 
                             QWidget* parent) 
    : QMainWindow(parent), name_(name), study_(study),
      options_(options), number_(number) {
    // don't keep unused windows in memory
    setAttribute(Qt::WA_DeleteOnClose);
    // probably reasonable initial size for these windows
    setMinimumSize(QSize(400,400));
}

const int SignalDisplayWindow::minPage;
const int SignalDisplayWindow::maxPage;

SignalDisplayWindow::SignalDisplayWindow(const QString& name, 
                                         Study* study,
					 const Options* const options,
                                         int number,
                                         QWidget *parent) 
    : DisplayWindow(name, study, options, number, parent), 
      currentPage_(minPage) {
    createCentralWidget();
    createActions();
    connect(this, SIGNAL(pageChanged(int)), this,
        SLOT(updateWindowTitle()));
    qDebug() << "parent = " << parent;
//    connect(this, SIGNAL(studyConfigurationChanged()),
//            parent->parent(), SLOT(updateStudyConfiguration()));
}

void SignalDisplayWindow::updateWindowTitle() {
    QString title = (number() == 0) ? 
        tr("%1: Page %2").arg(name()).arg(currentPage()) 
        : tr("%1 %2: Page %3").arg(name()).arg(number()).arg(currentPage());
    setWindowTitle(title);
}

void SignalDisplayWindow::studyConfiguration() {
    EpGui::StudyConfigurationDialog* dialog  = studyConfigurationDialog();
    if (dialog->exec()) {
        study()->saveStudyConfiguration();
        emit studyConfigurationChanged();
    }
    delete dialog;
}

void SignalDisplayWindow::setCurrentPage(int page) {
    currentPage_ = qBound(minPage, page, maxPage);
}

void SignalDisplayWindow::previousPage() {
    currentPage_ = qMax(--currentPage_, minPage);
    emit pageChanged(currentPage_);
}

void SignalDisplayWindow::nextPage() {
    currentPage_ = qMin(++currentPage_, maxPage); 
    emit pageChanged(currentPage_);
}

void SignalDisplayWindow::createCentralWidget() {
    signalWidget_ = new SignalWidget;
    setCentralWidget(signalWidget_);
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
