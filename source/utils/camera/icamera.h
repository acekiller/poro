/***************************************************************************
 *
 * Copyright (c) 2003 - 2011 Petri Purho
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


#ifndef INC_CENG_ICAMERA_H
#define INC_CENG_ICAMERA_H

#include "../rect/cpoint.h"
#include "../rect/crect.h"
#include "../math/cvector2.h"

namespace ceng {

///////////////////////////////////////////////////////////////////////////////

template< typename TVector2 = math::CVector2< float > >
class ICamera
{
public:
	typedef TVector2 Vec2;

	ICamera() { }
	virtual ~ICamera() { }
	
	virtual bool IsNull() const = 0;

	template< class T > 
	const T& Transform( const T& what ) 
	{
		static Vec2 temp;
		temp.x = what.x;
		temp.y = what.y;
		
		temp = Transform( temp );

		static T result;
		result.x = temp.x;
		result.y = temp.y;

		return result;
	}


	virtual Vec2 Transform( const Vec2& point ) = 0;

	template< class T > 
	const T& ConvertMousePosition( const T& p ) 
	{
		static CPoint< int > temp;
		temp = ConvertMousePosition( CPoint< int >( (int)p.x, (int)p.y ) );
		
		static T result;
		result.x = CastToType( result.x, temp.x );
		result.y = CastToType( result.y, temp.y );

		return result;
	}

	virtual CPoint< int > ConvertMousePosition( const CPoint< int >& p )
	{
		Vec2 result = Transform( Vec2( (float)p.x, (float)p.y ) );
		return CPoint< int >( (int)result.x, (int)result.y );
	}

	virtual void Update( unsigned int step ) { }

	static ICamera* CreateNull();

	template< class A, class B > 
	A CastToType( A type_a, B cast_this ) 
	{
		return (A)cast_this;
	}

};

///////////////////////////////////////////////////////////////////////////////
} // end of namespace ceng

#endif
