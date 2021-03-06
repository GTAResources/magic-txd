#include "StdInc.h"

#include <sys/stat.h>

#include "rwconf.hxx"

namespace rw
{

#pragma warning(push)
#pragma warning(disable:4996)

// The default file interface.
struct ANSIFileInterface : public FileInterface
{
    filePtr_t   OpenStream( const char *streamPath, const char *streamMode ) override
    {
        return (filePtr_t)fopen( streamPath, streamMode );
    }

    void    CloseStream( filePtr_t ptr ) override
    {
        fclose( (FILE*)ptr );
    }

    filePtr_t   OpenStreamW( const wchar_t *streamPath, const wchar_t *streamMode ) override
    {
        return (filePtr_t)_wfopen( streamPath, streamMode );
    }

    size_t  ReadStream( filePtr_t ptr, void *outBuf, size_t readCount ) override
    {
        return fread( outBuf, 1, readCount, (FILE*)ptr );
    }

    size_t  WriteStream( filePtr_t ptr, const void *inBuf, size_t writeCount ) override
    {
        return fwrite( inBuf, 1, writeCount, (FILE*)ptr );
    }

    bool    SeekStream( filePtr_t ptr, long streamOffset, int type ) override
    {
        return ( fseek( (FILE*)ptr, streamOffset, type ) == 0 );
    }

    long    TellStream( filePtr_t ptr ) override
    {
        return ftell( (FILE*)ptr );
    }

    bool    IsEOFStream( filePtr_t ptr ) override
    {
        return ( feof( (FILE*)ptr ) != 0 );
    }

    long    SizeStream( filePtr_t ptr ) override
    {
        struct stat stats;

        int result = fstat( fileno( (FILE*)ptr ), &stats );

        if ( result != 0 )
            return -1;

        return stats.st_size;
    }

    void    FlushStream( filePtr_t ptr ) override
    {
        fflush( (FILE*)ptr );
    }
};
static ANSIFileInterface _defaultFileInterface;

#pragma warning(pop)

// Use this function to change the location of library file activity.
void rwConfigBlock::SetFileInterface( FileInterface *intf )
{
    scoped_rwlock_writer <rwlock> lock( GetConfigLock() );

    this->customFileInterface = intf;
}

FileInterface* rwConfigBlock::GetFileInterface( void ) const
{
    scoped_rwlock_reader <rwlock> lock( GetConfigLock() );

    FileInterface *ourInterface = this->customFileInterface;

    if ( ourInterface == NULL )
    {
        ourInterface = &_defaultFileInterface;
    }

    return ourInterface;
}

void Interface::SetFileInterface( FileInterface *intf )
{
    EngineInterface *engineInterface = (EngineInterface*)this;

    GetEnvironmentConfigBlock( engineInterface ).SetFileInterface( intf );
}

FileInterface* Interface::GetFileInterface( void )
{
    const EngineInterface *engineInterface = (const EngineInterface*)this;

    return GetConstEnvironmentConfigBlock( engineInterface ).GetFileInterface();
}

};