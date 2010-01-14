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

#ifndef STUDYCONFIGURATION_H
#define STUDYCONFIGURATION_H

#include "amplifier.h"

#include <QBitArray>
#include <QColor>
#include <QCoreApplication>
#include <QList>
#include <QPoint>
#include <QString>

class QDataStream;

namespace EpHardware {
    namespace EpAmplifier {
        class Amplifier;
    }
}

namespace EpStudy {

class Channel {

public:
    friend QDataStream& operator<<(QDataStream&, const Channel&);
    friend QDataStream& operator>>(QDataStream&, Channel&);

    enum Clip {NoClip, C1, C2};

    Channel(int number = 1);
    ~Channel() {}

    void setNumber(int n) {number_ = n;}
    void setClip(Clip clip) {clip_ = clip;}
    void setColor(const QColor& color) {color_ = color;}
    void setDisplayPages(const QBitArray& pages) {displayPages_ = pages;}
    void setAlwaysSave(bool enable) {alwaysSave_ = enable;}

    int number() const {return number_;}
    Clip clip() const {return clip_;}
    QColor color() const {return color_;}
    QBitArray displayPages() const {return displayPages_;}
    bool alwaysSave() const {return alwaysSave_;}

private:
    int number_;
    Clip clip_;
    QColor color_; 
    QBitArray displayPages_;
    bool alwaysSave_;
};

class ColumnFormat {

public:
    friend QDataStream& operator<<(QDataStream&, const ColumnFormat&);
    friend QDataStream& operator>>(QDataStream&, ColumnFormat&);

    ColumnFormat(const QString& name = QString()) : name_(name) {}

private:
    QString name_;
};

class WindowSettings {

public:
    friend QDataStream& operator<<(QDataStream&, const WindowSettings&);
    friend QDataStream& operator>>(QDataStream&, WindowSettings&);

    WindowSettings(const QString& name = QString()) : name_(name) {}
    ~WindowSettings() {}

private:
    QString name_;

};

class MacroList {

public:
    friend QDataStream& operator<<(QDataStream&, const MacroList&);
    friend QDataStream& operator>>(QDataStream&, MacroList&);

    MacroList(const QString& name = QString()) : name_(name) {}
    ~MacroList() {}

    void setName(const QString& name) {name_ = name;}

    QString name() const {return name_;}

private:
    QString name_;
};



class Protocol {

public:
    friend QDataStream& operator<<(QDataStream&, const Protocol&);
    friend QDataStream& operator>>(QDataStream&, Protocol&);

    Protocol(const QString& name = QString()) : name_(name) {}
    ~Protocol() {}

    void setName(const QString& name) {name_ = name;}

    QString name() const {return name_;}
    
private:
    QString name_;
    Channel senseChannel_;
    ColumnFormat columnFormat_;
    WindowSettings windowSettings_;
    MacroList macroList_;
    bool updateReviewWindow_;
    QPoint focalPoint_;
    int displayPage_;
};

/**
 * A StudyConfiguration contains all the study settings that can
 * be manipulated by the StudyConfigurationDialog.  The StudyConfiguration
 * controls the settings of the Ampifier, and the Recorder display.
 * @author David Mann <mannd@epstudiossoftware.com>
 * Here are the settings:
 *      - Real Time Window
 *          - Channels
 *              - ECG                 12 channels
 *              - Pressure             4 channels
 *              - Catheter Block A    16 channels
 *              - Catherer Block B    16 channels
 *              - etc. up to G        etc.
 *              - Stim                4 stim channels
 *              - Input Channel       4 analog inputs
 *              - Imaging             configuration settings for images
 *              - Analog Out          configuration settings for analog out
 *              - Ablation            configuration settings for ablation
 *       - Measurements               configuration settings for computer
 *                                      assisted measurements
 *       - Protocol List              list of protocols available in the study
 *       - Activation Alignment       configuration for activation alignment
 *       - Mapping                    mapping configuration 
 */
class StudyConfiguration {
    Q_DECLARE_TR_FUNCTIONS(StudyConfiguration)

public:
    enum {MagicNumber = 0x88827393};

    StudyConfiguration(const QString& name = tr("<default>"));
    StudyConfiguration(const StudyConfiguration&);
    ~StudyConfiguration();

    friend QDataStream& operator<<(QDataStream&, const StudyConfiguration&);
    friend QDataStream& operator>>(QDataStream&, StudyConfiguration&);

    StudyConfiguration& operator=(const StudyConfiguration&);

    static const QString configFileName() {return configFileName_;}

    Channel& channel(int n);

    void setName(const QString& name) {name_ = name;}

    QString name() const {return name_;}
    EpHardware::EpAmplifier::Amplifier* amplifier() const {return amplifier_;}

private:
    void copyStudyConfiguration(const StudyConfiguration&);

    static const QString configFileName_;

    QString name_;
    QList<Protocol> protocolList_;
    QList<Channel> channelList_;
    EpHardware::EpAmplifier::Amplifier* amplifier_;
};

typedef QList<StudyConfiguration> StudyConfigList;

StudyConfigList readStudyConfigurations();
void writeStudyConfigurations(StudyConfigList);

class StudyConfigurations {
public:
    StudyConfigurations();

    void add(StudyConfiguration config);
    bool isPresent(const QString& name) const;
    void remove(StudyConfiguration config);
    int size() const {return configList_.size();}
    StudyConfiguration* studyConfiguration(const QString& name);

private:
    typedef QList<StudyConfiguration> StudyConfigurationList;
    StudyConfigurationList configList_;
};

}



#endif
