CREATE TABLE ArrhythmiaTolerances (ArrhythmiaToleranceID INTEGER PRIMARY KEY, 
       Name TEXT);
CREATE TABLE ArrhythmiaTypes (ArrhythmiaTypeID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE BlockDescriptions (BlockDescriptionID INTEGER PRIMARY KEY,
       Name TEXT);
CREATE TABLE Intervals (IntervalID INTEGER PRIMARY KEY, Name TEXT, 
       Mark1 INTEGER NOT NULL REFERENCES Marks,
       Mark2 INTEGER NOT NULL REFERENCES Marks,
       Width NUMERIC);
CREATE TABLE PacingSites (PacingSiteID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE Phases (PhaseID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE RefractoryLocations (RefractoryLocationID INTEGER PRIMARY KEY, 
       Name TEXT);
CREATE TABLE Marks (MarkID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE ColumnFormats (ColumnFormatID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE ColumnFormatInterval (ColumnFormatID INTEGER NOT NULL
       REFERENCES ColumnFormats, IntervalID INTEGER NOT NULL
       REFERENCES Intervals, SortOrder INTEGER NOT NULL,
       PRIMARY KEY (ColumnFormatID, IntervalID));
CREATE TABLE ChannelLabels (ChannelLabelID INTEGER PRIMARY KEY, Name TEXT,
       MeasurementTypeID INTEGER NOT NULL DEFAULT 1
        REFERENCES MeasurementTypes);
CREATE TABLE MeasurementTypes (MeasurementTypeID INTEGER PRIMARY KEY,
       Name TEXT);
CREATE TABLE WindowSettings (WindowSettingID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE MacroCategories (MacroCategoryID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE OnOff (OnOffID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE FocalPoints (FocalPointID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE DisplayPages (DisplayPageID INTEGER PRIMARY KEY, Name TEXT);
CREATE TABLE Protocols (ProtocolID INTEGER PRIMARY KEY, Name TEXT,
       SenseChannelLabelID INTEGER NOT NULL REFERENCES ChannelLabels,
       ColumnFormatID INTEGER NOT NULL REFERENCES ColumnFormats,
       WindowSettingID INTEGER NOT NULL REFERENCES WindowSettings,
       MacroCategoryID INTEGER NOT NULL REFERENCES MacroCategories,
       UpdateReviewWindowID INTEGER NOT NULL REFERENCES OnOff,
       FocalPointID INTEGER NOT NULL REFERENCES FocalPoints,
       DisplayPageID INTEGER NOT NULL REFERENCES DisplayPages);
CREATE TABLE Channels (ChannelID INTEGER PRIMARY KEY, ChannelNumber INTEGER,
       Label TEXT, Scale INTEGER, Clip INTEGER, Color INTEGER, DisplayPages INTEGER,
       AlwaysSave INTEGER, Type INTEGER, PosInput INTEGER, NegInput INTEGER,
       Gain INTEGER, HighPass NUMERIC, LowPass NUMERIC, Notch INTEGER);
CREATE TABLE StudyConfigurations (StudyConfigurationID INTEGER PRIMARY KEY,
       Name TEXT, NumChannels INTEGER, CurrentProtocolIndex INTEGER,
       SurfaceLeadChannel1Index INTEGER, SurfaceLeadChannel2Index INTEGER,
       SurfaceLeadChannel3Index INTEGER, AtrialChannelIndex INTEGER,
       HisBundleChannelIndex INTEGER, TriggerChannelIndex INTEGER,
       MinIntervalBetweenStimTrains INTEGER, HrChannel1HighThreshold INTEGER,
       HrChannel1LowThreshold INTEGER, HrChannel2HighThreshold INTEGER,
       HrChannel2LowThreshold INTEGER, SaveUsingHrChannel1 INTEGER,
       SaveUsingHrChannel2 INTEGER, SaveForStimDetection INTEGER,
       MonitoringBPIndex INTEGER, VitalLogBPIndex INTEGER, HrChannel1Index INTEGER,
       HrChannel2Index INTEGER, UseAutoNbp INTEGER, NbpTimeInterval INTEGER,
       CuffSize INTEGER, UseAutoVitalLog INTEGER, VitalLogTimeInterval INTEGER);
CREATE TABLE StudyConfigurationProtocol (StudyConfigurationID INTEGER NOT NULL
       REFERENCES StudyConfigurations, ProtocolID INTEGER NOT NULL
       REFERENCES Protocols, SortOrder INTEGER NOT NULL,
       PRIMARY KEY (StudyConfigurationID, ProtocolID));
CREATE TABLE StudyConfigurationChannel (StudyConfigurationID INTEGER NOT NULL
       REFERENCES StudyConfigurations, ChannelID INTEGER NOT NULL
       REFERENCES Channels, PRIMARY KEY (StudyConfigurationID, ChannelID));

-- Translation specific data is below

BEGIN TRANSACTION;
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("SOB");
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("Diaphoretic");
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("Conscious");
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("Unconscious");
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("Drop in BP");
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("Stable");
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("Unstable");
INSERT INTO ArrhythmiaTolerances (Name) VALUES ("Chest Pain");

INSERT INTO ArrhythmiaTypes (Name) VALUES ("SVT");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Sinus Node Reentry");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Inappropriate Sinus Tachycardia");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Ectopic Reentrant Tachycardia");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Multifocal Atrial Tachycardia");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Type 1 Atrial Flutter, CW");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Type 1 Atrial Flutter, CCW");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Type 2 Atrial Flutter");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Chronic Atrial Fibrillation");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Paroxysmal Atrial Fibrillation");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Premature Atrial Complexes");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Premature Ventricular Complexes");
INSERT INTO ArrhythmiaTypes (Name) VALUES 
       ("Nonsustained Ventricular Tachycardia");
INSERT INTO ArrhythmiaTypes (Name) VALUES 
       ("Sustained Monomorphic Ventricular Tachycardia");
INSERT INTO ArrhythmiaTypes (Name) VALUES 
       ("Sustained Polymorphic Ventricular Tachycardia");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Ventricular Fibrillation");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("2nd Degree AV Block Mobitz I");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("2nd Degree AV Block Mobitz II");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("2nd Degree AV Block 2:1");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("2nd Degree AV Block High Grade");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("3rd Degree AV Block");
INSERT INTO ArrhythmiaTypes (Name) VALUES ("Other");

INSERT INTO BlockDescriptions (Name) VALUES ("AV Nodal Block");
INSERT INTO BlockDescriptions (Name) VALUES ("1st Degree AV Block");
INSERT INTO BlockDescriptions (Name) VALUES ("Wenckebach");
INSERT INTO BlockDescriptions (Name) VALUES ("AV Mobitz II");
INSERT INTO BlockDescriptions (Name) VALUES ("2:1 AV Block");
INSERT INTO BlockDescriptions (Name) VALUES ("AV Dissociation");
INSERT INTO BlockDescriptions (Name) VALUES ("Complete Heart Block");
INSERT INTO BlockDescriptions (Name) VALUES ("Antegrade Wenckebach");
INSERT INTO BlockDescriptions (Name) VALUES ("VA Block");
INSERT INTO BlockDescriptions (Name) VALUES ("Retrograde");

INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (1, "None", 1, 1, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (2, "A1A1", 15, 15, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (3, "A1A2", 15, 16, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (4, "A2A3", 16, 17, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (5, "A3A4", 17, 18, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (6, "A1H1", 15, 20, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (7, "A2H2", 16, 21, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (8, "AA", 7, 7, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (9, "SCL", 7, 7, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (10, "PR", 2, 3, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (11, "QRS", 3, 4, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (12, "QT", 3, 6, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (13, "PA", 1, 7, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (14, "AH", 7, 8, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (15, "HV", 8, 3, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (16, "H1H1", 20, 20, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (17, "H1H2", 20, 21, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (18, "V1V1", 23, 24, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (19, "V1V2", 23, 24, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (20, "V2V3", 24, 25, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (21, "V3V4", 25, 26, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (22, "S1", 11, 11, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (23, "S2", 11, 12, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (24, "S3", 12, 13, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (25, "S4", 13, 14, 0);
INSERT INTO Intervals (IntervalID, Name, Mark1, Mark2, Width)
       VALUES (26, "S5", 14, 15, 0);

INSERT INTO PacingSites (Name) VALUES ("None");
INSERT INTO PacingSites (Name) VALUES ("HRA");
INSERT INTO PacingSites (Name) VALUES ("CS");
INSERT INTO PacingSites (Name) VALUES ("RVa");
INSERT INTO PacingSites (Name) VALUES ("RVot");
INSERT INTO PacingSites (Name) VALUES ("ABL");
INSERT INTO PacingSites (Name) VALUES ("HIS");

INSERT INTO Phases (Name) VALUES ("BASELINE");
INSERT INTO Phases (Name) VALUES ("Post-Drug");
INSERT INTO Phases (Name) VALUES ("Post-Ablation");
INSERT INTO Phases (Name) VALUES ("Isoproterenol");

INSERT INTO RefractoryLocations (Name) VALUES ("Atrial");
INSERT INTO RefractoryLocations (Name) VALUES ("AV Nodal");
INSERT INTO RefractoryLocations (Name) VALUES ("Retrograde");
INSERT INTO RefractoryLocations (Name) VALUES ("Ventricular");

INSERT INTO Marks (MarkID, Name) VALUES (1, "None");
INSERT INTO Marks (MarkID, Name) VALUES (2, "P");
INSERT INTO Marks (MarkID, Name) VALUES (3, "QRS onset");
INSERT INTO Marks (MarkID, Name) VALUES (4, "QRS offset");
INSERT INTO Marks (MarkID, Name) VALUES (5, "T onset");
INSERT INTO Marks (MarkID, Name) VALUES (6, "T offset");
INSERT INTO Marks (MarkID, Name) VALUES (7, "A");
INSERT INTO Marks (MarkID, Name) VALUES (8, "H");
INSERT INTO Marks (MarkID, Name) VALUES (9, "H offset");
INSERT INTO Marks (MarkID, Name) VALUES (10, "V");
INSERT INTO Marks (MarkID, Name) VALUES (11, "S1");
INSERT INTO Marks (MarkID, Name) VALUES (12, "S2");
INSERT INTO Marks (MarkID, Name) VALUES (13, "S3");
INSERT INTO Marks (MarkID, Name) VALUES (14, "S5");
INSERT INTO Marks (MarkID, Name) VALUES (15, "A1");
INSERT INTO Marks (MarkID, Name) VALUES (16, "A2");
INSERT INTO Marks (MarkID, Name) VALUES (17, "A3");
INSERT INTO Marks (MarkID, Name) VALUES (18, "A4");
INSERT INTO Marks (MarkID, Name) VALUES (19, "A5");
INSERT INTO Marks (MarkID, Name) VALUES (20, "H1");
INSERT INTO Marks (MarkID, Name) VALUES (21, "H2");
INSERT INTO Marks (MarkID, Name) VALUES (22, "H3");
INSERT INTO Marks (MarkID, Name) VALUES (23, "V1");
INSERT INTO Marks (MarkID, Name) VALUES (24, "V2");
INSERT INTO Marks (MarkID, Name) VALUES (25, "V3");
INSERT INTO Marks (MarkID, Name) VALUES (26, "V4");
INSERT INTO Marks (MarkID, Name) VALUES (27, "V5");

INSERT INTO ColumnFormats (ColumnFormatID, Name) VALUES (1, "BASELINE");
INSERT INTO ColumnFormats (ColumnFormatID, Name) VALUES (2, "A PACE");
INSERT INTO ColumnFormats (ColumnFormatID, Name) VALUES (3, "AEST");
INSERT INTO ColumnFormats (ColumnFormatID, Name) VALUES (4, "V PACE");
INSERT INTO ColumnFormats (ColumnFormatID, Name) VALUES (5, "VEST");

INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (1, 9, 0);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (1, 10, 1);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (1, 11, 2);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (1, 12, 3);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (1, 14, 4);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (1, 15, 5);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 22, 0);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 23, 1);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 24, 2);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 25, 3);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 26, 4);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 9, 5);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 2, 6);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 3, 7);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 4, 8);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 16, 9);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 17, 10);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 18, 11);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 19, 12);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 20, 13);
INSERT INTO ColumnFormatInterval (ColumnFormatID, IntervalID, SortOrder) VALUES (2, 21, 14);

INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (0, "P1", 0);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (1, "P2", 0);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (2, "P3", 0);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (3, "P4", 0);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES 
       (4, "HRA", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (5, "HRA-d", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (6, "RVA", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (7, "RVOT", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (8, "His-p", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (9, "His-m", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (10, "His-d", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (11, "CS 9,10", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (12, "CS 7,8", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (13, "CS 5,6", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (14, "CS 3,4", 1);
INSERT INTO ChannelLabels (ChannelLabelID, Name, MeasurementTypeID) VALUES
       (15, "CS 1,2", 1);

-- insert more labels here

INSERT INTO MeasurementTypes (MeasurementTypeID, Name) VALUES
       (0, "Pressure");
INSERT INTO MeasurementTypes (MeasurementTypeID, Name) VALUES
       (1, "Intracardiac");

INSERT INTO WindowSettings (WindowSettingID, Name) VALUES (1, "BASELINE");
-- insert more window settings here

INSERT INTO MacroCategories (MacroCategoryID, Name) VALUES (1, "BASELINE");
-- insert more macro categories here

INSERT INTO OnOff (OnOffID, Name) VALUES (0, "Off");
INSERT INTO OnOff (OnOffID, Name) VALUES (1, "On");

INSERT INTO FocalPoints (FocalPointID, Name) VALUES (0, "Left");
INSERT INTO FocalPoints (FocalPointID, Name) VALUES (1, "1/4");
INSERT INTO FocalPoints (FocalPointID, Name) VALUES (2, "Center");
INSERT INTO FocalPoints (FocalPointID, Name) VALUES (3, "3/4");
INSERT INTO FocalPoints (FocalPointID, Name) VALUES (4, "Right");

INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (1, "Page 1");
INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (2, "Page 2");
INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (3, "Page 3");
INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (4, "Page 4");
INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (5, "Page 5");
INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (6, "Page 6");
INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (7, "Page 7");
INSERT INTO DisplayPages (DisplayPageID, Name) VALUES (8, "Page 8");


INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (1, "BASELINE", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (2, "A PACE", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (3, "V PACE", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (4, "IMPLANT/TILT", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (5, "ABLATION", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (6, "RVOT", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (7, "AEST", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (8, "RAP", 1, 1, 1, 1, 0, 1, 1);
INSERT INTO Protocols (ProtocolID, Name, SenseChannelLabelID, ColumnFormatID,
       WindowSettingID, MacroCategoryID, UpdateReviewWindowID, FocalPointID, DisplayPageID) 
       VALUES (10, "VEST", 1, 1, 1, 1, 0, 1, 1);

INSERT INTO StudyConfigurations (StudyConfigurationID, Name, NumChannels, 
       CurrentProtocolIndex, SurfaceLeadChannel1Index, SurfaceLeadChannel2Index,
       SurfaceLeadChannel3Index, AtrialChannelIndex,
       HisBundleChannelIndex, TriggerChannelIndex,
       MinIntervalBetweenStimTrains, HrChannel1HighThreshold,
       HrChannel1LowThreshold, HrChannel2HighThreshold,
       HrChannel2LowThreshold, SaveUsingHrChannel1,
       SaveUsingHrChannel2, SaveForStimDetection,
       MonitoringBPIndex, VitalLogBPIndex, HrChannel1Index,
       HrChannel2Index, UseAutoNbp, NbpTimeInterval,
       CuffSize, UseAutoVitalLog, VitalLogTimeInterval) VALUES (0, "BASELINE",
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO StudyConfigurationProtocol (StudyConfigurationID, ProtocolID, SortOrder)
       VALUES (0, 1, 1);
INSERT INTO StudyConfigurationChannel (StudyConfigurationID, ChannelID)
       VALUES (0, 0);
INSERT INTO Channels (ChannelID, ChannelNumber, Label, Scale, Clip, Color,
       DisplayPages, AlwaysSave, Type, PosInput, NegInput, Gain, HighPass, 
       LowPass, Notch) VALUES (0, 1, "I", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

COMMIT TRANSACTION;
