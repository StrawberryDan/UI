#include "FontFace.hpp"


namespace Strawberry::UI
{
	Core::Result<FontFace, Core::IO::Error> FontFace::FromFile(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path)) return Core::IO::Error::NotFound;

		FontFace face(nullptr);
		auto error = FT_New_Face(FreeType::GetLibrary(), path.string().c_str(), 0, &face.mFace);
		switch (error)
		{
			case FT_Err_Ok:
				return face;
			default:
				Core::Unreachable();
		}
	}


	FontFace::FontFace(FontFace&& other) noexcept
			: mFace(std::exchange(other.mFace, nullptr))
	{}


	FontFace& FontFace::operator=(FontFace&& other) noexcept
	{
		if (this != &other)
		{
			std::destroy_at(this);
			std::construct_at(this, std::move(other));
		}
		return *this;
	}


	FontFace::~FontFace()
	{
		if (mFace)
		{
			FT_Done_Face(mFace);
		}
	}


	FontFace::FontFace(FT_Face face)
		: mFace(face)
	{}
}