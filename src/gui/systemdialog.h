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

#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include "ui_systemdialog.h"

#include <QDialog>
#include <QString>

class Options;
class QWidget;

class SystemDialog : public QDialog, private Ui::SystemDialog {
    Q_OBJECT

public:
    SystemDialog(Options* options, const QString& path, 
                 const QString& label, 
                 const QString& side,
                 QWidget* parent = 0,
                 bool allowAcquisitionChange = true);
    ~SystemDialog();

    void removeFilePathsTab();
    void setOptions();

public slots:
    void opticalStudyPathBrowse();
    void networkStudyPathBrowse();
    void exportFilePathBrowse();
    void enableExportFilePathLineEdit();
    void enableNetworkStudyPathLineEdit();

private:
    void setEnableNetworkStorage(bool enable);
    void setEnableFileExport(bool enable);

    long diskFreeSpace(const QString& path) const;
    long timeRemaining(long kBytes) const;

    Options* options_;
    QString path_;

};

#endif
