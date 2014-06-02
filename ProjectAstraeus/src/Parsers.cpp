/*
*
*	Project Astraeus
*	By: Darren Kent
*
*/

#include <fstream>
#include <iostream>
#include <sstream>

#include "Engine\Debug.h"

#include "Astroid.h"
#include "Parsers.h"

void CreateAstroidField( DemoState *state, int x, int y, int z, int width, int height,
						 int maxSize, int minSize, int count,
						 std::vector< std::string > files, std::vector< std::string > textures ) {
	DebugMessage( "Creating Astroid Field...", 3 );
	std::vector< Node* >	tAstroids;
	std::vector< GLuint >	tModelIds;
	std::vector< Texture* > tTextureIds;

	int tOreCount = 0;

	for ( unsigned int iTex = 0; iTex < textures.size(); ++iTex ) {
		Texture *tTex = state->GetTextureManager()->LoadTexture( "astroidTex" + std::to_string( (long double)iTex ), textures[iTex], false );
		tTextureIds.push_back( tTex );
	}

	for ( unsigned int iModel = 0; iModel < files.size(); ++iModel ) {
		Model* tModel = state->GetSceneManager()->AddNode( "astroid" + std::to_string( (long double)iModel ), files[iModel], NULL )->GetModel();
		state->GetSceneManager()->GetNode( "astroid" + std::to_string( (long double)iModel ) )->SetVisible( false );
		tModelIds.push_back( tModel->GetModelId() );
	}

	for ( int iRock = 0; iRock < count; ++iRock ) {
		// Place Astroid
		float tSize		= 1.0f;
		int	  tX		= 0;
		int   tY		= 0;
		int   tZ		= 0;
		while ( true ) {
			tSize	= ( rand() % ( maxSize - minSize ) + minSize ) / 100.0f;
			tX		= rand() % ( width * 2 ) + x - width;
			tY		= rand() % ( height * 2 ) + y - height;
			tZ		= z;

			bool tCollided = false;
			for ( unsigned int iCollision = 0; iCollision < tAstroids.size(); ++iCollision ) {
				float tColRadius = tAstroids[iCollision]->GetScale() * 100.0f;
				float tColX		 = tAstroids[iCollision]->GetXPos();
				float tColY		 = tAstroids[iCollision]->GetYPos();
				float tColDist	 = sqrt( std::pow( ( tColX - tX ), 2 ) + 
										 std::pow( ( tColY - tY ), 2 ) );
				if ( tColDist < tColRadius + tSize * 100.0f + 10.0f ) {
					tCollided = true;
					break;
				}
			}
			if ( !tCollided )
				break;
		}

		std::string tId = "astroid" + std::to_string( (long double)( iRock + count ) );
		int tModel		= rand() % tModelIds.size();
		int tTexNum		= rand() % tTextureIds.size();
		Node* tAstroid	= state->GetSceneManager()->AddNode( tId, tModelIds[tModel], tTextureIds[tTexNum] );
		tAstroid->SetPosition( (float)tX, (float)tY, (float)tZ );
		tAstroid->SetScale( tSize );
		tAstroid->GetModel()->SetShaderProgram( state->GetShaderManager()->GetProgram( "demoProgram" ) );
		tAstroids.push_back( tAstroid );

		Astroid* tAstroidInstance = new Astroid( tAstroid );
		
		// Set Ore Status		
		if ( tOreCount < 100 ) {
			tAstroidInstance->SetOre( true );
			tOreCount ++;
		} else {
			int tOreState = ( rand() % 100 );
			if ( tOreState < 10 ) {
				tAstroidInstance->SetOre( true );
				tOreCount ++;
			}
		}

		// Set Health
		tAstroidInstance->SetHealth( (int)( tSize * 1000 ) );

		// Set Spin
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
		float tPitchRot	= ( rand() % 100 ) / 10.0f * tPitchDir;
		float tRollRot	= ( rand() % 100 ) / 10.0f * tRollDir;

		tAstroidInstance->SetRotationVeloctiy( tYawRot, tPitchRot, tRollRot );
		state->AddAstroid( tAstroidInstance );
	}
}

void ParseSettings( Game *game ) {
	DebugMessage( "Loading Settings...", 3 );

	std::string		tLine;
	std::ifstream	tFile( "data/cfg/settings.cfg" );

	if ( tFile.is_open() ) {
		while ( tFile.good() ) {
			std::string		   tToken;
			std::istringstream tStringStream( tLine );

			getline( tFile, tLine );
			getline( tStringStream, tToken, ' ' );
			if ( tToken == "screenwidth" ) {
				getline( tStringStream, tToken, ' ' );
				float tScreenWidth = (float)atof( tToken.c_str() );
				game->SetScreenWidth( tScreenWidth );
			} else if ( tToken == "screenheight" ) {
				getline( tStringStream, tToken, ' ' );
				float tScreenHeight = (float)atof( tToken.c_str() );
				game->SetScreenHeight( tScreenHeight );
			} else if ( tToken == "fullscreen" ) {
				getline( tStringStream, tToken, ' ' );
				int tFullscreen = atoi( tToken.c_str() );
				game->SetFullscreen( tFullscreen );
			} else if ( tToken == "musicvolume" ) {
				getline( tStringStream, tToken, ' ' );
				float tMusicVolume = (float)atof( tToken.c_str() );
				game->SetMusicVolume( tMusicVolume );
			} else if ( tToken == "sfxvolume" ) {
				getline( tStringStream, tToken, ' ' );
				float tSFXVolume = (float)atof( tToken.c_str() );
				game->SetSFXVolume( tSFXVolume );
			}
		}
		tFile.close();
	} else {
		FatalError( "Unable to open level file: data/cfg/settings.cfg" );
	}
}

void ParseLevel( DemoState *state, irrklang::ISoundEngine *aMgr, std::string filename ) {
	DebugMessage( "Loading Level...", 3 );

	std::string		tLine;
	std::ifstream	tFile( filename );

	if ( tFile.is_open() ) {
		while ( tFile.good() ) {
			std::string		   tToken;
			std::istringstream tStringStream( tLine );

			getline( tFile, tLine );
			getline( tStringStream, tToken, '<' );
			getline( tStringStream, tToken, '>' );
			if ( tToken == "model" ) {
				float		tScale		= 1.0f;
				float		tX			= 0.0f;
				float		tY			= 0.0f;
				float		tZ			= 0.0f;
				float		tYaw		= 0.0f;
				float		tPitch		= 0.0f;
				float		tRoll		= 0.0f;
				std::string tFilename	= "";
				std::string tTexture	= "";
				std::string tId			= "";

				while ( tToken != "/model" ) {
					std::istringstream tStringStream( tLine );
					getline( tStringStream, tToken, '<' );
					getline( tStringStream, tToken, '>' );

					if ( tToken == "scale" ) {
						getline( tStringStream, tToken, '<' );
						tScale = (float)atof( tToken.c_str() );
					} else if ( tToken == "xpos" ) {
						getline( tStringStream, tToken, '<' );
						tX = (float)atof( tToken.c_str() );
					} else if ( tToken == "ypos" ) {
						getline( tStringStream, tToken, '<' );
						tY = (float)atof( tToken.c_str() );
					} else if ( tToken == "zpos" ) {
						getline( tStringStream, tToken, '<' );
						tZ = (float)atof( tToken.c_str() );
					} else if ( tToken == "yaw" ) {
						getline( tStringStream, tToken, '<' );
						tYaw = (float)atof( tToken.c_str() );
					} else if ( tToken == "pitch" ) {
						getline( tStringStream, tToken, '<' );
						tPitch = (float)atof( tToken.c_str() );
					} else if ( tToken == "roll" ) {
						getline( tStringStream, tToken, '<' );
						tRoll = (float)atof( tToken.c_str() );
					} else if ( tToken == "filename" ) {
						getline( tStringStream, tToken, '<' );
						tFilename = tToken;
					} else if ( tToken == "id" ) {
						getline( tStringStream, tToken, '<' );
						tId = tToken;
					} else if ( tToken == "texture" ) {
						getline( tStringStream, tToken, '<' );
						tTexture = tToken;
					}

					getline( tFile, tLine );
				}
				Texture *tTex	= state->GetTextureManager()->LoadTexture( tId + "Tex", tTexture, false );
				Node *tNode		= state->GetSceneManager()->AddNode( tId, tFilename, tTex );
				tNode->SetScale( tScale );
				tNode->SetPosition( tX, tY, tZ );
				tNode->SetRotation( tYaw, tPitch, tRoll );
			} else if ( tToken == "music" ) {
				getline( tStringStream, tToken, '<' );
				state->AddBGMusic( tToken );
			} else if ( tToken == "sfx" ) {
				std::string tKey = "";
				std::string tFilename = "";

				while ( tToken != "/sfx" ) {
					std::istringstream tStringStream( tLine );
					getline( tStringStream, tToken, '<' );
					getline( tStringStream, tToken, '>' );

					if ( tToken == "key" ) {
						getline( tStringStream, tToken, '<' );
						tKey = tToken;
					} else if ( tToken == "file" ) {
						getline( tStringStream, tToken, '<' );
						tFilename = tToken;
					}
					getline( tFile, tLine );
				}
				state->AddSFX( tKey, tFilename );
			} else if ( tToken == "astroidfield" ) {
				int tMinSize			= 1;
				int tMaxSize			= 1;
				int tX					= 0;
				int tY					= 0;
				int tZ					= 0;
				int tWidth				= 0;
				int tHeight				= 0;
				int tCount				= 0;
				std::vector< std::string > tFiles;
				std::vector< std::string > tTextures;

				while ( tToken != "/astroidfield" ) {
					std::istringstream tStringStream( tLine );
					getline( tStringStream, tToken, '<' );
					getline( tStringStream, tToken, '>' );

					if ( tToken == "minsize" ) {
						getline( tStringStream, tToken, '<' );
						tMinSize = atoi( tToken.c_str() );
					} else if ( tToken == "maxsize" ) {
						getline( tStringStream, tToken, '<' );
						tMaxSize = atoi( tToken.c_str() );
					} else if ( tToken == "xpos" ) {
						getline( tStringStream, tToken, '<' );
						tX = atoi( tToken.c_str() );
					} else if ( tToken == "ypos" ) {
						getline( tStringStream, tToken, '<' );
						tY = atoi( tToken.c_str() );
					} else if ( tToken == "zpos" ) {
						getline( tStringStream, tToken, '<' );
						tZ = atoi( tToken.c_str() );
					} else if ( tToken == "width" ) {
						getline( tStringStream, tToken, '<' );
						tWidth = atoi( tToken.c_str() );
					} else if ( tToken == "height" ) {
						getline( tStringStream, tToken, '<' );
						tHeight = atoi( tToken.c_str() );
					} else if ( tToken == "count" ) {
						getline( tStringStream, tToken, '<' );
						tCount = atoi( tToken.c_str() );
					} else if ( tToken == "filename" ) {
						getline( tStringStream, tToken, '<' );
						tFiles.push_back( tToken );
					} else if ( tToken == "texture" ) {
						getline( tStringStream, tToken, '<' );
						tTextures.push_back( tToken );
					}

					getline( tFile, tLine );
				}
				CreateAstroidField( state, tX, tY, tZ, tWidth, tHeight, tMaxSize, tMinSize, tCount, tFiles, tTextures );
			}
		}
		tFile.close();
	} else {
		FatalError( "Unable to open level file: " + filename );
	}
}

void SaveSettings( Game* game ) {
	std::ofstream tFile( "data/cfg/settings.cfg" );
	if( tFile.is_open() ) {
		tFile << "screenwidth " << game->GetScreenWidth() << std::endl;
		tFile << "screenheight " << game->GetScreenHeight() << std::endl;
		tFile << "fullscreen " << game->GetFullscreen() << std::endl;
		tFile << "musicvolume " << game->GetMusicVolume() << std::endl;
		tFile << "sfxvolume " << game->GetSFXVolume() << std::endl;

		tFile.close();
	} else {
		FatalError( "Unable to open level file: data/cfg/settings.cfg" );
	}
}