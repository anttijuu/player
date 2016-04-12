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

// PlayerObserver.h
//
// $Id: PlayerObserver.h,v 1.1.1.1 2005/03/01 18:27:34 Antti Exp $
// @author Antti Juustila
// @version $Revision: 1.1.1.1 $

#ifndef __PLAYEROBSERVER_H
#define __PLAYEROBSERVER_H

#include <e32std.h>

namespace MoboPlayer 
{
	
/** 
* This interface is used to notify an observer of the CPlayer class
* of events related to the playing of recordings.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class MPlayerObserver
	{
public:
	/** 
	* Called when there is an error in the player.
	* @param aError The error code of the player, usually one of the system error codes.
	*/
	virtual void HandlePlayerError(TInt aError) = 0;
	
	/** 
	* Called when the playing starts.
	* @param aIndexOfRecording The index number of the recording that is played.
	* @param aTotalDuration The duration of the recording in seconds.
	*/
	virtual void PlayingStarted(TInt aIndexOfRecording, TInt aTotalDuration) = 0;
	
	/**
	* Called when the playing has ended successfully.
	*/
	virtual void PlayingEnded() = 0;
	
	virtual ~MPlayerObserver() { /* Empty */};
	};

} // namespace

#endif
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerObserver.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/