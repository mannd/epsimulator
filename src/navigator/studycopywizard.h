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
#ifndef STUDYCOPYWIZARD_H
#define STUDYCOPYWIZARD_H

#include "studymovecopywizardbase.h"

#include <vector>

class Catalog;

/**
 * This is the base for the copy and move wizards.
 */
class StudyMoveCopyWizard: protected StudyMoveCopyWizardBase {
// protected inheritance rather than private, to allowed derived class below
    Q_OBJECT
public:
    StudyMoveCopyWizard(const QString& action, QWidget* parent = 0, const char *name = 0);

    ~StudyMoveCopyWizard();

    void setSourcePathName(const QString&);
    QString sourcePathName() const;
    QString destinationPathName() const;
    
    using StudyMoveCopyWizardBase::exec;

public slots:    
    void sourcePathBrowse();
    void destinationPathBrowse();
    void selectAll();    
    void enableFinishButton();
    void refreshList();
 //   virtual void showPage(QWidget*);

protected:
    std::vector<QString> studyNameList_;

private:
    Catalog* catalog_;
};

class StudyCopyWizard : public StudyMoveCopyWizard {
    Q_OBJECT
public:
    StudyCopyWizard(QWidget* parent = 0, const char* name = 0);
    void copy();
};

class StudyMoveWizard : public StudyMoveCopyWizard {
    Q_OBJECT
public:
    StudyMoveWizard(QWidget* parent = 0, const char * name = 0);
    void move();
};

#endif
