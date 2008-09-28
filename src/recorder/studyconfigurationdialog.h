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

#ifndef STUDYCONFIGURATIONDIALOG_H
#define STUDYCONFIGURATIONDIALOG_H

#include "ui_studyconfigurationdialog.h"

#include <QDialog>

class QModelIndex;
class QStandardItemModel;

namespace EpStudy {
    class StudyConfiguration;
}

namespace EpGui {

using EpStudy::StudyConfiguration;

class StudyConfigurationDialog : public QDialog, 
    protected Ui::StudyConfigurationDialog {
    Q_OBJECT

public:
    StudyConfigurationDialog(StudyConfiguration*, QWidget* parent = 0);
    ~StudyConfigurationDialog() = 0;

public slots:

protected:
    void setModel(QStandardItemModel* model) {model_ = model;}
    QStandardItemModel* model() const {return model_;}
    StudyConfiguration* studyConfiguration() {return studyConfiguration_;}

protected slots:
    void changePage(const QModelIndex&);
    void save();
    void saveAs();
    void amplifierReset();

private:
    QStandardItemModel* model_;
    StudyConfiguration* studyConfiguration_;
};

class RealTimeStudyConfigurationDialog : public StudyConfigurationDialog {
    Q_OBJECT

public:
    RealTimeStudyConfigurationDialog(StudyConfiguration*, QWidget* parent = 0);
};

class ReviewStudyConfigurationDialog: public StudyConfigurationDialog {
    Q_OBJECT

public:
    ReviewStudyConfigurationDialog(StudyConfiguration*, 
                                   QWidget* parent = 0, int windowNum = 1);
};

}

#endif
