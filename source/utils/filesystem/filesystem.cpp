#include "filesystem.h"

#include "../../poro/platform_defs.h"

#ifdef PORO_PLAT_WINDOWS
#	define CENG_PLATFORM_WINDOWS
#endif

#ifdef PORO_PLAT_MAC
#	define CENG_PLATFORM_MACOSX
#endif

#ifdef PORO_PLAT_LINUX
#	define CENG_PLATFORM_LINUX
#endif


#ifdef CENG_PLATFORM_WINDOWS
#include <io.h>
#include <windows.h>
#include <shlobj.h>
#endif
#ifdef CENG_PLATFORM_MACOSX
#import <Cocoa/Cocoa.h>
//#import "external/NSFileManager+DirectoryLocations.h"
#endif
#ifdef CENG_PLATFORM_LINUX
#include <sys/stat.h>
#include <fcntl.h>
#endif


#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

#ifdef CENG_PLATFORM_WINDOWS
// #undef UNICODE
#include <shlobj.h>
#endif


#include "../safearray/csafearray.h"
#include "../string/string.h"

#include "../debug.h"
namespace ceng {

namespace {
std::string separator = "/";
}

bool DoesExist( const std::string& filename )
{
	#ifdef CENG_PLATFORM_WINDOWS
		struct _finddata_t c_file;
		long hFile;
		std::string result;

		if((hFile = _findfirst(filename.c_str(), &c_file)) == -1L)
			return false;
	#elif defined(CENG_PLATFORM_MACOSX)
#if 0
		NSString *path = [[NSString alloc] initWithCString:filename.c_str() encoding:NSMacOSRomanStringEncoding];
		@try
		{
            return [[NSFileManager defaultManager] fileExistsAtPath:path]==TRUE;
		}
		@finally
		{
			[path release];
		}
#endif
	#elif defined(CENG_PLATFORM_LINUX)
        struct stat st;
        if(stat(filename.c_str(),&st) != 0)
            return false;
    #endif

	return true;
}

std::string GetDateForFile( const std::string& filename )
{
	std::string result;
	#ifdef CENG_PLATFORM_WINDOWS
	struct _finddata_t c_file;
	long hFile;

	if((hFile = _findfirst(filename.c_str(), &c_file)) == -1L)
	{
		return "";
	}
	else
	{
		std::stringstream ss;

		__time64_t time;
		if( c_file.time_write )
			time = c_file.time_write;
		else if( c_file.time_create )
			time = c_file.time_create;
		else
			time = c_file.time_access;
		result = ss.str();

		tm* timeinfo = localtime( &time );

		ss << std::setfill('0') << std::setw( 2 ) << ( timeinfo->tm_year - 100 );
		ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_mon + 1;
		ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_mday;
		ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_hour;
		ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_min;

		result = ss.str();

		_findclose(hFile);
	}
	#elif defined(CENG_PLATFORM_MACOSX)
#if 0
		NSFileManager *fileManager = [NSFileManager defaultManager];
		NSString *path = [[NSString alloc] initWithCString:filename.c_str() encoding:NSMacOSRomanStringEncoding];
		NSDictionary *fileAttributes = [fileManager fileAttributesAtPath:path traverseLink:YES];
		@try
		{
			if (fileAttributes != nil)
			{
				std::stringstream ss;
				NSDate *fileModDate = [fileAttributes objectForKey:NSFileModificationDate];
				time_t time = [fileModDate timeIntervalSince1970];
				tm* timeinfo = localtime( &time );

				ss << std::setfill('0') << std::setw( 2 ) << ( timeinfo->tm_year - 100 );
				ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_mon + 1;
				ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_mday;
				ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_hour;
				ss << std::setfill('0') << std::setw( 2 ) << timeinfo->tm_min;

				result = ss.str();
			}
		}
		@finally
		{
			[path release];
		}
#endif
    #elif defined(CENG_PLATFORM_LINUX) 
#if 0
        QFileInfo fileinfo( QString( filename.c_str() ) );
        QDateTime timeinfo = fileinfo.lastModified();

        QString date_str = timeinfo.toString( "yyMMddhhmm" );

        result = date_str.toUtf8().data();
        return result;
#endif
	#endif

	return result;
}


long ReadFileSize( std::fstream& file )
{
	cassert( file.is_open() != 0 );

	long begin;
	long end;
	long size;
	long current;

	current = file.tellg();

	file.seekg(0, std::ios::beg );
	begin	= file.tellg();

	file.seekg(0, std::ios::end );
	end		= file.tellg();

	size	= end - begin;

	file.seekg( current );

	return size;
}

long ReadFileSize( const std::string& filename )
{
	std::fstream file;
	file.open( filename.c_str(), std::ios::in | std::ios::binary );

	if ( file.is_open() == 0 )
	{
		logger_error << "Error! Could not open " << filename << " for reading" << std::endl;
		return 0;
	}

	long size = ReadFileSize( file );

	file.close();
	return size;
}

//format=0(or undefined) Don't format, format=1 Unix(NL), format=2 Windows(CR+NL)
void ReadFileToBuffer( const std::string& filename, CSafeArray< char, long >& buffer, int format )
{
	std::fstream file;
	file.open( filename.c_str(), std::ios::in | std::ios::binary );

	if ( file.is_open() == 0 )
	{
		logger_error << "Error! Could not open " << filename << " for reading" << std::endl;
		return;
	}

    if(format){
        std::stringstream ripped;
        long size = 0;
        char c,prev=0;
        
        while(file.get(c).good()) {
            
            if(format==1) {
                //To unix (NL)
                if(c != '\r') {
                    ripped.put(c);
                    ++size;
                }
            } else if(format==2) {
                //To windows (CR+NL)
                if(prev != '\r' && c == '\n') {
                    ripped.put('\r');
                    ++size;
                }
                ripped.put(c);
                ++size;
                prev=c;
            }
        }
        buffer.Resize( size );
        ripped.read( buffer.data, size );
        
    } else {
        long size = ReadFileSize( file );
        buffer.Resize( size );
        file.read( buffer.data, size );
    }
        
    file.close();
}

std::string GetFilename( const std::string& filename )
{
	unsigned int p = filename.find_last_of( separator );
	if( p < filename.size() )
		return filename.substr( p + 1 );

	return "";
}

// path /foo/bar
// returns /foo
std::string GetParentPath( const std::string& path )
{
	size_t pos = path.find_last_of( separator );
	if( pos == path.npos )
		return "";

    std::string result = path.substr( 0, pos );
	return result;
}

std::string GetFileExtension( const std::string& filename )
{
	unsigned int p = filename.find_last_of( "." );
	if( p < ( filename.size() - 1 ) )
		return ceng::Lowercase( filename.substr( p + 1 ) );

	return "";
}

std::string GetFilenameWithoutExtension( const std::string& filename )
{
	std::string result = GetFilename( filename );

	unsigned int p = result.find_last_of( "." );
	if( p < result.size() )
		return result.substr( 0, p );

	return result;
}

std::string MakeUniqueFilename( const std::string& file, const std::string& extension )
{
	if( ceng::DoesExist( file + "." + extension ) == false )
		return file;

	int i = 1;
	std::string filename = file + "1";
	while( ceng::DoesExist( filename + "." + extension ) )
	{
		i++;
		filename = file + ceng::CastToString( i );
	}

	return filename;
}


} // end o namespace ceng
