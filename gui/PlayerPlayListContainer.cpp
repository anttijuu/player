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

#include "PlayerPlayListContainer.h"
#include "PlayerPlayListInfo.h"
#include "PlayerDocument.h"
#include "Player.h"

#include "Player.hrh"

/** 
* Factory method for creating new playlist containers.
* @param aRect The area on the screen the control occupies.
* @returns The new playlist container control.
*/
MoboPlayer::CPlayerPlayListContainer* MoboPlayer::CPlayerPlayListContainer::NewL(const TRect& aRect)
	{
	CPlayerPlayListContainer* self = CPlayerPlayListContainer::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
	}

/**
* Factory method for creating new playlist containers.
* Puts the control container to the cleanup stack and returns.
* @param aRect The area on the screen the control occupies..
* @returns The new playlist container control.
*/
MoboPlayer::CPlayerPlayListContainer* MoboPlayer::CPlayerPlayListContainer::NewLC(const TRect& aRect)
	{
	CPlayerPlayListContainer* self = new (ELeave) CPlayerPlayListContainer;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self; 
	}

/** 
* The 2nd phase constructor, allocates the listbox and info controls.
* @param aRect The area occupied by this control.
*/
void MoboPlayer::CPlayerPlayListContainer::ConstructL(const TRect& aRect)
	{
	CreateWindowL();		// Create a window for the control
	SetRect(aRect);		// Set it's area
	SetBlank();				// This control need not draw
	
	const TInt KInfoRectHeight = 35;
	iTextListBox = new(ELeave) CEikTextListBox;
	iTextListBox->ConstructL(this, CEikListBox::ENoExtendedSelection|CEikListBox::ENoFirstLetterMatching);
	// Listbox is contained in this composite control.
	iTextListBox->SetContainerWindowL(*this);
	iTextListBox->SetPosition(TPoint(0,0));
	TRect rect(Rect());
	rect.SetHeight(rect.Height()-KInfoRectHeight);
	iTextListBox->SetRect(rect);
	// List has scroll bars.
	iTextListBox->CreateScrollBarFrameL();
	iTextListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto, CEikScrollBarFrame::EAuto);
	// This control observers the listbox events.
	iTextListBox->SetListBoxObserver(this);
	// This control observers the control events from listbox too.
	iTextListBox->SetObserver(this); // required to get the selection change events too.
	
	// Get a pointer to the model (engine; the player).
	CPlayerDocument * doc = static_cast<CPlayerDocument*>(CEikonEnv::Static()->EikAppUi()->Document());
	iPlayer = doc->Player();
		
	// Set the data source to the list box model
	CTextListBoxModel* model = iTextListBox->Model();
	// Data source is the player, which implements the MDesCArray interface.
	model->SetItemTextArray(iPlayer);
	// List box model does not own the data source; it deletes itself.
	model->SetOwnershipType(ELbmDoesNotOwnItemArray);
	// Show the items now.
	iTextListBox->HandleItemAdditionL();
	// Set keyboard focus to the listbox.
	iTextListBox->SetFocus(ETrue, EDrawNow);

	// Create the info control.
	rect.iTl.iY = rect.iBr.iY;
	rect.SetHeight(KInfoRectHeight);
	iInfoControl = CPlayerPlayListInfo::NewL(rect, iPlayer);
	// Info control is a component control of this compound control too.
	iInfoControl->SetContainerWindowL(*this);
	
	// Make view and components invisible until ViewActivated has been called
	SetComponentsToInheritVisibility();
	MakeVisible(ETrue);
	
	// Activate the control and all composite controls too.
	ActivateL();
	}

MoboPlayer::CPlayerPlayListContainer::~CPlayerPlayListContainer()
	{
	delete iTextListBox;
	delete iInfoControl;
	}

/**	
* Required for compound controls, that contain other controls.
* @returns The number of contained controls.
*/
TInt MoboPlayer::CPlayerPlayListContainer::CountComponentControls() const
	{
	return 2;
	}

/**
* Required for compound controls, that contain other controls.
* @param aIndex The index of the contained control.
* @returns A pointer to the contained control.
*/
CCoeControl* MoboPlayer::CPlayerPlayListContainer::ComponentControl(TInt aIndex) const
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
void MoboPlayer::CPlayerPlayListContainer::HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType)
	{
	if((aListBox == iTextListBox))
		{
		if (aEventType == EEventItemClicked || aEventType == EEventEnterKeyPressed)
			{
			// TODO: Get current selected row's text.
			TInt selected = iTextListBox->CurrentItemIndex();
			if (selected >= 0)
				{
				// Select the correspondign playlist as the current playlist.
				iPlayer->SelectPlayListL(selected);
				// Switch to recordings view.
				CEikonEnv::Static()->EikAppUi()->HandleCommandL(EPlayerSwitchToRecordingsView);
				}
			}
		}
	}

/** 
* Implements the MCoeControlObserver interface. This is needed to catch the
* events as user is scrolling the list; we want to see what is the active, selected
* row in the list. Then we tell the player to update the selected playlist too.
* This has to be done using the MCoeControlObserver interface, since MEikListBoxObserver
* interface does not let us know of the selection changed events.
* @param aControl The control that sends the event.
* @param aEventType The type of the event to handle.
*/
void MoboPlayer::CPlayerPlayListContainer::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
	{
	// User scrolls the list, select the corresponding playlist.
	if (aControl == iTextListBox && aEventType == EEventStateChanged)	
		{
		// Set active playlist in iPlayer according to the selection index.
		iPlayer->SelectPlayListL(iTextListBox->CurrentItemIndex());
		iInfoControl->DrawDeferred();
		}
	}

/** 
* Called by the framework when user presses keys on the phone. Control has to
* be on the control stack to get these key events.
* @param aKeyEvent Information about the key event.
* @param aType The type of the key event.
* @returns Was the key event handled or not.
* @see CCoeControl::OfferKeyEventL
*/
TKeyResponse MoboPlayer::CPlayerPlayListContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	// Offer the key events to the listbox.
	return iTextListBox->OfferKeyEventL(aKeyEvent, aType);
	}

/** 
* Called by the view when an executed command requires 
* that the list contained here needs to be updated.
*/
void MoboPlayer::CPlayerPlayListContainer::HandleListboxAdditionL()
	{
	iTextListBox->HandleItemAdditionL();
	}

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerPlayListContainer.cpp,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/