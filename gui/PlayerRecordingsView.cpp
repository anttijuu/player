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

#include <aknviewappui.h>
#include <avkon.hrh>
#include <aknconsts.h>
#include <aknnotewrappers.h>
#include <eikmenup.h>

#include <MoboPlayer.rsg>

#include "PlayerDocument.h"
#include "PlayerRecordingsView.h"
#include "PlayerRecordingsContainer.h"
#include "RecordingForm.h"

#include "Player.h"
#include "PlayList.h"

#include "Player.hrh"

/** 
* The factory method for creating new recording views.
* @returns The new recording view object.
*/
MoboPlayer::CPlayerRecordingsView* MoboPlayer::CPlayerRecordingsView::NewL()
	{
	CPlayerRecordingsView* self = CPlayerRecordingsView::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

/** 
* The factory method for creating new recording views.
* Places the view to the cleanup stack and returns.
* @returns The new recording view object.
*/
MoboPlayer::CPlayerRecordingsView* MoboPlayer::CPlayerRecordingsView::NewLC()
	{
	CPlayerRecordingsView* self = new (ELeave) CPlayerRecordingsView();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

/** 
* Default constructor for initializing non-leaving items. 
*/
MoboPlayer::CPlayerRecordingsView::CPlayerRecordingsView()
	{
	// no implementation required
	}

/** 
* Destroys the view and objects within.
*/
MoboPlayer::CPlayerRecordingsView::~CPlayerRecordingsView()
	{
	//  DoDeactivate is called by the base class destructor
	}

/** 
* The 2nd phase constructor for initializing leaving items.
*/
void MoboPlayer::CPlayerRecordingsView::ConstructL()
	{
	BaseConstructL(R_PLAYER_VIEW_RECORDINGS);
	}


/** 
* Returns the view id used in view switching.
* @returns The view id.
*/
TUid MoboPlayer::CPlayerRecordingsView::Id() const
	{
	return TUid::Uid(EPlayerRecordingsViewId);
	}

/** 
* Called when the view is activated. Initialize the view contents here.
* @param aPrevViewId The id of the view that was active before this view.
* @param aCustomMessageId Not used here.
* @param aCustomMessage Not used here. */
void MoboPlayer::CPlayerRecordingsView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
											TUid /*aCustomMessageId*/,
											const TDesC8& /*aCustomMessage*/)
	{
	ASSERT(iContainer == NULL);
	iContainer = CPlayerRecordingsContainer::NewL(ClientRect());
	AppUi()->AddToStackL(iContainer);
	}

/** 
* Called by the GUI framework when the view is deactivated. Release resources here.
*/
void MoboPlayer::CPlayerRecordingsView::DoDeactivate()
	{
	if (iContainer)
		{
		AppUi()->RemoveFromStack(iContainer);
		delete iContainer;
		iContainer = 0;
		}
	}

/** 
* Handles the menu commands specific to this view.
* @param aCommand The command id.
*/
void MoboPlayer::CPlayerRecordingsView::HandleCommandL(TInt aCommand)
	{   
	if (aCommand == EAknSoftkeyBack)
		{
		AppUi()->ActivateLocalViewL(TUid::Uid(EPlayerPlayListViewId));
		return;
		}

	CPlayList * list = (static_cast<CPlayerDocument*>(CEikonEnv::Static()->EikAppUi()->Document())->Player())->CurrentPlayList();
	if (!list)
		return;

	switch (aCommand)
		{
		case EPlayerEditRecording:
			{
			TRecording * recording = list->GetSelectedRecording();
			if (recording)
				{
				TRecording tempRecording = *recording;
				CRecordingForm * dlg = CRecordingForm::NewL(tempRecording);
				if (dlg->ExecuteLD(R_RECORDING_DIALOG))
					{
					*recording = tempRecording;
					CEikonEnv::Static()->EikAppUi()->Document()->SaveL();
					}
				}
			break;
			}

		case EPlayerNewRecording:
			{
			TRecording tempRecording = TRecording();
			CRecordingForm * dlg = CRecordingForm::NewL(tempRecording);
			if (dlg->ExecuteLD(R_RECORDING_DIALOG))
				{
				list->AddRecordingL(tempRecording);
				iContainer->HandleListboxAdditionL();
				CEikonEnv::Static()->EikAppUi()->Document()->SaveL();
				}
			break;
			}

		case EPlayerDeleteRecording:
			{
			HBufC * first = CEikonEnv::Static()->AllocReadResourceLC(R_DELETE_CONFIRMATION);
			TBufC<KMaxRecordingNameLen> recName;
			TRecording * recording = list->GetSelectedRecording();
			if (recording)
				{
				recName = recording->Name();
				}
			if (CEikonEnv::Static()->QueryWinL(*first, recName))
				{
				list->RemoveRecording(list->SelectedRecording());
				iContainer->HandleListboxRemovalL();
				CEikonEnv::Static()->EikAppUi()->Document()->SaveL();
				}
				CleanupStack::PopAndDestroy(); //first
			break;
			}
			
		case EPlayerSortByName:
			{
			list->Sort(CPlayList::EName);
			break;
			}
			
		case EPlayerSortByPerformer:
			{
			list->Sort(CPlayList::EPerformer);
			break;
			}
			
		case EPlayerSortByGenre:
			{
			list->Sort(CPlayList::EGenre);
			break;
			}
			
		case EPlayerSortByPerformed:
			{
			list->Sort(CPlayList::EPerformed);
			break;
			}
			
		default:
			{
			AppUi()->HandleCommandL(aCommand);
			}
		}
	}

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerRecordingsView.cpp,v $
* Revision 1.2  2005/06/09 08:58:59  Antti
* Added call to container's HandleListboxRemovalL.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/