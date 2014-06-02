/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#ifndef _SPLASH_STATE_
#define _SPLASH_STATE_

#include "Engine\Display.h"
#include "Engine\StateManager.h"

class SplashState : public State {
	public:
		SplashState( Display *dMgr, StateManager *sMgr );	// Constructor
		~SplashState();										// Deconstructor

		void DrawFrame();									// Draw one frame to the screen
		void GameLogic();									// Execute one frame of logic
		void InitializeState();								// Initialize the state
		void LoadSplashScreens();							// Loads splash screen
		void ProcessInput();								// Process the user input
	protected:
	private:
		unsigned int	mCurrentSplash;						// The index of the surrent splash screen
		unsigned int	mSplashCount;						// Number of splash screens
		double			mSplashTime;						// Amount of time the current splash screen has been up
		float			mMaxTime;							// The maximum time a splash screen will be up
};

#endif														// _SPLASH_STATE_