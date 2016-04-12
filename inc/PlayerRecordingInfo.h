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

#ifndef __PLAYER_RECORDINGINFO_H__
#define __PLAYER_RECORDINGINFO_H__

#include <coecntrl.h>

#include "PlayerAnimatorObserver.h"

namespace MoboPlayer 
{

class CPlayer;
class CPlayerAnimator;


/** This is an information control that shows information about
* the selected recording, and also shows an animation when the recordings
* are played.
* @todo Implement this class's public methods and 
* add necessary implementation details.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
* @see MPlayerAnimatorObserver
*/

class CPlayerRecordingInfo : public CCoeControl, public MPlayerAnimatorObserver
	{
public: 
	static CPlayerRecordingInfo* NewL(const TRect& aRect, CPlayer * aPlayer);

	void PlayingStarted(TInt aTotalDuration);
	void PlayingStopped();
	
	~CPlayerRecordingInfo();

	// From MPlayerAnimatorObserver
	virtual void AnimateBitmap();
	virtual void HandleAnimationError(TInt aError);

private:
	CPlayerRecordingInfo(CPlayer * aPlayer);
	void ConstructL(const TRect& aRect);
	
	// From CCoeControl
	void Draw(const TRect & aRect) const;
	
private:
	/** 
	* The player object needed in showing information about the state of the app.
	*/
	CPlayer				* iPlayer; // Does not own
	/** 
	* The animator used in showing animation when playing is going on. Owned by this object.
	*/
	CPlayerAnimator	* iAnimator; // Owns
	};

} //namespace 

#endif // __PLAYER_RECORDINGINFO_H__

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerRecordingInfo.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/