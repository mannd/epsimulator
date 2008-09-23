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

#include "amplifier.h"
#include "options.h"

#include <QHeaderView>
#include <QStandardItemModel>

namespace EpGui {

using EpCore::Options;
using EpHardware::EpAmplifier::Amplifier;

StudyConfigurationDialog::StudyConfigurationDialog(QWidget* parent)
    : QDialog(parent), Ui::StudyConfigurationDialog(),
      model_(new QStandardItemModel) {
    setupUi(this);
    connect(saveButton, SIGNAL(clicked()),
        this, SLOT(save()));
    connect(saveAsButton, SIGNAL(clicked()),
        this, SLOT(saveAs()));
}

StudyConfigurationDialog::~StudyConfigurationDialog() {
    delete model_;
}

void StudyConfigurationDialog::changePage(const QModelIndex& index) {
    QStandardItem* item = model_->itemFromIndex(index);
    if (!item->data().isNull())
        stackedWidget->setCurrentIndex(item->data().toInt());
}

void StudyConfigurationDialog::save() {

}

void StudyConfigurationDialog::saveAs() {

}

RealTimeStudyConfigurationDialog::RealTimeStudyConfigurationDialog(
    QWidget* parent) : StudyConfigurationDialog(parent) {
    QStandardItem* rootItem = new QStandardItem(tr("EP Simulator"));
    model()->appendRow(rootItem);
    QStandardItem* realTimeItem = new QStandardItem(tr("Real Time"));
    rootItem->appendRow(realTimeItem);
    QStandardItem* amplifierItem = new QStandardItem(tr("EP Sim Amplifier"));
    int pageNum = 0;
    amplifierItem->setData(pageNum++);
    realTimeItem->appendRow(amplifierItem);
    QStandardItem* ecgItem = new QStandardItem(tr("ECG"));
    ecgItem->setData(pageNum++);
    amplifierItem->appendRow(ecgItem);
    QStandardItem* pressureItem = new QStandardItem(tr("Pressure"));
    pressureItem->setData(pageNum++);
    amplifierItem->appendRow(pressureItem);
    char c = 'A';
    for (int i = 0; 
         i < Amplifier::numCIMConnections(Options::instance()->numChannels); ++i) {
        QStandardItem* block = new QStandardItem(tr("Catheter Block %1").arg(c++));
        block->setData(pageNum++);
        amplifierItem->appendRow(block);
    }
    QStandardItem* stimItem = new QStandardItem(tr("Stim"));
    stimItem->setData(pageNum++);
    amplifierItem->appendRow(stimItem);
    QStandardItem* inputItem = new QStandardItem(tr("Input"));
    realTimeItem->appendRow(inputItem);
    QStandardItem* channelItem = new QStandardItem(tr("Channel"));
    channelItem->setData(pageNum++);
    inputItem->appendRow(channelItem);
    QStandardItem* imagingItem = new QStandardItem(tr("Imaging"));
    imagingItem->setData(pageNum++);
    realTimeItem->appendRow(imagingItem);
    QStandardItem* analogOutItem = new QStandardItem(tr("Analog Out"));
    analogOutItem->setData(pageNum++);
    realTimeItem->appendRow(analogOutItem);
    QStandardItem* ablationItem = new QStandardItem(tr("Ablation"));
    ablationItem->setData(pageNum++);
    realTimeItem->appendRow(ablationItem);
    QStandardItem* measurementsItem = new QStandardItem(tr("Measurements"));
    measurementsItem->setData(pageNum++);
    rootItem->appendRow(measurementsItem);
    QStandardItem* protocolListItem = new QStandardItem(tr("Protocol List"));
    protocolListItem->setData(pageNum++);
    rootItem->appendRow(protocolListItem);
    QStandardItem* activationAlignmentItem = new QStandardItem(tr(
        "Activation Alignment"));
    activationAlignmentItem->setData(pageNum++);
    rootItem->appendRow(activationAlignmentItem);
    QStandardItem* mappingItem = new QStandardItem(tr("Mapping"));
    mappingItem->setData(pageNum++);
    rootItem->appendRow(mappingItem);

    treeView->setModel(model());
    treeView->header()->hide();
    treeView->expandAll();
    treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    QItemSelectionModel* selectionModel = treeView->selectionModel();
    QModelIndex rootIndex = model()->index(0, 0, QModelIndex());
    QModelIndex realTimeIndex = model()->index(0, 0, rootIndex);
    QModelIndex amplifierIndex = model()->index(0, 0, realTimeIndex);
    QModelIndex defaultIndex = model()->index(0, 0, amplifierIndex);
    QItemSelection selection(defaultIndex, defaultIndex);
    selectionModel->select(selection, QItemSelectionModel::Select);
    changePage(defaultIndex);

    connect(treeView, SIGNAL(activated(QModelIndex)),
        this, SLOT(changePage(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)),
        this, SLOT(changePage(QModelIndex)));
}

ReviewStudyConfigurationDialog::ReviewStudyConfigurationDialog(
    QWidget* parent, int windowNum) : StudyConfigurationDialog(parent) {
    Q_ASSERT(windowNum == 1 || windowNum == 2);
    QStandardItem* rootItem = new QStandardItem(tr("EP Simulator"));
    model()->appendRow(rootItem);
    QStandardItem* reviewItem = 
	new QStandardItem(tr("Review %1").arg(QString::number(windowNum)));
    rootItem->appendRow(reviewItem);
    QStandardItem* ecgItem = new QStandardItem(tr("ECG"));
    int pageNum = 1;  // skip amplifier page
    ecgItem->setData(pageNum++);
    reviewItem->appendRow(ecgItem);
    QStandardItem* pressureItem = new QStandardItem(tr("Pressure"));
    pressureItem->setData(pageNum++);
    reviewItem->appendRow(pressureItem);
    char c = 'A';
    for (int i = 0; 
         i < Amplifier::numCIMConnections(Options::instance()->numChannels); ++i) {
        QStandardItem* block = new QStandardItem(tr("Catheter Block %1").arg(c++));
        block->setData(pageNum++);
        reviewItem->appendRow(block);
    }
    QStandardItem* stimItem = new QStandardItem(tr("Stim"));
    stimItem->setData(pageNum++);
    reviewItem->appendRow(stimItem);
    /// TODO is this actually Analog In in review screen?
    QStandardItem* analogOutItem = new QStandardItem(tr("Analog Out"));
    analogOutItem->setData(pageNum++);
    reviewItem->appendRow(analogOutItem);

    treeView->setModel(model());
    treeView->header()->hide();
    treeView->expandAll();
    treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    QItemSelectionModel* selectionModel = treeView->selectionModel();
    QModelIndex rootIndex = model()->index(0, 0, QModelIndex());
    QModelIndex reviewIndex = model()->index(0, 0, rootIndex);
    QModelIndex defaultIndex = model()->index(0, 0, reviewIndex);
    QItemSelection selection(defaultIndex, defaultIndex);
    selectionModel->select(selection, QItemSelectionModel::Select);
    changePage(defaultIndex);

    connect(treeView, SIGNAL(activated(QModelIndex)),
        this, SLOT(changePage(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)),
        this, SLOT(changePage(QModelIndex)));

}

}
