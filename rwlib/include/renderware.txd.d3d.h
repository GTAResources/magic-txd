// Public Texture Dictionary Direct3D 9 RenderWare extensions.
// Use those if you need internal access to D3D native textures.
// Be cautious, as internals do rapidly change between RenderWare versions.

namespace d3dpublic
{

inline uint32 GetAPIRevision( void )
{
    // Second revision API.
    // Other than in the first revision, we have split Direct3D 8 and 9.
    return 2;
}

struct d3dNativeTextureInterface abstract
{
    // Returns the D3DFORMAT field of that native texture.
    // Since every Direct3D 9 native texture must have a D3DFORMAT field, this will
    // always return true.
    virtual bool GetD3DFormat( DWORD& d3dFormat ) const = 0;
};

/*
    This class manages conversion between extension D3DFORMAT native textures to original RW types.
    It is required so that specific D3DFORMAT textures can be integrated into this RenderWare framework.

    To use this interface you have to register it into the engine. The engine will then map D3DFORMAT to this interface.
    There can be only one handler for one D3DFORMAT type.
*/
struct nativeTextureFormatHandler abstract
{
	virtual unsigned int GetD3DFormat(void) const = 0;

    // Returns a zero-terminated c-string that is used when the format is described by RenderWare.
    virtual const char* GetFormatName( void ) const = 0;

    // Should return the size that a texture of width * height is supposed to have.
    // The returned value is used to allocate the storage for the pixels you get in ConvertFromRW.
    virtual size_t GetFormatTextureDataSize( unsigned int width, unsigned int height ) const = 0;

    // The raster format that this native texture has mapped, as original RW type has to stay the same.
    virtual void GetTextureRWFormat( eRasterFormat& rasterFormatOut, unsigned int& depthOut, eColorOrdering& colorOrderOut ) const = 0;

    // Converts the D3DFORMAT anonymous data to RW original types and returns it.
    virtual void ConvertToRW(
        const void *texData, unsigned int texMipWidth, unsigned int texMipHeight, size_t texDataSize,
        void *texOut    // preallocated memory.
    ) const = 0;

    // Converts original RW types into the D3DFORMAT plugin format.
    virtual void ConvertFromRW(
        unsigned int texMipWidth, unsigned int texMipHeight,
        const void *texelSource, eRasterFormat rasterFormat, unsigned int depth, eColorOrdering colorOrder, ePaletteType paletteType, const void *paletteData, unsigned int paletteSize,
        void *texOut    // preallocated memory.
    ) const = 0;
};

struct d3dNativeTextureDriverInterface abstract
{
	// Registration routines for plugin D3DFORMAT handling.
	virtual bool RegisterFormatHandler(DWORD format, nativeTextureFormatHandler *handler) = 0;
	virtual bool UnregisterFormatHandler(DWORD format) = 0;
};

};