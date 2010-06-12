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
#ifndef EDITSTUDYCONFIGSDIALOG_H
#define EDITSTUDYCONFIGSDIALOG_H

#include "ui_editstudyconfigsdialog.h"

#include "studyconfiguration.h"

#include <QDialog>

namespace EpCore {
    class Options;
    class User;
}

namespace EpNavigator {

class EditStudyConfigsDialog: public QDialog,
    private Ui::EditStudyConfigsDialog {
    Q_OBJECT

public:
    EditStudyConfigsDialog(const EpCore::User* const,
                           const EpCore::Options* const,
                           QWidget *parent = 0);

    EpStudy::StudyConfiguration config() const {
        return configList_[configListWidget->currentRow()];}

private slots:
    //void enableOkButton();
    void newStudyConfig();
    void editStudyConfig();
    void copyStudyConfig();
    void deleteStudyConfig();

private:
    void createConfigListWidget();
    void noSelectionError();

    const EpCore::User* user_;
    const EpCore::Options* options_;
    EpStudy::StudyConfigurations configList_;
};

}

#endif // EDITSTUDYCONFIGSDIALOG_H
