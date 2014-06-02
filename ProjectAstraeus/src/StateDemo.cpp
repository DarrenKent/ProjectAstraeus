/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include <GL\glew.h>

#include "Engine\Draw.h"
#include "Engine\Input.h"

#include "Parsers.h"
#include "StateDemo.h"

void JumpHandler( State * state ) {
	DemoState* mDS = dynamic_cast< DemoState* >( state );
	mDS->SetWin( true );
}

void ReloadHandler( State * state ) {
	DemoState* mDS = dynamic_cast< DemoState* >( state );
	mDS->SetReload( true );
}

DemoState::DemoState( Display *dMgr, Game *gMgr ) {
	mAstroidWave		= false;
	mDesertionZone		= false;
	mFired				= false;
	mFireTick			= false;
	mFPS				= false;
	mGameOver			= false;
	mHit				= false;
	mLeftCannon			= false;
	mMissileFired		= false;
	mMissileReloadTick	= false;
	mReloading			= false;
	mWin				= false;
	mDesertionTime		= 1.0f;
	mFTLCharge			= 0.0f;
	mXBoundary			= 0.0f;
	mYBoundary			= 0.0f;
	mAstraeusOreCount	= 0;
	mAstroidCount		= 10000;
	mGoalOreCount		= 50;
	mMaxPlayerOre		= 5;
	mOreCount			= 0;
	mPlayerOreCount		= 0;
	mReloadTime			= 0.0f;

	mAudioManager		= irrklang::createIrrKlangDevice();
	mCamera				= new OrbitalCamera();
	mDisplayManager		= dMgr;
	mGameManager		= gMgr;
	mShaderManager		= new ShaderManager();
	mTextureManager		= new TextureManager(); 
	mTimeManager		= new TimeManager();

}

DemoState::~DemoState() {
	if ( mAudioManager ) {
		delete mAudioManager;
		mAudioManager	= NULL;
	}
	if ( mCamera ) {
		delete mCamera;
		mCamera			= NULL;
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

void DemoState::DrawFrame() {
	mDisplayManager->SetPerspectiveProjection( 60.0f, 10.0f, 10000.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );

	float tPlayerX = mPlayer->GetXPos();
	float tPlayerY = mPlayer->GetYPos();
	float tPlayerZ = mPlayer->GetZPos();

	mCamera->SetTargetPos( tPlayerX, tPlayerY, tPlayerZ );
	mCamera->PositionCamera();

	mSceneManager->DrawSceneRange( tPlayerX, tPlayerY, tPlayerZ, 700.0f );

	DrawStarGrid();

	for ( unsigned int iWeapon = 0; iWeapon < mWeapons.size(); ++iWeapon ) {
		if ( mWeapons[iWeapon]->GetType() == Weapon::WEAPON_LASER )
			mWeapons[iWeapon]->DrawWeapon();
	}

	if ( mDesertionZone && mHit ) {
		glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
		glLineWidth( 2.0f );
		glBegin( GL_LINES );
			glVertex3f( mAstraeus->GetXPos(), mAstraeus->GetYPos(), 0.0f );
			glVertex3f( tPlayerX, tPlayerY, tPlayerZ );
		glEnd();
	}

	DrawGUI();
}

void DemoState::DrawGUI() {
	mDisplayManager->SetOrthographicProjection();
	glLoadIdentity();

	double	tActiveTime		= mTimeManager->GetActiveTime();
	float	tAstraeusX		= mAstraeus->GetXPos();
	float	tAstraeusY		= mAstraeus->GetYPos();
	float	tPlayerX		= mPlayer->GetXPos();
	float	tPlayerY		= mPlayer->GetYPos();
	float	tPlayerZ		= mPlayer->GetZPos();
	float	tSFXVolume		= mGameManager->GetSFXVolume();
	GLuint	tFontId			= mSceneManager->GetCurrentFontId();
	int		tFullscreen		= mDisplayManager->GetFullscreen();
	int		tScreenHeight	= mDisplayManager->GetScreenHeight();
	int		tScreenWidth	= mDisplayManager->GetScreenWidth();
	float	tScreenXCenter	= tScreenWidth / 2.0f;
	float	tScreenYCenter	= tScreenHeight / 2.0f;

	// Astraeus Ore Count
	if ( mAstraeusOreCount == mGoalOreCount )
		glColor4f( 0.0f, (float)cos( tActiveTime / 400.0f ) + 1.4f, 0.0f, 1.0f );
	else
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	std::string tAstraeusOreString = "Ore: " + std::to_string( (long double)( mAstraeusOreCount ) ) +
									 " / " + std::to_string( (long double)( mGoalOreCount ) );
	DrawText2D( tAstraeusOreString.c_str(), tFontId, 320.0f, 26.0f );

	// FTL Time
	if ( mFTLCharge == 100.0f )
		glColor4f( 0.0f, (float)cos( tActiveTime / 400.0f ) + 1.4f, 0.0f, 1.0f );
	else
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	std::string tFTLString = "FLT Charge: " + std::to_string( (long double)( (int)mFTLCharge ) ) + " / 100%% ";
	DrawText2D( tFTLString.c_str(), tFontId, 320.0f, 8.0f );

	// Player Ore Count
	if ( mPlayerOreCount == mMaxPlayerOre )
		glColor4f( (float)cos( tActiveTime / 400.0f ) + 1.4f, 0.0f, 0.0f, 1.0f );
	else
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	std::string tPlayerOreString = "Ore: " + std::to_string( (long double)( mPlayerOreCount ) ) +
								   " / " + std::to_string( (long double)( mMaxPlayerOre ) );
	DrawText2D( tPlayerOreString.c_str(), tFontId, tScreenWidth - 375.0f, 26.0f );

	// Astraeus Health Text
	glColor4f( 0.6f, 0.4f, 0.0f, 1.0f );
	std::string tAstraeusHealthString = "Health: " + std::to_string( (long double)mAstraeus->GetHealth() ) +
										" / " + std::to_string( (long double)mAstraeus->GetMaxHealth() );
	DrawText2D( tAstraeusHealthString.c_str(), tFontId, 60.0f, 23.0f );

	// Astraeus Shields Text
	std::string tAstraeusShieldString = "Shields: " + std::to_string( (long double)mAstraeus->GetShields() ) +
										" / " + std::to_string( (long double)mAstraeus->GetMaxShields() );
	DrawText2D( tAstraeusShieldString.c_str(), tFontId, 60.0f, 8.0f );

	// Health Text
	std::string tHealthString = "Health: " + std::to_string( (long double)mPlayer->GetHealth() ) + 
								" / " + std::to_string( (long double)mPlayer->GetMaxHealth() );
	DrawText2D( tHealthString.c_str(), tFontId, tScreenWidth - 190.0f, 23.0f );

	// Shields Text
	std::string tShieldString = "Shields: " + std::to_string( (long double)mPlayer->GetShields() ) + 
								" / " + std::to_string( (long double)mPlayer->GetMaxShields() );
	DrawText2D( tShieldString.c_str(), tFontId, tScreenWidth - 190.0f, 8.0f );

	// XPos
	std::string tXPos = "X: " + std::to_string( (long double)( (int)tPlayerX ) );
	DrawText2D( tXPos.c_str(), tFontId, tScreenWidth - 350.0f, 85.0f );

	// YPos
	std::string tYPos = "Y: " + std::to_string( (long double)( (int)tPlayerY ) );
	DrawText2D( tYPos.c_str(), tFontId, tScreenWidth - 350.0f, 70.0f );

	// Astraeus XPos
	std::string tAstraeusXPos = "X: " + std::to_string( (long double)( (int)tPlayerX ) );
	DrawText2D( tAstraeusXPos.c_str(), tFontId, 300.0f, 85.0f );

	// Astraeus YPos
	std::string tAstraeusYPos = "Y: " + std::to_string( (long double)( (int)tPlayerY ) );
	DrawText2D( tAstraeusYPos.c_str(), tFontId, 300.0f, 70.0f );

	// FPS Text
	if ( mFPS ) {
		float tFPS = mTimeManager->GetFramesPerSecond();
		if ( tFPS >= 60 )
			glColor3f( 0.0f, 1.0f, 0.0f );
		else if ( tFPS <= 28 )
			glColor3f( 1.0f, 0.0f, 0.0 );
		else
			glColor3f( 1.0f, 1.0f, 1.0f );
		std::string tFPSString = "FPS: " + std::to_string( (long double)tFPS );
		DrawText2D( tFPSString.c_str(), tFontId, 10.0f, 160.0f );
	}

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glDisable( GL_LIGHTING );
	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// Desertion Warning
	if ( mPlayer->GetAlive() && ( tPlayerX < mXBoundary * 2 || 
		 tPlayerX > mGridWidth * 1500 - mXBoundary * 2 ||
		 tPlayerY < mYBoundary * 2 || tPlayerY > mGridWidth * 1500 - mYBoundary * 2 ) ) {
		
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "warningDesertion" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, (float)cos( tActiveTime / 400.0f ) + 1.2f );
		DrawRectangle2D( tScreenXCenter - 200.0f, tScreenHeight - 250.0f, 
						 tScreenXCenter + 200.0f, tScreenHeight - 50.0f );
		mDesertionZone = true;
	} else {
		mDesertionZone = false;
	}

	// Game Over
	if ( mGameOver ) {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "gameOver" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		DrawRectangle2D( tScreenXCenter - 200.0f, tScreenHeight - 250.0f, 
						 tScreenXCenter + 200.0f, tScreenHeight - 50.0f );
	}

	// Win
	if ( mWin ) {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "missionSuccess" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		DrawRectangle2D( tScreenXCenter - 200.0f, tScreenHeight - 250.0f, 
						 tScreenXCenter + 200.0f, tScreenHeight - 50.0f );
	}

	// Center UI Panel
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "centerUIPanel" ) );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	DrawRectangle2D( 0.0f, 0.0f, (float)tScreenWidth, 100.0f );

	// Right UI Panel
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "rightUIPanel" ) );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	DrawRectangle2D( tScreenWidth - 300.0f, 0.0f, (float)tScreenWidth, 150.0f );

	// Left UI Panel
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "rightUIPanel" ) );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	DrawRectangle2D( 300.0f, 0.0f, 0.0f, 150.0f );

	// Health Warning
	if ( (float)mPlayer->GetHealth() / (float)mPlayer->GetMaxHealth() < 0.2 ) {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "hullLow" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, (float)cos( tActiveTime / 400.0f ) + 1.2f );
		DrawRectangle2D( tScreenWidth - 50.0f, 90.0f, tScreenWidth - 10.0f, 130.0f );
	}

	// Astraeus Health Warning
	if ( (float)mAstraeus->GetHealth() / (float)mAstraeus->GetMaxHealth() < 0.2 ) {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "hullLow" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, (float)cos( tActiveTime / 400.0f ) + 1.2f );
		DrawRectangle2D( 10.0, 90.0f, 50.0f, 130.0f );
	}

	// Shields Warning
	if ( (float)mPlayer->GetShields() / (float)mPlayer->GetMaxShields() < 0.2 ) {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "shieldsLow" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, (float)cos( tActiveTime / 400.0f ) + 1.2f );
		DrawRectangle2D( tScreenWidth - 100.0f, 90.0f, tScreenWidth - 60.0f, 130.0f );
	}

	// Astraeus Shields Warning
	if ( (float)mAstraeus->GetShields() / (float)mAstraeus->GetMaxShields() < 0.2 ) {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "shieldsLow" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, (float)cos( tActiveTime / 400.0f ) + 1.2f );
		DrawRectangle2D( 60.0, 90.0f, 100.0f, 130.0f );
	}

	// Missiles Warning
	if ( mPlayer->GetMissiles() <= 0 ) {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "missileEmpty" ) );
		glColor4f( 1.0f, 1.0f, 1.0f, (float)cos( tActiveTime / 400.0f ) + 1.2f );
		DrawRectangle2D( tScreenWidth - 150.0f, 90.0f, tScreenWidth - 110.0f, 130.0f );
	}

	// Missiles
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "missile" ) );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	for ( int iMissile = 0; iMissile < mPlayer->GetMissiles(); ++iMissile )
		DrawRectangle2D( tScreenWidth - 15.0f - 32.0f * ( iMissile + 1 ), tScreenHeight - 104.0f, 
						 tScreenWidth - 15.0f - 32.0f * iMissile, tScreenHeight - 40.0f );

	// Shields
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "shields" ) );
	glColor4f( 0.0f, mPlayer->GetShields() / (float)mPlayer->GetMaxShields(), 
			   mPlayer->GetShields() / (float)mPlayer->GetMaxShields(), 
			   mPlayer->GetShields() / (float)mPlayer->GetMaxShields() );
	DrawRectangle2D( mHealthModel->GetXPos() - 100.0f, mHealthModel->GetYPos() - 25.0f, 
					 mHealthModel->GetXPos() + 100.0f, mHealthModel->GetYPos() + 25.0f );

	// Astraeus Shields
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "shields" ) );
	glColor4f( 0.0f, mAstraeus->GetShields() / (float)mAstraeus->GetMaxShields(), 
			   mAstraeus->GetShields() / (float)mAstraeus->GetMaxShields(), 
			   mAstraeus->GetShields() / (float)mAstraeus->GetMaxShields() );
	DrawRectangle2D( mAstraeusHealth->GetXPos() - 125.0f, mAstraeusHealth->GetYPos() - 33.0f,
					 mAstraeusHealth->GetXPos() + 125.0f, mAstraeusHealth->GetYPos() + 33.0f );

	// Astraeus Location
	float tAstraeusDistance = std::sqrt( std::pow( tPlayerX - mAstraeus->GetXPos(), 2 ) + 
										 std::pow( tPlayerY - mAstraeus->GetYPos(), 2 ) );
	if ( tAstraeusDistance > 300.0f ) {
			float tSlope	= ( tPlayerY - tAstraeusY ) /  ( tPlayerX - tAstraeusX );
			float tB		= tScreenYCenter - tScreenXCenter * tSlope;
			float tXPos		= 0.0f;
			float tYPos		= 0.0f;

			if ( tPlayerX > tAstraeusX && tPlayerY > tAstraeusY ) {
				tXPos = -tB / tSlope;
				tYPos =	tB;
			} else if ( tPlayerX < tAstraeusX && tPlayerY > tAstraeusY ) {
				tXPos = -tB / tSlope;
				tYPos = tScreenWidth * tSlope + tB;
			} else if ( tPlayerX < tAstraeusX && tPlayerY < tAstraeusY ) {
				tXPos = ( tScreenHeight - tB ) / tSlope;
				tYPos = tScreenWidth * tSlope + tB;
			} else {
				tXPos = ( tScreenHeight - tB ) / tSlope;
				tYPos = tB;
			}

			if ( tXPos > tScreenWidth )
				tXPos = tScreenWidth - 50.0f;
			else if ( tXPos < 0 )
				tXPos = 0;
			if ( tYPos > tScreenHeight )
				tYPos = tScreenHeight - 50.0f;
			else if ( tYPos < 0 )
				tYPos = 0;
			
			float tX2Pos = tXPos + 50.0f;
			float tY2Pos = tYPos + 50.0f;

			glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "astraeusLoc" ) );
			glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
			DrawRectangle2D( tXPos, tYPos, tX2Pos, tY2Pos );
	}

	// Incomming Astroids
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "incoming" ) );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	for ( unsigned int iRock = 0; iRock < mIncoming.size(); ++iRock ) {
		float tRockXPos		= mIncoming[iRock]->GetXPos();
		float tRockYPos		= mIncoming[iRock]->GetYPos();
		float tRockDistance = std::sqrt( std::pow( tPlayerX - tRockXPos, 2 ) + 
										 std::pow( tPlayerY - tRockYPos, 2 ) );
		if ( tRockYPos < 7500.0f && tRockDistance > 100.0f ) {
			float tSlope	= (float)( tPlayerY - tRockYPos ) /  (float)( tPlayerX - tRockXPos );
			float tB		= tScreenYCenter - tScreenXCenter * tSlope;
			float tXPos		= 0.0f;
			float tYPos		= 0.0f;

			if ( tPlayerX > tRockXPos && tPlayerY > tRockYPos ) {
				tXPos = -tB / tSlope;
				tYPos = tB;
			} else if ( tPlayerX < tRockXPos && tPlayerY > tRockYPos ) {
				tXPos = -tB / tSlope;
				tYPos = tScreenWidth * tSlope + tB;
			} else if ( tPlayerX < tRockXPos && tPlayerY < tRockYPos ) {
				tXPos = ( tScreenHeight - tB ) / tSlope;
				tYPos = tScreenWidth * tSlope + tB;
			} else {
				tXPos = ( tScreenHeight - tB ) / tSlope;
				tYPos = tB;
			}

			if ( tXPos > tScreenWidth )
				tXPos = tScreenWidth - 50.0f;
			else if ( tXPos < 0.0f )
				tXPos = 0.0f;
			if ( tYPos > tScreenHeight )
				tYPos = tScreenHeight - 50.0f;
			else if ( tYPos < 0.0f )
				tYPos = 0.0f;
			
			float tX2Pos = tXPos + 50.0f;
			float tY2Pos = tYPos + 50.0f;

			DrawRectangle2D( tXPos, tYPos, tX2Pos, tY2Pos );
		}	
	}

	// Buttons
	float tButtonScale	= 0.6f;
	float tButtonWidth	= 128.0f;
	float tButtonHeight = 64.0f;
	if ( mAstraeusOreCount >= mGoalOreCount && mFTLCharge == 100.0f ) {
		mJumpButton->DrawButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
	} else {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "jumpGrey" ) );
		DrawRectangle2D( 360.0f, 65.0f, 
						 360.0f + tButtonWidth * tButtonScale, 
						 65.0f + tButtonHeight * tButtonScale );
	}

	if ( tAstraeusDistance < 100.0f ) {
		mReloadButton->DrawButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
	} else {
		glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "reloadGrey" ) );
		DrawRectangle2D( tScreenWidth - 445.0f, 65.0f,
						 tScreenWidth - 445.0f + tButtonWidth * tButtonScale, 
						 65.0f + tButtonHeight * tButtonScale );
	}

	// Cursor
	glBindTexture( GL_TEXTURE_2D, mTextureManager->GetTextureId( "cursor" ) );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	DrawRectangle2D( GetMouseX() - 16.0f, tScreenHeight - GetMouseY() - 14.0f, 
					 GetMouseX() + 16.0f, tScreenHeight - GetMouseY() + 14.0f );

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );

	// Health Model
	GLenum tHealthProgram = mShaderManager->GetProgram( "healthProgram" )->GetProgramHandle();
	mShaderManager->UseProgram( "healthProgram" );
	GLint tPlayerHealth = { mPlayer->GetHealth() };
	GLint tLocation = glGetUniformLocation( tHealthProgram, "tHealth" );
	glUniform1i( tLocation, tPlayerHealth );
	GLint tPlayerMaxHealth = { mPlayer->GetMaxHealth() };
	GLint tLocation2 = glGetUniformLocation( tHealthProgram, "tMaxHealth" );
	glUniform1i( tLocation2, tPlayerMaxHealth );
	mHealthModel->SetRotation( 0.0f, (float)tActiveTime / 20.0f, -90.0f );
	mHealthModel->DrawNode();
	mShaderManager->ClearProgram();

	// Astraeus Health Model
	mShaderManager->UseProgram( "healthProgram" );
	GLint tAstraeusHealth = { mAstraeus->GetHealth() };
	GLint tLocation3 = glGetUniformLocation( tHealthProgram, "tHealth" );
	glUniform1i( tLocation3, tAstraeusHealth );
	GLint tAstraeusMaxHealth = { mAstraeus->GetMaxHealth() };
	GLint tLocation4 = glGetUniformLocation( tHealthProgram, "tMaxHealth" );
	glUniform1i( tLocation4, tAstraeusMaxHealth );
	mAstraeusHealth->SetRotation( 0.0f, (float)tActiveTime / 20.0f, -90.0f );
	mAstraeusHealth->DrawNode();
	mShaderManager->ClearProgram();
}

void DemoState::DrawStarGrid() {
	int tGridXLoc = ( (int)mPlayer->GetXPos() / 1500 );
	int tGridYLoc = ( (int)mPlayer->GetYPos() / 1500 );
	
	unsigned int tLoc1 = tGridXLoc + tGridYLoc * mGridWidth;
	if ( tLoc1 >= 0 && tLoc1 < mStarFields.size() )
		mStarFields[tLoc1]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc2 = ( tGridXLoc - 1 ) + tGridYLoc * mGridWidth;
	if ( tLoc2 >= 0 && tLoc2 < mStarFields.size() )
		mStarFields[tLoc2]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc3 = tGridXLoc + ( tGridYLoc - 1 ) * mGridWidth;
	if ( tLoc3 >= 0 && tLoc3 < mStarFields.size() )
		mStarFields[tLoc3]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc4 = ( tGridXLoc - 1 ) + ( tGridYLoc - 1 ) * mGridWidth;
	if ( tLoc4 >= 0 && tLoc4 < mStarFields.size() )
		mStarFields[tLoc4]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc5 = ( tGridXLoc - 1 ) + ( tGridYLoc + 1 ) * mGridWidth;
	if ( tLoc5 >= 0 && tLoc5 < mStarFields.size() )
		mStarFields[tLoc5]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc6 = ( tGridXLoc ) + ( tGridYLoc + 1 ) * mGridWidth;
	if ( tLoc6 >= 0 && tLoc6 < mStarFields.size() )
		mStarFields[tLoc6]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc7 = ( tGridXLoc + 1 ) + ( tGridYLoc + 1 ) * mGridWidth;
	if ( tLoc7 >= 0 && tLoc7 < mStarFields.size() )
		mStarFields[tLoc7]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc8 = ( tGridXLoc + 1 ) + ( tGridYLoc ) * mGridWidth;
	if ( tLoc8 >= 0 && tLoc8 < mStarFields.size() )
		mStarFields[tLoc8]->Draw( mTimeManager->GetDeltaTimeSeconds() );
	unsigned int tLoc9 = ( tGridXLoc + 1 ) + ( tGridYLoc - 1 ) * mGridWidth;
	if ( tLoc9 >= 0 && tLoc9 < mStarFields.size() )
		mStarFields[tLoc9]->Draw( mTimeManager->GetDeltaTimeSeconds() );
}

void DemoState::InitializeState() {
	mSceneManager	= new SceneManager( mDisplayManager->GetHDC() );
	mSceneManager->AddFont( "arial", L"arial", 16 );
	mSceneManager->GetFont( "arial" )->SetWeight( 1000 );
	mSceneManager->SetFont( "arial" );

	ShowCursor( false );

	mCamera->SetCameraPos( -1000.0f, -1000.0f, 100.0f );
	mCamera->SetCameraRadius( 100.0f );
	mCamera->SetCameraTheta( 89.0f );
	mCamera->SetCameraAlpha( -90.0f );
	mCamera->PositionCamera();

	mGridWidth	= 7;
	mGridHeight	= 7;
	mXBoundary	= 1500.0f;
	mYBoundary	= 1500.0f;

	for( int iSectorY = 0; iSectorY < mGridHeight; ++iSectorY ) {
		for ( int iSectorX = 0; iSectorX < mGridWidth; ++iSectorX ) {
			int tXPos = iSectorX * 1500 + 750;
			int tYPos = iSectorY * 1500 + 750;
			StarField *tStarField = new StarField( tXPos, tYPos, -1000, 750, 100, 750, 800 );
			mStarFields.push_back( tStarField );
		}
	}

	mShaderManager->CreateProgram( "demoProgram" );
	mShaderManager->CreateShader( "demoVertexShader", "data/shaders/demo.vert", VERTEX_SHADER );
	mShaderManager->CreateShader( "demoFragmentShader", "data/shaders/demo.frag", FRAGMENT_SHADER );
	mShaderManager->AttachShader( "demoProgram", "demoVertexShader" );
	mShaderManager->AttachShader( "demoProgram", "demoFragmentShader" );

	mShaderManager->CreateProgram( "healthProgram" );
	mShaderManager->CreateShader( "healthVertexShader", "data/shaders/health.vert", VERTEX_SHADER );
	mShaderManager->CreateShader( "healthFragmentShader", "data/shaders/health.frag", FRAGMENT_SHADER );
	mShaderManager->AttachShader( "healthProgram", "healthVertexShader" );
	mShaderManager->AttachShader( "healthProgram", "healthFragmentShader" );

	mShaderManager->CreateProgram( "oreProgram" );
	mShaderManager->CreateShader( "oreVertexShader", "data/shaders/ore.vert", VERTEX_SHADER );
	mShaderManager->CreateShader( "oreFragmentShader", "data/shaders/ore.frag", FRAGMENT_SHADER );
	mShaderManager->AttachShader( "oreProgram", "oreVertexShader" );
	mShaderManager->AttachShader( "oreProgram", "oreFragmentShader" );

	mTextureManager->LoadTexture( "warningDesertion", "data/textures/warningDesertion.tga", false );
	mTextureManager->LoadTexture( "rightUIPanel", "data/textures/rightUIPanel.tga", false );
	mTextureManager->LoadTexture( "shields", "data/textures/shields.tga", false );
	mTextureManager->LoadTexture( "centerUIPanel", "data/textures/centerUIPanel.tga", false );
	mTextureManager->LoadTexture( "hullLow", "data/textures/hulllow.tga", false );
	mTextureManager->LoadTexture( "shieldsLow", "data/textures/shieldslow.tga", false );
	mTextureManager->LoadTexture( "missileEmpty", "data/textures/missileempty.tga", false );
	mTextureManager->LoadTexture( "missile", "data/textures/missile.tga", false );
	mTextureManager->LoadTexture( "gameOver", "data/textures/gameover.tga", false );
	mTextureManager->LoadTexture( "missionSuccess", "data/textures/missioncomplete.tga", false );
	mTextureManager->LoadTexture( "astraeusLoc", "data/textures/astraeusLoc.tga", false );
	mTextureManager->LoadTexture( "missileTex", "data/textures/missiletex.tga", false );
	mTextureManager->LoadTexture( "incoming", "data/textures/incoming.tga", false );
	mTextureManager->LoadTexture( "cursor", "data/textures/cursor.tga", false );
	mTextureManager->LoadTexture( "jumpGrey", "data/textures/jumpgrey.tga", false );
	mTextureManager->LoadTexture( "jumpReady", "data/textures/jumpready.tga", false );
	mTextureManager->LoadTexture( "jumpHover", "data/textures/jumphover.tga", false );
	mTextureManager->LoadTexture( "reloadGrey", "data/textures/rrgrey.tga", false );
	mTextureManager->LoadTexture( "reloadReady", "data/textures/rrready.tga", false );
	mTextureManager->LoadTexture( "reloadHover", "data/textures/rrhover.tga", false );

	std::string tClickSound = "data/sounds/sfx2.wav";
	std::string tHoverSound = "data/sounds/sfx3.wav";
	mJumpButton = new Button( 360.0f, 65.0f, mTextureManager->GetTexture( "jumpReady" ), 
							  mTextureManager->GetTexture( "jumpHover" ), JumpHandler, this, 
							  mAudioManager, tClickSound, tHoverSound ); 
	mJumpButton->SetScale( 0.6f );
	mReloadButton = new Button( mDisplayManager->GetScreenWidth() - 445.0f, 65.0f,
								mTextureManager->GetTexture( "reloadReady" ), 
								mTextureManager->GetTexture( "reloadHover" ), ReloadHandler, this, 
								mAudioManager, tClickSound, tHoverSound ); 
	mReloadButton->SetScale( 0.6f );

	ParseLevel( this, mAudioManager, "data/levels/demo.xml" );
	mOreParent			= mSceneManager->AddNode( "oreParent", "data/models/astroid1.obj", NULL );
	mOreParent->SetVisible( false );
	mMissileParent		= mSceneManager->AddNode( "missileParent", "data/models/missile.obj", NULL );
	mMissileParent->SetVisible( false );

	mPlayer				= new Player( mSceneManager->GetNode( "player" ) );
	mSceneManager->GetNode( "player" )->GetModel()->SetShaderProgram( mShaderManager->GetProgram( "demoProgram" ) );
	mAstraeus			= new Astraeus( mSceneManager->GetNode( "astraeus" ) );
	mSceneManager->GetNode( "astraeus" )->GetModel()->SetShaderProgram( mShaderManager->GetProgram( "demoProgram" ) );

	mHealthModel		= new Node( "healthModel", mSceneManager->GetNode( "player" )->GetModel()->GetModelId(), NULL );
	mHealthModel->SetScale( 0.02f );
	mHealthModel->SetPosition( mDisplayManager->GetScreenWidth() - 120.0f, 65.0f, 0.0f );
	mHealthModel->SetRotation( 0.0f, 120.0f, -95.0 );

	mAstraeusHealth		= new Node( "astraeusHealth", mSceneManager->GetNode( "astraeus" )->GetModel()->GetModelId(), NULL );
	mAstraeusHealth->SetScale( 0.35f );
	mAstraeusHealth->SetPosition( 125.0f, 65.0f, 0.0f );
	mAstraeusHealth->SetRotation( 0.0f, 120.0f, -95.0f );

	mTimeManager->StartTime();
}

void DemoState::GameLogic() {
	mTimeManager->TimeFrame();

	float tAstraeusX	= mAstraeus->GetXPos();
	float tAstraeusY	= mAstraeus->GetYPos();
	float tPlayerX		= mPlayer->GetXPos();
	float tPlayerY		= mPlayer->GetYPos();
	float tPlayerZ		= mPlayer->GetZPos();

	//Player
	if ( !mGameOver && !mWin ) {
		if ( mPlayer->GetHealth() <= 0 ) {
			mGameOver = true;
			mPlayer->KillPlayer();
		} else if ( (float)mPlayer->GetHealth() / (float)mPlayer->GetMaxHealth() < 0.2f ) {
			if ( mSFX.find( "playerHealthLow" ) != mSFX.end() && 
				!mAudioManager->isCurrentlyPlaying( mSFX[ "playerHealthLow" ] ) ) {

				mSFX["playerHealthLow"]->setDefaultVolume( mGameManager->GetSFXVolume() );
				mAudioManager->play2D( mSFX["playerHealthLow"] );
			}
		}

		if ( mAstraeus->GetHealth() <= 0 )
			mGameOver = true;
		else
			mAstraeus->AstraeusLogic( mTimeManager->GetDeltaTimeSeconds() );

		if ( (float)mAstraeus->GetHealth() / (float)mAstraeus->GetMaxHealth() < 0.2f ) {
			if ( mSFX.find( "astraeusHealthLow" ) != mSFX.end() && 
				 !mAudioManager->isCurrentlyPlaying( mSFX[ "astraeusHealthLow" ] ) ) {

				mSFX["astraeusHealthLow"]->setDefaultVolume( mGameManager->GetSFXVolume() );
				mAudioManager->play2D( mSFX["astraeusHealthLow"] );
			}
		}

		// Astroid Wave;
		int tWaveTime = 900;
		if ( !mAstroidWave && (int)( mTimeManager->GetActiveTime() / 100 ) % tWaveTime == 0 ) {
			mAstroidWave	= true;
			std::vector< Node* > tAstroids;
			int tRockCount	= rand() % 4 + 2;
			int tMaxSize	= 30;
			int tMinSize	= 5;
			for ( int iRock = 0; iRock < tRockCount; ++iRock ) {
				float tSize		= 1.0f;
				int	  tX		= 0;
				int   tY		= 0;
				int   tZ		= 0;
				while ( true ) {
					tSize = ( rand() % ( tMaxSize - tMinSize ) + tMinSize ) / 100.0f;
					tX	  = rand() % 1500 + 5000;
					tY    = rand() % 1000 + ( mGridHeight - 1 ) * 1500;
					tZ    = 0;

					// Astroid Collision with Astroids
					bool tCollided = false;
					for ( unsigned int iCollision = 0; iCollision < tAstroids.size(); ++iCollision ) {
						float tColRadius	 = tAstroids[iCollision]->GetScale() * 100.0f;
						float tColX			 = tAstroids[iCollision]->GetXPos();
						float tColY			 = tAstroids[iCollision]->GetYPos();
						float tCollisionDist = sqrt( std::pow( ( tColX - tX ), 2 ) + 
													 std::pow( ( tColY - tY ), 2 ) );
						if ( tCollisionDist < tColRadius + tSize * 100.0f + 10.0f ) {
							tCollided = true;
							break;
						}
					}
					if ( !tCollided )
						break;
				}

				std::string tId = "astroid" + std::to_string( (long double)( mAstroidCount ) );
				mAstroidCount ++;
				int    tModel	= rand() % 10;
				GLuint tModelId = mSceneManager->GetNode( "astroid" + std::to_string( (long double)tModel ) )->GetModel()->GetModelId();
				Node*  tAstroid	= mSceneManager->AddNode( tId, tModelId, mTextureManager->GetTexture( "astroidTex0" ) );
				tAstroid->SetPosition( (float)tX, (float)tY, (float)tZ );
				tAstroid->SetScale( tSize );
				tAstroid->GetModel()->SetShaderProgram( mShaderManager->GetProgram( "demoProgram" ) );

				tAstroids.push_back( tAstroid );
				Astroid *tAstroidInstance = new Astroid( tAstroid );

				int tOreState = ( rand() % 100 );
				if ( tOreState < 10 )
					tAstroidInstance->SetOre( true );

				tAstroidInstance->SetHealth( (int)( tSize * 1000 ) );

				int tYawDir		= ( rand() % 3 );
				if ( tYawDir == 2 )
					tYawDir		= -1;
				int tPitchDir	= ( rand() % 3 );
				if ( tPitchDir == 2 )
					tPitchDir	= -1;
				int tRollDir	= ( rand() % 3 );
				if ( tRollDir == 2 )
					tRollDir	= -1;

				float tYawRot	= ( rand() % 100 ) / 10.0f * tYawDir;
				float tPitchRot = ( rand() % 100 ) / 10.0f * tPitchDir;
				float tRollRot	= ( rand() % 100 ) / 10.0f * tRollDir;

				tAstroidInstance->SetRotationVeloctiy( tYawRot, tPitchRot, tRollRot );

				float tDir	= atan2( mAstraeus->GetYPos() - tY, mAstraeus->GetXPos() - tX );
				float tDX	= cos( tDir ) * 50.0f;
				float tDY	= sin( tDir ) * 50.0f;

				tAstroidInstance->SetVelocity( tDX, tDY, 0.0f );
				AddAstroid( tAstroidInstance );
				mIncoming.push_back( tAstroidInstance );
			}
		} else if ( (int)( mTimeManager->GetActiveTime() / 100 ) % tWaveTime != 0 ) {
			mAstroidWave = false;
		}

		// Player Firing Rate
		if ( !mFireTick && ( (int)( mTimeManager->GetActiveTime() / 100 ) % 2 == 0 ) ) {
			mFired		= false;
			mFireTick	= true;
		} else if ( ( (int)( mTimeManager->GetActiveTime() / 100 ) % 2 != 0 ) ) {
			mFireTick	= false;
		}

		// FTL Charging
		mFTLCharge += mTimeManager->GetDeltaTimeSeconds() / 2.0f;
		if ( mFTLCharge > 100.0f )
			mFTLCharge	= 100.0f;

		// Player Boundry
		if ( tPlayerX < mXBoundary - 500.0f )
			mPlayer->SetXPos( mXBoundary - 500.0f );
		if ( tPlayerX > mGridWidth * 1500.0f - mXBoundary + 500.0f )
			mPlayer->SetXPos(  mGridWidth * 1500.0f - mXBoundary + 500.0f );
		if ( tPlayerY < mYBoundary - 500.0f )
			mPlayer->SetYPos( mYBoundary - 500.0f );
		if ( tPlayerY >  mGridHeight * 1500.0f - mYBoundary + 500.0f )
			mPlayer->SetYPos(  mGridHeight * 1500.0f - mYBoundary + 500 );

		// Desertion Zone
		if ( mDesertionZone )
			mDesertionTime += mTimeManager->GetDeltaTimeSeconds();
		else
			mDesertionTime = 1.0f;

		if ( mPlayer->GetAlive() && mDesertionZone && (int)mDesertionTime % 6 == 0 && !mHit ) {
			mPlayer->DealDamage( 200 );
			if ( mSFX.find( "astraeusFire" ) != mSFX.end() ) {
				mSFX["astraeusFire"]->setDefaultVolume( mGameManager->GetSFXVolume() );
				mAudioManager->play2D( mSFX["astraeusFire"] );
			}
			mHit = true;
		} else if ( !mDesertionZone || mDesertionZone && (int)mDesertionTime % 6 != 0 ) {
			mHit = false;
		}

		if ( mPlayerOreCount < mMaxPlayerOre ) {
			for ( unsigned int iOre = 0; iOre < mOre.size(); ++iOre ) {
				float tDistance = sqrt( std::pow( tPlayerX - mOre[iOre]->GetXPos(), 2 ) + 
										std::pow( tPlayerY - mOre[iOre]->GetYPos(), 2 ) );
				if ( tDistance < 10.0f + mOre[iOre]->GetScale() ) {
					mPlayerOreCount ++;
					mSceneManager->RemoveNode( mOre[iOre]->GetId() );
					mOre.erase( mOre.begin() + iOre );
					iOre --;
					if ( mSFX.find( "orePickup" ) != mSFX.end() ) {
						mSFX["orePickup"]->setDefaultVolume( mGameManager->GetSFXVolume() );
						mAudioManager->play2D( mSFX["orePickup"] );
					}
				}
			}
		}

		float tAstraeusDistance = sqrt( std::pow( tPlayerX - tAstraeusX, 2 ) + 
										std::pow( tPlayerY - tAstraeusY, 2 ) );
		if ( mPlayerOreCount > 0 && tAstraeusDistance < 50.0f ) {
			mAstraeusOreCount += mPlayerOreCount;
			mPlayerOreCount	  = 0;
			if ( mSFX.find( "oreDropOff" ) != mSFX.end() ) {
				mSFX["oreDropOff"]->setDefaultVolume( mGameManager->GetSFXVolume() );
				mAudioManager->play2D( mSFX["oreDropOff"] );
			}
		}

		// Buttons
		int   tScreenWidth	= mDisplayManager->GetScreenWidth();
		int	  tScreenHeight = mDisplayManager->GetScreenHeight();
		float tSFXVolume	= mGameManager->GetSFXVolume();
		int	  tFullscreen	= mDisplayManager->GetFullscreen();
		if ( mAstraeusOreCount >= mGoalOreCount && mFTLCharge == 100 )
			mJumpButton->ExecuteButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );
		if ( tAstraeusDistance < 100.0f )
			mReloadButton->ExecuteButton( tScreenWidth, tScreenHeight, tFullscreen, tSFXVolume );

		// Reload Missiles and Health
		if ( mReloading ) {
			mSceneManager->GetNode( "player" )->SetVisible( false );
			mReloadTime += mTimeManager->GetDeltaTimeSeconds();
			mPlayer->SetPosition( 5000.0f, 5000.0f, 0.0f );
			mPlayer->SetVelocity( 0.0f, 0.0f, 0.0f );
			if ( !mMissileReloadTick && (int)mReloadTime % 2 == 0 ) {
				mMissileReloadTick = true;
				mPlayer->ChangeHealth( (int)( mPlayer->GetMaxHealth() / 10.0f ) );
				mPlayer->ChangeShields( (int)( mPlayer->GetMaxShields() / 10.0f ) );
				mPlayer->AddMissile();
			} else if ( (int)mReloadTime % 2 != 0 ) {
				mMissileReloadTick = false;
			}

			if ( mReloadTime >= 18.0f ) {
				mSceneManager->GetNode( "player" )->SetVisible( true );
				mReloading			= false;
				mReloadTime			= 0.0f;
				mMissileReloadTick	= false;
			}
		}
		mPlayer->PlayerLogic( mTimeManager->GetDeltaTimeSeconds() );
	}

	// Weapons
	for ( unsigned int iWeapon = 0; iWeapon < mWeapons.size(); ++iWeapon ) {
		mWeapons[iWeapon]->WeaponLogic( mTimeManager->GetDeltaTimeSeconds() );

		// Out of Bounds Delete
		if ( mWeapons[iWeapon]->GetDestroy() ) {
			if ( mWeapons[iWeapon]->GetType() == Weapon::WEAPON_MISSILE )
				mSceneManager->RemoveNode( mWeapons[iWeapon]->GetNode()->GetId () );
			mWeapons.erase( mWeapons.begin() + iWeapon );
		} else if ( mWeapons[iWeapon]->GetXPos() < 0 ) {
			if ( mWeapons[iWeapon]->GetType() == Weapon::WEAPON_MISSILE )
				mSceneManager->RemoveNode( mWeapons[iWeapon]->GetNode()->GetId () );
			mWeapons.erase( mWeapons.begin() + iWeapon );
		} else if ( mWeapons[iWeapon]->GetXPos() > mGridWidth * 1500.0f ) {
			if ( mWeapons[iWeapon]->GetType() == Weapon::WEAPON_MISSILE )
				mSceneManager->RemoveNode( mWeapons[iWeapon]->GetNode()->GetId () );
			mWeapons.erase( mWeapons.begin() + iWeapon );
		} else if ( mWeapons[iWeapon]->GetYPos() < 0 ) {
			if ( mWeapons[iWeapon]->GetType() == Weapon::WEAPON_MISSILE )
				mSceneManager->RemoveNode( mWeapons[iWeapon]->GetNode()->GetId () );
			mWeapons.erase( mWeapons.begin() + iWeapon );
		} else if ( mWeapons[iWeapon]->GetYPos() >  mGridHeight * 1500.0f ) {
			if ( mWeapons[iWeapon]->GetType() == Weapon::WEAPON_MISSILE )
				mSceneManager->RemoveNode( mWeapons[iWeapon]->GetNode()->GetId () );
			mWeapons.erase( mWeapons.begin() + iWeapon );
		}
	}

	// Rocks
	for ( unsigned int iRock = 0; iRock < mAstroids.size(); ++iRock ) {
		mAstroids[iRock]->AstroidLogic( mTimeManager->GetDeltaTimeSeconds() );
		if ( mPlayer->GetAlive() ) {
			bool tPlayerCollided = mAstroids[iRock]->CheckCollision( tPlayerX, tPlayerY, tPlayerZ, 10.0f );
			if ( tPlayerCollided ) {
				float tDX = mAstroids[iRock]->GetDX() + mPlayer->GetDX();
				float tDY = mAstroids[iRock]->GetDY() + mPlayer->GetDY();
				float tDZ = mAstroids[iRock]->GetDZ() + mPlayer->GetDZ();
				float tRatio = 0.003f;
				float tRadius = mAstroids[iRock]->GetRadius();
				mPlayer->DealDamage( (int)( abs( 0.1f * ( mPlayer->GetDX() + mPlayer->GetDY() ) ) +	
									 abs( tRadius * ( mAstroids[iRock]->GetDX() + mAstroids[iRock]->GetDY() ) ) ) );
				if ( mAstroids[iRock]->GetDX() == 0 && mAstroids[iRock]->GetDY() == 0 )
					mAstroids[iRock]->SetVelocity( tDX * tRatio, tDY * tRatio, tDZ * tRatio );
				if ( mSFX.find( "playerCollision" ) != mSFX.end() && !mAstroids[iRock]->PlayerCollisionPlaying() ) {
					mSFX["playerCollision"]->setDefaultVolume( mGameManager->GetSFXVolume() );
					mAudioManager->play2D( mSFX["playerCollision"] );
					mAstroids[iRock]->SetPlayerCollisionPlaying( true );
				}
			} else {
				mAstroids[iRock]->SetPlayerCollisionPlaying( false );
			}
		}

		bool tAstraeusCollided = mAstroids[iRock]->CheckCollision( tAstraeusX, tAstraeusY, 0.0f, 100.0f );
		if ( tAstraeusCollided ) {
			float tRadius = mAstroids[iRock]->GetRadius();
			mAstraeus->DealDamage( (int)( abs( 0.5 * tRadius * ( mAstroids[iRock]->GetDX() + mAstroids[iRock]->GetDY() ) ) ) );
			mAstroids[iRock]->SetVelocity( mAstroids[iRock]->GetDX(), mAstroids[iRock]->GetDY(), 10.0f );
			if ( mSFX.find( "astraeusCollision" ) != mSFX.end() && !mAstroids[iRock]->AstraeusCollisionPlaying() ) {
				mSFX["astraeusCollision"]->setDefaultVolume( mGameManager->GetSFXVolume() );
				mAudioManager->play2D( mSFX["astraeusCollision"] );
				mAstroids[iRock]->SetAstraeusCollisionPlaying( true );
			}
		} else {
			mAstroids[iRock]->SetAstraeusCollisionPlaying( false );
		}

		if ( mAstroids[iRock]->GetDX() != 0 || mAstroids[iRock]->GetDY() != 0 ) {
			for ( unsigned int iRock2 = 0; iRock2 < mAstroids.size(); ++iRock2 ) {
				if ( iRock2 != iRock && mAstroids[iRock]->CheckCollision( mAstroids[iRock2]->GetXPos(), mAstroids[iRock2]->GetYPos(),
																		  mAstroids[iRock2]->GetZPos(), mAstroids[iRock2]->GetRadius() * 50.0f ) ){
					float tRatio	= mAstroids[iRock]->GetRadius();
					float tDX		= mAstroids[iRock2]->GetDX() * tRatio;
					float tDY		= mAstroids[iRock2]->GetDY() * tRatio;
					float tDZ		= mAstroids[iRock2]->GetDZ() * tRatio;

					float tRatio2	= mAstroids[iRock2]->GetRadius();
					float tDX2		= mAstroids[iRock]->GetDX() * tRatio2;
					float tDY2		= mAstroids[iRock]->GetDY() * tRatio2;
					float tDZ2		= mAstroids[iRock]->GetDZ() * tRatio2;

					mAstroids[iRock]->SetVelocity( tDX, tDY, tDZ );
					mAstroids[iRock2]->SetVelocity( tDX2, tDY2, tDZ2 );
					mAstroids[iRock2]->AstroidLogic( mTimeManager->GetDeltaTimeSeconds() );
					mAstroids[iRock2]->AstroidLogic( mTimeManager->GetDeltaTimeSeconds() );
					mAstroids[iRock2]->AstroidLogic( mTimeManager->GetDeltaTimeSeconds() );
				}
			}
		}

		if( mAstroids[iRock]->GetHealth() <= 0 ) {
			if ( mAstroids[iRock]->GetOre() ) {
				Node* tOre = mSceneManager->AddNode( "ore" + mOreCount, mOreParent->GetModel()->GetModelId(), NULL );
				tOre->SetDiffuse( 0.0f, 1.0f, 0.0f, 1.0f );
				tOre->SetPosition( mAstroids[iRock]->GetXPos(), mAstroids[iRock]->GetYPos(), mAstroids[iRock]->GetZPos() );
				tOre->GetModel()->SetShaderProgram( mShaderManager->GetProgram( "oreProgram" ) );
				tOre->SetScale( 0.025f );
				mOre.push_back( tOre );
				mOreCount ++;
			}

			for ( unsigned int iRock2 = 0; iRock2 < mIncoming.size(); ++iRock2 ) {
				if ( mAstroids[iRock]->GetId() == mIncoming[iRock2]->GetId() ) {
					mIncoming.erase( mIncoming.begin() + iRock2 );
					break;
				}
			}
			mSceneManager->RemoveNode( mAstroids[iRock]->GetId() );
			mAstroids.erase( mAstroids.begin() + iRock );
		} else if ( mAstroids[iRock]->GetXPos() < 0.0f ) {
			mSceneManager->RemoveNode( mAstroids[iRock]->GetId() );
			mAstroids.erase( mAstroids.begin() + iRock );
		} else if ( mAstroids[iRock]->GetXPos() > mGridWidth * 1500.0f ) {
			mSceneManager->RemoveNode( mAstroids[iRock]->GetId() );
			mAstroids.erase( mAstroids.begin() + iRock );
		} else if ( mAstroids[iRock]->GetYPos() < 0.0f ) {
			mSceneManager->RemoveNode( mAstroids[iRock]->GetId() );
			mAstroids.erase( mAstroids.begin() + iRock );
		} else if ( mAstroids[iRock]->GetYPos() >  mGridHeight * 1500.0f ) {
			mSceneManager->RemoveNode( mAstroids[iRock]->GetId() );
			mAstroids.erase( mAstroids.begin() + iRock );
		} else if ( mAstroids[iRock]->GetZPos() > mCamera->GetCameraZ() ) {
			mSceneManager->RemoveNode( mAstroids[iRock]->GetId() );
			for ( unsigned int iRock2 = 0; iRock2 < mIncoming.size(); ++iRock2 ) {
				if ( mIncoming[iRock2]->GetId() == mAstroids[iRock]->GetId() ) {
					mIncoming.erase( mIncoming.begin() + iRock2 );
					break;
				}
			}
			mAstroids.erase( mAstroids.begin() + iRock );
		} else {
			for ( unsigned int iWeapon = 0; iWeapon < mWeapons.size(); ++iWeapon ) {
				if ( mAstroids[iRock]->CheckCollision( mWeapons[iWeapon]->GetXPos(), mWeapons[iWeapon]->GetYPos(), 
													   mWeapons[iWeapon]->GetZPos(), 5 ) ) {
					mAstroids[iRock]->ChangeHealth( -mWeapons[iWeapon]->GetDamage() );
					if ( mWeapons[iWeapon]->GetType() == Weapon::WEAPON_MISSILE ) {
						mSceneManager->RemoveNode( mWeapons[iWeapon]->GetNode()->GetId() );
						if ( mSFX.find( "missileHit" ) != mSFX.end() ) {
							mSFX["missileHit"]->setDefaultVolume( mGameManager->GetSFXVolume() );
							mAudioManager->play2D( mSFX["missileHit"] );
						}
					}
					mWeapons.erase( mWeapons.begin() + iWeapon );
					iWeapon --;
				}
			}
		}
	}

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
}

void DemoState::ProcessInput() {
	if ( !mGameOver && !mWin && !mReloading ) {
		if ( GetKeyHeld( 'w' ) ) {
			mPlayer->MoveForward( 250.0f, mTimeManager->GetDeltaTimeSeconds() );
		}
		if ( GetKeyHeld( 's' ) ) {
			mPlayer->MoveForward( -250.0f, mTimeManager->GetDeltaTimeSeconds() );
		}

		int tMouseX = GetMouseX();
		int tMouseY = GetMouseY();
		float tOffset = 0.0f;
		if ( !mDisplayManager->GetFullscreen() )
			tOffset = 40.0f;
		float tRotation = atan2( tMouseX - mDisplayManager->GetScreenWidth() / 2.0f, 
							     tMouseY + tOffset - mDisplayManager->GetScreenHeight() / 2.0f );
		tRotation = tRotation * 180.0f / 3.14159f - 90.0f;
		mPlayer->SetRotation( tRotation );

		if ( !mFired && GetLeftClick() ) {
			Weapon* tLaser	= new Weapon( 10, Weapon::WEAPON_LASER );
			float tAngle	= 0.0f;
			if ( mLeftCannon ) {
				tAngle		= 9.0f;
				mLeftCannon = false;
			} else {
				tAngle		= -9.0f;
				mLeftCannon = true;
			}
			float tX = mPlayer->GetXPos() + cos( ( tAngle + mPlayer->GetRotation() ) * 3.14159f / 180.0f ) * 8.0f;
			float tY = mPlayer->GetYPos() + sin( ( tAngle + mPlayer->GetRotation() ) * 3.14159f / 180.0f ) * 8.0f;
			float tZ = mPlayer->GetZPos();
			tLaser->SetPosition( tX, tY, tZ );
			float tDX = cos( mPlayer->GetRotation() * 3.14159f / 180.0f ) * 250.0f;
			float tDY = sin( mPlayer->GetRotation() * 3.14159f / 180.0f ) * 250.0f;
			if ( ( tDX > 0.0f && mPlayer->GetDX() > 0.0f ) || ( tDX < 0 && mPlayer->GetDX() < 0.0f ) )
				tDX += mPlayer->GetDX();
			if ( ( tDY > 0.0f && mPlayer->GetDY() > 0.0f ) || ( tDY < 0 && mPlayer->GetDY() < 0.0f ) )
				tDY += mPlayer->GetDY();
			tLaser->Fire( tDX, tDY, 0.0f );
			mWeapons.push_back( tLaser );
			if ( mSFX.find( "laserFire" ) != mSFX.end() ) {
				mSFX["laserFire"]->setDefaultVolume( mGameManager->GetSFXVolume() );
				mAudioManager->play2D( mSFX["laserFire"], false );
			}
			mFired = true;
		} else if ( !GetLeftClick() ) {
			mFired = false;
		}

		if ( !mMissileFired && GetRightClick() ) {
			if ( mPlayer->GetMissiles() > 0 ) {
				Node* tMissileNode = mSceneManager->AddNode( "missile" + mPlayer->GetMissiles(), 
									 mMissileParent->GetModel()->GetModelId(), mTextureManager->GetTexture( "missileTex" ) );
				tMissileNode->SetRotation( mPlayer->GetRotation() - 90.0f, 0.0f, 0.0f );
				tMissileNode->SetScale( 0.025f );
				tMissileNode->GetModel()->SetShaderProgram( mShaderManager->GetProgram( "demoProgram" ) );
				mPlayer->FireMissile();
				Weapon* tMissile = new Weapon( 300, Weapon::WEAPON_MISSILE, tMissileNode );
				float tX = mPlayer->GetXPos() + cos( mPlayer->GetRotation() * 3.14159f / 180.0f );
				float tY = mPlayer->GetYPos() + sin( mPlayer->GetRotation() * 3.14159f / 180.0f );
				float tZ = mPlayer->GetZPos();
				tMissile->SetPosition( tX, tY, tZ );
				float tDX = cos( mPlayer->GetRotation() * 3.14159f / 180.0f ) * 150;
				float tDY = sin( mPlayer->GetRotation() * 3.14159f / 180.0f ) * 150;
				if ( ( tDX > 0.0f && mPlayer->GetDX() > 0.0f ) || ( tDX < 0.0f && mPlayer->GetDX() < 0.0f ) )
					tDX += mPlayer->GetDX();
				if ( ( tDY > 0.0f && mPlayer->GetDY() > 0.0f ) || ( tDY < 0.0f && mPlayer->GetDY() < 0.0f ) )
					tDY += mPlayer->GetDY();
				tMissile->Fire( tDX, tDY, 0.0f );
				mWeapons.push_back( tMissile );
				mMissileFired = true;

				if ( mSFX.find( "missileFire" ) != mSFX.end() ) {
					mSFX["missileFire"]->setDefaultVolume( mGameManager->GetSFXVolume() );
					mAudioManager->play2D( mSFX["missileFire"], false );
				}
			}
		} else if ( !GetRightClick() ) {
			mMissileFired = false;
		}
	}

	if ( GetKeyPressed( VK_ESCAPE ) ) {
		mGameManager->ChangeState( "mainMenu" );
		mGameManager->DeleteState( "demo" );
		mAudioManager->stopAllSounds();
		ShowCursor( true );
	}

}

void DemoState::AddAstroid( Astroid* astroid ) {
	mAstroids.push_back( astroid );
}

void DemoState::AddBGMusic( std::string filename ) {
	mBGMusic = mAudioManager->addSoundSourceFromFile( filename.c_str() );
	mBGMusic->setDefaultVolume( mGameManager->GetMusicVolume() );
	mAudioManager->play2D( mBGMusic, true );
}

void DemoState::AddSFX( std::string key, std::string filename ) {
	irrklang::ISoundSource *tSFX = mAudioManager->addSoundSourceFromFile( filename.c_str() );
	mSFX[key]					 = tSFX;
}

void DemoState::SetReload( bool reload ) {
	mReloading = reload;
}

void DemoState::SetWin( bool win ) {
	mWin = win;
}