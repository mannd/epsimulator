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

#ifndef INTERVAL_H
#define INTERVAL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QCoreApplication>
#include <QtCore/QList>
#include <QtCore/QString>

class QDataStream;

namespace EpCore {

class Interval {
    Q_DECLARE_TR_FUNCTIONS(Interval)

public:
    friend QDataStream& operator<<(QDataStream&, const Interval&);
    friend QDataStream& operator>>(QDataStream&, Interval&);
    friend bool operator<(const Interval&, const Interval&);

    Interval(const QString& name = QString());
    Interval(const Interval&);

    Interval& operator=(const Interval&);

    static int magicNumber() {return MagicNumber;}
    static QString fileName() {return QString("intervals.dat");}
    static QStringList defaultNames();

    void setName(const QString& name) {name_  = name;}
    void setMark1(const QString& mark) {mark1_ = mark;}
    void setMark2(const QString& mark) {mark2_ = mark;}
    void setWidth(int width) {width = width_;}

    QString name() const {return name_;}
    QString mark1() const {return mark1_;}
    QString mark2() const {return mark2_;}
    int width() const {return width_;}

private:
    enum {MagicNumber = 0x99f00010};

    void copyInterval(const Interval&);

    QString name_;
    QString mark1_;
    QString mark2_;
    int width_;
};

class Intervals {
    Q_DECLARE_TR_FUNCTIONS(Intervals)
public:
    typedef QList<Interval> IntervalList;
    Intervals();
    Intervals(const Intervals&);

    Intervals& operator=(const Intervals& rhs);
    Interval& operator[](const int index);

    int count() const;
    void sort();
    int size() const;
    void removeAt(int i);
    void insert(int i, const Interval& interval);


    void update() {save();}

private:
    enum {MagicNumber = 0x99f00010};


    void load();
    void save();

    void makeDefault();
    void copyIntervals(const Intervals&);

    const static QString fileName_;

    IntervalList intervalList_;
};

class IntervalModel : public QAbstractListModel {
public:
    IntervalModel(QObject* parent = 0);

    void setIntervals(Intervals& intervals) {intervals_ = intervals;}
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool removeRows(int row, int count, const QModelIndex& parent);
    bool insertRows(int row, int count, const QModelIndex& parent);

    Intervals intervals() const {return intervals_;}

private:
    Intervals intervals_;
};

}

#endif // INTERVAL_H
