#pragma once


//======================================================================================================================
//	Include
//======================================================================================================================
#include <atomic>
#include "ft2build.h"
#include FT_FREETYPE_H


//======================================================================================================================
//	Class Declaration
//======================================================================================================================
namespace Strawberry::UI
{
	class FreeType
	{
		public:
			static void       Initialise();
			static void       Terminate();
			static bool       IsInitialised();
			static FT_Library GetLibrary();

		private:
			static FT_Library sLibrary;
	};


	class FreeTypeUser
	{
		public:
			FreeTypeUser();
			~FreeTypeUser();

		protected:
			static std::atomic<size_t> sInstanceCount;
	};
}
