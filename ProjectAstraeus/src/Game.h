/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _GAME_
#define _GAME_

#include "Engine\StateManager.h"

class Game: public StateManager {
	public:
		Game();

		float GetMusicVolume();								// Returns the Current Music Volume
		float GetScreenHeight();							// Returns the Screen Height that will be next Restart
		float GetScreenWidth();								// Returns the Screen Width that will be next Restart
		float GetSFXVolume();								// Returns the Current SFX Volume
		int   GetFullscreen();								// Returns the Fullscreen that will be next Restart

		void  SetFullscreen( int fullscreen );				// Set the Fullscreen for next Restart
		void  SetMusicVolume( float volume );				// Set the current Music Volume
		void  SetScreenHeight( float height );				// Set the Screen Height for next Restart
		void  SetScreenWidth( float width );				// Set the Screen Width for next Restart
		void  SetSFXVolume( float volume );					// Set the current SFX Volume
	protected:
	private:
		float mScreenWidth, mScreenHeight;					// Screen Resolution for next Restart
		float mMusicVolume, mSFXVolume;						// Current Volumes
		int mFullscreen;									// Fullscreen status for next Restart
};

#endif														// _GAME_