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

// Recording.cpp
// (c) Antti Juustila, 2005
//
// $Id: Recording.cpp,v 1.4 2005/04/08 16:58:22 Antti Exp $

#include <s32strm.h>
#include <f32file.h>

#include "Recording.h"


/** 
* Default constructor for creating empty recordings.
* Initialize member variable data if necessary. This is the
* only constructor that is required to be exported to the application UI side.
*/
EXPORT_C MoboPlayer::TRecording::TRecording()
	: iLength(0)
	{
	iPerformed.HomeTime();
	}

/**
* Creates a recording with a name only.
*/
MoboPlayer::TRecording::TRecording(const TDesC & aName)
	: iName(aName), iLength(0)
	{
	iPerformed.HomeTime();
	}

/**
* Creates a recording by providing values for all of the member variables.
*/
MoboPlayer::TRecording::TRecording(const TDesC & aName, const TDesC & aGenre, const TDesC & aPerformer, const TTime & aPerformed, TTimeIntervalSeconds aLength)
	: iName(aName), iPerformer(aPerformer), iLength(aLength), iGenre(aGenre), iPerformed(aPerformed)
	{
	
	}

/**
* Assignment operator for copying recordings.
*/
EXPORT_C MoboPlayer::TRecording & MoboPlayer::TRecording::operator = (const TRecording & aRecording)
	{
	if (this == &aRecording)
		{
		return *this;
		}
	iLength = aRecording.iLength;
	iName = aRecording.iName;
	iGenre = aRecording.iGenre;
	iPerformer = aRecording.iPerformer;
	iPerformed = aRecording.iPerformed;
	iFileName = aRecording.iFileName;
	return *this;
	}

/** 
* Provides the name of the recording.
* @returns The name of the recording.
*/
EXPORT_C const TDesC & MoboPlayer::TRecording::Name() const
	{
	return iName;
	}

/** 
* Provides the length of the recording.
* @returns The length of the recording in seconds.
*/
EXPORT_C TTimeIntervalSeconds MoboPlayer::TRecording::Length() const
	{
	return iLength;
	}


/** 
* Provides the genre of the recording.
* @returns The genre of the recording.
*/
EXPORT_C const TDesC & MoboPlayer::TRecording::Genre() const
	{
	return iGenre;
	}

/** 
* Provides the artist of the recording.
* @returns The name of the artist.
*/
EXPORT_C const TDesC & MoboPlayer::TRecording::Performer() const
	{
	return iPerformer;
	}

/** 
* Provides the date of the recording.
* @returns The date of the recording.
*/
EXPORT_C const TTime & MoboPlayer::TRecording::Performed() const
	{
	return iPerformed;
	}

/** 
* Provides the file name of the recording.
* @returns The full file name with path, of the recording.
*/
EXPORT_C const TDesC & MoboPlayer::TRecording::FileName() const
	{
	return iFileName;
	}

/** 
* Provides detailed information of the recording.
* Details can contain, for example, the performer, genre and duration
* of the recording (e.g. "Jimi Hendrix, Blues, 6.34").
* @param aDetails Will contain the details of the recording when called.
*/
EXPORT_C void MoboPlayer::TRecording::GetDetails(TDes & aDetails) const
	{
	_LIT(KSeparator, ", ");
	aDetails = iPerformer;
	aDetails.Append(KSeparator);
	aDetails.Append(iGenre);
	aDetails.Append(KSeparator);
	aDetails.AppendNum(iLength.Int()/60);
	TLocale locale;
	aDetails.Append(locale.TimeSeparator(2));
	TInt minutes = iLength.Int() % 60;
	aDetails.AppendNum(minutes);
	if (minutes < 10)
		{
		aDetails.AppendNum(0);
		}
	}

/** 
* Sets the name of the recording.
* @param aName The new name for the recording.
*/
EXPORT_C void MoboPlayer::TRecording::SetName(const TDesC & aName)
	{
	iName = aName;
	}

/** 
* Sets the length of the recording.
* @param aLength The new length for the recording in seconds.
*/
EXPORT_C void MoboPlayer::TRecording::SetLength(TTimeIntervalSeconds aLength)
	{
	iLength = aLength;
	}

/**
* Sets the genre of the recording.
* @param aGenre The new genre for the recording.
*/
EXPORT_C void MoboPlayer::TRecording::SetGenre(const TDesC & aGenre)
	{
	iGenre = aGenre;
	}

/** 
* Sets the name of the performer.
* @param aPerformer The new performer for the recording.
*/
EXPORT_C void MoboPlayer::TRecording::SetPerformer(const TDesC & aPerformer)
	{
	iPerformer = aPerformer;
	}

/**
* Sets the date of the recording.
* @param aPerformed The new date of the recording.
*/
EXPORT_C void MoboPlayer::TRecording::SetPerformed(const TTime & aPerformed)
	{
	iPerformed = aPerformed;
	}

/**
* Sets the file name of the recording.
* The example solution is implemented so that if the parameter does not
* contain drive and path information, this is added to the recording's file
* name. A default path "E:\Sounds\Digital\" is used as a hard-coded value.
* This makes it easier for the user to insert the file name, since the path to
* the memory disk need not to be written by the user. Later it is easier to change
* the path to c:, for example, if the recording is actually in phone internal memory.<p>
* Most user friendly way to enter file name would be to provide a selection UI that
* lists the sound files from the internal memory and/or memory card, and user just picks
* the relevant file from the list. This can be implemented as an optional feature to the
* UI side.
* @param aFileName The new file name for the recording.
* @see TParsePtrC for parsing the drive and path from the file name.
*/
EXPORT_C void MoboPlayer::TRecording::SetFileName(const TDesC & aFileName)
	{
	iFileName = KNullDesC;
	// Check the file name. If it does not contain a drive and path,
	// add the memory card default path for digital sounds as the path
	// of the sound file. Makes it easier for the user to add the file name.
	TParsePtrC parser(aFileName);
	TPtrC ptr = parser.DriveAndPath();
	if (ptr.Length() == 0)
		{
		// Use PathInfo from 2.0 to get the path to digital sounds.
		_LIT(KNokiaSoundsPathOnE, "E:\\Sounds\\Digital\\");
		iFileName = KNokiaSoundsPathOnE;
		}
	iFileName.Append(aFileName);
	}

// begin REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!
// This is needed only if the students want to implement the optional sorting
// feature to the playlists.
	

/**
* <b>OPTIONAL feature</b> Compares if two recordings are the same (have the same name).
* @param aFirst The first recording.
* @param aSecond The second recording.
*/
TBool MoboPlayer::TRecording::MatchName(const TRecording & aFirst, const TRecording & aSecond)
	{
	return (aFirst.Name() == aSecond.Name());
	}

/**
* <b>OPTIONAL feature</b> Compares the recordings by name (alphabetically).
* Used to make the recordings in a playlist ordered.
* @param aFirst The First recording.
* @param aSecond The second recording.
* @returns -1 if first is smaller, 0 if first equals second,
* and 1 if first is greater than first.
*/
TInt MoboPlayer::TRecording::CompareNames(const TRecording & aFirst, const TRecording & aSecond)
	{
	if (aFirst.Name() < aSecond.Name())
		return -1;
	if (aFirst.Name() > aSecond.Name())
		return 1;
	return 0;
	}
	
/**
* <b>OPTIONAL feature</b> Compares the recordings by genre (alphabetically).
* Used to make the recordings in a playlist ordered.
* @param aFirst The First recording.
* @param aSecond The second recording.
* @returns -1 if first is smaller, 0 if first equals second,
* and 1 if first is greater than first.
*/
TInt MoboPlayer::TRecording::CompareGenre(const TRecording & aFirst, const TRecording & aSecond)
	{
	if (aFirst.Genre() < aSecond.Genre())
		return -1;
	if (aFirst.Genre() > aSecond.Genre())
		return 1;
	return 0;
	}
	
/**
* <b>OPTIONAL feature</b> Compares the recordings by performer's name (alphabetically).
* Used to make the recordings in a playlist ordered.
* @param aFirst The First recording.
* @param aSecond The second recording.
* @returns -1 if first is smaller, 0 if first equals second,
* and 1 if first is greater than first.
*/
TInt MoboPlayer::TRecording::ComparePerformer(const TRecording & aFirst, const TRecording & aSecond)
	{
	if (aFirst.Performer() < aSecond.Performer())
		return -1;
	if (aFirst.Performer() > aSecond.Performer())
		return 1;
	return 0;
	}

/**
* <b>OPTIONAL feature</b> Compares the recordings by the date of the performance.
* Used to make the recordings in a playlist ordered.
* @param aFirst The First recording.
* @param aSecond The second recording.
* @returns -1 if first is smaller, 0 if first equals second,
* and 1 if first is greater than first.
*/
TInt MoboPlayer::TRecording::ComparePerformed(const TRecording & aFirst, const TRecording & aSecond)
	{
	if (aFirst.Performed() < aSecond.Performed())
		return -1;
	if (aFirst.Performed() > aSecond.Performed())
		return 1;
	return 0;
	}

/**
* <b>OPTIONAL feature</b> The key offset to the iName property.
* @returns The key offset to iName using _FOFF macro.
*/
TInt MoboPlayer::TRecording::NameKeyOffset()
	{
	return _FOFF(TRecording, iName);
	}
	
/**
* <b>OPTIONAL feature</b> The key offset to the iGenre property.
* @returns The key offset to iGenre using _FOFF macro.
*/
TInt MoboPlayer::TRecording::GenreKeyOffset()
	{
	return _FOFF(TRecording, iGenre);
	}

/**
* <b>OPTIONAL feature</b> The key offset to the iPerformer property.
* @returns The key offset to iPerformer using _FOFF macro.
*/
TInt MoboPlayer::TRecording::PerformerKeyOffset()
	{
	return _FOFF(TRecording, iPerformer);
	}

/**
* <b>OPTIONAL feature</b> The key offset to the iPerformed property.
* @returns The key offset to iPerformed using _FOFF macro.
*/
TInt MoboPlayer::TRecording::PerformedKeyOffset()
	{
	return _FOFF(TRecording, iPerformed);
	}

// end REMOVE THIS BLOCK FROM THE PUBLIC INTERFACE GIVEN TO STUDENTS !!!


/** 
* Externalizes the recording to a stream.
* @param aStream The stream to save the object into.
*/
void MoboPlayer::TRecording::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(iLength.Int());
	aStream << iName;
	aStream << iGenre;
	aStream << iPerformer;
	// There is no WriteInt64L, so have to save the date
	// as two 32 bit integers.
	aStream.WriteInt32L(iPerformed.Int64().High());
	aStream.WriteInt32L(iPerformed.Int64().Low());
	aStream << iFileName;
	}
	
/**
* Internalizes the recording from a stream.
* @param aStream The stream to read the the object from.
*/
void MoboPlayer::TRecording::InternalizeL(RReadStream& aStream)
	{
	iLength = aStream.ReadInt32L();
	
	aStream >> iName; 
	aStream >> iGenre;
	aStream >> iPerformer;
	// There is no ReadInt64L, so have to read the date
	// as two 32 bit integers.
	TInt high;
	TInt low;
	high = aStream.ReadInt32L();
	low = aStream.ReadInt32L();
	TInt64 timeInt;
	timeInt.Set(high, low);
	iPerformed = timeInt;
	aStream >> iFileName;
	}



/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: Recording.cpp,v $
* Revision 1.4  2005/04/08 16:58:22  Antti
* Added an extra '0' to seconds in GetDetails, if seconds < 10.
*
* Revision 1.3  2005/03/02 08:14:18  Antti
* Updated documentation.
*
* Revision 1.2  2005/03/02 07:28:17  Antti
* Added handling of assigning object to itself to operator =.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/