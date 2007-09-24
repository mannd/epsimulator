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
#include "studycopywizard.h"

#include "catalog.h"
#include "epfuns.h"
#include "opticaldisk.h"
#include "options.h"
#include "study.h"

#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qstring.h>

StudyMoveCopyWizard::StudyMoveCopyWizard(const QString& action, QWidget* parent, 
	const char* name) : StudyMoveCopyWizardBase(parent, name), catalog_(0) {
    connect(sourceLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(enableFinishButton()));
    connect(destinationLineEdit, SIGNAL(textChanged(const QString&)),
	this, SLOT(enableFinishButton()));
    connect(studiesListBox, SIGNAL(highlighted(int)),
	this, SLOT(enableFinishButton()));
    connect(selectAllButton, SIGNAL(clicked()),
        this, SLOT(enableFinishButton()));
    connect(browseSourceButton, SIGNAL(clicked()),
        this, SLOT(enableFinishButton()));
    connect(browseDestinationButton, SIGNAL(clicked()),
        this, SLOT(enableFinishButton()));

    QString upperCase = action;
    upperCase[0] = action[0].upper();
    setCaption(tr("Study %1 Wizard").arg(upperCase));
    instructionsLabel->setText(instructionsLabel->text().arg(action).arg(action));
    destinationLabel->setText(destinationLabel->text().arg(upperCase));    
    finishButton()->setText(upperCase);
    setHelpEnabled(page(0), false);
    setHelpEnabled(page(1), false);
    studiesListBox->clear();
}

// void StudyMoveCopyWizard::showPage(QWidget* page) {
//    //setHelpEnabled(page, false);
// }

void StudyMoveCopyWizard::setSourcePathName(const QString& s) {
    sourceLineEdit->setText(s);
}

QString StudyMoveCopyWizard::sourcePathName() const {
    return sourceLineEdit->text();
}
    
QString StudyMoveCopyWizard::destinationPathName() const {
    return destinationLineEdit->text();
}

void StudyMoveCopyWizard::sourcePathBrowse() {
    EpFuns::browseFilePaths(this, sourceLineEdit);
}

void StudyMoveCopyWizard::destinationPathBrowse() {
    EpFuns::browseFilePaths(this, destinationLineEdit);
}
	
void StudyMoveCopyWizard::enableFinishButton() {
    bool enable = !sourceLineEdit->text().isEmpty() &&
                                !destinationLineEdit->text().isEmpty() &&
                                (studiesListBox->currentItem() >= 0 ||
                                 studiesListBox->selectedItem() >= 0);
    setFinishEnabled(page(1), enable);
}

void StudyMoveCopyWizard::selectAll() {
//     for (unsigned int i = 0; i < studiesListBox->count(); ++i)
//         studiesListBox->setSelected(i, true);
    studiesListBox->selectAll(true);
}

void StudyMoveCopyWizard::refreshList() {
    studyNameList_.clear();
    QString studyName;
    studiesListBox->clear();
    delete catalog_;
    catalog_ = new Catalog(sourceLineEdit->text());
    /// TODO oversimplistic, need a map with row number and study key, to retrieve studies to be moved.
    for (Catalog::CatalogMap::const_iterator it = catalog_->begin(); 
         it != catalog_->end(); ++it) {
        studyName = it.data().study.name().fullName() + " " 
                    + it.data().study.dateTime().toString();
        studyNameList_.push_back(studyName);
        studiesListBox->insertItem(studyName);
    }
}

StudyMoveCopyWizard::~StudyMoveCopyWizard() {
    delete catalog_;
}

StudyCopyWizard::StudyCopyWizard(QWidget* parent, const char* name)
    : StudyMoveCopyWizard(tr("copy"), parent, name) {
}

void StudyCopyWizard::copy() {
 /*   OpticalDisk* disk = new OpticalDisk(destinationLineEdit->text());
    disk->readLabel();
    if (!disk_->isLabeled()) 
        relabelDisk();
    for (unsigned int i = 0; i < studiesListBox->count(); ++i) {
        if (studiesListBox->isSelected(i))
            ;       // copy study
    }   
    delete disk;
    // copy studies from source to destination
    // create/update destination*/ //catalog
}

StudyMoveWizard::StudyMoveWizard(QWidget* parent, const char* name)
    : StudyMoveCopyWizard(tr("move"), parent, name) {
}

void StudyMoveWizard::move() {
//     OpticalDisk* disk = new OpticalDisk(destinationLineEdit->text());
//     disk->readLabel();
//     if (!disk_->isLabeled()) 
//         relabelDisk();
//     for (unsigned int i = 0; i < studiesListBox->count(); ++i) {
//         if (studiesListBox->isSelected(i))
//             ;       // move study
//     }
//     delete disk;
//     // copy studies from source to destination
//     // create/update destination catalog
//     // update source catalog
//     // update tracking catalogs
}

