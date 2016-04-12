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

#ifndef __PLAYER_PLAYLISTINFO_H__
#define __PLAYER_PLAYLISTINFO_H__

#include <coecntrl.h>

namespace MoboPlayer 
{

class CPlayer;


/** This is an information control that shows information about
* the selected playlist.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class CPlayerPlayListInfo : public CCoeControl
	{
public: 
	static CPlayerPlayListInfo* NewL(const TRect& aRect, CPlayer * aPlayer);

	~CPlayerPlayListInfo();

private:
	CPlayerPlayListInfo(CPlayer * aPlayer);
	void ConstructL(const TRect& aRect);
	
	// From CCoeControl
	void Draw(const TRect & aRect) const;
	
private:
	/** 
	* The player object needed in showing information about the state of the app.
	*/
	CPlayer	* iPlayer; // Does not own
	};

} // namespace

#endif // __PLAYER_PLAYLISTINFO_H__

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerPlayListInfo.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/