/***************************************************************************
 *
 * Copyright (c) 2010 Petri Purho, Dennis Belfrage
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ***************************************************************************/

#include "keyboard.h"
#include "poro_macros.h"
#include <iostream>
#include <algorithm>


namespace poro {

void Keyboard::AddKeyboardListener( IKeyboardListener* listener )
{
	poro_assert( listener );
	poro_logger << "Added keyboard listener" << std::endl;

	mListeners.push_back(listener);
}

void Keyboard::RemoveKeyboardListener( IKeyboardListener* listener )
{
	poro_logger << "Remove keyboard listener" << std::endl;
	
	std::vector< IKeyboardListener* >::iterator i = 
		std::find( mListeners.begin(), mListeners.end(), listener );

	// poro_assert( i != mListeners.end() );

	if( i != mListeners.end() )
		mListeners.erase( i );
}

void Keyboard::FireKeyDownEvent( int button, types::charset unicode )
{
	for( std::size_t i = 0; i < mListeners.size(); ++i )
	{
		poro_assert( mListeners[ i ] );
		mListeners[ i ]->OnKeyDown( button, unicode );
	}
}

void Keyboard::FireKeyUpEvent( int button, types::charset unicode )
{
	for( std::size_t i = 0; i < mListeners.size(); ++i )
	{
		poro_assert( mListeners[ i ] );
		mListeners[ i ]->OnKeyUp( button, unicode );
	}
}


} // end of namespace poro