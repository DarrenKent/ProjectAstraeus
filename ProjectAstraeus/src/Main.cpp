/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include <Windows.h>

#include "Engine\Debug.h"

#include "Game.h"
#include "StateMainMenu.h"
#include "StateSplash.h"

void InitializeStates( Game *GAME ) {
	MainMenuState *tMainMenu = new MainMenuState( GAME->GetDisplay(), GAME );
	GAME->AddState( "mainMenu", tMainMenu );

	SplashState *tSplashState = new SplashState( GAME->GetDisplay(), GAME );
	GAME->AddState( "splash", tSplashState );
	GAME->SetState( "splash" );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	SetDebugMode( 0 );

	Game *GAME = new Game();
	InitializeStates( GAME );
	GAME->Start();
}