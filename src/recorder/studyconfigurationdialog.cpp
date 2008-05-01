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

#include "studyconfigurationdialog.h"

#include <QHeaderView>
#include <QStandardItemModel>

using EpGui::StudyConfigurationDialog;

StudyConfigurationDialog::StudyConfigurationDialog(QWidget* parent)
    : QDialog(parent), Ui::StudyConfigurationDialog() {
    setupUi(this);
    model_ = new QStandardItemModel;
    QStandardItem* rootItem = new QStandardItem(tr("EP Simulator"));
    model_->appendRow(rootItem);
    QStandardItem* realTimeItem = new QStandardItem(tr("Real Time"));
    rootItem->appendRow(realTimeItem);
    QStandardItem* amplifierItem = new QStandardItem(tr("EP Sim Amplifier"));
    realTimeItem->appendRow(amplifierItem);
    QStandardItem* ecgItem = new QStandardItem(tr("ECG"));
    QStandardItem* pressureItem = new QStandardItem(tr("Pressure"));
    amplifierItem->appendRow(ecgItem);
    amplifierItem->appendRow(pressureItem);
    /// TODO change this to check number of blocks in amplifier
    for (char c = 'A'; c <= 'G'; ++c) {
        QStandardItem* block = new QStandardItem(tr("Catheter Block %1").arg(c));
        amplifierItem->appendRow(block);
    }
    QStandardItem* stimItem = new QStandardItem(tr("Stim"));
    amplifierItem->appendRow(stimItem);
    QStandardItem* inputItem = new QStandardItem(tr("Input"));
    realTimeItem->appendRow(inputItem);
    QStandardItem* channelItem = new QStandardItem(tr("Channel"));
    inputItem->appendRow(channelItem);
    QStandardItem* imagingItem = new QStandardItem(tr("Imaging"));
    realTimeItem->appendRow(imagingItem);
    QStandardItem* analogOutItem = new QStandardItem(tr("Analog Out"));
    realTimeItem->appendRow(analogOutItem);
    QStandardItem* ablationItem = new QStandardItem(tr("Ablation"));
    realTimeItem->appendRow(ablationItem);
    QStandardItem* measurementsItem = new QStandardItem(tr("Measurement"));
    rootItem->appendRow(measurementsItem);
    QStandardItem* protocolListItem = new QStandardItem(tr("Protocol List"));
    rootItem->appendRow(protocolListItem);
    QStandardItem* activationAlignmentItem = new QStandardItem(tr(
        "Activation Alignment"));
    rootItem->appendRow(activationAlignmentItem);
    QStandardItem* mappingItem = new QStandardItem(tr("Mapping"));
    rootItem->appendRow(mappingItem);
    
    treeView->setModel(model_);
    treeView->header()->hide();
    treeView->expandAll();
}

StudyConfigurationDialog::~StudyConfigurationDialog() {
    delete model_;
}



