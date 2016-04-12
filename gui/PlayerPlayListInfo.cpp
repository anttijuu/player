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

#include <eikenv.h>

#include "PlayerPlayListInfo.h"
#include "Player.h"

/** 
* Factory method for creating the info control.
* @param aRect The area the control occupies.
* @param aPlayer The player for getting recording information
* and playing status notifications.
* @returns The new recording info control.
*/
MoboPlayer::CPlayerPlayListInfo* MoboPlayer::CPlayerPlayListInfo::NewL(const TRect& aRect, MoboPlayer::CPlayer * aPlayer)
	{
	CPlayerPlayListInfo* self = new (ELeave) CPlayerPlayListInfo(aPlayer);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop(self);
	return self;
	}

/**
* 2nd phase constructor for creating the info control.
* @param aRect The area the control occupies.
*/
void MoboPlayer::CPlayerPlayListInfo::ConstructL(const TRect& aRect)
	{
	SetRect(aRect);
	}

/** 
* Constructor for initializing non-leaving items.
* @param aPlayer The player used for showing the information.
*/
MoboPlayer::CPlayerPlayListInfo::CPlayerPlayListInfo(MoboPlayer::CPlayer * aPlayer) : iPlayer(aPlayer)
	{
	// Empty
	}
	
/**
* Destructor for the info control.
*/
MoboPlayer::CPlayerPlayListInfo::~CPlayerPlayListInfo()
	{
	}
	
/** Draws the control to the screen. Draws information about the selected
* playlist (description of it).
* @param aRect The area that needs to be drawn. */
void MoboPlayer::CPlayerPlayListInfo::Draw(const TRect & aRect) const
	{
	CWindowGc & gc = SystemGc();
	gc.SetClippingRect(aRect);
	const CFont * font = CEikonEnv::Static()->DenseFont();
	gc.UseFont(font);
	TRect rect(Rect());
	rect.Shrink(2,2);
	gc.DrawLine(rect.iTl, TPoint(rect.iBr.iX, rect.iTl.iY));
	TBuf<100> info;
	iPlayer->GetCurrentPlayListDescription(info);
	gc.DrawText(info, rect, font->AscentInPixels() + 4);
	gc.CancelClippingRect();
	}
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerPlayListInfo.cpp,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/