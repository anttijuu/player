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

// PlayerDefines.h
//
// $Id: PlayerDefines.h,v 1.2 2005/03/02 08:14:39 Antti Exp $
//
// @author Antti Juustila
// @version $Revision: 1.2 $

#ifndef __PLAYERDEFINES_H
#define __PLAYERDEFINES_H


/** The application UID for the MoboPlayer application. */
#define KMoboPlayerAppUID 0x01772299

// You can use these constant values when defining
// descriptors for holding text data. For example,
// a buffer descriptor for recording's name (30 chars) would 
// look like this: TBuf<KMaxRecordingNameLen> nameBuf.

/** Max length for the name of the recording. */
#define KMaxRecordingNameLen 30
/** Max length for the genre of the recording. */
#define KMaxGenreNameLen 30
/** Max length for the performer's name. */
#define KMaxPerformerNameLen 30

/** Max length for the playlist's name. */
#define KMaxPlayListNameLen 30
/** Max length for the playlist's description. */
#define KMaxPlayListDescriptionLen 200



#endif

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerDefines.h,v $
* Revision 1.2  2005/03/02 08:14:39  Antti
* Updated documentation.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/