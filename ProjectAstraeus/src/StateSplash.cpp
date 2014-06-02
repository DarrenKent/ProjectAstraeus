/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include <fstream>
#include <iostream>

#include "Engine\Debug.h"
#include "Engine\Draw.h"
#include "Engine\Input.h"

#include "StateSplash.h"

SplashState::SplashState( Display *dMgr, StateManager *sMgr ) : State() {
	DebugMessage( "Creating Splash State...", 2 );

	mStateManager	= sMgr;
	mDisplayManager = dMgr;
	mSceneManager	= new SceneManager( dMgr->GetHDC() );
	mTextureManager = new TextureManager();
	mTimeManager	= new TimeManager();

	mSplashCount	= 0;
	mCurrentSplash	= 0;
	mSplashTime		= 0.0;
	mMaxTime		= 10.0f;

	LoadSplashScreens();
}

SplashState::~SplashState() {
	DebugMessage( "Deleting Splash State...", 2 );
	if ( mSceneManager ) {
		delete mSceneManager;
		mSceneManager	= NULL;
	}
	if ( mTextureManager ) {
		delete mTextureManager;
		mTextureManager = NULL;
	}
	if ( mTimeManager ) {
		delete mTimeManager;
		mTimeManager	= NULL;
	}
}

void SplashState::DrawFrame() {
	glDisable( GL_LIGHTING );

	mDisplayManager->SetOrthographicProjection();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glLoadIdentity();

	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_FALSE );

	if ( mSplashTime < 3.0 )
		glColor4f( 1.0f, 1.0f, 1.0f, (GLfloat)( mSplashTime / 3.0f ) );
	else if ( mSplashTime > 7.0 )
		glColor4f( 1.0f, 1.0f, 1.0f, (GLfloat)( 1.0f - ( mSplashTime - 7.0f ) / 3.0f ) );
	else
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	std::string tTexId = "splash" + std::to_string( (long double)mCurrentSplash );
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( tTexId ) );

	float tSplashW	= mTextureManager->GetTexture( tTexId )->GetImageWidth();
	float tSplashH	= mTextureManager->GetTexture( tTexId )->GetImageHeight();
	float tSplashX	= ( mDisplayManager->GetScreenWidth() / 2.0f ) - ( tSplashW / 2.0f );
	float tSplashY	= ( mDisplayManager->GetScreenHeight() / 2.0f ) - ( tSplashH / 2.0f );

	DrawRectangle2D( tSplashX, tSplashY, tSplashX + tSplashW, tSplashY + tSplashH );

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}

void SplashState::GameLogic() {
	mTimeManager->TimeFrame();
	mSplashTime	+= mTimeManager->GetDeltaTimeSeconds();

	if ( mSplashTime >= mMaxTime ) {
		mCurrentSplash ++;
		mSplashTime = 0.0;
		if ( mCurrentSplash >= mSplashCount ) {
			mStateManager->SetState( "mainMenu" );
			mStateManager->DeleteState( "splashState" );
		}
	}
}

void SplashState::InitializeState() {
	DebugMessage( "Initializing Splash State...", 2 );
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

	mTimeManager->StartTime();
}

void SplashState::LoadSplashScreens() {
	DebugMessage( "Loading Splash Screens...", 3 );

	std::ifstream tFile( "data/cfg/splash.cfg" );
	std::string tLine;

	if ( tFile.is_open() ) {
		while ( tFile.good() ) {
			getline( tFile, tLine );
			std::string tTexId = "splash" + std::to_string( (long double)mSplashCount );
			mTextureManager->LoadTexture( tTexId, tLine, false );
			mSplashCount ++;
		}
	}
}

void SplashState::ProcessInput() {
	if ( GetKeyPressed( VK_ESCAPE ) ) {
		mCurrentSplash ++;
		mSplashTime = 0.0;
		if ( mCurrentSplash >= mSplashCount ) {
			mStateManager->SetState( "mainMenu" );
			mStateManager->DeleteState( "splashState" );
		}
	}
}

