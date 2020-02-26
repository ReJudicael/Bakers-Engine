#pragma once

#include <string>
#include <vector>

namespace Ext
{
	inline const std::vector<std::string> Explode(const std::string& s, char c) noexcept
	{
		std::string buff{ "" };
		std::vector<std::string> v;

		for (auto n : s)
		{
			if (n != c)
				buff += n;
			else
				if (n == c && buff != "")
					v.push_back(buff); buff = "";
		}
		if (buff != "")
			v.push_back(buff);

		return v;
	}
}
