/*
 *  platform_mac.cpp
 *  
 *	
 *  Created by Dennis Belfrage on 7.6.10.
 *  Copyright 2010 Kloonigames. All rights reserved.
 *
 */

#include "platform_mac.h"

#include "../libraries.h"
#include "../poro_main.h"

#include "../windows/graphics_win.h"
#include "../windows/soundplayer_win.h"
#include "../windows/texture_win.h"
//#include "../windows/graphics_buffer_win.h"


namespace poro {
	
PlatformMac::PlatformMac() :
	mGraphics( NULL ),
	mFrameCount( 0 ),
	mFrameRateCount( 0 ),
	mFrameRate( 0 ),
	mFrameTimePrevious( 0 ),
	mTargetFrameTime( 0 ),
	mFrameRateUpdateTime( 0 ),
	mWidth( 0 ),
	mHeight( 0 ),
	mMouse( NULL ),
	mKeyboard( NULL ),
	mSoundPlayer( NULL ),
	mRunning( 0 ),
	mMousePos()
{
	
}	
	
	
void PlatformMac::Init(IApplication * application, int w, int h, bool fullscreen, std::string title) {
	
	mRunning = true;
	mFrameCount = 1;
	mFrameRate = -1.0f;
	mWidth = w;
	mHeight = h;
	mApplication = application;
	
	mGraphics = new GraphicsWin;
	mGraphics->Init(w, h, fullscreen, title.c_str());
	
	mSoundPlayer = new SoundPlayerWin;
	mSoundPlayer->Init();
			
	mMouse = new Mouse;
	mKeyboard = new Keyboard;

	SetFrameRate(60);
		
	SDL_EnableUNICODE(1);
	
	// Commented out here and moved to StartMainLoop
	//		bad policy to have init's in StartMainLoop(), but this the way 
	//		to make it deterministic with windows and iPhone platforms.
	//
	// if( mApplication )
	// 	mApplication->Init();

	SetWorkingDir();
}


void PlatformMac::StartMainLoop() {

	// added by Petri on 01.11.2010
	mRunning = true;

	
	if( mApplication )
		mApplication->Init();
	
	while( mRunning )
	{
		SingleLoop();
		
		if( mApplication && mApplication->IsDead() == true )
			mRunning = false;
		
		//Target framerate
		float elapsedTime = (float)(GetUpTime() - mFrameTimePrevious);
		if(elapsedTime < mTargetFrameTime){
			Sleep(mTargetFrameTime - elapsedTime);
		}
		mFrameTimePrevious = GetUpTime();
		
		//Frame rate
		if (mFrameTimePrevious > mFrameRateUpdateTime+1000) {
			mFrameRate=mFrameRateCount;
			mFrameRateCount=0;
			mFrameRateUpdateTime = mFrameTimePrevious;
		}
	}
	
	if( mApplication )
		mApplication->Exit();
	
}

void PlatformMac::Destroy() {
	delete mGraphics;
	delete mSoundPlayer;
	delete mMouse;
}

void PlatformMac::SetFrameRate( int targetRate) {
	mTargetFrameTime = 1000.f/(float)targetRate;
}

int	PlatformMac::GetFrameNum() {
	return mFrameCount;
}

int	PlatformMac::GetUpTime() {
	return (int)SDL_GetTicks();
}

void PlatformMac::Sleep(int millis){
	SDL_Delay( (Uint32)millis );
}

void PlatformMac::SingleLoop() {
	
	HandleEvents();
	
	poro::IPlatform::Instance()->GetApplication()->Update( 16 );
	
	mGraphics->BeginRendering();
	poro::IPlatform::Instance()->GetApplication()->Draw(mGraphics);
	mGraphics->EndRendering();
	
	mFrameCount++;
	mFrameRateCount++;
	
}

void PlatformMac::HandleEvents() {

	//----------------------------------------------------------------------------

	SDL_Event event;
	while( SDL_PollEvent( &event ) ) 
	{
		switch( event.type ) 
		{
			case SDL_KEYDOWN:
			{
				switch( event.key.keysym.sym )
				{
					case SDLK_ESCAPE:
						mRunning = false;
						break;
					default:
						break;
				}
				
				if( mKeyboard )
					mKeyboard->FireKeyDownEvent(static_cast< int >( event.key.keysym.sym ),
												static_cast< types::charset >( event.key.keysym.unicode ) );
			}
			break;
				
			case SDL_KEYUP:
			{
				if( mKeyboard )
					mKeyboard->FireKeyUpEvent(static_cast< int >( event.key.keysym.sym ),
											  static_cast< types::charset >( event.key.keysym.unicode ) );
			}
			break;
					
			case SDL_QUIT:
				mRunning = 0;
			break;

			case SDL_MOUSEBUTTONDOWN:
				poro_assert( mMouse );
				if( event.button.button == SDL_BUTTON_LEFT )
				{
					mMouse->FireMouseDownEvent( mMousePos, Mouse::MOUSE_BUTTON_LEFT );
				}
				else if( event.button.button == SDL_BUTTON_RIGHT )
				{
					mMouse->FireMouseDownEvent( mMousePos, Mouse::MOUSE_BUTTON_RIGHT );
				}
				else if( event.button.button == SDL_BUTTON_MIDDLE )
				{
					mMouse->FireMouseDownEvent( mMousePos, Mouse::MOUSE_BUTTON_MIDDLE );
				}
				else if( event.button.button == SDL_BUTTON_WHEELUP )
				{
					mMouse->FireMouseDownEvent( mMousePos, Mouse::MOUSE_BUTTON_WHEEL_UP );
				}
				else if( event.button.button == SDL_BUTTON_WHEELDOWN )
				{
					mMouse->FireMouseDownEvent( mMousePos, Mouse::MOUSE_BUTTON_WHEEL_DOWN );
				}
				break;

			case SDL_MOUSEBUTTONUP:
				poro_assert( mMouse );
				if( event.button.button == SDL_BUTTON_LEFT )
				{
					mMouse->FireMouseUpEvent( mMousePos, Mouse::MOUSE_BUTTON_LEFT );
				}
				else if( event.button.button == SDL_BUTTON_RIGHT )
				{
					mMouse->FireMouseUpEvent( mMousePos, Mouse::MOUSE_BUTTON_RIGHT );
				}
				else if( event.button.button == SDL_BUTTON_MIDDLE )
				{
					mMouse->FireMouseUpEvent( mMousePos, Mouse::MOUSE_BUTTON_MIDDLE );
				}
				break;

			case SDL_MOUSEMOTION:
				poro_assert( mMouse );
				{
					mMousePos = ConvertMouseToInternalSize( event.motion.x, event.motion.y );
					mMouse->FireMouseMoveEvent( mMousePos );
				}
				break;
		}
	}
}

types::vec2	PlatformMac::ConvertMouseToInternalSize( int x, int y ) {
	types::vec2 result( (types::Float32)x, (types::Float32)y );

	result.x *= GetInternalWidth() / (types::Float32)GetWidth();
	result.y *= GetInternalHeight() / (types::Float32)GetHeight();

	return result;
}

void PlatformMac::SetWorkingDir(poro::types::string dir){
	//Set the working directory inside the app pacakge for MAC and iPHOHE.
	int maxpath = 1024;
	char path[maxpath];
	
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, maxpath))
	{
		std::cerr << "Failed to change the working directory!" << std::endl;
		assert(false);
	}
	CFRelease(resourcesURL);
	
	chdir(path);
	poro_logger << "Changing working dir to " << path << std::endl;
}
	
	IGraphics* PlatformMac::GetGraphics() {
		return mGraphics;
	}
	
} // end o namespace poro