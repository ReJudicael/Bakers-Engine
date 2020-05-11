#pragma once

#include <string>
#include <vector>

namespace Ext
{
	/**
	 * Separate given string into several strings separated by char
	 * @param s: String to explode
	 * @param c: Char to separate each fragments of the string
	 */
	inline std::vector<std::string> Explode(const std::string& s, char c) noexcept
	{
		std::string buff{ "" };
		std::vector<std::string> v;

		for (char n : s)
		{
			if (n != c)
				buff += n;
			else
				if (n == c && buff != "")
				{
					v.push_back(buff);
					buff = "";
				}
		}
		if (buff != "")
			v.push_back(buff);

		return v;
	}
}
