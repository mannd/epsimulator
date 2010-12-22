/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.  *
 *   mannd@epstudiossoftware.com * * This program is free software;
 *   you can redistribute it and/or modify * it under the terms of the
 *   GNU General Public License as published by * the Free Software
 *   Foundation; either version 2 of the License, or * (at your
 *   option) any later version.  * * This program is distributed in
 *   the hope that it will be useful, * but WITHOUT ANY WARRANTY;
 *   without even the implied warranty of * MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the * GNU General Public License
 *   for more details.  * * You should have received a copy of the GNU
 *   General Public License * along with this program; if not, write
 *   to the * Free Software Foundation, Inc., * 59 Temple Place -
 *   Suite 330, Boston, MA 02111-1307, USA.  *
 ***************************************************************************/

#ifndef STUDYCONFIGURATION_H
#define STUDYCONFIGURATION_H

#include "columnformat.h"
#include "fileutilities.h"
#include "windowsetting.h"

#include <QtCore/QBitArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtGui/QColor>

class QDataStream;

namespace EpStudy {

class Channel {

public:
    friend QDataStream& operator<<(QDataStream&, const Channel&);
    friend QDataStream& operator>>(QDataStream&, Channel&);

    enum Clip {NoClip, C1, C2};
    enum Scale {OneSixteenth, OneEighth, OneQuarter, OneHalf, One};
    enum ChannelType {NotUsed, ECG, Pressure, Bipolar, 
                      UnipolarWCT, UnipolarAuxRef,
                      AblateRecord, Stim1, Stim2, Stim3, Stim4};
    explicit Channel(int number = 1);
    ~Channel() {}

    // display settings
    int number() const {return number_;}
    QString label() const {return label_;} 
    Clip clip() const {return clip_;}
    QColor color() const {return color_;}
    QBitArray displayPages() const {return displayPages_;}
    bool alwaysSave() const {return alwaysSave_;}

    // hardware settings
    ChannelType channelType() const {return channelType_;}
    int negInput() const {return negInput_;}
    int posInput() const {return posInput_;}
    int gain() const {return gain_;}
    double highPassFilter() const {return highPassFilter_;}
    double lowPassFilter() const {return lowPassFilter_;}
    bool notchFilter() const {return notchFilter_;}

    // display settings
    void setNumber(int n) {number_ = n;}
    void setLabel(const QString& label) {label_ = label;}
    void setClip(Clip clip) {clip_ = clip;}
    void setColor(const QColor& color) {color_ = color;}
    void setDisplayPages(const QBitArray& pages) {displayPages_ = pages;}
    void setAlwaysSave(bool enable) {alwaysSave_ = enable;}

    // hardware settings
    void setChannelType(const ChannelType& type) {channelType_ = type;}
    void setNegInput(int n) {negInput_ = n;}
    void setPosInput(int n) {posInput_ = n;}
    void setGain(int gain) {gain_ = gain;}
    void setHighPassFilter(double freq) {highPassFilter_ = freq;}
    void setLowPassFilter(double freq) {lowPassFilter_ = freq;}
    void setNotchFilter(bool enable) {notchFilter_ = enable;}

private:
    int number_;
    QString label_;
    Clip clip_;
    QColor color_; 
    QBitArray displayPages_;
    bool alwaysSave_;
    Scale scale_;
    ChannelType channelType_;
    int posInput_;
    int negInput_;
    int gain_;
    double highPassFilter_;
    double lowPassFilter_;
    bool notchFilter_;
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
    Q_DECLARE_TR_FUNCTIONS(Protocol)

public:
    friend QDataStream& operator<<(QDataStream&, const Protocol&);
    friend QDataStream& operator>>(QDataStream&, Protocol&);

    enum FocalPoint {Left, OneQuarter, Center, ThreeQuarters, Right};

    Protocol(const QString& name = tr("<default>"),
             const Channel& senseChannel = Channel(),
             const EpCore::ColumnFormat& columnFormat
                =  EpCore::ColumnFormat(),
             const EpRecorder::WindowSetting& windowSetting
                = EpRecorder::WindowSetting(),
             const MacroList& macroList = MacroList(),
             bool updateReviewWindow = true,
             const FocalPoint& focalPoint = OneQuarter,
             int displayPage = 2)
                 : name_(name), senseChannel_(senseChannel),
                 columnFormat_(columnFormat),
                 windowSetting_(windowSetting),
                 macroList_(macroList),
                 updateReviewWindow_(updateReviewWindow),
                 focalPoint_(focalPoint),
                 displayPage_(displayPage) {}
    Protocol(const Protocol&);

    Protocol& operator=(const Protocol&);

    bool operator==(const Protocol& rhs) const {
        return name_ == rhs.name_;
    }

    static unsigned int magicNumber() {return MagicNumber;}
    static QString fileName() {return fileName_;}
    static QList<Protocol> defaultItems();
    static QStringList protocolNames(const QList<Protocol>&);

    void setName(const QString& name) {name_ = name;}
    void setSenseChannel(const Channel& channel) {senseChannel_ = channel;}
    void setColumnFormat(const EpCore::ColumnFormat& columnFormat) {
        columnFormat_ = columnFormat;}
    void setWindowSetting(const EpRecorder::WindowSetting& w) {
        windowSetting_ = w;}
    void setMacroList(const MacroList& macroList) {
        macroList_ = macroList;}
    void setUpdateReviewWindow(bool state) {updateReviewWindow_ = state;}
    void setFocalPoint(const FocalPoint focalPoint) {
        focalPoint_ = focalPoint;}
    void setDisplayPage(int page) {displayPage_ = page;}


    QString name() const {return name_;}
    Channel senseChannel() const {return senseChannel_;}
    EpCore::ColumnFormat columnFormat() const {return columnFormat_;}
    EpRecorder::WindowSetting windowSetting() const {
        return windowSetting_;}
    MacroList macroList() const {return macroList_;}
    bool updateReviewWindow() const {return updateReviewWindow_;}
    FocalPoint focalPoint() const {return focalPoint_;}
    int displayPage() const {return displayPage_;}
    
private:
    void copyProtocol(const Protocol&);

    enum {MagicNumber = 0x12435ffe};

    static const QString fileName_;

    QString name_;
    Channel senseChannel_;
    EpCore::ColumnFormat columnFormat_;
    EpRecorder::WindowSetting windowSetting_;
    MacroList macroList_;
    bool updateReviewWindow_;
    FocalPoint focalPoint_;
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
 * A pointer to the current StudyConfiguration is passed to each
 * Recorder subwindow and to the Amplifier.  The ChannelSettings are
 * used to set up each subwindow and the Channels in the Amplifier.
 * The StudyConfiguration doesn't care how it's used; it is just a 
 * receptical for configuration data.  
 *
 * The StudyConfiguration does need to know the number of amplifier 
 * channels in use, so as not to display unused channels.  It is
 * passed the number of channels in its constructor.  If the user
 * changes the amplifier, study channels are either truncated or
 * displayed as unused channels, depending on whether the number of 
 * channels increases or decreases.
 */                             
class StudyConfiguration {
    Q_DECLARE_TR_FUNCTIONS(StudyConfiguration)

public:

    StudyConfiguration(const QString& name = tr("<default>"),
		       int numChannels = 32);
    StudyConfiguration(const StudyConfiguration&);
    ~StudyConfiguration();

    friend QDataStream& operator<<(QDataStream&, const StudyConfiguration&);
    friend QDataStream& operator>>(QDataStream&, StudyConfiguration&);

    StudyConfiguration& operator=(const StudyConfiguration&);

    static unsigned int magicNumber() {return MagicNumber;}
    static QString fileName() {return configFileName_;}

    int numChannels() const {return numChannels_;}

    Channel& channel(int n);
    QList<Protocol> protocolList() const {return protocolList_;}
    QList<Protocol> unselectedProtocols() const;
    int currentProtocolIndex() const {return currentProtocolIndex_;}
    Protocol currentProtocol() const {return protocolList_[currentProtocolIndex_];}

    void setName(const QString& name) {name_ = name;}
    void setCurrentProtocolIndex(int index) {currentProtocolIndex_ = index;}
    void setProtocolList(const QList<Protocol>& list) {protocolList_ = list;}
    

    QString name() const {return name_;}

private:
    enum {MagicNumber = 0x88827393};

    void copyStudyConfiguration(const StudyConfiguration&);

    static const QString configFileName_;

    QString name_;
    int numChannels_;
    QList<Protocol> protocolList_;  // == selectedProtocols_

    QList<Channel> channelList_;
    int currentProtocolIndex_;

    // Measurements | Measurements tab
    int surfaceLeadChannel1Index_;
    int surfaceLeadChannel2Index_;
    int surfaceLeadChannel3Index_;
    int atrialChannelIndex_;
    int hisBundleChannelIndex_;
    int ventricularChannelIndex_;
    // int markPosition_;
    int triggerChannelIndex_;
    int minIntervalBetweenStimTrains_;
    // Measurements | AutoSave Setting tab
    int hrChannel1HighThreshold_;
    int hrChannel1LowThreshold_;
    int hrChannel2HighThreshold_;
    int hrChannel2LowThreshold_;
    bool saveUsingHrChannel1_;
    bool saveUsingHrChannel2_;
    bool saveForStimDetection_;
    // Measurements | Vital Configuration
    int monitoringBPIndex_;
    int vitalLogBPIndex_;
    int hrChannel1Index_;
    int hrChannel2Index_;
    bool useAutoNbp_;
    int nbpTimeInterval_;
    int cuffSize_;
    bool useAutoVitalLog_;
    int vitalLogTimeInterval_;

};

} // namespace EpStudy



#endif
