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

#ifndef __RECORDINGFORM_H__
#define __RECORDINGFORM_H__

#include <aknform.h>


namespace MoboPlayer 
{

class TRecording;


/**
* The form (dialog) for editing recordings.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class CRecordingForm : public CAknForm
	{
public:  
	static CRecordingForm* NewL(TRecording & aRecording);
	
	virtual ~CRecordingForm();

protected:
	
private: 
	CRecordingForm(TRecording & aRecording);
	
	void ConstructL();
	
	void PreLayoutDynInitL();
	
	TBool OkToExitL(TInt aButtonId);

private:
	/** 
	* Reference to the recording object that the user is editing.
	*/
	TRecording & iRecording;
	};

} // namespace

#endif 

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: RecordingForm.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/