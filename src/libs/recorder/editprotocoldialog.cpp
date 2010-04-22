/***************************************************************************
 *   Copyright (C) %YEAR% by EP Studios, Inc.                                *
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

#include "editprotocoldialog.h"

#include "columnformat.h"

using EpGui::EditProtocolDialog;
using EpStudy::Protocol;

EditProtocolDialog::EditProtocolDialog(AbstractEditItemsDialog::EditorType type,
                                       QWidget *parent)
                                           : QDialog(parent){
    setupUi(this);
    QString title = type == AbstractEditItemsDialog::NewItem
                                       ? tr("New") : tr("Edit");
    title = tr("%1 Protocol").arg(title);
    setWindowTitle(title);
//    senseChannelLabelComboBox->
    QStringList list;
    for (int i = 0; i < columnFormats_.size(); ++i)
        list << columnFormats_[i].name();
    list.sort();
    columnFormatComboBox->insertItems(0, list);
    list.clear();
    for (int i = 0; i < windowSettings_.size(); ++i)
        list << windowSettings_[i].name();
    list.sort();
    windowSettingComboBox->insertItems(0, list);
    list.clear();
    list << tr("On") << tr("Off");
    updateReviewComboBox->insertItems(0, list);
    list.clear();
    list << tr("Left") << tr("1/4")
            << tr("Center") << tr("3/4")
            << tr("Right");
    focalPointComboBox->insertItems(0, list);
    list.clear();
    for (int i = 1; i <= 8; ++i)
        list.append(tr("Page %1").arg(i));
    list.append(tr("<Current Page>"));
    displayPageComboBox->insertItems(0, list);

    connect(nameLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(enableOkButton(const QString&)));
    enableOkButton(nameLineEdit->text());
}

void EditProtocolDialog::setProtocol(const Protocol& protocol) {
    nameLineEdit->setText(protocol.name());
    columnFormatComboBox->setCurrentIndex(
            columnFormats_.index(protocol.columnFormat().name()));
    windowSettingComboBox->setCurrentIndex(
            windowSettings_.index(protocol.windowSetting().name()));
    displayPageComboBox->setCurrentIndex(protocol.displayPage() - 1);
    updateReviewComboBox->setCurrentIndex(protocol.updateReviewWindow()
                                                ? 0 : 1);
    focalPointComboBox->setCurrentIndex(protocol.focalPoint());



}

Protocol EditProtocolDialog::protocol() const {
    Protocol protocol;
    protocol.setName(nameLineEdit->text());
    protocol.setColumnFormat(columnFormats_.value(
            columnFormatComboBox->currentIndex()));
    protocol.setWindowSetting(windowSettings_.value(
            windowSettingComboBox->currentIndex()));
    protocol.setDisplayPage(displayPageComboBox->currentIndex() + 1);
    protocol.setUpdateReviewWindow(updateReviewComboBox->
                                   currentIndex() ? false : true);
    protocol.setFocalPoint(static_cast<Protocol::FocalPoint>
                          (focalPointComboBox->currentIndex()));
    return protocol;
}

void EditProtocolDialog::enableOkButton(const QString& text) {
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
}
