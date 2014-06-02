/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _DEMO_STATE_
#define _DEMO_STATE_

#include "Engine\Display.h"
#include "Engine\OrbitalCamera.h"
#include "Engine\SceneManager.h"
#include "Engine\ShaderManager.h"
#include "Engine\StateManager.h"
#include "Engine\Texture.h"
#include "Engine\TimeManager.h"
#include "IK\irrKlang.h"

#include "Astraeus.h"
#include "Astroid.h"
#include "Button.h"
#include "Game.h"
#include "Player.h"
#include "StarField.h"
#include "Weapon.h"

class DemoState : public State {
	public:
		DemoState( Display *dMgr, Game *gMgr );				// Constructor
		~DemoState();										// Deconstructor

		void DrawFrame();									// Draw One Frame
		void DrawGUI();										// Draw User Interface
		void DrawStarGrid();								// Draw Stars based on player location
		void InitializeState();								// Loads the State
		void GameLogic();									// Execute One Frame of Logic
		void ProcessInput();								// Process the user input

		void AddAstroid( Astroid* astroid );				// Adds an Astroid to the Level
		void AddBGMusic( std::string filename );			// Adds Music
		void AddSFX( std::string key, std::string filename );// Adds a SFX to be used

		void SetReload( bool reload );						// Sets Reloading Flag
		void SetWin( bool win );							// Sets Win Flag
	protected:
	private:
		Astraeus				*mAstraeus;					// Astraeus
		bool					mAstroidWave;				// Astroid Wave Flag
		bool					mDesertionZone;				// Entered into Desertion zone
		bool					mFired;						// Cannon Fired
		bool					mFireTick;					// Cannon Reload Tick
		bool					mFPS;						// Display the FPS
		bool					mGameOver;					// Game Over Flag
		bool					mHit;						// Hit by Astraeus
		bool					mLeftCannon;				// Cannon Firing
		bool					mMissileFired;				// Missle Fired flag
		bool					mMissileReloadTick;			// Reloaded a missile
		bool					mReloading;					// Reload Flag
		bool					mWin;						// Win Flag
		Button					*mJumpButton;				// Jump Button
		Button					*mReloadButton;				// Reload Button
		float					mDesertionTime;				// Time in Desertion Zone
		float					mFTLCharge;					// FTL 
		float					mReloadTime;				// Time Reloading
		float					mXBoundary, mYBoundary;		// Player Boundries
		Game					*mGameManager;				// Game Manager
		int						mAstraeusOreCount;			// Current Astraeus Ore Count
		int						mAstroidCount;				// Astroid Incrementer
		int						mGoalOreCount;				// Ore Count Goal
		int						mGridHeight, mGridWidth;	// Map Height and Width
		int						mMaxPlayerOre;				// Maximum Ore the Player can Carry
		int						mOreCount;					// Number of Ore created
		int						mPlayerOreCount;			// Current Player Ore Count
		irrklang::ISoundEngine	*mAudioManager;				// Audio Manager
		irrklang::ISoundSource*	mBGMusic;					// Background Music
		std::map< std::string, irrklang::ISoundSource* > mSFX;// Sound Effects List
		Node*					mAstraeusHealth;			// Astraeus Health Model;
		Node*					mHealthModel;				// Health Model
		Node*					mMissileParent;				// Missile Parent Node
		Node*					mOreParent;					// Parent node to instance ore off of
		OrbitalCamera			*mCamera;					// Scene Camera
		Player					*mPlayer;					// Player
		std::vector< Astroid* > mAstroids;					// Astroids
		std::vector< Astroid* > mIncoming;					// Incoming Astroids
		std::vector< Node* >	mOre;						// List of Ore
		std::vector< StarField* >mStarFields;				// StarFields
		std::vector< Weapon* >	mWeapons;					// Player Weapons
};	

#endif // _DEMO_STATE_