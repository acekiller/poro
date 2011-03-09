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

#ifndef INC_GRAPHICS_WIN_H
#define INC_GRAPHICS_WIN_H

#include "../poro_types.h"
#include "../igraphics.h"


namespace poro {

class ITexture;
class IGraphicsBuffer;

class GraphicsWin : public IGraphics
{
public:

	virtual bool		Init( int width, int height, bool fullscreen, const types::string& caption );
	virtual void		SetInternalSize( types::Float32 width, types::Float32 height );
    virtual void        SetWindowSize(int width, int height);
    virtual void        SetFullscreen(bool fullscreen);
    
	virtual ITexture*	CreateTexture( int width, int height );
	virtual ITexture*	CloneTexture( ITexture* other );
	virtual void		SetTextureData(ITexture* texture, void* data );
	virtual ITexture*	LoadTexture( const types::string& filename );
	virtual void		ReleaseTexture( ITexture* texture );

	virtual void		DrawTexture( ITexture* texture, 
									types::Float32 x, 
									types::Float32 y, 
									types::Float32 w, 
									types::Float32 h, 
									const types::fcolor& color, 
									types::Float32 rotation = 0.0f );

	virtual void		DrawTexture( ITexture* texture, 
										types::vec2* vertices, 
										types::vec2* tex_coords, 
										int count, 
										const types::fcolor& color );
	
	virtual void		DrawTextureWithAlpha( 
		ITexture* texture, 
		types::vec2* vertices, 
		types::vec2* tex_coords, 
		int count, 
		const types::fcolor& color,
		ITexture* alpha_texture, 
		types::vec2* alpha_vertices, 
		types::vec2* alpha_tex_coords, 
		const types::fcolor& alpha_color );


	virtual void		BeginRendering();
	virtual void		EndRendering();
	
	virtual void		DrawLines( const std::vector< poro::types::vec2 >& vertices, const types::fcolor& color );
	virtual void		DrawFill( const std::vector< poro::types::vec2 >& vertices, const types::fcolor& color );
	
	virtual IGraphicsBuffer* CreateGraphicsBuffer(int width, int height);
	virtual void DestroyGraphicsBuffer(IGraphicsBuffer* buffer);
	
	types::vec2     ConvertToInternalPos( int x, int y );

	//types::vec2 GetViewportOffset() const { return mViewportOffset; }
	//types::vec2 GetViewportSize() const { return mViewportSize; }
	
private:
    void    ResetWindow();
    
    bool    mClearBackground;
    bool    mFullscreen;
    int     mWindowWidth;
    int     mWindowHeight;
    types::vec2 mViewportOffset;
    types::vec2 mViewportSize;
   
};

} // end o namespace poro
#endif