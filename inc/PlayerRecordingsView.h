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

#ifndef __PLAYER_VIEW2_H__
#define __PLAYER_VIEW2_H__


#include <aknview.h>

namespace MoboPlayer 
{

class CPlayerRecordingsContainer;

/**
* This view contains the controls that show the recordings and
* the state of the playing of a recording.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class CPlayerRecordingsView: public CAknView
	{
public:

	static CPlayerRecordingsView* NewL();
	static CPlayerRecordingsView* NewLC();

	~CPlayerRecordingsView();

	TUid Id() const;

	void HandleCommandL(TInt aCommand);

	void DoActivateL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage);
	void DoDeactivate();

private:

	CPlayerRecordingsView();
	void ConstructL();

private:

	/** 
	* The container control, containing the subcontrols of the view.
	*/
	CPlayerRecordingsContainer* iContainer;
	};

} // namespace 

#endif

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerRecordingsView.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/