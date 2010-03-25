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

#ifndef LISTSELECTOR_H
#define LISTSELECTOR_H

class QListView;
class QStringList;
class QStringListModel;

namespace EpGui {

class ListSelector {
public:
    ListSelector(QListView* unselectedListView,
                 QListView* selectedListView);
    ListSelector(const QStringList& unselected,
                 const QStringList& selected,
                 QListView* unselectedListView,
                 QListView* selectedListView);
    ~ListSelector();

    void initialize(const QStringList& unselected,
                    const QStringList& selected);

    void select();
    void unselect();
    void selectAll();
    void unselectAll();

    QStringList selected();
    QStringList all();

private:
    void move(QListView* sourceView,
              QListView* destView,
              QStringListModel* sourceModel,
              QStringListModel* destModel);

    QStringListModel* unselectedModel_;
    QStringListModel* selectedModel_;
    QListView* unselectedListView_;
    QListView* selectedListView_;

};

}

#endif // LISTSELECTOR_H
