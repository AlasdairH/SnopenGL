#include "PCH.h"
#include "IOUtilities.h"

namespace SnowGL
{
	std::string IOUtilities::loadText(std::string _filepath)
	{
		{
			std::ifstream inFile;
			//open the input file
			inFile.open(_filepath);

			if ((inFile.rdstate() & std::ifstream::failbit) != 0)
			{
				CONSOLE_ERROR("Failed to Open File: " << _filepath);
				return std::string("ERROR");
			}

			std::stringstream strStream;
			// read the file
			strStream << inFile.rdbuf();
			// store the string
			std::string str = strStream.str();

			CONSOLE_WARNING("Loaded File: " << _filepath);
			return str;
		}
	}
}