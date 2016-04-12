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
#include <eikmenup.h>

#include <MoboPlayer.rsg>

#include "Player.h"
#include "PlayList.h"

#include "PlayerDocument.h"
#include "PlayerPlayListView.h"
#include "PlayerPlayListContainer.h"
#include "PlayListForm.h"

#include "Player.hrh"

/** 
* The factory method for creating new playlist views.
* @returns The new playlist view object.
*/
MoboPlayer::CPlayerPlayListView* MoboPlayer::CPlayerPlayListView::NewL()
    {
    CPlayerPlayListView* self = CPlayerPlayListView::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

/** 
* The factory method for creating new playlist views.
* Places the view to the cleanup stack and returns.
* @returns The new playlist view object.
*/
MoboPlayer::CPlayerPlayListView* MoboPlayer::CPlayerPlayListView::NewLC()
    {
    CPlayerPlayListView* self = new (ELeave) CPlayerPlayListView();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

/** 
* Default constructor for initializing non-leaving items.
*/
MoboPlayer::CPlayerPlayListView::CPlayerPlayListView()
    {
    // no implementation required
    }

/** 
* Destroys the view.
*/
MoboPlayer::CPlayerPlayListView::~CPlayerPlayListView()
    {
    //  DoDeactivate is called by the base class destructor
    }

/** 
* The 2nd phase constructor for initializing leaving items.
*/
void MoboPlayer::CPlayerPlayListView::ConstructL()
    {
    BaseConstructL(R_PLAYER_VIEW_PLAYLISTS);
    }


/**
* Returns the view id used in view switching.
*/
TUid MoboPlayer::CPlayerPlayListView::Id() const
    {
    return TUid::Uid(EPlayerPlayListViewId);
    }

/** 
* Called when the view is activated. Initialize the view contents here.
* @param aPrevViewId The id of the view that was active before this view.
* @param aCustomMessageId Not used here.
* @param aCustomMessage Not used here.
*/
void MoboPlayer::CPlayerPlayListView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
                                    TUid /*aCustomMessageId*/,
                                    const TDesC8& /*aCustomMessage*/)
    {
    ASSERT(iContainer == NULL);
    // Create the container when the playlist view is activated.
    iContainer = CPlayerPlayListContainer::NewL(ClientRect());
    // Let the control handle keyboard events.
    AppUi()->AddToStackL(iContainer);
    }

	/** Called by the GUI framework when the view is deactivated. Release resources here. */
void MoboPlayer::CPlayerPlayListView::DoDeactivate()
    {
    if (iContainer)
        {
        // No keyboard events to this control anymore.
        AppUi()->RemoveFromStack(iContainer);
        delete iContainer;
        iContainer = 0;
        }
    }

/** 
* Handles the menu commands specific to this view.
* @param aCommand The command id.
*/
void MoboPlayer::CPlayerPlayListView::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EPlayerEditPlayList:
			{
			// Get a pointer to the current playlist to edit it.
			CPlayList * list = (static_cast<CPlayerDocument*>(CEikonEnv::Static()->EikAppUi()->Document())->Player())->CurrentPlayList();
			if (list)
				{
				// Create the form to edit the list.
				CPlayListForm * dlg = CPlayListForm::NewL(*list);
				if (dlg->ExecuteLD(R_PLAYLIST_DIALOG))
					{
					CEikonEnv::Static()->EikAppUi()->Document()->SaveL();
					}
				}
			break;
			}

		case EPlayerNewPlayList:
			{
			// Create e new list.
			CPlayList * list = CPlayList::NewL();
			// Push to cs in case something leaves.
			CleanupStack::PushL(list);
			CPlayListForm * dlg = CPlayListForm::NewL(*list);
			if (dlg->ExecuteLD(R_PLAYLIST_DIALOG))
				{
				// Add the new playlist to the engine.
				static_cast<CPlayerDocument*>(CEikonEnv::Static()->EikAppUi()->Document())->Player()->AddPlayListL(list);
				// Tell the container to update the listbox contents.
				iContainer->HandleListboxAdditionL();
				// Save the data now.
				CEikonEnv::Static()->EikAppUi()->Document()->SaveL();
				CleanupStack::Pop();
				return;
				}
			CleanupStack::PopAndDestroy(); // Cancelled, so destroy the created playlist.
			break;
			}

		case EPlayerDeletePlayList:
			{
			CPlayer * player = (static_cast<CPlayerDocument*>(CEikonEnv::Static()->EikAppUi()->Document())->Player());
			if (player)
				{
				// Ask user if s/he really wants to delete the playlist.
				HBufC * first = CEikonEnv::Static()->AllocReadResourceLC(R_DELETE_CONFIRMATION);
				TBufC<KMaxPlayListNameLen> listName;
				CPlayList * list = player->CurrentPlayList();
				if (list)
					{
					listName = list->Name();
					}
				if (CEikonEnv::Static()->QueryWinL(*first, listName))
					{
					player->RemoveCurrentPlayListL();
					CEikonEnv::Static()->EikAppUi()->Document()->SaveL();
					}
				}
			CleanupStack::PopAndDestroy(); // first
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
* $Log: PlayerPlayListView.cpp,v $
* Revision 1.2  2005/03/02 07:28:53  Antti
* Changed RemoveCurrentPlayList to RemoveCurrentPlayListL.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/