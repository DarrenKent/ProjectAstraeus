/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include "Engine\Debug.h"
#include "Engine\Draw.h"
#include "Engine\Input.h"

#include "Parsers.h"
#include "StateDemo.h"
#include "StateMainMenu.h"

void DemoHandler( State *state ) {
	MainMenuState* tMainMenu	= dynamic_cast< MainMenuState* >( state );
	DemoState* tDemoState		= new DemoState( tMainMenu->GetDisplay(),
												 tMainMenu->GetGameMgr() );
	tMainMenu->StopSounds();
	tMainMenu->GetGameMgr()->AddState( "demo", tDemoState );
	tMainMenu->GetGameMgr()->SetState( "demo" );
}

void SettingsHandler( State *state ) {
	MainMenuState* tMainMenu = dynamic_cast< MainMenuState* >( state );
	tMainMenu->SetSettingsActive( true );
}

void QuitHandler( State *state ) {
	MainMenuState* tMainMenu = dynamic_cast< MainMenuState* >( state );
	exit(0);
}

void BackHandler( State *state ) {
	MainMenuState* tMainMenu = dynamic_cast< MainMenuState* >( state );
	tMainMenu->SetSettingsActive( false );
	SaveSettings( tMainMenu->GetGameMgr() );
}

MainMenuState::MainMenuState( Display *dMgr, Game *gMgr ) {
	DebugMessage( "Creating Main Menu State...", 2 );

	mAudioManager		= irrklang::createIrrKlangDevice();
	mDisplayManager		= dMgr;
	mGameManager		= gMgr;
	mSceneManager		= new SceneManager( dMgr->GetHDC() );
	mShaderManager		= new ShaderManager();
	mTextureManager		= new TextureManager();
	mTimeManager		= new TimeManager();

	mSettingsActive		= false;
	mFullscreenFlag		= false;
	mMusicSliderFlag	= false;
	mSFXSliderFlag		= false;
	mMouseXClick		= 0.0f;
}

MainMenuState::~MainMenuState() {
	if ( mAudioManager ) {
		delete mAudioManager;
		mAudioManager	= NULL;
	}
	if ( mSceneManager ) {
		delete mSceneManager;
		mSceneManager	= NULL;
	}
	if ( mShaderManager ) {
		delete mShaderManager;
		mShaderManager	= NULL;
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

void MainMenuState::DrawFrame() {
	mDisplayManager->SetPerspectiveProjection( 60, 10, 10000 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );

	mCamera->PositionCamera();

	mShaderManager->UseProgram( "mainMenuProgram" );
	mSceneManager->DrawScene();

	mShaderManager->ClearProgram();
	mStarField->Draw( mTimeManager->GetDeltaTimeSeconds() );

	mDisplayManager->SetOrthographicProjection();
	glLoadIdentity();

	DrawGUI();
}

void MainMenuState::DrawGUI() {
	glDisable( GL_LIGHTING );
	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	float tMusicVolume	= mGameManager->GetMusicVolume();
	float tSFXVolume	= mGameManager->GetSFXVolume();
	int	  tFullscreen	= mDisplayManager->GetFullscreen();
	int	  tScreenHeight = mDisplayManager->GetScreenHeight();
	int	  tScreenWidth	= mDisplayManager->GetScreenWidth();

	// Astraeus Logo
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "astraeusLogo" ) );
	float tLogoW	= mTextureManager->GetTexture( "astraeusLogo" )->GetImageWidth();
	float tLogoH	= mTextureManager->GetTexture( "astraeusLogo" )->GetImageHeight();
	float tLogoX	= ( tScreenWidth / 2.0f ) - ( tLogoW / 2.0f );
	float tLogoY	= ( tScreenHeight - tLogoH * 1.75f );
	DrawRectangle2D( tLogoX, tLogoY, tLogoX + tLogoW, tLogoY + tLogoH );

	if ( !mSettingsActive ) {
		// Menu Background
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "menuBG" ) );
		float tMenuW	= mTextureManager->GetTexture( "menuBG" )->GetImageWidth();
		float tMenuH	= mTextureManager->GetTexture( "menuBG" )->GetImageHeight();
		float tMenuX	= tScreenWidth - ( tMenuW + 50.0f );
		float tMenuY	= ( tScreenHeight / 2.0f ) - ( tMenuH / 2.0f );
		DrawRectangle2D( tMenuX, tMenuY, tMenuX + tMenuW, tMenuY + tMenuH );

		// Draw Buttons
		mDemo->DrawButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
		mSettings->DrawButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
		mQuit->DrawButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
	} else {
		// Settings Background
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "settingsBG" ) );
		float tBackW	= mTextureManager->GetTexture( "settingsBG" )->GetImageWidth();
		float tBackH	= mTextureManager->GetTexture( "settingsBG" )->GetImageHeight();
		float tBackX	= ( tScreenWidth / 2.0f ) - ( tBackW / 2.0f );
		float tBackY	= ( tScreenHeight / 2.0f ) - ( tBackH / 2.0f );
		DrawRectangle2D( tBackX, tBackY, tBackX + tBackW, tBackY + tBackH );

		// CheckBoxes
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "check" ) );
		float tCenterX	= tScreenWidth / 2.0f;
		float tCenterY	= tScreenHeight / 2.0f;

		if ( mGameManager->GetFullscreen() == 1 )
			DrawRectangle2D( tCenterX - 219.0f, tCenterY + 178.0f,
							 tCenterX - 219.0f + 16.0f, tCenterY + 178.0f + 16.0f );
		if ( mGameManager->GetScreenWidth() == 1024.0f )
			DrawRectangle2D( tCenterX - 219.0f, tCenterY + 146.0f,
							 tCenterX - 219.0f + 16.0f, tCenterY + 146.0f + 16.0f );
		if ( mGameManager->GetScreenWidth() == 1600.0f )
			DrawRectangle2D( tCenterX + 30.0f, tCenterY + 146.0f,
							 tCenterX + 30.0f + 16.0f, tCenterY + 146.0f + 16.0f );
		if ( mGameManager->GetScreenWidth() == 1366.0f )
			DrawRectangle2D( tCenterX - 219.0f, tCenterY + 115.0f,
							 tCenterX - 219.0f + 16.0f, tCenterY + 115.0f + 16.0f );

		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "boxHover" ) );
		float tOffset	= 0.0f;
		if ( !tFullscreen )
			tOffset		= -40.0f;
		float tMouseX	= (float)GetMouseX();
		float tMouseY	= mDisplayManager->GetScreenHeight() - GetMouseY() + tOffset;
		if ( tMouseX > tCenterX - 227.0f && tMouseX < tCenterX - 227.0f + 32.0f && 
			 tMouseY > tCenterY + 170.0f && tMouseY < tCenterY + 170.0f + 32.0f ) {
			DrawRectangle2D( tCenterX - 227.0f, tCenterY + 170.0f,
							 tCenterX - 227.0f + 32.0f, tCenterY + 170.0f + 32.0f );
			if ( !mFullscreenFlag && GetLeftClick() ) {
				mGameManager->SetFullscreen( !mGameManager->GetFullscreen() );
				mFullscreenFlag = true;
			}
		} else if ( tMouseX > tCenterX - 227.0f && tMouseX < tCenterX - 227.0f + 32.0f &&
					tMouseY > tCenterY + 139.0f && tMouseY < tCenterY + 139.0f + 32.0f ) {
			DrawRectangle2D( tCenterX - 227.0f, tCenterY + 139.0f,
							 tCenterX - 227.0f + 32.0f, tCenterY + 139.0f + 32.0f );
			if ( GetLeftClick() ) {
				mGameManager->SetScreenWidth( 1024.0f );
				mGameManager->SetScreenHeight( 768.0f );
			}
		} else if ( tMouseX > tCenterX + 22.0f && tMouseX < tCenterX + 22.0f + 32.0f &&
					tMouseY > tCenterY + 139.0f && tMouseY < tCenterY + 139.0f + 32.0f ) {
			DrawRectangle2D( tCenterX + 22.0f, tCenterY + 139.0f,
							 tCenterX + 22.0f + 32.0f, tCenterY + 139.0f + 32.0f );
			if ( GetLeftClick() ) {
				mGameManager->SetScreenWidth( 1600.0f );
				mGameManager->SetScreenHeight( 900.0f );
			}
		} else if ( tMouseX > tCenterX - 227.0f && tMouseX < tCenterX - 227.0f + 32.0f &&
					tMouseY > tCenterY + 108.0f && tMouseY < tCenterY + 108.0f + 32.0f ) {
			DrawRectangle2D( tCenterX - 227.0f, tCenterY + 108.0f,
							 tCenterX - 227.0f + 32.0f, tCenterY + 108.0f + 32.0f );
			if ( GetLeftClick() ) {
				mGameManager->SetScreenWidth( 1366.0f );
				mGameManager->SetScreenHeight( 768.0f );
			}
		}

		float tMusicDiff	= 0.0f;
		if ( mMusicSliderFlag )
			tMusicDiff		= tMouseX - mMouseXClick;

		float tSFXDiff		= 0.0f;
		if ( mSFXSliderFlag )
			tSFXDiff		= tMouseX - mMouseXClick;

		// Sliders
		float tMusicSliderXPos	= tCenterX - 186.0f + tMusicVolume * 300.0f + tMusicDiff;
		float tSFXSliderXPos	= tCenterX - 186.0f + tSFXVolume * 300.0f + tSFXDiff;

		if ( tMusicSliderXPos < tCenterX - 186.0f )
			tMusicSliderXPos	= tCenterX - 186.0f;
		if ( tMusicSliderXPos > tCenterX - 186.0f + 300.0f )
			tMusicSliderXPos	= tCenterX - 186.0f + 300.0f;
		if ( tSFXSliderXPos < tCenterX - 186.0f )
			tSFXSliderXPos		= tCenterX - 186.0f;
		if ( tSFXSliderXPos > tCenterX - 186.0f + 300.0f )
			tSFXSliderXPos		= tCenterX - 186.0f + 300.0f;

		if ( !mSFXSliderFlag && tMouseX > tMusicSliderXPos &&
			 tMouseX < tMusicSliderXPos + 16.0f && tMouseY > tCenterY - 44.0f &&
			 tMouseY < tCenterY - 44.0f + 32.0f ) {

			glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "sliderHover" ) );
			if ( !mMusicSliderFlag && GetLeftClick() ) {
				mMusicSliderFlag	= true;
				mMouseXClick		= tMouseX;
			}
		} else {
			glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "slider" ) );
		}
		DrawRectangle2D( tMusicSliderXPos, tCenterY - 44.0f,
						 tMusicSliderXPos + 16.0f, tCenterY - 44.0f + 32.0f );

		if ( !mMusicSliderFlag && tMouseX > tSFXSliderXPos &&
			 tMouseX < tSFXSliderXPos + 16.0f && tMouseY > tCenterY - 110.0f &&
			 tMouseY < tCenterY - 110.0f + 32.0f ) {

			glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "sliderHover" ) );
			if ( !mSFXSliderFlag && GetLeftClick() ) {
				mSFXSliderFlag		= true;
				mMouseXClick		= tMouseX;
			}
		} else {
			glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "slider" ) );
		}
		DrawRectangle2D( tSFXSliderXPos, tCenterY - 110.0f,
						 tSFXSliderXPos + 16.0f, tCenterY - 110.0f + 32.0f );

		// Draw Buttons
		mBack->DrawButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
	}

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}

void MainMenuState::GameLogic() {
	mTimeManager->TimeFrame();

	float tSFXVolume	= mGameManager->GetSFXVolume();
	int	  tFullscreen	= mDisplayManager->GetFullscreen();
	int   tScreenHeight = mDisplayManager->GetScreenHeight();
	int   tScreenWidth	= mDisplayManager->GetScreenWidth();

	if ( !mSettingsActive ) {
		mQuit->ExecuteButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
		mDemo->ExecuteButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
		mSettings->ExecuteButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
	} else {
		mBack->ExecuteButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
	}
}

void MainMenuState::InitializeState() {
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	int   tScreenHeight		= mDisplayManager->GetScreenHeight();
	int   tScreenWidth		= mDisplayManager->GetScreenWidth();
	float tScreenXCenter	= tScreenWidth / 2.0f;
	float tScreenYCenter	= tScreenHeight / 2.0f;


	mCamera	= new OrbitalCamera();
	mCamera->SetCameraPos( 0.0f, 0.0f, 100.0f );
	mCamera->SetCameraRadius( 100.0f );
	mCamera->SetCameraTheta( 90.0f );
	mCamera->SetCameraAlpha( 0.0f );
	mCamera->PositionCamera();

	Texture *tDemoTex			= mTextureManager->LoadTexture( "demo", "data/textures/demo.tga", false );
	Texture *tDemoHoverTex		= mTextureManager->LoadTexture( "demoHover", "data/textures/demoHover.tga", false );
	Texture *tSettingsTex		= mTextureManager->LoadTexture( "settings", "data/textures/settings.tga", false );
	Texture *tSettingsHoverTex	= mTextureManager->LoadTexture( "settingsHover", "data/textures/settingshover.tga", false );
	Texture *tQuitTex			= mTextureManager->LoadTexture( "quit", "data/textures/quit.tga", false );
	Texture *tQuitHoverTex		= mTextureManager->LoadTexture( "quitHover", "data/textures/quithover.tga", false );
	Texture *tShipTex			= mTextureManager->LoadTexture( "shipTex", "data/textures/ship.tga", false );
	Texture *tBackTex			= mTextureManager->LoadTexture( "back", "data/textures/back.tga", false );
	Texture *tBackHoverTex		= mTextureManager->LoadTexture( "backHover", "data/textures/backhover.tga", false );
	mTextureManager->LoadTexture( "menuBG", "data/textures/menubg.tga", false );
	mTextureManager->LoadTexture( "astraeusLogo", "data/textures/astraeusLogo.tga", false );
	mTextureManager->LoadTexture( "settingsBG", "data/textures/settingsbg.tga", false );
	mTextureManager->LoadTexture( "check", "data/textures/check.tga", false );
	mTextureManager->LoadTexture( "boxHover", "data/textures/boxhover.tga", false );
	mTextureManager->LoadTexture( "slider", "data/textures/slider.tga", false );
	mTextureManager->LoadTexture( "sliderHover", "data/textures/sliderhover.tga", false );

	// Main Menu Buttons
	float		tRightMargin	= 50.0f;
	float		tMenuYOffset	= 60.0f;
	float		tMenuHeight		= mTextureManager->GetTexture( "menuBG" )->GetImageHeight();
	float		tButtonWidth	= tDemoTex->GetImageWidth();
	float		tButtonHeight	= tDemoTex->GetImageHeight();
	float		tButtonX		= tScreenWidth - tDemoTex->GetImageWidth() - tRightMargin;
	float		tMenuSpacing	= 52.0f;
	float		tMenuTop		= tScreenYCenter + ( tMenuHeight / 2.0f ) - tMenuYOffset;
	float		tSettingsY		= tMenuTop - tMenuSpacing * 2;
	float		tQuitY			= tMenuTop - tMenuSpacing * 3;
	std::string tHoverSFX		= "data/sounds/sfx2.wav";
	std::string tClickSFX		= "data/sounds/sfx3.wav";

	mDemo		= new Button( tButtonX, tMenuTop, tDemoTex, tDemoHoverTex, DemoHandler, this, mAudioManager, tClickSFX, tHoverSFX );
	mSettings	= new Button( tButtonX, tSettingsY, tSettingsTex, tSettingsHoverTex, SettingsHandler, this, mAudioManager, tClickSFX, tHoverSFX  );
	mQuit		= new Button( tButtonX, tQuitY, tQuitTex, tQuitHoverTex, QuitHandler, this, mAudioManager, tClickSFX, tHoverSFX  );

	// Settings Buttons
	float tLeftEdge		= tScreenXCenter + 256.0f;
	float tBackRight	= tLeftEdge - 148.0f;
	float tBackBottom	= tScreenYCenter - 236.0f;
	mBack				= new Button( tBackRight, tBackBottom, tBackTex, tBackHoverTex, BackHandler, this, mAudioManager, tClickSFX, tHoverSFX );

	// Ships
	mSceneManager->AddNode( "ship", "data/models/ship.obj", tShipTex );
	mSceneManager->GetNode( "ship" )->SetPosition( 10.0f, -25.0f, 0.0f );
	mSceneManager->GetNode( "ship" )->SetScale( 0.01f );
	mSceneManager->GetNode( "ship" )->SetRotation( -20.0f, -90.0f, 0.0f );

	GLuint tShipId	= mSceneManager->GetNode( "ship" )->GetModel()->GetModelId();
	mSceneManager->AddNode( "ship2", tShipId , tShipTex );
	mSceneManager->GetNode( "ship2" )->SetPosition( -20.0f, -50.0f, -50.0f );
	mSceneManager->GetNode( "ship2" )->SetScale( 0.01f );
	mSceneManager->GetNode( "ship2" )->SetRotation( -18.0f, -92.0f, 5.0f );

	mSceneManager->AddNode( "ship3", tShipId, tShipTex );
	mSceneManager->GetNode( "ship3" )->SetPosition( -10.0f, -10.0f, -150.0f );
	mSceneManager->GetNode( "ship3" )->SetScale( 0.01f );
	mSceneManager->GetNode( "ship3" )->SetRotation( -20.0f, -85.0f, 0.0f );

	// Star Field
	mStarField = new StarField( 0, 0, -1000, 600, 100, 1400, 2000 );

	// Shaders
	mShaderManager->CreateProgram( "mainMenuProgram" );
	mShaderManager->CreateShader( "mainMenuVertexShader", "data/shaders/menu.vert", VERTEX_SHADER );
	mShaderManager->CreateShader( "mainMenuFragmentShader", "data/shaders/menu.frag", FRAGMENT_SHADER );
	mShaderManager->AttachShader( "mainMenuProgram", "mainMenuVertexShader" );
	mShaderManager->AttachShader( "mainMenuProgram", "mainMenuFragmentShader" );

	mBackgroundMusic = mAudioManager->addSoundSourceFromFile( "data/sounds/song5.wav" );
	mBackgroundMusic->setDefaultVolume( mGameManager->GetMusicVolume() );
	mAudioManager->play2D( mBackgroundMusic, true );

	mTimeManager->StartTime();
}

void MainMenuState::ProcessInput() {
	if ( !GetLeftClick() ) {
		mFullscreenFlag = false;
		if ( mMusicSliderFlag || mSFXSliderFlag ) {
			float tMouseX		= (float)GetMouseX();
			float tCenterX		= mDisplayManager->GetScreenWidth() / 2.0f;
			float tLeftX		= tCenterX - 186.0f;

			float tMusicDiff	= 0.0f;
			if ( mMusicSliderFlag )
				tMusicDiff		= tMouseX - mMouseXClick;

			float tSFXDiff		= 0.0f;
			if ( mSFXSliderFlag )
				tSFXDiff		= tMouseX - mMouseXClick;

			float tMusicXPos	= tLeftX + mGameManager->GetMusicVolume() * 300.0f + tMusicDiff;
			float tSFXSXPos		= tLeftX + mGameManager->GetSFXVolume() * 300.0f + tSFXDiff;

			if ( tMusicXPos < tLeftX )
				tMusicXPos		= tLeftX;
			if ( tMusicXPos > tLeftX + 300.0f )
				tMusicXPos		= tLeftX + 300.0f;
			if ( tSFXSXPos < tLeftX )
				tSFXSXPos		= tLeftX;
			if ( tSFXSXPos > tLeftX + 300.0f )
				tSFXSXPos		= tLeftX + 300.0f;

			mFullscreenFlag		= false;
			if ( mMusicSliderFlag ) {
				mMusicSliderFlag = false;
				mGameManager->SetMusicVolume( ( tMusicXPos - tLeftX ) / 300.0f );
				mAudioManager->stopAllSounds();
				mBackgroundMusic->setDefaultVolume( mGameManager->GetMusicVolume() );
				mAudioManager->play2D( mBackgroundMusic, true );
			}
			if ( mSFXSliderFlag ) {
				mSFXSliderFlag = false;
				mGameManager->SetSFXVolume( ( tSFXSXPos - tLeftX ) / 300.0f );
			}
		}
	}
}

void MainMenuState::SetSettingsActive( bool settings ) {
	mSettingsActive = settings;
}

void MainMenuState::StopSounds() {
	mAudioManager->stopAllSounds();
}

Game* MainMenuState::GetGameMgr() {
	return mGameManager;
}

Display* MainMenuState::GetDisplay() {
	return mDisplayManager;
}