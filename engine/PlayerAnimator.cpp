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

// Recording.cpp
// (c) Antti Juustila, 2005
//
// $Id: PlayerAnimator.cpp,v 1.3 2005/06/09 08:56:33 Antti Exp $

#include <fbs.h>

#include "PlayerAnimator.h"
#include "PlayerAnimatorObserver.h"


/** 
* Factory method to creating a new animator.
* @param aBitmapFile Full path and file name of the mbm file that contains the bitmaps to show.
* @param aObserver The observer that gets notified when it is time to show a bitmap again.
* @returns The new animator object.
*/
EXPORT_C MoboPlayer::CPlayerAnimator * MoboPlayer::CPlayerAnimator::NewL(const TDesC & aBitmapFile, MPlayerAnimatorObserver * aObserver)
	{
	CPlayerAnimator * self = new (ELeave) CPlayerAnimator(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aBitmapFile);
	CleanupStack::Pop();
	return self;
	}
	
/** 
* Destructor releses/deletes the allocated resources.
* Also call CActive::Cancel() to make sure pending outstanding
* requests are first cancelled.
*/
EXPORT_C MoboPlayer::CPlayerAnimator::~CPlayerAnimator()
	{
	// Cancel the possible outstanding timer request.
	Cancel();
	// Close the timer resource
	iTimer.Close();
	// Destroy the bitmaps.
	iBitmaps.ResetAndDestroy();
	}

/** 
* Constructor, saves the pointer to the observer.
* Also, call CActive constructor to set the priority of this
* active object.
*/
MoboPlayer::CPlayerAnimator::CPlayerAnimator(MPlayerAnimatorObserver * aObserver)
	: CActive(CActive::EPriorityStandard), iObserver(aObserver)
	{
	}

/** 
* Second phase constructor. Here the animator loads the bitmaps 
* from the file into an array of bitmaps
* Also, add this active object to the active scheduler using
* CActiveScheduler::Add().
* @param aBitmapFile The full path to the file that contains the bitmaps.
*/
void MoboPlayer::CPlayerAnimator::ConstructL(const TDesC & aBitmapFile)
	{
	// Initialize the timer resource.
	User::LeaveIfError(iTimer.CreateLocal());
	
	// Read the bitmaps from the file.
	CFbsBitmap * bmp;
	TInt counter = 0;
	while (counter < EMaxNumberOfBitmaps)
		{
		bmp = new (ELeave) CFbsBitmap;
		CleanupStack::PushL(bmp);
		User::LeaveIfError(bmp->Load(aBitmapFile, counter));
		User::LeaveIfError(iBitmaps.Append(bmp));
		CleanupStack::Pop(); // bmp
		counter++;
		}
	// Add the active object to the active scheduler.
	CActiveScheduler::Add(this);
	}


/** 
* Starts the animation timer.
* As the timer fires, the RunL of CPlayerAnimator is called. In RunL, the animator
* selects the next bitmap to show, and notifies the observer using MPlayerAnimatorObserver::AnimateBitmap()
* that it should now request the new bitmap to show using Bitmap().
*/
EXPORT_C void MoboPlayer::CPlayerAnimator::Start()
	{
	// First cancel possible outstanding request.
	Cancel();
	// Start the timer.
	iTimer.After(iStatus, 500000);
	// And activate active object.
	SetActive();
	}

/** 
* Stops the animation timer. Here any outstanding timer requests should be cancelled.
*/
EXPORT_C void MoboPlayer::CPlayerAnimator::Stop()
	{
	// Cancel the timer.
	Cancel();
	}

/** 
* Used by the animator observer to get the bitmap to be shown next.
* @returns the bitmap to be shown next.
*/
EXPORT_C const CFbsBitmap * MoboPlayer::CPlayerAnimator::Bitmap() const
	{
	// return the current bitmapl
	return iBitmaps[iCurrentBitmapIndex];
	}

/** 
* RunL is called as the timer fires. The observer is notified and the
* bitmap index to show is incremented. Also, a new timer request is activated.
*/
void MoboPlayer::CPlayerAnimator::RunL()
	{
	// Select next bitmap, second if previous was the last.
	// Second, because first is the idle playing bitmap.
	iCurrentBitmapIndex++;
	if (iCurrentBitmapIndex >= iBitmaps.Count())
		{
		iCurrentBitmapIndex = 1;
		}
	// Tell the observer to animate.
	iObserver->AnimateBitmap();
	// And request the timer event again.
	iTimer.After(iStatus, 500000);
	// Don't forget to set the active object active.
	SetActive();
	}

/** 
* Called when the animator needs to be cancelled.
* Implement the cancelling protocol here. In this case,
* this means that the timer is cancelled and the current
* bitmap index initialized.
*/
void MoboPlayer::CPlayerAnimator::DoCancel()
	{
	// Called by CActive::Cancel
	iTimer.Cancel();
	iCurrentBitmapIndex = 0;
	}

/** 
* Called when RunL leaves.
* Cancel any outstanding requests, and reset the active
* object. Also, notify the observer of the error.
* @param aError The reason for the leave in RunL.
* @returns The error code. If this is other than KErrNone, active scheduler
* will panic the application.
*/
TInt MoboPlayer::CPlayerAnimator::RunError(TInt aError)
	{
	// Called if RunL leaves.
	iCurrentBitmapIndex = 0;
	iObserver->HandleAnimationError(aError);
	return KErrNone;
	}

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerAnimator.cpp,v $
* Revision 1.3  2005/06/09 08:56:33  Antti
* Removed iTimer.Cancel from RunError; not needed.
*
* Revision 1.2  2005/03/02 07:25:27  Antti
* Added braces for if blocks.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/