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

#include <e32std.h>
#include <eikdialg.h>
#include <avkon.hrh>
#include <akndialog.h>

#include <MoboPlayer.rsg>

#include "Player.pan"
#include "PlayerAppUi.h"
#include "PlayerDocument.h"
#include "PlayerPlayListView.h"
#include "PlayerRecordingsView.h"
#include "Player.hrh"

#include "Player.h"
#include "Recording.h"


/**
* Second phase constructor function. Called by the application
* framework when the application is initialized.
*/
void MoboPlayer::CPlayerAppUi::ConstructL()
	{
	BaseConstructL(); // Must call before doing anything else

	// Get a pointer to the model.
	CPlayerDocument * doc = static_cast<CPlayerDocument*>(Document());
	iPlayer = doc->Player();
	
	// Create the playlist view.
	iAppView1 = CPlayerPlayListView::NewL();
	// Create the recordings view.
	iAppView2 = CPlayerRecordingsView::NewL();

	// Call CAknViewAppUi::AddViewL to add the views to the app ui.
	AddViewL(iAppView1); // transfer ownership to base class 
	AddViewL(iAppView2); // transfer ownership to base class 

	// Playlist view is the default view.
	SetDefaultViewL(*iAppView1);
	}

/**
* Handles the commands user has selected. The command is first
* offered to the active view, and if it is not handling it, the
* view can then offer the command to the ui object.
* @param aCommand The command to handle.
*/
void MoboPlayer::CPlayerAppUi::HandleCommandL(TInt aCommand)
	{
	switch(aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit: 
			{
			Exit();
			break;
			}

		case EPlayerPlayAll:
			{
			iPlayer->PlayAllL();
			break;
			}

		case EPlayerSwitchToRecordingsView:
			{
			ActivateLocalViewL(TUid::Uid(EPlayerRecordingsViewId));
			break;
			}

		default:
			{
			Panic(EMoboPlayerNoCommandHandler);
			break;
			}
		}
	}

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerAppUi.cpp,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/