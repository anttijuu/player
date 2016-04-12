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
PlayListForm.cpp
*/

#include <eikedwin.h>
#include <eikenv.h>

#include <moboplayer.rsg>

#include "PlayListForm.h"
#include "PlayList.h"
#include "Player.hrh"

/** 
* The factory method for creating a new form. 
* @param aPlayList The playlist to edit with the form.
* @returns The new form.
* @see CRecordingForm for implementation hints.
*/
MoboPlayer::CPlayListForm* MoboPlayer::CPlayListForm::NewL(CPlayList & aPlayList)
	{
	CPlayListForm * self = new (ELeave) CPlayListForm(aPlayList);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
/** 
* Destructor for deinitializing the form.
*/
MoboPlayer::CPlayListForm::~CPlayListForm()
	{	
	}

/** 
* Default constructor.
* @param aPlayList The playlist to edit with the form.
*/
MoboPlayer::CPlayListForm::CPlayListForm(CPlayList & aPlayList)
	: iPlayList(aPlayList)
	{
	}

/** 
* The 2nd phase constructor for initializing leaving items in the form.
* Call CAknForm::ConstructL().
*/
void MoboPlayer::CPlayListForm::ConstructL()
	{
	CAknForm::ConstructL();
	}

/** 
* Called by the framework just before showing the form.
* Place data from the playlist to the dialog controls here.
*/
void MoboPlayer::CPlayListForm::PreLayoutDynInitL()
	{	
	CCoeControl * control = ControlOrNull(EPlayListNameControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->SetTextL(&iPlayList.Name());
		}

	control = ControlOrNull(EPlayListDescriptionControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->SetTextL(&iPlayList.Description());
		}
	}

/** 
* Called by the framework when user selects a button/command when the dialog
* is active. Not called when the cancel command is selected.
* Since OK and Cancel are all the possible buttons, and cancel is not
* called when it is selected, we can ignore the parameter, since it
* can only be the command id of the OK command, when this method is called.
*/
TBool MoboPlayer::CPlayListForm::OkToExitL(TInt /*aButtonId*/)
	{
	TBuf<200> buf; // make sure buf is large enough for all text data!

	CCoeControl * control = ControlOrNull(EPlayListNameControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->GetText(buf);
		if (buf.Length() == 0)
			{
			CEikonEnv::Static()->LeaveWithInfoMsg(R_ENTER_NAME);
			}
		iPlayList.SetName(buf);
		}

	control = ControlOrNull(EPlayListDescriptionControl);
	if (control)
		{
		CEikEdwin * edwin = static_cast<CEikEdwin*>(control);
		edwin->GetText(buf);
		iPlayList.SetDescription(buf);
		}
	return ETrue;
	}

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayListForm.cpp,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/