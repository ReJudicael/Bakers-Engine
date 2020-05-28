#ifndef __CHARACTER__
#define __CHARACTER__

#include "CoreMinimal.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Resources
{
	struct Character
	{
		unsigned int	textureID;
		int				sizeX;
		int				sizeY;
		int				bearingX;
		int				bearingY;
		unsigned int	advance;

		Character(FT_Face&, const char&);
	};
}

#endif
