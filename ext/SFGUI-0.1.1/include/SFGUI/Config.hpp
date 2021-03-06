#ifndef _SFGUI_CONFIG_HPP__
#define _SFGUI_CONFIG_HPP__

#if defined( _WIN32 ) || defined( __WIN32__ )
	#define SFGUI_SYSTEM_WINDOWS
	#define WIN32_LEAN_AND_MEAN

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#if defined( SFGUI_SYSTEM_WINDOWS ) && !defined( SFGUI_STATIC )
	#ifdef SFGUI_EXPORTS
		#define SFGUI_API
	#else
		#define SFGUI_API
	#endif
#else
	#define SFGUI_API
#endif

#ifdef _MSC_VER
	#pragma warning(disable : 4251) // Suppress a warning which is meaningless for us
	#pragma warning(disable : 4503) // Suppress warnings about truncated names. Enable again if linker errors occur.
#endif

#if !defined( NDEBUG )
	#define SFGUI_DEBUG
	#include <iostream> // XXX Only for debugging purposes.
#endif

#endif
