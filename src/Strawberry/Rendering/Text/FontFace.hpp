#pragma once


//======================================================================================================================
//	Includes
//======================================================================================================================
#include <Strawberry/Core/Types/Result.hpp>
#include <Strawberry/Core/IO/Error.hpp>
#include "Freetype.hpp"
#include <filesystem>


namespace Strawberry::UI
{
    class FontFace
		: public FreeTypeUser
	{
	public:
		static Core::Result<FontFace, Core::IO::Error> FromFile(const std::filesystem::path& path);


		FontFace(const FontFace&) = delete;
		FontFace& operator=(const FontFace&) = delete;
		FontFace(FontFace&& other);
		FontFace& operator=(FontFace&& other) noexcept;
		~FontFace();


	protected:
		FontFace(FT_Face face);


	private:
		FT_Face mFace;
	};
}