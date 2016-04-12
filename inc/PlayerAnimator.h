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

// PlayerAnimator.h
//
// $Id: PlayerAnimator.h,v 1.2 2005/03/02 07:30:32 Antti Exp $

#ifndef __PLAYERANIMATOR_H
#define __PLAYERANIMATOR_H

#include <e32base.h>

#include "PlayerDefines.h"

class CFbsBitmap;

namespace MoboPlayer 
{

class MPlayerAnimatorObserver;

/** 
* The animator class handles the presentation of animations in the
* MoboPlayer. It loads the bitmap files used in the animation, starts
* a timer to present the animations and notifies the observer of the
* animator when the new bitmap in the series of animations should
* be shown. The observer then uess the Bitmap() method to get the
* next bitmap to show. CPlayerAnimator is an Active object.
* @see CActive.
* @todo Implement this class's public methods and 
* add necessary implementation details.
* @author Antti Juustila
* @version $Revision: 1.2 $
*/

class CPlayerAnimator : public CActive
	{
public:
	IMPORT_C static CPlayerAnimator * NewL(const TDesC & aBitmapFile, MPlayerAnimatorObserver * aObserver);
	IMPORT_C ~CPlayerAnimator();
	
	IMPORT_C void Start();
	IMPORT_C void Stop();
	
	IMPORT_C const CFbsBitmap * Bitmap() const;
	
protected:
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
private:
	CPlayerAnimator(MPlayerAnimatorObserver * aObserver);
	void ConstructL(const TDesC & aBitmapFile);	
	
private:
	/**
	* This enum specifies how many bitmaps is used in animation. 
	*/
	enum 
		{
		EMaxNumberOfBitmaps = 4 
		};
	/** The bitmap array for the animation bitmaps. */
	RPointerArray<CFbsBitmap>	iBitmaps;
	/** The handle to the system timer. */
	RTimer						iTimer;
	/** The index of the current bitmap to show. */
	TInt							iCurrentBitmapIndex;
	/** The observer that gets notified as the timer fires.*/
	MPlayerAnimatorObserver	* iObserver;
	};

} // namespace

#endif
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerAnimator.h,v $
* Revision 1.2  2005/03/02 07:30:32  Antti
* Updated class documentation a bit.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/