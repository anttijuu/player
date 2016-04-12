/*Copyright   Feb 21, 2005 Antti Juustila ----------------------

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html
---------------------------------------------------------------*/

/*
RecordingForm.cpp
*/

#include <eikmfne.h> // duration editor
#include <eikedwin.h>
#include <eikenv.h>

#include <moboplayer.rsg>

#include "RecordingForm.h"
#include "Recording.h"
#include "Player.hrh"

/** 
* The factory method for creating the form.
* @param aRecording The recording to edit with the form.
*/
MoboPlayer::CRecordingForm* MoboPlayer::CRecordingForm::NewL(MoboPlayer::TRecording & aRecording)
	{
	CRecordingForm * self = new (ELeave) CRecordingForm(aRecording);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
/** 
* The destructor.
*/
MoboPlayer::CRecordingForm::~CRecordingForm()
	{	
	}

/** 
* Constructor for the form.
* @param aRecording The recording to edit with the form.
*/
MoboPlayer::CRecordingForm::CRecordingForm(MoboPlayer::TRecording & aRecording)
	: iRecording(aRecording)
	{
	}

/** 
* The 2nd phase constructor for leaving initialization.
* Call CAknForm::ConstructL()
*/
void MoboPlayer::CRecordingForm::ConstructL()
	{
	CAknForm::ConstructL();
	}

/** 
* Called by the GUI framework just before the form is shown.
* Here place the values from the recording into the form controls.
*/
void MoboPlayer::CRecordingForm::PreLayoutDynInitL()
	{	
	TBuf<100> buf; // make sure buf is large enough for all text data!

	CCoeControl * control = ControlOrNull(ERecordingNameControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		buf = iRecording.Name();
		edwin->SetTextL(&buf);
		}

	control = ControlOrNull(ERecordingPerformerControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->SetTextL(&iRecording.Performer());
		}

	control = ControlOrNull(ERecordingDurationControl);
	if (control)
		{
		CEikDurationEditor * editor = static_cast<CEikDurationEditor*>(control);
		editor->SetDuration(iRecording.Length());
		}

	control = ControlOrNull(ERecordingGenreControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->SetTextL(&iRecording.Genre());
		}

	control = ControlOrNull(ERecordingPerformedControl);
	if (control)
		{
		CEikDateEditor * editor = static_cast<CEikDateEditor*>(control);
		editor->SetDate(iRecording.Performed());
		}

	control = ControlOrNull(ERecordingSoundFileControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->SetTextL(&iRecording.FileName());
		}

	}

/** 
* Called when the user selects any other than cancel command in the form.
* @param aButtonId The id of the button/command that was selected by the user.
*/
TBool MoboPlayer::CRecordingForm::OkToExitL(TInt /*aButtonId*/)
	{
	TBuf<100> buf; // make sure buf is large enough for all text data!

	CCoeControl * control = ControlOrNull(ERecordingNameControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->GetText(buf);
		if (buf.Length() == 0)
			{
			CEikonEnv::Static()->LeaveWithInfoMsg(R_ENTER_NAME);
			}
		iRecording.SetName(buf);
		}

	control = ControlOrNull(ERecordingPerformerControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->GetText(buf);
		if (buf.Length() == 0)
			{
			CEikonEnv::Static()->LeaveWithInfoMsg(R_ENTER_PERFORMER);
			}
		iRecording.SetPerformer(buf);
		}

	control = ControlOrNull(ERecordingDurationControl);
	if (control)
		{
		CEikDurationEditor * editor = static_cast<CEikDurationEditor*>(control);
		iRecording.SetLength(editor->Duration());
		}

	control = ControlOrNull(ERecordingGenreControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->GetText(buf);
		iRecording.SetGenre(buf);
		}

	control = ControlOrNull(ERecordingPerformedControl);
	if (control)
		{
		CEikDateEditor * editor = static_cast<CEikDateEditor*>(control);
		iRecording.SetPerformed(editor->Date());
		}

	control = ControlOrNull(ERecordingSoundFileControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->GetText(buf);
		iRecording.SetFileName(buf);
		}

	return ETrue;
	}

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: RecordingForm.cpp,v $
* Revision 1.2  2005/04/12 05:49:24  Antti
* Removed unneeded initialization of iRecording
* from the constructor body.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/