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

// Player.cpp
// (c) Antti Juustila, 2005
//
// $Id: Player.cpp,v 1.8 2005/06/09 08:57:24 Antti Exp $

#include <bautils.h>
#include <s32strm.h>

#include "Player.h"
#include "PlayerObserver.h"

/**
* Factory method for creating the CPlayer.
* @returns The new allocated player object.
*/	
EXPORT_C MoboPlayer::CPlayer * MoboPlayer::CPlayer::NewL()
	{
	CPlayer * self = new (ELeave) CPlayer;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
/** 
* The second phase constructor for the CPlayer initializes the things
* that might leave.
*/
void MoboPlayer::CPlayer::ConstructL()
	{
#ifdef _DEBUG
	// Create some test data
//	PopulateTestDataL();
#endif
	// Open file server session, so we can check if
	// the sound file to be played really exist.
	User::LeaveIfError(iFsSession.Connect());
	}
	
/**
* Default constructor initializes the non-leaving parts of CPlayer.
*/
MoboPlayer::CPlayer::CPlayer() : iState(EIdle), iVolume(-1)
	{
	// Empty	
	}
	
/**
* Destructor, deallocate reserved resources here.
*/
EXPORT_C MoboPlayer::CPlayer::~CPlayer()
	{
	iPlayLists.ResetAndDestroy();
	iFsSession.Close();
	delete iPlayer;
	}

/**
* Sets the observer that observers the CPlayer. Observer should be notified
* of the playing of the recordings. In UI, there will be a control that
* displays animation as the recording is played. Player will tell this observer
* when the playing starts, stops or there is an error in the playing.
* @param aObserver The observer of CPlayer.
*/
EXPORT_C void MoboPlayer::CPlayer::SetObserver(MPlayerObserver * aObserver)
	{
	iObserver = aObserver;
	}

/**
* Adds a new playlist to the list of playlists.
* Leaves if the new item cannot be added.
* @param aPlayList The new playlist to be added.
*/
EXPORT_C void MoboPlayer::CPlayer::AddPlayListL(CPlayList * aPlayList)
	{
	User::LeaveIfError(iPlayLists.Append(aPlayList));
	}
	
/**
* Removes the currently selected playlist from the list of playlists.
*/
EXPORT_C void MoboPlayer::CPlayer::RemoveCurrentPlayListL()
	{
	if (iCurrentPlayListIndex >= 0 && iCurrentPlayListIndex < iPlayLists.Count())
		{
		// Get the object and delete it.
		CPlayList * item = iPlayLists[iCurrentPlayListIndex];
		delete item;
		// Remove the pointer from the array
		iPlayLists.Remove(iCurrentPlayListIndex);
		// Compress the array
		iPlayLists.Compress();
		// Update selected index.
		if (--iCurrentPlayListIndex < 0)
			{
			iCurrentPlayListIndex = 0;
			}
		SelectPlayListL(iCurrentPlayListIndex);
		}
	}

/**
* The number of recordings in the currently selected playlist.
* @returns The number of recordings in the current playlist.
*/
EXPORT_C TInt MoboPlayer::CPlayer::RecordingCount() const
	{
	if (iPlayLists.Count() > 0)
		{
		return (iPlayLists[iCurrentPlayListIndex])->RecordingCount();
		}
	return 0;
	}

/**
* Selects a new current playlist by index number.
* @param aPlayListIndex The playlist that is now current.
*/
EXPORT_C void MoboPlayer::CPlayer::SelectPlayListL(TInt aPlayListIndex)
	{
	// Check index for out of bounds error.
	if (aPlayListIndex >= 0 && aPlayListIndex < iPlayLists.Count())
		{
		// Set the index of current playlist
		iCurrentPlayListIndex = aPlayListIndex;
		if (iPlayLists.Count() > 0)
			// Set the current recording
			iPlayLists[iCurrentPlayListIndex]->SetSelectedRecording(0);
		}
	}

/**
* Get the current playlist object.
* @returns The current playlist object, null if there is no current playlist.
*/
EXPORT_C MoboPlayer::CPlayList * MoboPlayer::CPlayer::CurrentPlayList()
	{
	if (iPlayLists.Count() > 0)
		return iPlayLists[iCurrentPlayListIndex];
	else
		return 0; // No playlists.
	}

/**
* Get the current playlist's description.
* @param aDescription Copy the playlist's description here,
* or an empty descriptor if no current playlist exists.
*/
EXPORT_C void MoboPlayer::CPlayer::GetCurrentPlayListDescription(TDes & aDescription) const
	{
	if (iCurrentPlayListIndex >= 0 && iCurrentPlayListIndex < iPlayLists.Count())
		{
		aDescription = iPlayLists[iCurrentPlayListIndex]->Description();
		}
	else
		{
		aDescription = KNullDesC;
		}
	}

/**
* Get the current recording's details.
* @param aDetails The descriptor to save the details to. Copy the
* recording's details here, or an empty descriptor if no current
* recording.
* @see TRecording::GetDetails()
*/
EXPORT_C void MoboPlayer::CPlayer::GetCurrentRecordingDetails(TDes & aDetails) const
	{
	if (iCurrentPlayListIndex >= 0 && iCurrentPlayListIndex < iPlayLists.Count())
		{
		const TRecording * rec = iPlayLists[iCurrentPlayListIndex]->GetSelectedRecording();
		if (rec)
			{
			rec->GetDetails(aDetails);
			return;
			}
		}
	aDetails = KNullDesC;
	}
	
/**
* Stops the player if it is playing, and notifies the observer that the player has ended.
*/
EXPORT_C void MoboPlayer::CPlayer::Stop()
	{
	if (iPlayer && iState == EPlaying)
		{
		iPlayAll = EFalse;
		iPlayer->Stop();
		iState = EIdle;
		// Notify observer too.
		if (iObserver)
			iObserver->PlayingEnded();
		}
	}

/** 
* Resets the player.
* Reset clears all playlists and recordings and stops playing.
*/	
EXPORT_C void MoboPlayer::CPlayer::Reset()
	{
	Stop();
	iPlayLists.ResetAndDestroy();
	}
	
/**
* Plays all recordings in the currently selected playlist.
* Use CMdaAudioPlayerUtility to start the playing. Get the first recording
* and use that with CMdaAudioPlayerUtility. After the first
* file has been played, get the next from the current playlist and play it similarily.
* Hint: reuse CPlayer::PlayL() when implementing this functionality.
* @see CPlayer::PlayL()
*/
EXPORT_C void MoboPlayer::CPlayer::PlayAllL()
	{
	iPlayAll = ETrue;
	iCurrentlyPlayingIndex = 0;
	// Get the current playlist
	CPlayList * list = iPlayLists[iCurrentPlayListIndex];
	if (list)
		{
		// If there are recordings...
		if (list->RecordingCount() > 0)
			{
			// ...get the first recording...
			const TRecording & tmp =  (*list->Recordings())[iCurrentlyPlayingIndex];
			// ..and play it.
			PlayL(tmp);
			// Playing the next recording is initiated in MapcPlayComplete.
			}
		}
	}

/**
* Plays the recording by recording index number in the current playlist.
* Use CMdaAudioPlayerUtility to play the file related to the current recording.
* Call CMdaAudioPlayerUtility::NewFilePlayerL(), which will initialize the player with
* the file. As the initialization is finished, CMdaAudioPlayerUtility calls MapcInitComplete().
* <b>There</b> you can then <b>actually</b> start playing the file using CMdaAudioPlayerUtility::Play(). 
* Also notify the observer (MPlayerObserver) that playing has now started.
* Note that if playing is ongoing, and you want to start playing another file, first you have to 
* -stop playing the current file
* -delete the CMdaAudioPlayerUtility object
* -create a new player object using CMdaAudioPlayerUtility::NewFilePlayerL.
* Note also, that you should check that the file to be played really exists.
* You can do this, for example, by using BaflUtils::FileExists().
* @param aRecording The index number of the recording in current playlist to play.
*/
EXPORT_C void MoboPlayer::CPlayer::PlayL(TInt aRecording)	
	{
	// The first part here checks that if the playing is going on now.
	// If it is, stop it, if user selects the same recording again
	// while it is playing. This enables the user to stop playing the currently
	// playing recording.
	TBool justStoppedPlaying = EFalse;
	if (iPlayer && iState == EPlaying)
		{
		if (aRecording == iCurrentlyPlayingIndex)
			justStoppedPlaying = ETrue;
		Stop();
		}
	// If we just didn't stop playing, then start playing.
	if (!justStoppedPlaying)
		{
		iCurrentlyPlayingIndex = aRecording;
		CPlayList * list = iPlayLists[iCurrentPlayListIndex];
		const TRecording & tmp =  (*list->Recordings())[iCurrentlyPlayingIndex];
		PlayL(tmp);
		}
	}

/**
* Plays a recording.
* Private implementation detail, not documented for the students.
* Issues relating to this method have been documented in
* CPlayer::PlayL(TInt aRecording).
* @param aRecording a recording object specifying the recording to play.
*/
void MoboPlayer::CPlayer::PlayL(const TRecording & aRecording)
	{
	// No need to check if playing is going on and stop it, since
	// this method is only called from PlayL(TInt) which does all this.
	
	// Check if the file exists.
	if (BaflUtils::FileExists(iFsSession, aRecording.FileName()))
		{
		delete iPlayer;
		iPlayer = 0;
		iPlayer = CMdaAudioPlayerUtility::NewFilePlayerL(aRecording.FileName(), *this);
		// MapcInitComplete is called when the initialization is finished. Start playing there.
		}
	else // useful detail; if couldn't play the song, try next one.
		{
		// When playing all: file didn't exist, try playing the next recording then (if any).
		if (iPlayAll && ++iCurrentlyPlayingIndex < iPlayLists[iCurrentPlayListIndex]->Recordings()->Count())
			{
			PlayL(iCurrentlyPlayingIndex);
			}
		else
			{
			// Error, file not found, not playing the next one.
			if (iObserver)
				iObserver->HandlePlayerError(KErrNotFound);
			}
		}
	}

/** 
* Is the player currently playing or not.
* @returns ETrue, if playing now, EFalse if not.
*/
EXPORT_C TBool MoboPlayer::CPlayer::IsPlaying() const
	{
	return (iState == EPlaying);
	}

/**
* Sets the volume of the sound. 
* Use CMdaAudioPlayerUtility::SetVolume() to set the volume. 
* Note that if you are not playing at the moment, the player
* object should be null, so you need to save this value
* to a member variable and use it to set the volume after creating
* the player object when the playing begins.
* @param aVolume The new volume to be set.
*/
EXPORT_C void MoboPlayer::CPlayer::SetVolume(TInt aVolume)
	{
	if (iPlayer)
		{
		TInt maxVolume = iPlayer->MaxVolume();
		if (aVolume >= 0 && aVolume < maxVolume)
			{
			iVolume = aVolume;
			iPlayer->SetVolume(iVolume);
			}
		}
	}

/**
* Nudges the volume up by one step.
* Use CMdaAudioPlayerUtility::SetVolume() to set the volume.
* See the note on CPlayer::SetVolume() on using the
* CMdaAudioPlayerUtility::SetVolume()
*/
EXPORT_C void MoboPlayer::CPlayer::VolumeUp()
	{
	if (iPlayer)
		{
		TInt maxVolume = iPlayer->MaxVolume();
		SetVolume(iVolume + (maxVolume / 10));
		}
	}

/**
* Nudges the volume down by one step.
* Use CMdaAudioPlayerUtility::SetVolume() to set the volume.
* See the note on CPlayer::SetVolume() on using the
* CMdaAudioPlayerUtility::SetVolume()
*/
EXPORT_C void MoboPlayer::CPlayer::VolumeDown()
	{
	if (iPlayer)
		{
		TInt maxVolume = iPlayer->MaxVolume();
		SetVolume(iVolume - (maxVolume / 10));
		}
	}

/** 
* The current value of volume. If playing is not
* going on right now, CMdaAudioPlayerUtility object is null.
* In this case, return the member variable that has the currently
* set volume. Otherwise, just use CMdaAudioPlayerUtility::GetVolume().
* Note: CMdaAudioPlayerUtility::GetVolume() is not available on older
* Series 60 SDKs, in that case just return CPlayer's volume member variable.
* @returns The current volume.
*/
EXPORT_C TInt MoboPlayer::CPlayer::Volume() const
	{
	return iVolume;
	}

/**
* Called when the sound file to be played has been initialized by calling
* CMdaAudioPlayerUtility::NewFilePlayerL().
* Inherited from the interface MMdaAudioPlayerCallback, called by CMdaAudioPlayerUtility.
* Now when the sound file has been successfully initialized, the playing can commence.
* use CMdaAudioPlayerUtility::Play() to start playing. Set the volume first.
* Notify the observer about playing started or about the error.
* @param aError Other than KErrNone, if initialization failed.
* @param aDuration The duration of the sound file in microseconds.
*/
void MoboPlayer::CPlayer::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration)
	{
	if (aError == KErrNone)
		{
		// Initialization succeeded, start playing
		iDuration = aDuration;
		iState = EPlaying;
		SetVolume(iVolume);
		iPlayer->Play();
		// Notify the observer too.
		if (iObserver)
			iObserver->PlayingStarted(iCurrentlyPlayingIndex, aDuration.Int64().GetTInt() / 1000000);
		} 
	else
		{
		// Initialization failed, cannot play. Do not try to play the next recording.
		iState = EError;
		iDuration = 0;
		if (iObserver)
			iObserver->HandlePlayerError(aError);
		}
	}
	
/**
* Called when the sound file has been played.
* Inherited from the interface MMdaAudioPlayerCallback, called by CMdaAudioPlayerUtility.
* If user selected to play all, get next recording and play it's sound file now.
* Also notify the observer that playing has ended (or about an error).
* @param aError If playing finished OK, value is KErrNone.
*/
void MoboPlayer::CPlayer::MapcPlayComplete(TInt aError)
	{
	if (aError == KErrNone)
		{
		// Playing completed successfully.
		iState = EIdle;
		// Notify the observer.
		if (iObserver)
			{
			iObserver->PlayingEnded();
			}
		if (iPlayAll)
			{
			// If playing all, go to next recording and play it.
			TInt nextIndex = iCurrentlyPlayingIndex + 1;
			if (nextIndex < iPlayLists[iCurrentPlayListIndex]->Recordings()->Count())
				{
				// MapcPlayComplete cannot leave so must handle it here.
				TRAPD(err, PlayL(nextIndex));
				if (err != KErrNone)
					{
					// Error in playing the recording.
					iState = EError;
					if (iObserver)
						{
						iObserver->HandlePlayerError(err);
						}
					}
				}
			else
				{
				// Played all.
				iCurrentlyPlayingIndex = 0;
				iPlayAll = EFalse;
				}
			}
		} 
	else
		{
		// Error in playing the recording.
		iPlayAll = EFalse;
		iState = EError;
		if (iObserver)
			{
			iObserver->HandlePlayerError(aError);
			}
		}
	}
	
/**
* Implements the MDesCArray interface by providing the number of descriptors
* in the descriptor array (number of playlists in the playlist array).
* This is used in UI, so that this object can be used as a data source for
* the list box model (see how CTextListBoxModel::SetItemTextArray()
* is used in CPlayerPlayListContainer::ConstructL()).
* @returns The number playlists.
*/
EXPORT_C TInt MoboPlayer::CPlayer::MdcaCount() const
	{
	return iPlayLists.Count();
	}
	
/**
* Implements the MDesCArray interface by providing the nth descriptor.
* This is used in UI, so that this object can be used as a data source for
* the list box model (see how CTextListBoxModel::SetItemTextArray()
* is used in CPlayerPlayListContainer::ConstructL()).
* @returns A pointer descriptor to the name of the nth playlist object.
*/
EXPORT_C TPtrC MoboPlayer::CPlayer::MdcaPoint(TInt aIndex) const
	{
	return iPlayLists[aIndex]->Name();
	}

/**
* Restores the player data from a stream in a stream store.
* Called from CPlayerDocument::RestoreL().
* -Open a stream from the store using the stream id, using RStoreReadStream.
* -Internalize the CPlayer from the stream
* -Close the stream using CleanupStack::PopAndDestroy()
* @param aStore The store containing the player stream.
* @param aStreamId The id of the stream containing the player data.
*/
EXPORT_C void MoboPlayer::CPlayer::RestoreL(const CStreamStore& aStore, const TStreamId & aStreamId)
	{
	RStoreReadStream stream;
	stream.OpenLC(aStore,aStreamId);
	InternalizeL(stream);
	CleanupStack::PopAndDestroy(); // stream
	}

/** 
* Stores the player data into a stream in a store.
* Called from CPlayerDocument::StoreL().
* -Create a new stream using RStoreWriteStream::CreateLC().
* -Externalize the CPlayer into the stream
* -Commit the stream
* -return the stream's id.
* @param aStore The store to save the stream into.
* @returns The id of the stream where the data was stored into.
*/
EXPORT_C TStreamId MoboPlayer::CPlayer::StoreL(CStreamStore& aStore) const
	{
	RStoreWriteStream stream;
	TStreamId id = stream.CreateLC(aStore);
	ExternalizeL(stream);
	stream.CommitL();
	CleanupStack::PopAndDestroy(); // stream
	return id;
	}

/**
* Externalizes the player into a stream.
* Externalize the volume, playlist count and each playlist object.
* @param aStream To externalize the player into. 
*/
void MoboPlayer::CPlayer::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(iVolume);
	aStream.WriteInt32L(iPlayLists.Count());
	for (TInt counter = 0; counter < iPlayLists.Count(); counter++)
		{
		aStream << *(iPlayLists[counter]);
		}
	}

/** Internalizes the player from a stream.
* Internalize the volume, playlist count and create
* and internalize playlist objects, adding them to the list.
* @param aStream To read the data of the player from. */
void MoboPlayer::CPlayer::InternalizeL(RReadStream& aStream)
	{
	iVolume = aStream.ReadInt32L();
	TInt playListCount = aStream.ReadInt32L();
	for (TInt counter = 0; counter < playListCount; counter++)
		{
		CPlayList *list = CPlayList::NewL();
		CleanupStack::PushL(list);
		aStream >> *list;
		User::LeaveIfError(iPlayLists.Append(list));
		CleanupStack::Pop();
		}
	}

#ifdef _DEBUG
/** 
* Utility method to populate the arrays with playlists 
* and recordings for testing. 
*/
void MoboPlayer::CPlayer::PopulateTestDataL()
	{
	CPlayList *list = CPlayList::NewL();
	CleanupStack::PushL(list);
	TTime now;
	now.HomeTime();
	
	list->SetName(_L("Short recordings"));
	list->SetDescription(_L("Testing a list"));

	// Have to allocate TRecordings dynamically here, since some SDK:s cause a 
	// stack overflow because of too many big TRecording objects on the stack
	// at the same time, if allocating many TRecordings on the stack as usual.
	
	// Using _L is deprecated and not recommended, but here in test code we don' care...
	TRecording * rec = new (ELeave) TRecording(_L("Too much time on my hands"), _L("Rock"), _L("Styx"), now, 20);
	CleanupDeletePushL(rec);
	rec->SetFileName(_L("c:\\nokia\\Sounds\\Digital\\2muchtime.wav"));
	list->AddRecordingL(*rec);
	CleanupStack::PopAndDestroy(); // rec
	
	rec = new (ELeave) TRecording(_L("Rocket countdown"), _L("Recording"), _L("NASA"), now, 20);
	CleanupDeletePushL(rec);
	rec->SetFileName(_L("c:\\nokia\\Sounds\\Digital\\rocketcountdown.wav"));
	list->AddRecordingL(*rec);
	CleanupStack::PopAndDestroy(); // rec

	rec = new (ELeave) TRecording(_L("Don't worry"), _L("Recording"), _L("Movie clip"), now, 13);
	CleanupDeletePushL(rec);
	rec->SetFileName(_L("c:\\nokia\\Sounds\\Digital\\whip 2.wav"));
	list->AddRecordingL(*rec);
	CleanupStack::PopAndDestroy(); // rec

	User::LeaveIfError(iPlayLists.Append(list));
	CleanupStack::Pop(); // list

	//////////////////////////
	list = CPlayList::NewL();
	CleanupStack::PushL(list);
	now.HomeTime();
	
	list->SetName(_L("From unknown sources"));
	list->SetDescription(_L("Testing a second list"));

	rec = new (ELeave) TRecording(_L("Angels"), _L("Pop"), _L("Unknown"), now, 12);
	CleanupDeletePushL(rec);
	rec->SetFileName(_L("c:\\nokia\\Sounds\\Digital\\angels.wav"));
	list->AddRecordingL(*rec);
	CleanupStack::PopAndDestroy(); // rec
	
	rec = new (ELeave) TRecording(_L("Yupi Kay Aye"), _L("Folk"), _L("Unknown"), now, 252);
	CleanupDeletePushL(rec);
	rec->SetFileName(_L("c:\\nokia\\Sounds\\Digital\\YupiKayAye.wav"));
	list->AddRecordingL(*rec);
	CleanupStack::PopAndDestroy(); // rec

	rec = new (ELeave) TRecording(_L("Human motorcycle"), _L("Humor"), _L("Unknown"), now, 31);
	CleanupDeletePushL(rec);
	rec->SetFileName(_L("c:\\nokia\\Sounds\\Digital\\Human Motorcycle.wav"));
	list->AddRecordingL(*rec);
	CleanupStack::PopAndDestroy(); // rec

	rec = new (ELeave) TRecording(_L("Mission impossible"), _L("Humor"), _L("Unknown"), now, 21);
	CleanupDeletePushL(rec);
	rec->SetFileName(_L("c:\\nokia\\Sounds\\Digital\\Mission Impossible.wav"));
	list->AddRecordingL(*rec);
	CleanupStack::PopAndDestroy(); // rec

	User::LeaveIfError(iPlayLists.Append(list));
	CleanupStack::Pop(); // list
	}
#endif
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: Player.cpp,v $
* Revision 1.8  2005/06/09 08:57:24  Antti
* Added the compression of the array after removing an item from the array.
* Also fixed a small bug in getting the recording count.
*
* Revision 1.7  2005/04/13 12:13:42  Antti
* Removed unnecessary stopping in PlayL(TRecording &), since
* the check is done already in PlayL(TIntI and the former is called
* only from there.
*
* Revision 1.6  2005/04/13 11:53:31  Antti
* Fixed a bug where playall was left on, when stopping the playing.
*
* Revision 1.5  2005/04/08 16:59:37  Antti
* Fixed a bug in GetCurrentRecordingDetails.
*
* Revision 1.4  2005/03/14 14:33:27  Antti
* Fixed a bug in PlayAllL, where PlayL was called with param tmp.Name(). Should be tmp only. Using tmp.Name() resulted a call into TRecording constructor with TDesC parameter, creating a temporary recording with only a name attribute. Changing that constructor to explicit would be something to consider too.
*
* Revision 1.3  2005/03/02 13:42:41  Antti
* Added a missing comment of Stop and fixed the getting volume, since S60 SDK 1.2 does not have the CMdaAudioPlayerUtility::GetVolume.
*
* Revision 1.2  2005/03/02 07:24:55  Antti
* Changed RemoveCurrentPlayList to RemoveCurrentPlayListL, handled leave in MapcPlayComplete with TRAPD, changed return value of MdcaPoint from TPtrC16 to TPtrC.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/