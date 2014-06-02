/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include "Engine\Draw.h"
#include "Engine\Input.h"
#include "Engine\StateManager.h"

#include "Button.h"

Button::Button( float x1, float y1, Texture *texture, Texture *hover, std::function< void( State* ) > handler, State *state, 
				irrklang::ISoundEngine *audio, std::string clickSFX, std::string hoverSFX ) {

	mClickPlayed	= false;
	mHoverPlayed	= false;
	mHeight			= texture->GetImageHeight();
	mWidth			= texture->GetImageWidth();
	mScale			= 1.0f;
	mState			= state;
	mXPos			= x1;
	mYPos			= y1;
	mButtonHandler	= handler;
	mAudioManager	= audio;
	mClickSFX		= clickSFX;
	mHoverSFX		= hoverSFX;
	mHoverTexture	= hover;
	mTexture		= texture;
}

void Button::DrawButton( int screenWidth, int screenHeight, int fullscreen, float sfxVolume ) {
	float offset  = 0;
	if ( !fullscreen )
		offset    = 40;
	float tMouseX = (float)GetMouseX();
	float tMouseY = screenHeight - GetMouseY() - offset;

	if ( tMouseX > mXPos && tMouseX < mXPos + mWidth * mScale && 
		 tMouseY > mYPos && tMouseY < mYPos + mHeight * mScale ) {

		glBindTexture( GL_TEXTURE_2D, mHoverTexture->GetTextureId() );
		if ( !mHoverPlayed ) {
			irrklang::ISound *tSFX = mAudioManager->play2D( mHoverSFX.c_str(), false, true );
			tSFX->setVolume( sfxVolume );
			tSFX->setIsPaused( false );
			mHoverPlayed = true;
		}
	} else {
		glBindTexture( GL_TEXTURE_2D, mTexture->GetTextureId() );
		mHoverPlayed = false;
	}
		
	DrawRectangle2D( mXPos, mYPos, mXPos + mWidth * mScale, mYPos + mHeight * mScale );
}

void Button::ExecuteButton( int screenWidth, int screenHeight, int fullscreen, float sfxVolume ) {
	float offset  = 0;
	if ( !fullscreen )
		offset    = 40;
	float tMouseX = (float)GetMouseX();
	float tMouseY = screenHeight - GetMouseY() - offset;

	if ( tMouseX > mXPos && tMouseX < mXPos + mWidth * mScale && 
		 tMouseY > mYPos && tMouseY < mYPos + mHeight * mScale ) {

		if ( mButtonHandler && GetLeftClick() ) {
			if ( !mClickPlayed ) {
				irrklang::ISound *tSFX = mAudioManager->play2D( mClickSFX.c_str(), false, true );
				tSFX->setVolume( sfxVolume );
				tSFX->setIsPaused( false );
				mClickPlayed = true;
			}
			mButtonHandler( mState );
		} else {
			mClickPlayed = false;
		}
	}
}

void Button::SetScale( float scale ) {
	mScale = scale;
}