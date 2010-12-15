#include "joystick.h"

namespace poro {
namespace {

const int JOYSTICK_ANALOG_BUTTON_COUNT = ( Joystick::JOY_BUTTON_ANALOG_09_MOVED - Joystick::JOY_BUTTON_ANALOG_00_MOVED + 1 );

}

Joystick::Joystick( int id ) :
	mListeners(),
	mId( id ),
	mLeftStick( 0, 0 ),
	mRightStick( 0, 0 ),
	mButtonsDown( Joystick::JOY_BUTTON_COUNT ),
	mAnalogButtons( JOYSTICK_ANALOG_BUTTON_COUNT ),
	mConnected( false )
{
	// just making sure everything is set to 0
	for( std::size_t i = 0; i < mButtonsDown.size(); ++i )
		mButtonsDown[ i ] = false;

	for( std::size_t i = 0; i < mAnalogButtons.size(); ++i )
		mAnalogButtons[ i ] = 0.f;
}

Joystick::~Joystick()
{
}

void Joystick::AddListener( IJoystickListener* listener )
{
	mListeners.push_back( listener );
}

void Joystick::RemoveListener( IJoystickListener* listener )
{
	for( std::size_t i = 0; i < mListeners.size(); ++i )
	{
		if( mListeners[ i ] == listener ) {
			mListeners[ i ] = mListeners[ mListeners.size() - 1 ];
			mListeners.pop_back();
		}
	}
}

void Joystick::SetConnected( bool value ) 
{ 
	if( mConnected != value ) {
		mConnected = value; 
		
		for( std::size_t i = 0; i < mListeners.size(); ++i ) {
			if( value )
				mListeners[ i ]->OnJoystickConnected( this );
			else
				mListeners[ i ]->OnJoystickDisconnected( this );
		}
	}
}

void Joystick::SetButtonState( int button, bool is_down )
{
	poro_assert( button >= 0 );
	poro_assert( button < (int)mButtonsDown.size() );

	if( mButtonsDown[ button ] != is_down )
	{
		mButtonsDown[ button ] = is_down;
	
		for( std::size_t i = 0; i < mListeners.size(); ++i ) {
			if( is_down )
				mListeners[ i ]->OnJoystickButtonDown( this, button );
			else
				mListeners[ i ]->OnJoystickButtonUp( this, button );
		}
	}
}

void Joystick::SetLeftStick( const types::vec2& value ) 
{ 
	if( mLeftStick.x != value.x ||
		mLeftStick.y != value.y )
	{
		mLeftStick.x = value.x;
		mLeftStick.y = value.y;

		for( std::size_t i = 0; i < mListeners.size(); ++i ) {
			mListeners[ i ]->OnJoystickButtonDown( this, Joystick::JOY_BUTTON_LEFT_STICK_MOVED );
		}
	}
}

void Joystick::SetRightStick( const types::vec2& value ) 
{ 
	if( mRightStick.x != value.x ||
		mRightStick.y != value.y )
	{
		mRightStick.x = value.x;
		mRightStick.y = value.y;

		for( std::size_t i = 0; i < mListeners.size(); ++i ) {
			mListeners[ i ]->OnJoystickButtonDown( this, Joystick::JOY_BUTTON_RIGHT_STICK_MOVED );
		}
	}
}

void Joystick::SetAnalogButton( int button, float value )
{
	poro_assert( button >= 0 );
	poro_assert( button < (int)mAnalogButtons.size() );

	if( mAnalogButtons[ button ] != value )
	{
		mAnalogButtons[ button ] = value;

		for( std::size_t i = 0; i < mListeners.size(); ++i ) {
			mListeners[ i ]->OnJoystickButtonDown( this, ( Joystick::JOY_BUTTON_ANALOG_00_MOVED + button ) );
		}
	}
}



} // end o namespace poro