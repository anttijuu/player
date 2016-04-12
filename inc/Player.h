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

// Player.h
//
// $Id: Player.h,v 1.2 2005/03/02 07:29:22 Antti Exp $

#ifndef __PLAYER_H
#define __PLAYER_H

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <bamdesca.h>
#include <s32std.h>
#include <MdaAudioSamplePlayer.h>

#include "PlayerDefines.h"
#include "PlayList.h"


/**
* The namespace <b>MoboPlayer</b> contains all the classes that form the 
* exercise work of the Mobile Systems Programming course for the Spring 2005.<p>
* Some of the classes are given already implemented to the students. Some of
* the classes are only given partially: students will get header files with class definitions.
* The class definitions contain only the public interface of the classes.<p>
* Students are required to implement the public interface and add protected and private
* properties (both methods and data) to the classes so that the functionality of the
* application fulfills the requirements for the exercise work.<p>
* Some of the features are optional. See the more detailed description from
* http://www.tol.oulu.fi/~antti/Mobo2005/
*/

namespace MoboPlayer 
{

class MPlayerObserver;

/**
* The player class is the main interface of the application engine to the user side.
* The app document instantiates the CPlayer and cooperates with it to save app data.
* UI objects (views, app ui) pass user commands to the player, and the view shows the
* state of the app to the user.
* CPlayer implements the interface MDesCArray, which means that it can be used as
* a data source for listbox models (see CEikTextListBox).
* CPlayer implements MMdaAudioPlayerCallback, so it can handle the playing initialized
* and the playing completed events notified by CMdaAudioPlayerUtility.
* CPlayer also notifies of the play events (start, stop, error) to MPlayerObserver, which
* is set by calling SetObserver().
* @see CMdaAudioPlayerUtility
* @see MMdaAudioPlayerCallback
* @see MDesCArray
* @see MPlayerObserver
* @todo Implement this class's public methods and 
* add necessary implementation details.
* @author Antti Juustila
* @version $Revision: 1.2 $
*/

class CPlayer : public CBase, public MMdaAudioPlayerCallback, public MDesCArray 
	{
public:
	IMPORT_C static CPlayer * NewL();	
	IMPORT_C virtual ~CPlayer();

	IMPORT_C void AddPlayListL(CPlayList * aPlayList)	;
	IMPORT_C void RemoveCurrentPlayListL();
	IMPORT_C TInt RecordingCount() const;
	IMPORT_C void SelectPlayListL(TInt aPlayListIndex);
	IMPORT_C CPlayList * CurrentPlayList();
	IMPORT_C void GetCurrentPlayListDescription(TDes & aDescription) const;
	IMPORT_C void GetCurrentRecordingDetails(TDes & aDetails) const;
	
	IMPORT_C void Reset();
	IMPORT_C void PlayAllL();
	IMPORT_C void PlayL(TInt aRecording);
	IMPORT_C void Stop();
	IMPORT_C TBool IsPlaying() const;
	
	IMPORT_C void SetVolume(TInt aVolume);
	IMPORT_C void VolumeUp();
	IMPORT_C void VolumeDown();
	IMPORT_C TInt Volume() const;
	
	// From MMdaAudioPlayerCallback:
	virtual void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
	virtual void MapcPlayComplete(TInt aError);

	// From MDesCArray:
	IMPORT_C virtual TInt MdcaCount() const;
	IMPORT_C virtual TPtrC MdcaPoint(TInt aIndex) const;
	
	IMPORT_C void SetObserver(MPlayerObserver * aObserver);

	IMPORT_C void RestoreL(const CStreamStore& aStore, const TStreamId & aStreamId);
	IMPORT_C TStreamId StoreL(CStreamStore& aStore) const;

private:

	/**
	* The states of the player. */
	enum TState 
		{
		EIdle, /**<Idle, not playing anything. */
		EPlaying, /**<Playing a song right now. */
		EError, /**<Error, something went wrong in playing. */
		};

	CPlayer();
	void ConstructL();
	
	void PlayL(const TRecording & aRecording);
	
	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	
#ifdef _DEBUG
	void PopulateTestDataL();
#endif

private:
	
	/** The array of playlists. */
	RPointerArray<CPlayList>				iPlayLists;
	/** The index of the currently selected playlist.*/
	TInt											iCurrentPlayListIndex;
	/** Should all recordings be played or only the current one. */
	TBool											iPlayAll;	
	/** The state of the player. 
	* @see TState
	*/
	TState										iState;
	/** The index of the currently playing recording. */
	TInt											iCurrentlyPlayingIndex;
	
	/** The observer observing the player. */
	MPlayerObserver							* iObserver;
	
	/** Current volume to play with. */
	TInt											iVolume;	
	/** The duration of the currently playing recording. */
	TTimeIntervalMicroSeconds				iDuration;
	/** The player object that actually plays the recordings. */
	CMdaAudioPlayerUtility					* iPlayer;
	/** The file server session object used to check if the sound files really exist. */
	RFs											iFsSession;
	};

} // namespace

#endif
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: Player.h,v $
* Revision 1.2  2005/03/02 07:29:22  Antti
* Changed RemoveCurrentPlayList to RemoveCurrentPlayListL, changed return value of MdcaPoint from TPtrC16 to TPtrC.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/