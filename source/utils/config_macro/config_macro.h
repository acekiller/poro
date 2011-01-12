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


///////////////////////////////////////////////////////////////////////////////
//
// config_macro.h
// ==================
//
//	This is a collection of macros that are used to create configuration classes
//	extremely easily and as little repeated lines as possible
//	You can check the test file for more examples
//
//  Here's an example how to use these macros
/*
	
	#define CONFIG_TEST(list_) \
		list_(float, player_speed,	10)                 

	DEFINE_CONFIG( ConfigPlayer, CONFIG_TEST )
	
*/
//
//
//	Here's the output of the macro example above:
/*
	class ConfigPlayer {
	public:

		float speed;

		ConfigPlayer() {
			speed = 10;
		}

		void Serialize( ceng::CXmlFileSys* filesys ) {
			XML_BindAttribute( filesys, speed );
		}
	};
*/
//	
//	As you can see the macros are there to make it faster and more efficient
//	to do configuration classes. 
//
//
// Created 23.10.2010 by Petri Purho (petri.purho@gmail.com)
//.............................................................................
//
//=============================================================================
#ifndef INC_CONFIG_MACRO_H
#define INC_CONFIG_MACRO_H

namespace ceng {

#define CONFIG_HEADER_DECLARE(type, name, value) \
    type name;

#define CONFIG_XML_BIND_DECLARE(type, name, value) \
	XML_BindAttribute( filesys, name );

#define CONFIG_INIT_MEMBERS(type, name, value) \
	name = value;


#define DEFINE_CONFIG(name, list)                 \
    class name { \
	public: \
		name() { \
			list(CONFIG_INIT_MEMBERS) \
		} \
		\
        list(CONFIG_HEADER_DECLARE)   \
		\
		void Serialize( ceng::CXmlFileSys* filesys ) \
		{ \
			list(CONFIG_XML_BIND_DECLARE) \
		} \
	};


} // end of namespace ceng

#endif
