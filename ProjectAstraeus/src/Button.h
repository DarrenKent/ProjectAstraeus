/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _BUTTON_
#define _BUTTON_

#include <functional>

#include "IK\irrKlang.h"

#include "Engine\StateManager.h"
#include "Engine\Texture.h"

class Button {
	public:
		Button( float x1, float y1, Texture *texture,
				Texture *hover,
				std::function< void( State* ) > handler,
				State *state,
				irrklang::ISoundEngine *audio,
				std::string clickSFX,
				std::string hoverSFX );						// Constructor

		void DrawButton( int screenWidth, int screenHeight,
						 int fullscreen, float sfxVolume );	// Draw the Button
		void ExecuteButton( int screenWidth, 
							int screenHeight,
							int fullscreen,
							float sfxVolume );				// Execute the Button Handler

		void SetScale( float scale );						// Set Scale of button
	protected:
	private:
		bool							mClickPlayed;		// Click SFX played flag
		bool							mHoverPlayed;		// Hover SFX played flag
		float							mScale;				// Scale of Button
		float							mWidth, mHeight;	// Button Width and Height
		float							mXPos, mYPos;		// Button Position ( Bottom Left Corner )
		std::function< void( State* ) >	mButtonHandler;		// Button Handler
		irrklang::ISoundEngine			*mAudioManager;		// Audio Manager
		State							*mState;			// State that this button belongs to
		std::string						mClickSFX;			// Click SFX file
		std::string						mHoverSFX;			// Hover SFX file
		Texture							*mHoverTexture;		// Hover Texture
		Texture							*mTexture;			// Button Texture
};

#endif														// _BUTTON_