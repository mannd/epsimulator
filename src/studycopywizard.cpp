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

#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qstring.h>

StudyMoveCopyWizard::StudyMoveCopyWizard(QWidget* parent, 
	const char* name) : StudyMoveCopyWizardBase(parent, name) {
	connect(sourceLineEdit, SIGNAL(textChanged(const QString&)),
		this, SLOT(enableFinishButton()));
	connect(destinationLineEdit, SIGNAL(textChanged(const QString&)),
		this, SLOT(enableFinishButton()));
	connect(studiesListBox, SIGNAL(selectionChanged()),
		this, SLOT(enableFinishButton()));
}
	
void StudyMoveCopyWizard::enableFinishButton() {
	bool enable = !sourceLineEdit->text().isEmpty() &&
				  !destinationLineEdit->text().isEmpty() &&
				  studiesListBox->currentItem() >= 0;
	setFinishEnabled(page(1), enable);
}

StudyCopyWizard::StudyCopyWizard(QWidget* parent, const char* name)
    : StudyMoveCopyWizard(parent, name) {
    setCaption(tr("Study Copy Wizard"));
    instructionsLabel->setText(instructionsLabel->text().arg("copy"));
}

StudyMoveWizard::StudyMoveWizard(QWidget* parent, const char* name)
    : StudyMoveCopyWizard(parent, name) {
    setCaption(tr("Study Move Wizard"));
    instructionsLabel->setText(instructionsLabel->text().arg("move"));

}
