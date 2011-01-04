#ifndef INC_IGRAPHICS_H
#define INC_IGRAPHICS_H

#include "poro_types.h"
#include "poro_macros.h"
#include "itexture.h"
//#include "igraphics_buffer.h"

#include <vector>
#include <stack>

namespace poro {

	class IGraphicsBuffer;
	
class IGraphics 
{
public:
	//-------------------------------------------------------------------------
	
	IGraphics() : mVertexMode(VERTEX_MODE_TRIANGLE_FAN) { 
		mFillColor[0] = 0.f;mFillColor[1] = 0.f; mFillColor[2] = 0.f; mFillColor[3] = 1.f; 
	}

	virtual ~IGraphics() { }
	//-------------------------------------------------------------------------

	virtual bool		Init( int width, int height, bool fullscreen, const types::string& caption ) = 0;
	virtual void		SetInternalSize( types::Float32 width, types::Float32 height ) { poro_assert( false ); /* You have to implement this */ }

	//-------------------------------------------------------------------------

	virtual void				SetFillColor( const poro::types::fcolor& c ) { mFillColor = c; }
	virtual poro::types::fcolor	GetFillColor() const { return mFillColor; }

	//-------------------------------------------------------------------------

	virtual ITexture*	CreateTexture( int width, int height );
	virtual ITexture*	CloneTexture( ITexture* other ) { poro_assert( false ); return NULL; /* you should implement this */ }
	void				SetTextureData(ITexture* texture, unsigned char* data ){ SetTextureData(texture, (void*)data ); }
	void				SetTextureData(ITexture* texture, float* data ){ SetTextureData(texture, (void*)data ); }
	virtual void		SetTextureData(ITexture* texture, void* data );
		
	virtual ITexture*	LoadTexture( const types::string& filename ) = 0;
	virtual void		ReleaseTexture( ITexture* texture )  = 0;
	
	//-------------------------------------------------------------------------

	virtual void		BeginRendering() = 0;
	virtual void		EndRendering() = 0;

	//-------------------------------------------------------------------------

	enum {
		VERTEX_MODE_TRIANGLE_FAN = 0,
		VERTEX_MODE_TRIANGLE_STRIP = 1
	};
	virtual void		PushVertexMode(int vertex_mode);
	virtual void		PopVertexMode();

	//-------------------------------------------------------------------------
	
	virtual void		DrawTexture(	ITexture* texture, 
										types::Float32 x, types::Float32 y, types::Float32 w, types::Float32 h, 
										const types::fcolor& color, types::Float32 rotation )  = 0;
	
	virtual void		DrawTexture( ITexture* texture, types::vec2* vertices, types::vec2* tex_coords, int count, const types::fcolor& color ) = 0;

	virtual void		DrawTextureWithAlpha( ITexture* texture, types::vec2* vertices, types::vec2* tex_coords, int count, const types::fcolor& color,
		ITexture* alpha_texture, types::vec2* alpha_vertices, types::vec2* alpha_tex_coords, const types::fcolor& alpha_color ) { poro_assert( false && "Needs to be implemented" ); }

	// haxored this for tesselation - Petri
	// virtual void		DrawTriangle( ITexture* texture, types::vec2 vertices[ 3 ], types::vec2 tex_coords[ 3 ], const types::fcolor& color ) { }
	// removed this as it should actually be replaced by DrawTexture() that takes an arbitary number of vertices


	//-------------------------------------------------------------------------

	virtual void		DrawLines( const std::vector< poro::types::vec2 >& vertices, const types::fcolor& color ) { }
	virtual void		DrawFill( const std::vector< poro::types::vec2 >& vertices, const types::fcolor& color ) { }

	//-------------------------------------------------------------------------

	virtual IGraphicsBuffer* CreateGraphicsBuffer(int width, int height);
	virtual void DestroyGraphicsBuffer(IGraphicsBuffer* buffer);

	//-------------------------------------------------------------------------

protected:
	poro::types::fcolor mFillColor;
	int mVertexMode;
	std::stack<int> mVertexModes;
	
};
	
///////////////////////////////////////////////////////////////////////////////

inline void	IGraphics::PushVertexMode(int vertex_mode) {
	mVertexModes.push(mVertexMode);
	mVertexMode=vertex_mode;
}
inline void	IGraphics::PopVertexMode() {
	mVertexMode = mVertexModes.top();
	mVertexModes.pop();
}
	
inline ITexture* IGraphics::CreateTexture(int width, int height) { 
	// If this fails, it means you have not implemented create texture
	poro_assert( false );
	return NULL;
}

inline void IGraphics::SetTextureData(ITexture* texture, void* data ) {
	// If this fails, it means you have not implemented set texture data
	poro_assert( false );
}

inline IGraphicsBuffer* IGraphics::CreateGraphicsBuffer(int width, int height) { 
	// If this fails, it means you should implement grafics buffer on your end of things
	poro_assert( false );
	return NULL;
}
inline void IGraphics::DestroyGraphicsBuffer(IGraphicsBuffer* buffer){
	// If this fails, it means you should implement grafics buffer on your end of things
	poro_assert( false );
}
	
///////////////////////////////////////////////////////////////////////////////

} // end o namespace poro

#endif