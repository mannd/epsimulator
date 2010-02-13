/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#include "interval.h"

#include "fileutilities.h"
#include "options.h"

#include <QDataStream>
#include <QStringList>

namespace EpCore {

QDataStream& operator<<(QDataStream& out, const Interval& interval) {
    out << interval.name_ << interval.mark1_ << interval.mark2_
            << (qint32)interval.width_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Interval& interval) {
    in >> interval.name_ >> interval.mark1_ >> interval.mark2_
            >> interval.width_;
    return in;
}

bool operator<(const Interval& value1, const Interval& value2) {
    return value1.name() < value2.name();
}

}

using EpCore::Interval;
using EpCore::IntervalModel;
using EpCore::Intervals;

Interval::Interval(const QString& name)
    : name_(name), mark1_(QString()), mark2_(QString()), width_(0) {}

Interval::Interval(const Interval& interval) {
    copyInterval(interval);
}

Interval& Interval::operator=(const Interval& rhs) {
    if (this == &rhs)
        return *this;
    copyInterval(rhs);
    return *this;
}

QStringList Interval::defaultNames() {
    QStringList names = QStringList() <<
                        tr("A1A1") << tr("A1A2");
    return names;
}

void Interval::copyInterval(const Interval& interval) {
    name_ = interval.name_;
    mark1_ = interval.mark1_;
    mark2_ = interval.mark2_;
    width_ = interval.width_;
}

const QString Intervals::fileName_ = "intervals.dat";

Intervals::Intervals()
    : intervalList_() {
    load();
    if (intervalList_.isEmpty()) {
        makeDefault();
        save();
    }
}

Intervals::Intervals(const Intervals& intervals) {
    copyIntervals(intervals);
}

Intervals& Intervals::operator=(const Intervals& rhs) {
    if (this == &rhs)
        return *this;
    copyIntervals(rhs);
    return *this;
}

void Intervals::load() {
    try {
        EpCore::loadSystemData(MagicNumber, fileName_,
                               intervalList_, EpCore::Options::instance());
    }
    catch (EpCore::IoError&) {
        // ignore failure to read, leave list empy
    }
}


void Intervals::save() {
    EpCore::saveSystemData(MagicNumber, fileName_, intervalList_,
                           EpCore::Options::instance());
}

void Intervals::makeDefault() {
    intervalList_.clear();
    intervalList_.append(Interval(tr("A1A2")));
    intervalList_.append(Interval(tr("A1A1")));
    intervalList_.append(Interval(tr("A2A3")));
    intervalList_.append(Interval(tr("A1H1")));
    intervalList_.append(Interval(tr("A2H2")));
    intervalList_.append(Interval(tr("H1H1")));
    intervalList_.append(Interval(tr("H1H2")));
    intervalList_.append(Interval(tr("S1S1")));
    intervalList_.append(Interval(tr("S1S2")));
    intervalList_.append(Interval(tr("S2S3")));
    intervalList_.append(Interval(tr("S3S4")));
    intervalList_.append(Interval(tr("S4S5")));
    intervalList_.append(Interval(tr("V1V1")));
    intervalList_.append(Interval(tr("V1V2")));
    intervalList_.append(Interval(tr("V2V3")));
    intervalList_.append(Interval(tr("V3V4")));
    intervalList_.append(Interval(tr("V4V5")));
    intervalList_.append(Interval(tr("CSNRT")));
    intervalList_.append(Interval(tr("SNRT")));
}

void Intervals::copyIntervals(const Intervals& intervals) {
    intervalList_ = intervals.intervalList_;
}

int Intervals::count() const {
    return intervalList_.count();
}

void Intervals::sort() {
    qSort(intervalList_.begin(), intervalList_.end());
}

int Intervals::size() const {
    return intervalList_.size();
}

void Intervals::removeAt(int i) {
    intervalList_.removeAt(i);
}

void Intervals::insert(int i, const Interval& interval) {
    intervalList_.insert(i, interval);
}

Interval& Intervals::operator[](const int index) {
    return intervalList_[index];
}

IntervalModel::IntervalModel(QObject* parent)
    : QAbstractListModel(parent) {}

int IntervalModel::rowCount(const QModelIndex& /* parent */) const {
    return intervals().count();
}

QVariant IntervalModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    if (index.row() >= intervals_.size())
        return QVariant();
    if (role == Qt::DisplayRole) {
        return intervals()[index.row()].name();
    }
    if (role == Qt::EditRole)
        return intervals()[index.row()].name();
    return QVariant();
}

bool IntervalModel::setData(const QModelIndex& index, const QVariant& value,
                            int role) {
    if (index.isValid() && role == Qt::EditRole) {
        intervals_[index.row()].setName(value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags IntervalModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool IntervalModel::removeRows(int row, int count,
                               const QModelIndex& /* parent */) {
    beginRemoveRows(QModelIndex(), row, row+count-1);

    for (int i = 0; i < count; ++i) {
        intervals_.removeAt(row);
    }

    endRemoveRows();
    return true;
}

bool IntervalModel::insertRows(int row, int count,
                               const QModelIndex& /* parent */) {
    beginInsertRows(QModelIndex(), row, row+count-1);

    for (int i = 0; i < count; ++i) {
        intervals_.insert(row, Interval());
    }

    endInsertRows();
    return true;
}



