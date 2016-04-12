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

#ifndef __PLAYER_DOCUMENT_H__
#define __PLAYER_DOCUMENT_H__

#include <akndoc.h>

// Forward references
class CPlayerAppUi;
class CEikApplication;

namespace MoboPlayer 
{

class CPlayer;

/**
* The document class for the MoboPlayer.
* Document creates the model (the Player), is responsible
* of initiating the storing and restoring of the application data
* and also creates the application's UI object when the application
* is launched.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class CPlayerDocument : public CAknDocument
	{
public:

	static CPlayerDocument* NewL(CEikApplication& aApp);
	static CPlayerDocument* NewLC(CEikApplication& aApp);

	~CPlayerDocument();

	void NewDocumentL();

	CFileStore* OpenFileL(TBool aDoOpen,const TDesC& aFilename,RFs& aFs);	

	void RestoreL(const CStreamStore& aStore,const CStreamDictionary& aStreamDic);
	void StoreL(CStreamStore& aStore,CStreamDictionary& aStreamDic) const;

	MoboPlayer::CPlayer * Player();

public:
	// from CAknDocument
	CEikAppUi* CreateAppUiL();

private:
	void ConstructL();
	CPlayerDocument(CEikApplication& aApp);
	void ResetModelL();

private:
	/**
	* The application model, alias engine; the player of music.
	* This must be allocated and destroyed by the document.
	*/
	CPlayer * iPlayer;
	};

} // namespace 

#endif // __PLAYER_DOCUMENT_H__

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerDocument.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/