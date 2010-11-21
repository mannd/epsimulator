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

#ifndef MARK_H
#define MARK_H

#include <QMap>
#include <QPoint>
#include <QString>

class QStringList;

namespace EpCore {

class Mark {
public:
    friend class ColumnFormat;
    friend QDataStream& operator<<(QDataStream& out, const Mark&);
    friend QDataStream& operator>>(QDataStream& in, Mark&);

    enum MarkType {None, P, QRSonset, QRSoffset, Tonset, Toffset,
                   A, H, Hoffset, V,
                   S1, S2, S3, S4, S5,
                   A1, A2, A3, A4, A5,
                   H1, H2, H3,
                   V1, V2, V3, V4, V5,
                   LastType = V5};

    Mark(MarkType type = None);
    Mark(const Mark&);

    Mark& operator=(const Mark&);

    static QString markName(MarkType n) {return markMap_.value(n);}
    static QStringList markNames();

    void setType(MarkType type) {type_ = type;}
    void setX(int x) {point_.setX(x);}
    void setY(int y) {point_.setY(y);}
    void setPoint(int x, int y ) {setX(x); setY(y);}

    QString name() const {return markMap_[type_];}
    MarkType type() const {return type_;}
    int x() const {return point_.x();}
    int y() const {return point_.y();}
    QPoint point() const {return point_;}

private:
    MarkType type_;
    // ?? whether to use QPoint or QPointF in EP Simulator
    QPoint point_;
    static QMap<MarkType, QString> initMarkMap() {
        QMap<MarkType, QString> map;
        map[None] = "None";
        map[P] = "P";
        map[QRSonset] = "QRS onset";
        map[QRSoffset] = "QRS offset";
        map[Tonset] = "T onset";
        map[Toffset] = "T offset";
        map[A] = "A";
        map[H] = "H";
        map[Hoffset] = "H offset";
        map[V] = "V";
        map[S1] = "S1";
        map[S2] = "S2";
        map[S3] = "S3";
        map[S4] = "S4";
        map[S5] = "S5";
        map[A1] = "A1";
        map[A2] = "A2";
        map[A3] = "A3";
        map[A4] = "A4";
        map[A5] = "A5";
        map[H1] = "H1";
        map[H2] = "H2";
        map[H3] = "H3";
        map[V1] = "V1";
        map[V2] = "V2";
        map[V3] = "V3";
        map[V4] = "V4";
        map[V5] = "V5";
        return map;
    }
    static const QMap<MarkType, QString> markMap_;
};

}

#endif // MARK_H
