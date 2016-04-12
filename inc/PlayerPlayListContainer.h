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

#ifndef __PLAYER_CONTAINER1_H__
#define __PLAYER_CONTAINER1_H__

#include <aknview.h>
#include <eiklbo.h>
#include <coecobs.h>

class CEikTextListBox;
namespace MoboPlayer 
{

class CPlayer;
class CPlayerPlayListInfo;

/**
* Container control that contains the listbox for showing playlist 
* information.
* This container is contained within the CPlayerPlayListView.
* @author Antti Juustila
* @version $Revision: 1.1.1.1 $
*/

class CPlayerPlayListContainer : public CCoeControl,  public MEikListBoxObserver, public MCoeControlObserver
	{
public: 

	// From MEikListBoxObserver
	void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

	// From CCoeControl
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

	// From MCoeControlObserver
	void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
	
	static CPlayerPlayListContainer* NewL(const TRect& aRect);
	static CPlayerPlayListContainer* NewLC(const TRect& aRect);
	/** Destructs the container and child controls with it. */
	~CPlayerPlayListContainer();

	void HandleListboxAdditionL();

	// from CoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;

private:
	void ConstructL(const TRect& aRect);

private:
	/**
	* The listbox in this control, shows the list of playlists.
	* Is deleted by the container control when it is deleted.
	*/
	CEikTextListBox		* iTextListBox;
	
	/** 
	* The infocontrol in this control, shows the playlist info.
	* Is deleted by the container control when it is deleted.
	*/
	CPlayerPlayListInfo	* iInfoControl;
	
	/** 
	* The player used to get information about the playlists.
	* Is not deleted by the container control when it is deleted!
	*/
	CPlayer	* iPlayer;
	};

} // namespace

#endif // __PLAYER_CONTAINER1_H__

/**
* Do not edit text here. These comments are maintained by CVS!
*
* $Log: PlayerPlayListContainer.h,v $
* Revision 1.1.1.1  2005/03/01 18:27:34  Antti
* Importing first public release.
*
*
*/