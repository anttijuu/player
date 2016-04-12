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

#include <eiktxlbx.h>
#include <eiktxlbm.h>
#include <eikenv.h>
#include <eikappui.h>

#include <MoboPlayer.rsg>

#include "PlayerRecordingsContainer.h"
#include "PlayerRecordingInfo.h"
#include "PlayerDocument.h"
#include "Player.h"

/** 
* Factory method for creating new recordingcontainers.
* @param aRect The area on the screen the control occupies.
* @returns The new recordings container control.
*/
MoboPlayer::CPlayerRecordingsContainer* MoboPlayer::CPlayerRecordingsContainer::NewL(const TRect& aRect)
	{
	CPlayerRecordingsContainer* self = CPlayerRecordingsContainer::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
	}

/** 
* Factory method for creating new recordingcontainers.
* Puts the control container to the cleanup stack and returns.
* @param aRect The area on the screen the control occupies.
* @returns The new recordings container control.
*/
MoboPlayer::CPlayerRecordingsContainer* MoboPlayer::CPlayerRecordingsContainer::NewLC(const TRect& aRect)
	{
	CPlayerRecordingsContainer* self = new (ELeave) CPlayerRecordingsContainer;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

/** 
* Destructs the container and child controls with it.
* Also, stops the player, when this container is destroyed.
* This happens when the recording view is switched from to another
* view (either in this app or in another). This effectively stops
* playing if another app activates during playing (e.g. the Phone app).
*/
MoboPlayer::CPlayerRecordingsContainer::~CPlayerRecordingsContainer()
	{
	if (iPlayer)
		{
		iPlayer->Stop();
		}
	delete iTextListBox;
	delete iInfoControl;
	}
	
/** 
* The 2nd phase constructor, allocates the listbox and info controls.
* These are the controls contained in this compound control.
* @param aRect The area occupied by this control.
*/
void MoboPlayer::CPlayerRecordingsContainer::ConstructL(const TRect& aRect)
	{
	CreateWindowL();
	SetRect(aRect);
	SetBlank();
	
	const TInt KInfoRectHeight = 35;
	iTextListBox = new(ELeave) CEikTextListBox;
	iTextListBox->ConstructL(this, CEikListBox::ENoExtendedSelection|CEikListBox::ENoFirstLetterMatching);
	iTextListBox->SetContainerWindowL(*this);
	iTextListBox->SetPosition(TPoint(0,0));
	TRect rect(Rect());
	rect.SetHeight(rect.Height()-KInfoRectHeight);
	iTextListBox->SetRect(rect);
	iTextListBox->CreateScrollBarFrameL();
	iTextListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto, CEikScrollBarFrame::EAuto);
	iTextListBox->SetListBoxObserver(this);
	iTextListBox->SetObserver(this); // required to get the selection change events too.

	CPlayerDocument * doc = static_cast<CPlayerDocument*>(CEikonEnv::Static()->EikAppUi()->Document());
	iPlayer = doc->Player();
	iPlayer->SetObserver(this);

	// Get the list box model.	
	CTextListBoxModel* model = iTextListBox->Model();
	// Say that the model should get it's data from the current playlist.
	model->SetItemTextArray(iPlayer->CurrentPlayList());
	// Model does not own the items, but the player.
	model->SetOwnershipType(ELbmDoesNotOwnItemArray);
	iTextListBox->HandleItemAdditionL();
	iTextListBox->SetFocus(ETrue, EDrawNow);

	rect.iTl.iY = rect.iBr.iY;
	rect.SetHeight(KInfoRectHeight);
	iInfoControl = CPlayerRecordingInfo::NewL(rect, iPlayer);
	iInfoControl->SetContainerWindowL(*this);

	// Make view and components invisible until ViewActivated has been called
	SetComponentsToInheritVisibility();
	MakeVisible(ETrue);
	ActivateL();
}

/** 
* Called by the view when an executed command requires 
* that the list contained here needs to be updated because of
* a new added item.
*/
void MoboPlayer::CPlayerRecordingsContainer::HandleListboxAdditionL()
	{
	iTextListBox->HandleItemAdditionL();
	}

/** 
* Called by the view when an executed command requires 
* that the list contained here needs to be updated because of
* a removed item.
*/
void MoboPlayer::CPlayerRecordingsContainer::HandleListboxRemovalL()
	{
	iTextListBox->HandleItemRemovalL();
	}


/**
* Required for compound controls, that contain other controls.
* @returns The number of contained controls.
*/
TInt MoboPlayer::CPlayerRecordingsContainer::CountComponentControls() const
	{
	return 2;
	}

/**
* Required for compound controls, that contain other controls.
* @param aIndex The index of the contained control.
* @returns A pointer to the contained control.
*/
CCoeControl* MoboPlayer::CPlayerRecordingsContainer::ComponentControl(TInt aIndex) const
	{
	switch (aIndex)
		{
		case 0: 
			{
			return iTextListBox;
			}
		case 1:
			{
			return iInfoControl;
			}
		default:
			{
			break;
			}
		}
	return 0;
	}

/** 
* Implements the MEikListBoxObserver interface. This is called when
* the user manipulates the listbox (selects an item in the list).
* @param aListBox The listbox object that generated the event.
* @param aEventType The type of the listbox event.
*/
void MoboPlayer::CPlayerRecordingsContainer::HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType)
	{
	if((aListBox == iTextListBox) && (aEventType == EEventItemClicked || aEventType == EEventEnterKeyPressed))
		{
		// TODO: Get current selected row's text.
		TInt selected = iTextListBox->CurrentItemIndex();
		if (selected >= 0)
			{
			iPlayer->PlayL(selected);
			}
		}
	}

/** 
* Implements the MCoeControlObserver interface. This is needed to catch the
* events as user is scrolling the list; we want to see what is the active, selected
* row in the list. Then we tell the player to update the selected recording too.
* This has to be done using the MCoeControlObserver interface, since MEikListBoxObserver
* interface does not let us know of the selection changed events.
* @param aControl The control that sends the event.
* @param aEventType The type of the event to handle.
*/
void MoboPlayer::CPlayerRecordingsContainer::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
	{
	if (aControl == iTextListBox && aEventType == EEventStateChanged)	
		{
		// Set active playlist in iPlayer according to the selection index.
		iPlayer->CurrentPlayList()->SetSelectedRecording(iTextListBox->CurrentItemIndex());
		iInfoControl->DrawDeferred();
		}
	}

/**
* Implements the MPlayerObserver interface.
* @see MPlayerObserver::HandlePlayerError
* @param aError The error code, one of system error codes. 
*/
void MoboPlayer::CPlayerRecordingsContainer::HandlePlayerError(TInt aError)
	{
	iTextListBox->SetCurrentItemIndexAndDraw(0);
	iInfoControl->PlayingStopped();
	CEikonEnv::Static()->InfoMsg(R_PLAYING_ENGINE_ERROR, aError);
	}
	
/** 
* Implements the MPlayerObserver interface.
* @see MPlayerObserver::PlayingStarted
* @param aIndexOfRecording The index of the recording in list, that is now played.
* @param aTotalDuration Duration of the recording in seconds.
*/
void MoboPlayer::CPlayerRecordingsContainer::PlayingStarted(TInt aIndexOfRecording, TInt aTotalDuration)
	{
	iTextListBox->SetCurrentItemIndexAndDraw(aIndexOfRecording);
	iInfoControl->PlayingStarted(aTotalDuration);
	}
	
/** 
* Implements the MPlayerObserver interface.
* @see MPlayerObserver::PlayingEnded
*/
void MoboPlayer::CPlayerRecordingsContainer::PlayingEnded()
	{
	iTextListBox->SetCurrentItemIndexAndDraw(0);
	iInfoControl->PlayingStopped();
	}

/** 
* Called by the framework when user presses keys on the phone. Control has to
* be on the control stack to get these key events.
* @param aKeyEvent Information about the key event.
* @param aType The type of the key event.
* @returns Was the key event handled or not.
* @see CCoeControl::OfferKeyEventL
*/
TKeyResponse MoboPlayer::CPlayerRecordingsContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	if (iPlayer->IsPlaying())
		{
		if (aType == EEventKey)
			{
			switch (aKeyEvent.iCode)
				{
				case EKeyUpArrow:
					iPlayer->VolumeUp();
					return EKeyWasConsumed;
				case EKeyDownArrow:
					iPlayer->VolumeDown();
					return EKeyWasConsumed;
				}
			}
		}
	return iTextListBox->OfferKeyEventL(aKeyEvent, aType);
	}

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerRecordingsContainer.cpp,v $
* Revision 1.3  2005/06/09 08:58:13  Antti
* Updated comments.
*
* Revision 1.2  2005/03/02 08:14:29  Antti
* Updated documentation.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/