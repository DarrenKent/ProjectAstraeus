/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _MAIN_MENU_STATE_
#define _MAIN_MENU_STATE_

#include "Engine\Display.h"
#include "Engine\OrbitalCamera.h"
#include "Engine\SceneManager.h"
#include "Engine\ShaderManager.h"
#include "Engine\StateManager.h"
#include "Engine\Texture.h"
#include "Engine\TimeManager.h"
#include "IK\irrKlang.h"

#include "Button.h"
#include "Game.h"
#include "StarField.h"

class MainMenuState : public State {
	public:
		MainMenuState( Display *dMgr, Game *gMgr );			// Constructor
		~MainMenuState();									// Deconstructor

		void		DrawFrame();							// Draw one frame to the screen
		void		DrawGUI();								// Draw user unterface
		void		GameLogic();							// Execute One Frame of Logic
		void		InitializeState();						// Loads the state
		void		ProcessInput();							// Process the user input
		void		SetSettingsActive( bool settings );		// Opens the settings tab
		void		StopSounds();							// Stop the audio manager

		Display*	GetDisplay();							// Returns the main menu state's display manager
		Game*		GetGameMgr();							// Returns the state's game manager
	protected:
	private:
		bool					mFullscreenFlag;			// Fullscreen button click
		bool					mMusicSliderFlag;			// Music slider button click
		bool					mSettingsActive;			// Settings active
		bool					mSFXSliderFlag;				// SFX slider button click
		Button					*mDemo, *mSettings, *mQuit,
								*mBack;						// Buttons
		float					mMouseXClick;				// Last click x position
		Game					*mGameManager;				// Pointer to primary state manager
		irrklang::ISoundEngine	*mAudioManager;				// Audio manager
		irrklang::ISoundSource	*mBackgroundMusic;			// BG music
		OrbitalCamera			*mCamera;					// Scene camera
		StarField				*mStarField;				// Star field
};

#endif														// _MAIN_MENU_STATE_