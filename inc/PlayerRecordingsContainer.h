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

#ifndef __PLAYER_CONTAINER2_H__
#define __PLAYER_CONTAINER2_H__

#include <aknview.h>
#include <eiklbo.h>
#include <coecobs.h>

#include "PlayerObserver.h"

class CEikTextListBox;
namespace MoboPlayer
{

class CPlayer;
class CPlayerRecordingInfo;

/**
* Container control that contains the listbox and information control
* for showing recording information. Also observers the CPlayer object
* and shows information for playing related events (playing started, etc.).
* This container is contained within the CPlayerRecordingsView.
* @author Antti Juustila
* @version $Revision: 1.3 $
*/

class CPlayerRecordingsContainer : 	public CCoeControl,
									public MEikListBoxObserver,
									public MCoeControlObserver,
									public MPlayerObserver
	{
public: 

	static CPlayerRecordingsContainer* NewL(const TRect& aRect);
	static CPlayerRecordingsContainer* NewLC(const TRect& aRect);
	
	~CPlayerRecordingsContainer();

	// From MEikListBoxObserver
	void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

	// From MCoeControl
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

	// From MCoeControlObserver
	void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

	// From MPlayerObserver
	virtual void HandlePlayerError(TInt aError);	
	virtual void PlayingStarted(TInt aIndexOfRecording, TInt aTotalDuration);
	virtual void PlayingEnded();

	void HandleListboxAdditionL();
	void HandleListboxRemovalL();

	// from CoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;

private:
	void ConstructL(const TRect& aRect);

private:
	/** 
	* The listbox in this control, shows the list of recordings.
	* Is deleted by the container control when it is deleted.
	*/
	CEikTextListBox					* iTextListBox;	// Owns
	
	/** 
	* The infocontrol in this control, shows the recording info and playing status.
	* Is deleted by the container control when it is deleted.
	*/
	CPlayerRecordingInfo				* iInfoControl; // Owns
	
	/** 
	* The player used to control the playing and show
	* information about the recordings and the playing of the recordings.
	* Is not deleted by the container control when it is deleted!
	*/
	CPlayer								* iPlayer;	// Does not own
	};

} // namespace
#endif // __PLAYER_CONTAINER2_H__

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerRecordingsContainer.h,v $
* Revision 1.3  2005/06/09 09:00:17  Antti
* Added HandleListboxRemovalL.
*
* Revision 1.2  2005/03/02 08:14:39  Antti
* Updated documentation.
*
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/