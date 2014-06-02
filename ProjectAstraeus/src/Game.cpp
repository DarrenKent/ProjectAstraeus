/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include "Game.h"
#include "Parsers.h"

Game::Game() : StateManager() {
	mFullscreen		= 0;
	mMusicVolume	= 1.0f;
	mScreenWidth	= 1024.0f;
	mScreenHeight	= 768.0f;
	mSFXVolume		= 1.0f;
	ParseSettings( this );
	Initialize( L"ProjectAstraeus", (int)mScreenWidth, (int)mScreenHeight, mFullscreen );
}

float Game::GetMusicVolume() {
	return mMusicVolume;
}

float Game::GetScreenHeight() {
	return mScreenHeight;
}

float Game::GetScreenWidth() {
	return mScreenWidth;
}

float Game::GetSFXVolume() {
	return mSFXVolume;
}

int Game::GetFullscreen() {
	return mFullscreen;
}

void Game::SetFullscreen( int fullscreen ) {
	mFullscreen = fullscreen;
}

void Game::SetMusicVolume( float volume ) {
	mMusicVolume = volume;
}

void Game::SetScreenHeight( float height ) {
	mScreenHeight = height;
}

void Game::SetScreenWidth( float width ) {
	mScreenWidth = width;
}

void Game::SetSFXVolume( float volume ) {
	mSFXVolume = volume;
}