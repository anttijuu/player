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

// PlayList.cpp
// (c) Antti Juustila, 2005
//
// $Id: PlayList.cpp,v 1.4 2005/06/09 08:50:16 Antti Exp $

#include <s32strm.h>

#include "PlayList.h"
#include "Player.pan"

/**
* Default constructor for non-leaving initialization.
*/
MoboPlayer::CPlayList::CPlayList() : iSortOrder(EName)
	{
	}

/** 
* Factory method for creating new playlists.
* @returns The new playlist object.
*/
EXPORT_C MoboPlayer::CPlayList * MoboPlayer::CPlayList::NewL()
	{
	CPlayList * self = new (ELeave) CPlayList;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
/** 
* 2nd phase constructor for leaving initialization.
*/
void MoboPlayer::CPlayList::ConstructL()
	{
	// Create the array for the recordings.
	iRecordings = new (ELeave) CArrayFixSeg<TRecording>(8);
	}
	
/** 
* Destructor, for releasing the allocated resources.
*/
MoboPlayer::CPlayList::~CPlayList()
	{
	delete iRecordings;
	}


/** 
* Get the name of the playlist.
* @returns The name of the playlist.
*/
EXPORT_C const TDesC & MoboPlayer::CPlayList::Name() const
	{
	return iName;
	}

/** 
* To get the description of the playlist.
* @returns The description of the playlist.
*/
EXPORT_C const TDesC & MoboPlayer::CPlayList::Description() const
	{
	return iDescription;
	}


/**
* Sets a new name to the playlist.
* @param aName The new name.
*/
EXPORT_C void MoboPlayer::CPlayList::SetName(const TDesC & aName)
	{
	iName = aName;
	}

/** 
* Sets a new description to the playlist.
* @param aDescription The new description.
*/
EXPORT_C void MoboPlayer::CPlayList::SetDescription(const TDesC & aDescription)
	{
	iDescription = aDescription;
	}

/** 
* Sets the current recording of the playlist by the index number.
* @param aIndex The index number of the recording to set as current.
*/	
EXPORT_C void MoboPlayer::CPlayList::SetSelectedRecording(TInt aIndex)
	{
	iSelectedRecording = aIndex;	
	}
	
/**
* To get the index of the currently selected recording.
* @returns The index of the currently selected recording.
*/
EXPORT_C TInt MoboPlayer::CPlayList::SelectedRecording() const
	{
	return iSelectedRecording;
	}

/** 
* To get the currently selected recording (const version).
* @returns The pointer to the currently selected recording.
*/
EXPORT_C const MoboPlayer::TRecording * MoboPlayer::CPlayList::GetSelectedRecording() const
	{
	if (iRecordings->Count())
		{
		return &((*iRecordings)[iSelectedRecording]);
		}
	return 0;
	}

/** 
* To get the currently selected recording (non-const version).
* @returns The pointer to the currently selected recording.
*/
EXPORT_C MoboPlayer::TRecording * MoboPlayer::CPlayList::GetSelectedRecording()
	{
	if (iRecordings->Count())
		{
		return &((*iRecordings)[iSelectedRecording]);
		}
	return 0;
	}


/** 
* To get the count of recordings in the playlist.
* @returns The count of recordings.
*/
TInt MoboPlayer::CPlayList::RecordingCount() const
	{
	return iRecordings->Count();
	}

/** Adds a new recording to the playlist. Leaves if could not add the recording.
* <b>Optionally</b>, add the recordings in a sorted order.
* @param aRecording The new recording to add. This is copied to the array.
*/
EXPORT_C void MoboPlayer::CPlayList::AddRecordingL(const TRecording & aRecording)
	{
	switch (iSortOrder)
		{
		case EName: 
			{ 
			TKeyArrayFix key(TRecording::NameKeyOffset(), ECmpFolded);
			iRecordings->InsertIsqAllowDuplicatesL(aRecording, key);
			break;
			}
		case EPerformer: 
			{
			TKeyArrayFix key(TRecording::PerformerKeyOffset(), ECmpFolded);
			iRecordings->InsertIsqAllowDuplicatesL(aRecording, key);
			break;
			}
		case EGenre: 
			{
			TKeyArrayFix key(TRecording::GenreKeyOffset(), ECmpFolded);
			iRecordings->InsertIsqAllowDuplicatesL(aRecording, key);
			break;
			}
		case EPerformed: 
			{
			TKeyArrayFix key(TRecording::PerformedKeyOffset(), ECmpTInt32);
			iRecordings->InsertIsqAllowDuplicatesL(aRecording, key);
			break;
			}
		default:
			{
			Panic(EMoboPlayerInvalidSwitchCaseValue);
			}
		}
	}

/** 
* Removes a recording from the playlist.
* @param aIndex The recording to remove, identified by the index number.
*/
EXPORT_C void MoboPlayer::CPlayList::RemoveRecording(TInt aIndex)
	{
	// Check index.
	if (aIndex >= 0 && aIndex < iRecordings->Count())
		{
		iRecordings->Delete(aIndex);
		iRecordings->Compress();
		}
	// If the removed one was the current one, change the current index.
	if (aIndex == iSelectedRecording)
		{
		iSelectedRecording--;
		if (iSelectedRecording < 0)
			{
			iSelectedRecording = 0;
			}
		}
	}

/** 
* Removes all recordings from the list.
*/
void MoboPlayer::CPlayList::Reset()
	{
	iRecordings->Reset();
	}


/** 
* Sorts the recordings.
* <b>OPTIONAL</b>, you do not have to implement this!<p>
* @param aOrder Specifies the ordering principle.
*/
EXPORT_C void MoboPlayer::CPlayList::Sort(TSortOrder aOrder)
	{
	iSortOrder = aOrder;
	switch (aOrder)
		{
		case EName: 
			{
			TKeyArrayFix key(TRecording::NameKeyOffset(), ECmpFolded);
			iRecordings->Sort(key);
			break;
			}
		case EPerformer: 
			{
			TKeyArrayFix key(TRecording::PerformerKeyOffset(), ECmpFolded);
			iRecordings->Sort(key);
			break;
			}
		case EGenre: 
			{
			TKeyArrayFix key(TRecording::GenreKeyOffset(), ECmpFolded);
			iRecordings->Sort(key);
			break;
			}
		case EPerformed: 
			{
			TKeyArrayFix key(TRecording::PerformedKeyOffset(), ECmpTInt32);
			iRecordings->Sort(key);
			break;
			}
		default:
			{
			Panic(EMoboPlayerInvalidSwitchCaseValue);
			}
		}
	}

// begin REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!
// This is needed only in the implementation of CPlayer.

MoboPlayer::CPlayList::CRecordingArray * MoboPlayer::CPlayList::Recordings()
	{
	return iRecordings;
	}

// end REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!

	
/** 
* Implements the MDesCArray interface by providing the number of descriptors
* in the descriptor array.
* This is needed in the UI so that we can give this object as a data source
* to the text list box model (see CEikTextListBox::Model() and
* CTextListBoxModel::SetItemTextArray().
* @returns The number of recordings in the playlist.
*/
TInt MoboPlayer::CPlayList::MdcaCount() const
	{
	return iRecordings->Count();
	}
	
/** 
* Implements the MDesCArray interface by providing the nth descriptor.
* This is needed in the UI so that we can give this object as a data source
* to the text list box model (see CEikTextListBox::Model() and
* CTextListBoxModel::SetItemTextArray().
* @returns A descriptor pointer to the nth recording's name in the playlist.
*/
TPtrC MoboPlayer::CPlayList::MdcaPoint(TInt aIndex) const
	{
	return (*iRecordings)[aIndex].Name();
	}

/** 
* Externalizes the playlist and it's recordings to a stream.
* @param aStream The stream to store the playlist.
* @see RWriteStream
*/
void MoboPlayer::CPlayList::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iName;
	aStream << iDescription;
	aStream.WriteInt32L(iSortOrder);
	aStream.WriteInt32L(iRecordings->Count());
	for (TInt counter = 0; counter < iRecordings->Count(); counter++)
		{
		aStream << (*iRecordings)[counter];
		}
	}
	
/** 
* Internalizes the playlist and related recordings from a stream.
* @param aStream The stream to read the data from.
* @see RReadStream
*/
void MoboPlayer::CPlayList::InternalizeL(RReadStream& aStream)
	{
	aStream >> iName;
	aStream >> iDescription;
	iSortOrder = (TSortOrder)aStream.ReadInt32L();
	TInt recCount = aStream.ReadInt32L();
	TRecording rec;
	for (TInt counter = 0; counter < recCount; counter++)
		{
		aStream >> rec;
		iRecordings->AppendL(rec);
		}
	}

	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayList.cpp,v $
* Revision 1.4  2005/06/09 08:50:16  Antti
* Added the compression of the array after removing an item from the array.
*
* Revision 1.3  2005/03/02 08:14:05  Antti
* Updated documentation.
*
* Revision 1.2  2005/03/02 07:27:50  Antti
* Added braces for if blocks, handles now invalid case values with a panic, changed return value of MdcaPoint from TPtrC16 to TPtrC.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/