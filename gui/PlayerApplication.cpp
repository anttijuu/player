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

#include "PlayerDocument.h"
#include "PlayerApplication.h"
#include "PlayerDefines.h"


/** 
* Creates the document object and returns it to
* the application framework.
*/
CApaDocument* MoboPlayer::CPlayerApplication::CreateDocumentL()
    {  
    // Create an MultiViews document, and return a pointer to it
    CApaDocument* document = CPlayerDocument::NewL(*this);
    return document;
    }
    
/**
* Provides the application UID to the system.
* @returns The UID of the application.
*/
TUid MoboPlayer::CPlayerApplication::AppDllUid() const
    {
    // Return the UID for the MultiViews application
    return TUid::Uid(KMoboPlayerAppUID);
    }

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerApplication.cpp,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/