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
#include "listselector.h"
#include "studyconfiguration.h"

#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItemModel>



namespace EpGui {

using EpCore::Options;
using EpCore::User;
using EpHardware::EpAmplifier::Amplifier;
using EpStudy::Protocol;
using EpStudy::StudyConfiguration;
using EpStudy::StudyConfigurations;
\
StudyConfigurationDialog::StudyConfigurationDialog(StudyConfiguration* config,
                                                   bool administrationAllowed,
                                                   QWidget* parent)
    : QDialog(parent), Ui::StudyConfigurationDialog(),
    model_(new QStandardItemModel), studyConfiguration_(config),
    administrationAllowed_(administrationAllowed) {
    setupUi(this);
    protocolListSelector_ = new ListSelector(availableListView,
                                             selectedListView);
    QStringList selectedNames = Protocol::protocolNames(
            studyConfiguration_->protocolList());
    QStringList unselectedNames = Protocol::protocolNames(
            studyConfiguration_->unselectedProtocols());
    protocolListSelector_->initialize(unselectedNames, selectedNames);

    updateWindowTitle();
    connect(selectPushButton, SIGNAL(clicked()), this, SLOT(selectProtocols()));
    connect(unselectPushButton, SIGNAL(clicked()), this, SLOT(unselectProtocols()));
    connect(selectAllPushButton, SIGNAL(clicked()),
            this, SLOT(selectAllProtocols()));
    connect(unselectAllPushButton, SIGNAL(clicked()),
            this, SLOT(unselectAllProtocols()));
    connect(availableListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enableProtocolSelectButtons()));
    connect(selectedListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enableProtocolSelectButtons()));
    connect(availableListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(selectProtocols()));
    connect(selectedListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(unselectProtocols()));
    connect(saveButton, SIGNAL(clicked()),
        this, SLOT(save()));
    connect(saveAsButton, SIGNAL(clicked()),
        this, SLOT(saveAs()));
    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(updateStudyConfiguration()));
    enableProtocolSelectButtons();
}

StudyConfigurationDialog::~StudyConfigurationDialog() {
    delete protocolListSelector_;
    delete model_;
}

void StudyConfigurationDialog::updateWindowTitle() {
    setWindowTitle(tr("Study Configuration - %1")
                   .arg(studyConfiguration_->name()));
}

void StudyConfigurationDialog::changePage(const QModelIndex& index) {
    QStandardItem* item = model_->itemFromIndex(index);
    if (!item->data().isNull())
        stackedWidget->setCurrentIndex(item->data().toInt());
}

void StudyConfigurationDialog::updateStudyConfiguration() {
    // update studyConfiguration_ before saving or returning
    // bunch of other updating here
    QStringListIterator iter(protocolListSelector_->selected());
    EpCore::ItemList<Protocol> allProtocols;
    QList<Protocol> protocols;
    while (iter.hasNext())
        protocols.append(allProtocols[iter.next()]);
    studyConfiguration_->setProtocolList(protocols);
}

void StudyConfigurationDialog::save() {
    if (administrationAllowed_) {
        StudyConfigurations configList;
        updateStudyConfiguration();
        configList.replace(*studyConfiguration_);
    }
    else
        QMessageBox::information(this, tr("Administrator account required"),
                                 tr("You must be an Administrator to permanently "
                                    "change study configurations"));
}

void StudyConfigurationDialog::saveAs() {
    if (administrationAllowed_) {
            QString configName = studyConfiguration_->name();
            StudyConfigurations configList;
            bool ok;
            QString text =
                    QInputDialog::getText(this,
                                          tr("Enter Study Configuration Name"),
                                          tr("Study configuration name:"),
                                          QLineEdit::Normal,
                                          configName, &ok);
            if (ok && !text.isEmpty()) {
                // search for duplicate study configuration name
                if (configList.isPresent(text)) {
                    int result =
                        QMessageBox::warning(this,
                                             tr("Duplicate Study Configuration Name"),
                                             tr("Study configuration name "
                                                "already exists.  Overwrite?"));
                    if (result != QMessageBox::Ok)
                        return;
                    // remove study configuration with the same name
                    configList.remove(text);
                }
                studyConfiguration_->setName(text);
                updateStudyConfiguration();
                configList.add(*studyConfiguration_);
                updateWindowTitle();
            }
        }
    else
        QMessageBox::information(this, tr("Administrator account required"),
                                 tr("You must be an Administrator to permanently "
                                    "change study configurations"));
}

void StudyConfigurationDialog::amplifierReset() {
    studyConfiguration_->amplifier()->reset();
    QMessageBox::information(this, tr("Amplifier Reset"),
			     tr("Amplifier successfully reset"));
}

void StudyConfigurationDialog::enableProtocolSelectButtons() {
    bool availableItemIsSelected =
            availableListView->currentIndex().row() > -1;
    bool selectedItemIsSelected =
            selectedListView->currentIndex().row() > -1;
    selectPushButton->setEnabled(availableItemIsSelected);
    unselectPushButton->setEnabled(selectedItemIsSelected);
}

void StudyConfigurationDialog::selectProtocols() {
    protocolListSelector_->select();
    enableProtocolSelectButtons();
}

void StudyConfigurationDialog::unselectProtocols() {
    protocolListSelector_->unselect();
    enableProtocolSelectButtons();
}

void StudyConfigurationDialog::selectAllProtocols() {
    protocolListSelector_->selectAll();
    enableProtocolSelectButtons();
}

void StudyConfigurationDialog::unselectAllProtocols() {
    protocolListSelector_->unselectAll();
    enableProtocolSelectButtons();
}

RealTimeStudyConfigurationDialog::RealTimeStudyConfigurationDialog(
    StudyConfiguration* config,
    bool administrationAllowed, QWidget* parent)
    : StudyConfigurationDialog(config, administrationAllowed, parent) {
    QStandardItem* rootItem = new QStandardItem(tr("EP Simulator"));
    int pageNum = 0;
    rootItem->setData(pageNum);
    model()->appendRow(rootItem);
    QStandardItem* realTimeItem = new QStandardItem(tr("Real Time"));
    realTimeItem->setData(pageNum++);
    rootItem->appendRow(realTimeItem);
    QStandardItem* amplifierItem = new QStandardItem(tr("EP Sim Amplifier"));
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
         i < Amplifier::numCIMConnections(epOptions->numChannels); ++i) {
        QStandardItem* block = new QStandardItem(tr("Catheter Block %1").arg(c++));
        block->setData(pageNum++);
        amplifierItem->appendRow(block);
    }
    QStandardItem* stimItem = new QStandardItem(tr("Stim"));
    /// TODO a hack
    pageNum = 11;   // stimPage
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
    connect(amplifierResetButton, SIGNAL(clicked()),
        this, SLOT(amplifierReset()));
}

ReviewStudyConfigurationDialog::ReviewStudyConfigurationDialog(
    StudyConfiguration* config,
    bool administrationAllowed, QWidget* parent, int windowNum)
    : StudyConfigurationDialog(config, administrationAllowed, parent) {
    Q_ASSERT(windowNum == 1 || windowNum == 2);
    QStandardItem* rootItem = new QStandardItem(tr("EP Simulator"));
    int pageNum = 0;  // blank page
    rootItem->setData(pageNum);
    model()->appendRow(rootItem);
    QStandardItem* reviewItem = 
	new QStandardItem(tr("Review %1").arg(QString::number(windowNum)));
    reviewItem->setData(pageNum);  // blank page
    rootItem->appendRow(reviewItem);
    QStandardItem* ecgItem = new QStandardItem(tr("ECG"));
    pageNum = 2;  // skip amplifier page
    ecgItem->setData(pageNum++);
    reviewItem->appendRow(ecgItem);
    QStandardItem* pressureItem = new QStandardItem(tr("Pressure"));
    pressureItem->setData(pageNum++);
    reviewItem->appendRow(pressureItem);
    char c = 'A';
    for (int i = 0; 
         i < Amplifier::numCIMConnections(epOptions->numChannels); ++i) {
        QStandardItem* block = new QStandardItem(tr("Catheter Block %1").arg(c++));
        block->setData(pageNum++);
        reviewItem->appendRow(block);
    }
    pageNum = 11;   // stimPage
    QStandardItem* stimItem = new QStandardItem(tr("Stim"));
    stimItem->setData(pageNum++);
    reviewItem->appendRow(stimItem);
    /// TODO is this actually Analog In in review screen?
    QStandardItem* analogInItem = new QStandardItem(tr("Analog In"));
    analogInItem->setData(pageNum++);
    reviewItem->appendRow(analogInItem);

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

    saveButton->hide();
    saveAsButton->hide();

    connect(treeView, SIGNAL(activated(QModelIndex)),
        this, SLOT(changePage(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)),
        this, SLOT(changePage(QModelIndex)));

}

}
