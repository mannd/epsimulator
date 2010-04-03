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

class ListSelector;

class StudyConfigurationDialog : public QDialog, 
    protected Ui::StudyConfigurationDialog {
    Q_OBJECT

public:
    StudyConfigurationDialog(EpStudy::StudyConfiguration*,
                             QWidget* parent = 0);
    ~StudyConfigurationDialog() = 0;

public slots:

protected:
    void setModel(QStandardItemModel* model) {model_ = model;}
    QStandardItemModel* model() const {return model_;}
    EpStudy::StudyConfiguration* studyConfiguration() {
        return studyConfiguration_;}

protected slots:
    void changePage(const QModelIndex&);
    void enableProtocolSelectButtons();
    void save();
    void saveAs();
    void amplifierReset();

private:
    void updateWindowTitle();

    QStandardItemModel* model_;
    EpStudy::StudyConfiguration* studyConfiguration_;
    ListSelector* protocolListSelector_;

};

class RealTimeStudyConfigurationDialog : public StudyConfigurationDialog {
    Q_OBJECT

public:
    RealTimeStudyConfigurationDialog(EpStudy::StudyConfiguration*,
                                     QWidget* parent = 0);
};

class ReviewStudyConfigurationDialog: public StudyConfigurationDialog {
    Q_OBJECT

public:
    ReviewStudyConfigurationDialog(EpStudy::StudyConfiguration*,
                                   QWidget* parent = 0,
                                   int windowNum = 1);
};

}

#endif
