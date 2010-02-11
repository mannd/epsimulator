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
#include "fileutilities.h"
#include "opticaldisk.h"

#include <QList>
#include <QListWidgetItem>

using EpHardware::EpOpticalDisk::OpticalDisk;
using EpNavigator::MoveCopyStudyDialog;
using EpNavigator::CopyStudyDialog;
using EpNavigator::MoveStudyDialog;
using EpNavigator::StudyListWidgetItem;

StudyListWidgetItem::StudyListWidgetItem(const QString& studyName, 
    const QString& studyPath, QListWidget* parent) 
    : QListWidgetItem(studyName, parent), studyPath_(studyPath) {
}

MoveCopyStudyDialog::MoveCopyStudyDialog(QWidget* parent, 
                                         OpticalDisk* opticalDisk) : 
                                         QDialog(parent), 
                                         Ui::MoveCopyStudyDialog(),
                                         opticalDisk_(opticalDisk) {
    setupUi(this);
    sourcePathLineEdit->setText(addOpticalToPath(opticalDisk->path()));
    destinationPathLineEdit->setText(addOpticalToPath(opticalDisk->path()));
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

QString MoveCopyStudyDialog::sourcePath() {
    return removeOpticalFromPath(sourcePathLineEdit->text());
}

QString MoveCopyStudyDialog::destinationPath() {
    return removeOpticalFromPath(destinationPathLineEdit->text());
}

QList<QString> MoveCopyStudyDialog::selectedItems() {
    QList<QString> studyPaths;
    QList<QListWidgetItem*> items = studiesListWidget->selectedItems();
    for (int i = 0; i < items.size(); ++i) {
        if (StudyListWidgetItem* s = dynamic_cast<StudyListWidgetItem*>(items[i]))
            studyPaths.push_back(s->studyPath());
    }
    return studyPaths;
}


void MoveCopyStudyDialog::sourcePathBrowse() {
    sourcePathLineEdit->setUpdatesEnabled(false);
    sourcePathLineEdit->setText(
        removeOpticalFromPath(sourcePathLineEdit->text()));
    EpGui::browseFilePaths(this, sourcePathLineEdit);
    sourcePathLineEdit->setText(
        addOpticalToPath(sourcePathLineEdit->text()));
    sourcePathLineEdit->setUpdatesEnabled(true);
}

void MoveCopyStudyDialog::destinationPathBrowse() {
    destinationPathLineEdit->setUpdatesEnabled(false);
    destinationPathLineEdit->setText(
        removeOpticalFromPath(destinationPathLineEdit->text()));
    EpGui::browseFilePaths(this, destinationPathLineEdit);
    destinationPathLineEdit->setText(
        addOpticalToPath(destinationPathLineEdit->text()));
    destinationPathLineEdit->setUpdatesEnabled(true);
}

void MoveCopyStudyDialog::setUpLabels(const QString& label) {
    setWindowTitle(tr("Study %1 Wizard").arg(label));
    finishButton->setText(label);
    destinationLabel->setText(destinationLabel->text().arg(label));
}

QString MoveCopyStudyDialog::addOpticalToPath(const QString& path) {
    if (path == opticalDisk_->path())
        return EpCore::joinPaths(opticalDisk_->path(),tr("(Optical)"));
    else
        return path;
}

QString MoveCopyStudyDialog::removeOpticalFromPath(const QString& path) {
    if (path == addOpticalToPath(opticalDisk_->path()))
        return opticalDisk_->path();
    else
        return path;
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
    QString path = removeOpticalFromPath(sourcePathLineEdit->text());
    if (path == opticalDisk_->path())   // source is optical disk
        path = opticalDisk_->labelPath();
    Catalog catalog(path);
    for (Catalog::CatalogMap::const_iterator it = catalog.begin(); 
        it != catalog.end(); ++it) {
        StudyListWidgetItem* item = 
            new StudyListWidgetItem(QString((*it).study.name()
            .lastFirst() + 
            " [" + (*it).study.dateTime().toString() + "]"),
            (*it).study.path(), studiesListWidget);
        studiesListWidget->addItem(item);
    }
}

void MoveCopyStudyDialog::selectAll() {
    studiesListWidget->selectAll();
}

MoveCopyStudyDialog::~MoveCopyStudyDialog() {}

CopyStudyDialog::CopyStudyDialog(QWidget* parent, OpticalDisk* opticalDisk) 
    : MoveCopyStudyDialog(parent, opticalDisk) {
    setUpLabels(tr("Copy"));
}

CopyStudyDialog::~CopyStudyDialog() {}

MoveStudyDialog::MoveStudyDialog(QWidget* parent, OpticalDisk* opticalDisk) 
    : MoveCopyStudyDialog(parent, opticalDisk) {
    setUpLabels(tr("Move"));
}

MoveStudyDialog::~MoveStudyDialog() {}
