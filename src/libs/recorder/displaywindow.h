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

#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

class QAction;
class QSettings;

namespace EpCore {
    class Options;
}

namespace EpGui {
    class StudyConfigurationDialog;
}

namespace EpStudy {
    class Study;
}

namespace EpRecorder {

class SignalWidget;

/**
 * Abstract base class for all MdiSubWindows in Recorder MdiArea.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class DisplayWindow : public QMainWindow {
    Q_OBJECT
public:
    virtual void writeSettings(QSettings&) = 0;
    virtual void readSettings(QSettings&) = 0;
    // this is the key used to read/write settings
    virtual QString key() const = 0; 

    QString name() const {return name_;}
    // for windows with same name but different numbers, e.g. Review1 and Review2.
    int number() const {return number_;}

protected:
    DisplayWindow(const QString& name, EpStudy::Study* study, 
		  const EpCore::Options* const options,
                  int number = 0, 
                  QWidget* parent = 0); 
    EpStudy::Study* study() const {return study_;}
    const EpCore::Options* options() const {return options_;}

private:
    const QString name_;
    EpStudy::Study* study_;
    const EpCore::Options* const options_;
    const int number_;
};

/**
 * Abstract base class for RealTimeWindow and ReviewWindow.  Implements common 
 * functionality, such as changing pages and sweep speed, but does not 
 * implement toolbars and signal displays.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class SignalDisplayWindow : public DisplayWindow  {
    Q_OBJECT

public:
    void setCurrentPage(int page);
    int currentPage() const {return currentPage_;}

    virtual void createCentralWidget();
    virtual void createActions();
    virtual void createToolBars() = 0;

public slots:
    void previousPage();
    void nextPage();
    void updateWindowTitle();
    //void setSweepSpeed(int);
    void studyConfiguration();

signals:
    void pageChanged(int newPage);
    void studyConfigurationChanged();

protected:
    SignalDisplayWindow(const QString& name, EpStudy::Study* study,
			const EpCore::Options* const options, 
			int number = 0,
			QWidget* parent = 0);
    virtual EpGui::StudyConfigurationDialog* studyConfigurationDialog() = 0;

private:
    static const int minPage = 1;
    static const int maxPage = 8;

    SignalWidget* signalWidget_;
    int currentPage_;

    QAction* previousPageAction_;
    QAction* nextPageAction_;
    QAction* increaseSweepSpeedAction_;
    QAction* decreaseSweepSpeedAction_;
};

}

#endif
