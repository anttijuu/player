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

#include "PlayerAppUi.h"
#include "PlayerDocument.h"

#include "Player.h"
#include "PlayerDefines.h"

/** 
* Factory method for creating the document object.
* @param aApp The application object, passed to the CEikDocument.
* @returns The document object.
*/
MoboPlayer::CPlayerDocument* MoboPlayer::CPlayerDocument::NewL(CEikApplication& aApp)
	{
	CPlayerDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

/** 
* Factory method for creating the document object.
* Puts the doc object to the cleanup stack and returns it.
* @param aApp The application object, passed to the CEikDocument.
* @returns The document object.
*/
MoboPlayer::CPlayerDocument* MoboPlayer::CPlayerDocument::NewLC(CEikApplication& aApp)
	{
	CPlayerDocument* self = new (ELeave) CPlayerDocument(aApp);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

/** 
* 2nd phase constructor for the document.
* The application model (the engine) is created here.
*/
void MoboPlayer::CPlayerDocument::ConstructL()
	{
	iPlayer = CPlayer::NewL();
	}    

/** 
* C++ constructor.
* @param aApp The application object, passed to CEikDocument constructor.
*/
MoboPlayer::CPlayerDocument::CPlayerDocument(CEikApplication& aApp)
		 : CAknDocument(aApp) 
	{
	// no implementation required
	}   

/** 
* Destructor frees the resources allocated. Especially document
* is responsible for destroying the model (the player).
*/
MoboPlayer::CPlayerDocument::~CPlayerDocument()
	{
	delete iPlayer;
	}

/** 
* Creates the application's UI object. Called by the framework.
* @returns The application UI object.
*/
CEikAppUi* MoboPlayer::CPlayerDocument::CreateAppUiL()
	{
	return new (ELeave) CPlayerAppUi;
	}

/** 
* Used by the framework classes to get access to the app's model (the Player).
*/
MoboPlayer::CPlayer * MoboPlayer::CPlayerDocument::Player()
	{
	return iPlayer;
	}
	
/** 
* Resets the model (the player).
*/
void MoboPlayer::CPlayerDocument::ResetModelL()
	{
	iPlayer->Reset();
	}

/**
* Resets the application model.
*/
void MoboPlayer::CPlayerDocument::NewDocumentL()
	{
	ResetModelL();
	}

/** 
* Overloading OpenFile is necessary to make the Avkon application
* framework to support handling of files using the SaveL, RestoreL 
* and StoreL methods inherited from the CEikDocument. It is enough to
* Call CEikDocument::OpenFileL() to enable this support.
* @param aDoOpen Shall the file be really opened.
* @param aFilename The file to open.
* @param aFs The file server session
* @returns The file store object of the opened file.
* @see CEikDocument::OpenFileL()
*/
CFileStore* MoboPlayer::CPlayerDocument::OpenFileL(TBool aDoOpen,const TDesC& aFilename,RFs& aFs)
	{
	return CEikDocument::OpenFileL(aDoOpen, aFilename, aFs);
	}

/** 
* Called by the framework when the file of the app needs to be opened.
* This is done when the application is launched.
* @param aStore The opened file store object of the application.
* @param aStreamDic The stream dictionary that contains the stream id's of the app.
* @see CEikDocument::RestoreL()
*/
void MoboPlayer::CPlayerDocument::RestoreL(const CStreamStore& aStore,const CStreamDictionary& aStreamDic)
	{
	TStreamId id = aStreamDic.At(TUid::Uid(KMoboPlayerAppUID));
	if (id != KNullStreamId)
		{
		iPlayer->RestoreL(aStore,id);
		}

	SetChanged(EFalse);
	}

/** 
* Called by the framework when the data of the app needs to be saved.
* This is done when document's SaveL is called by the app code.
* @param aStore The opened file store object of the application.
* @param aStreamDic The stream dictionary that contains the stream id's of the app.
* @see CEikDocument::StoreL()
*/
void MoboPlayer::CPlayerDocument::StoreL(CStreamStore& aStore,CStreamDictionary& aStreamDic) const
	{
	TStreamId id = iPlayer->StoreL(aStore);
	aStreamDic.AssignL(TUid::Uid(KMoboPlayerAppUID), id);
	}

	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerDocument.cpp,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/