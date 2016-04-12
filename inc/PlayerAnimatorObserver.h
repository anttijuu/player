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

// PlayerAnimatorObserver.h
//
// $Id: PlayerAnimatorObserver.h,v 1.1.1.1 2005/03/01 18:27:34 Antti Exp $

#ifndef __PLAYERANIMATOROBSERVER_H
#define __PLAYERANIMATOROBSERVER_H

#include <e32def.h>

namespace MoboPlayer 
{
/** This interface class (mixin) is used by the CPlayerAnimator to
* notify the observer (implementing this interface) of the animation
* events.
* @see CPlayerAnimator
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class MPlayerAnimatorObserver 
	{
public:
	/**
	* Animator notifies the observer that it should now animate
	* the current bitmap. 
	*/
	virtual void AnimateBitmap() = 0;
	
	/** 
	* Animator notifies the observer that there was an error in the animation.
	*/
	virtual void HandleAnimationError(TInt aError) = 0;
	};

} // namespace

#endif
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerAnimatorObserver.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/