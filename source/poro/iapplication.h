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

#ifndef INC_IAPPLICATION_H
#define INC_IAPPLICATION_H

namespace poro {

class IGraphics;

class IApplication{
	
public:
	
	IApplication() { }
	virtual ~IApplication() { }
	
	virtual void Init() { }
	virtual void Update(int deltaTime) = 0;
	virtual void Draw(poro::IGraphics * graphics) { }
	virtual void Exit() { }
	
	// haxored for Maze of Space 
	virtual bool IsDead() const { return false; }
};

} // end o namespace poro


#endif