#include "Freetype.hpp"
#include "Strawberry/Core/Markers.hpp"


namespace Strawberry::UI
{
	FT_Library FreeType::sLibrary = nullptr;


	void FreeType::Initialise()
	{
		auto error = FT_Init_FreeType(&sLibrary);
		if (error)
		{
			Core::Unreachable();
		}
	}


	void FreeType::Terminate()
	{
		auto error = FT_Done_FreeType(sLibrary);
		if (error)
		{
			Core::Unreachable();
		}
	}


	bool FreeType::IsInitialised()
	{
		return sLibrary != nullptr;
	}


	FT_Library FreeType::GetLibrary()
	{
		return sLibrary;
	}


	std::atomic<size_t> FreeTypeUser::sInstanceCount = 0;


	FreeTypeUser::FreeTypeUser()
	{
		if (sInstanceCount == 0)
		{
			FreeType::Initialise();
		}

		++sInstanceCount;
	}


	FreeTypeUser::~FreeTypeUser()
	{
		--sInstanceCount;

		if (sInstanceCount == 0)
		{
			FreeType::Terminate();
		}
	}
}
