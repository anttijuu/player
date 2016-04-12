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

// PlayList.h
//
// $Id: PlayList.h,v 1.3 2005/03/02 08:14:39 Antti Exp $

#ifndef __PLAYLIST_H
#define __PLAYLIST_H

#include <e32std.h>
#include <e32base.h>
#include <bamdesca.h>

#include "PlayerDefines.h"
#include "Recording.h"

namespace MoboPlayer 
{

/**
* Playlist contains recordings in a sorted order.
* You can retrieve add, remove and edit recordings in a playlist.
* Playlists can be saved and sorted (optional feature).
* Playlists implement the MDesCArray interface, so they can be used
* as a data source with a text list box model (see CTextListBoxModel::SetItemTextArray()
* and CPlayerPlayListContainer::ConstructL()).
* @see TRecording
* @see MDesCArray
* @todo Implement this class's public methods and 
* add necessary implementation details.
* @author Antti Juustila
* @version $Revision: 1.3 $
*/
	
class CPlayList : public CBase, public MDesCArray 
	{
public:
	/**
	* <b>OPTIONAL feature</b> The different sorting orders of the recordings in a playlist.
	* Sorting of recordings is an optional feature that you do not have to implement.
	* Leave this definition here though, since it is used in the UI code.
	*/
	enum TSortOrder 
		{
		EName, /**<Recordings sorted by name.*/
		EPerformer, /**<Recordings sorted by perfomer's name.*/
		EGenre, /**<Recordings sorted by the genre of the recording.*/
		EPerformed /**<Recordings sorted by the date of the performance.*/
		};
	
	IMPORT_C static CPlayList * NewL();	
	virtual ~CPlayList();
	
	IMPORT_C const TDesC & Name() const;	
	IMPORT_C const TDesC & Description() const;
	
	IMPORT_C void SetName(const TDesC & aName);
	IMPORT_C void SetDescription(const TDesC & aDescription);

	IMPORT_C void SetSelectedRecording(TInt aIndex);
	IMPORT_C TInt SelectedRecording() const;
	
	IMPORT_C const TRecording * GetSelectedRecording() const;
	IMPORT_C TRecording * GetSelectedRecording();
	
	TInt RecordingCount() const;
	
	IMPORT_C void AddRecordingL(const TRecording & aRecording);
	IMPORT_C void RemoveRecording(TInt aIndex);
	
	void Reset();
	
	IMPORT_C void Sort(TSortOrder aOrder);

	// From MDesCArray:
	virtual TInt MdcaCount() const;
	virtual TPtrC MdcaPoint(TInt aIndex) const;

	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	
// begin REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!
// This is needed only in the implementation of CPlayer.

	/** A new type name for arrays of recordings. */
	typedef CArrayFix<TRecording> CRecordingArray;

	/** To get the recordings.
	* @returns The array of recording objects.
	*/
	CRecordingArray * Recordings();
// end REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!
	
private:
	CPlayList();
	void ConstructL();
	
private:
	/** The name of the playlist.*/
	TBuf<KMaxPlayListNameLen>			iName;
	/** The description of the playlist. */
	TBuf<KMaxPlayListDescriptionLen>	iDescription;
	
	/** The array of recordings inside a playlist.*/
	CRecordingArray						*iRecordings;
	/** The current sort order of the recordings.*/
	TSortOrder 								iSortOrder;
	/** The index to the currently selected recording. */
	TInt										iSelectedRecording;
	};

} // namespace

#endif
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayList.h,v $
* Revision 1.3  2005/03/02 08:14:39  Antti
* Updated documentation.
*
* Revision 1.2  2005/03/02 07:30:59  Antti
* Changed return value of MdcaPoint from TPtrC16 to TPtrC.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/