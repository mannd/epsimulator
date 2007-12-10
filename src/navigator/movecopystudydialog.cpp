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


#include "movecopystudydialog.h"

#include "actions.h"
#include "catalog.h"

MoveCopyStudyDialog::MoveCopyStudyDialog(QWidget* parent, 
                                         const QString& sourcePath)
    : QDialog(parent), Ui::MoveCopyStudyDialog()
{
    setupUi(this);
    sourcePathLineEdit->setText(sourcePath);
    finishButton->setEnabled(false);
    fillStudiesListView();
    connect(sourcePathBrowseButton, SIGNAL(clicked()), this,
        SLOT(sourcePathBrowse()));
    connect(destinationPathBrowseButton, SIGNAL(clicked()), this,
        SLOT(destinationPathBrowse()));
    connect(sourcePathLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(activateFinishButton()));
    connect(destinationPathLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(activateFinishButton()));
    connect(studiesListWidget, SIGNAL(itemSelectionChanged()), 
        this, SLOT(activateFinishButton()));
    connect(refreshButton, SIGNAL(clicked()), 
        this, SLOT(fillStudiesListView()));
    connect(selectAllButton, SIGNAL(clicked()),
        this, SLOT(selectAll()));
}

void MoveCopyStudyDialog::sourcePathBrowse() {
    EpGui::browseFilePaths(this, sourcePathLineEdit);
}

void MoveCopyStudyDialog::destinationPathBrowse() {
    EpGui::browseFilePaths(this, destinationPathLineEdit);
}

void MoveCopyStudyDialog::setUpLabels(const QString& label) {
    setWindowTitle(tr("%1 Study Wizard").arg(label));
    finishButton->setText(label);
    destinationLabel->setText(destinationLabel->text().arg(label));
}

/**
 * Checks to see if there is an entry in the source and finish line edits,
 * and makes sure that there is at least one selected study.  If so,
 * activates button.
 */
void MoveCopyStudyDialog::activateFinishButton() {
    bool result = !sourcePathLineEdit->text().isEmpty() &&
                  !studiesListWidget->selectedItems().isEmpty() &&
                  !destinationPathLineEdit->text().isEmpty();
    finishButton->setEnabled(result);
}

void MoveCopyStudyDialog::fillStudiesListView() {
    studiesListWidget->clear();
    Catalog catalog(sourcePathLineEdit->text());
    for (Catalog::CatalogMap::const_iterator it = catalog.begin(); 
        it != catalog.end(); ++it)
        studiesListWidget->addItem(QString(it.data().study.name().fullName(true) + 
            " [" + it.data().study.dateTime().toString() + "]"));
}

void MoveCopyStudyDialog::selectAll() {
    studiesListWidget->selectAll();
}

MoveCopyStudyDialog::~MoveCopyStudyDialog() {
}

CopyStudyDialog::CopyStudyDialog(QWidget* parent, const QString& sourcePath) 
    : MoveCopyStudyDialog(parent, sourcePath) {
    setUpLabels(tr("Copy"));
}

CopyStudyDialog::~CopyStudyDialog() {
}

MoveStudyDialog::MoveStudyDialog(QWidget* parent, const QString& sourcePath) 
    : MoveCopyStudyDialog(parent, sourcePath) {
    setUpLabels(tr("Move"));
}

MoveStudyDialog::~MoveStudyDialog() {
}

