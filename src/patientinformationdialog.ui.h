/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

///TODO This doesn't work right, ? why?  change to subclass and rename
// PatientInformationDialog to PatientInformationDialogBase

void PatientInformationDialog::Init()
{
    studyDateEdit->setDate(QDate::currentDate());
}
