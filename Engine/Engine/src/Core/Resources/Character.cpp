#include "CoreMinimal.h"

#include "Character.h"

#include <iostream>

namespace Resources
{
	Character::Character(FT_Face& face, const char& c)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		int error {FT_Load_Char(face, c, FT_LOAD_RENDER)};
		if (error)
		{
			BAKERS_LOG_ERROR("FREETYPE: Failed to load Glyph " + std::to_string(c) + " with error code " + std::to_string(error));
			return;
		}
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
			BAKERS_LOG_ERROR("cannot render glyph: " + std::to_string(error));
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		unsigned width {face->glyph->bitmap.width}, height {face->glyph->bitmap.rows};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height,
				0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		sizeX = face->glyph->bitmap.width;
		sizeY = face->glyph->bitmap.rows;
		bearingX = face->glyph->bitmap_left;
		bearingY = face->glyph->bitmap_top;
		advance = face->glyph->advance.x;
	}
}
