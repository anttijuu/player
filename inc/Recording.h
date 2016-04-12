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

// Recording.h
//
// $Id: Recording.h,v 1.3 2005/04/08 16:57:46 Antti Exp $

#ifndef __RECORDING_H
#define __RECORDING_H

#include <e32std.h>

#include "PlayerDefines.h"

class RWriteStream;
class RReadStream; 

namespace MoboPlayer 
{

/**
* This class is responsible for maintaining information about recordings.
* Recordings are contained in a playlist. Each recording has a name, performer,
* genre, time of the performation and the length of the performation in seconds.<p>
* A recording could be, for example Hey Joe (name), Jimi Hendrix (performer), Blues (genre),
* 25.6.1967 (time of performation), 6.34 (the length or duration of the recording).<p>
* TRecordings are big objects, so not too many of them should be on the stack
* at the same time.
* @see CPlayList
* @todo Implement this class's public methods and 
* add necessary implementation details.
* @author Antti Juustila
* @version $Revision: 1.3 $
*/	

class TRecording 
	{
public:
	IMPORT_C TRecording();
	explicit TRecording(const TDesC & aName);
	TRecording(const TDesC & aName, const TDesC & aGenre, const TDesC & aPerformer, const TTime & aPerformed, TTimeIntervalSeconds aLength = 0);

	IMPORT_C TRecording & operator = (const TRecording & aRecording);
	
	IMPORT_C const TDesC & Name() const;
	IMPORT_C const TDesC & Performer() const;
	IMPORT_C TTimeIntervalSeconds Length() const;
	IMPORT_C const TDesC & Genre() const;
	IMPORT_C const TTime & Performed() const;
	IMPORT_C const TDesC & FileName() const;

	IMPORT_C void GetDetails(TDes & aDetails) const;
	
	IMPORT_C void SetName(const TDesC & aName);
	IMPORT_C void SetPerformer(const TDesC & aPerformer);
	IMPORT_C void SetLength(TTimeIntervalSeconds aLength);
	IMPORT_C void SetGenre(const TDesC & aGenre);
	IMPORT_C void SetPerformed(const TTime & aPerformed);
	IMPORT_C void SetFileName(const TDesC & aFileName);
	
// begin REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!
// This is needed only if the students want to implement the optional sorting
// feature to the playlists.
	
	static TBool MatchName(const TRecording & aFirst, const TRecording & aSecond);
	
	static TInt CompareNames(const TRecording & aFirst, const TRecording & aSecond);
	static TInt CompareGenre(const TRecording & aFirst, const TRecording & aSecond);
	static TInt ComparePerformer(const TRecording & aFirst, const TRecording & aSecond);
	static TInt ComparePerformed(const TRecording & aFirst, const TRecording & aSecond);

	static TInt NameKeyOffset();
	static TInt GenreKeyOffset();
	static TInt PerformerKeyOffset();
	static TInt PerformedKeyOffset();
	
// end REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!

	void ExternalizeL(RWriteStream& aStream) const;
	void InternalizeL(RReadStream& aStream);
	
private:
	/** The name of the recording.*/
	TBuf<KMaxRecordingNameLen>		iName;
	
	/** The name of the performer.*/
	TBuf<KMaxPerformerNameLen>		iPerformer;
	
	/** The length of the recording in seconds.*/
	TTimeIntervalSeconds				iLength;
	
	/** The genre of the recording.*/
	TBuf<KMaxGenreNameLen>			iGenre;
	
	/** The date of the recording's performance.*/
	TTime									iPerformed;
	
	/** The full file name of the recording, with path info, including the drive letter.*/
	TFileName							iFileName;
	};

} // namespace

#endif
	
/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: Recording.h,v $
* Revision 1.3  2005/04/08 16:57:46  Antti
* Changed the TRecording(const TDesC &) to explicit.
*
* Revision 1.2  2005/03/02 08:14:39  Antti
* Updated documentation.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/