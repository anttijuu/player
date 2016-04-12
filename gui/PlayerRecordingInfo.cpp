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
#include <eikappui.h>
#include <eikapp.h>

#include <MoboPlayer.rsg>

#include "PlayerRecordingInfo.h"
#include "Player.h"
#include "PlayList.h"
#include "PlayerAnimator.h"
#include "PlayerDefines.h"

/** 
* Factory method for creating the info control.
* @param aRect The area the control occupies.
* @param aPlayer The player for getting recording information
* and playing status notifications.
* @returns The new recording info control.
*/
MoboPlayer::CPlayerRecordingInfo* MoboPlayer::CPlayerRecordingInfo::NewL(const TRect& aRect, MoboPlayer::CPlayer * aPlayer)
	{
	CPlayerRecordingInfo* self = new (ELeave) CPlayerRecordingInfo(aPlayer);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop(self);
	return self;
	}

/** 
* 2nd phase constructor for creating the info control.
* First, use SetRect to set the are for the control.
* Then, define the file name with path for the bitmap file that contains
* the animation bitmaps that are showed when music plays. Then create the animator object.
* The bitmap file should be located in the same directory as the application.
* The dombm.bat in Player\Bitmaps directory generates and copies the bitmap file
* to the app directory in c: drive in the emulator environment. <b>Modify</b> this bat so that it
* works in your environment.<p>
* You can hardcode this file location in emulator environment to c:\System\Apps\MoboPlayer\,
* where to the bat file copies the bitmap file.
* In a real device, you should use CEikonEnv::Static()->EikAppUi()->Application()->AppFullName()
* to get the path to the app directory and TParsePtr to get the drive and
* path to the bitmap file, then just add the file name after this path and you
* have the location for the bitmap file. Pass this file name with path to the
* CPlayerAnimator::NewL().
* In code, you can use #ifdef _DEBUG to use the hardcoded path, and in #else,
* provide the code that uses the EikonEnv provided path to the app location.
* @param aRect The area the control occupies.
*/
void MoboPlayer::CPlayerRecordingInfo::ConstructL(const TRect& aRect)
	{
	SetRect(aRect);
#ifdef _DEBUG
	_LIT(KPath, "c:\\System\\Apps\\MoboPlayer\\");
	TFileName mbmFile(KPath);
#else
	TFileName mbmFile = CEikonEnv::Static()->EikAppUi()->Application()->AppFullName();
	TParsePtr parser(mbmFile);
	mbmFile = parser.DriveAndPath();
#endif
	_LIT(KAnimationBitmapFileName, "PlayerAnimations.mbm");
	mbmFile.Append(KAnimationBitmapFileName);
	iAnimator = CPlayerAnimator::NewL(mbmFile, this);
	}

/** 
* Constructor for initializing non-leaving items.
* @param aPlayer The player used for showing the information.
*/
MoboPlayer::CPlayerRecordingInfo::CPlayerRecordingInfo(CPlayer * aPlayer) : iPlayer(aPlayer)
	{
	// Empty
	}
	
/** 
* Destructor for the info control.
* Stop the animator, if it has been created, and delete it.
*/
MoboPlayer::CPlayerRecordingInfo::~CPlayerRecordingInfo()
	{
	if (iAnimator)
		{
		iAnimator->Stop();
		delete iAnimator;
		}
	}
	
/** 
* Draws the control to the screen.
* Draw the current recording's details to the control's area.
* Draw also a line at the top of the area to show the border between
* the info control and the list box control above. Then, get the
* bitmap from the animator object and draw that bitmap using BitBlt().
* @param aRect The area that needs to be drawn.
*/
void MoboPlayer::CPlayerRecordingInfo::Draw(const TRect & aRect) const
	{
	CWindowGc & gc = SystemGc();
	gc.SetClippingRect(aRect);
	const CFont * font = CEikonEnv::Static()->DenseFont();
	gc.UseFont(font);
	TRect rect(Rect());
	rect.Shrink(2,2);
	gc.DrawLine(rect.iTl, TPoint(rect.iBr.iX, rect.iTl.iY));
	TBuf<200> details;
	iPlayer->GetCurrentRecordingDetails(details);
	gc.DrawText(details, rect, font->AscentInPixels() + 4);
	const CFbsBitmap * bmp = iAnimator->Bitmap();
	if (bmp)
		{
		rect = Rect();
		rect.iTl.iX = rect.iBr.iX - bmp->SizeInPixels().iWidth;
		rect.SetSize(bmp->SizeInPixels());
		TRect bmpPieceRect(TPoint(0,0), bmp->SizeInPixels());
		gc.Clear(bmpPieceRect);
		gc.BitBlt(rect.iTl, bmp, bmpPieceRect); //gc.BitBltMasked(rect.iTl, bmp, bmpPieceRect, mask, EFalse);
		}
	gc.CancelClippingRect();
	}

/** 
* Called when the playing has started. Called by the recording container control
* which is an observer to the player.
* Start the animator and call DrawNow to update the control area. Optionally,
* also save the total duration and draw both the time left and the total duration
* to the screen in Draw().
* @param aTotalDuration The duration of the sound file in seconds.
* @see CPlayerRecordingsContainer
*/
void MoboPlayer::CPlayerRecordingInfo::PlayingStarted(TInt /*aTotalDuration*/)
	{
	// TODO: show also countdown of the clip length as text under the bitmap.
	iAnimator->Start();
	DrawNow();
	}
	
/** 
* Called when the playing is stopped. Called by the recording container control
* which is an observer to the player. Stop the animator and call DrawNow() to
* update the control area.
* @see CPlayerRecordingsContainer
*/
void MoboPlayer::CPlayerRecordingInfo::PlayingStopped()
	{
	iAnimator->Stop();
	DrawNow();
	}

/** 
* Implements the MPlayerAnimatorObserver interface.
* Called by the CPlayerAnimator when it is time to show the 
* next animation bitmap. Just call DrawNow(), it will show the
* next bitmap.
*/
void MoboPlayer::CPlayerRecordingInfo::AnimateBitmap()
	{
	DrawNow();
	}
	
/** 
* Implements the MPlayerAnimatorObserver interface.
* Called by the CPlayerAnimator when there is an error in the animation.
* Show an infomsg that animator cannot be run, with the error code, and
* stop the animator. See the resource file for the message to be shown.
* @param aError The error code, one of the system wide error codes.
* @see CEikonEnv::InfoMsg()
*/
void MoboPlayer::CPlayerRecordingInfo::HandleAnimationError(TInt aError)
	{
	CEikonEnv::Static()->InfoMsg(R_CANNOT_RUN_ANIMATOR, aError);
	iAnimator->Stop();
	}


/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerRecordingInfo.cpp,v $
* Revision 1.2  2005/03/02 09:09:15  Antti
* Updated documentation.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/