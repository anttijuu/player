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

#ifndef __PLAYER_APPUI_H__
#define __PLAYER_APPUI_H__

#include <aknViewAppUi.h>

#include "Player.h"

namespace MoboPlayer 
{

class CPlayerPlayListView;
class CPlayerRecordingsView;

/** 
* The application ui class constructs the application views and
* handles any commands that the views do not handle.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class CPlayerAppUi : public CAknViewAppUi
	{
public:

	void ConstructL();
	void HandleCommandL(TInt aCommand);

private:

	/** 
	* The playlist view, the default view for the application.
	*/
	CPlayerPlayListView* iAppView1;
	
	/**
	* The recordings view, shown when the user selects a playlist
	* from the list of playlists.
	*/
	CPlayerRecordingsView* iAppView2;

	/** 
	* The engine of the application; player.
	*/
	MoboPlayer::CPlayer * iPlayer;
	};

} // namespace

#endif // __PLAYER_APPUI_H__

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerAppUi.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/