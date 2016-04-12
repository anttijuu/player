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

#ifndef __PLAYER_PAN__
#define __PLAYER_PAN__

/** MoboPlayer application panic codes */
enum TMoboPlayerPanics 
	{
	EMoboPlayerNoCommandHandler = 1,
	EMoboPlayerSaveOnCloseFailed,
	EMoboPlayerInvalidSwitchCaseValue
	};

/** The panic utility function that is called by the app when a 
* panic is needed.
* @param aReason The reason code for the panic.
*/

inline void Panic(TMoboPlayerPanics aReason)
	{
	_LIT(KApplicationName,"MoboPlayer");
	User::Panic(KApplicationName, aReason);
	}

#endif // __PLAYER_PAN__

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: Player.pan,v $
* Revision 1.2  2005/03/02 07:30:09  Antti
* Added a new panic code, fixed the const _LIT name to conform to Symbian naming conventions.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/