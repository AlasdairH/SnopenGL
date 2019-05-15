#include "PCH.h"
#include "ParticleSettings.h"

namespace SnowGL
{
	void ParticleSettings::fromSettingsFile(const std::string &_filename)
	{
		std::string text = IOUtilities::loadText(_filename);
		if (text == "ERROR")
		{
			LOG(LOG_WARNING) << "Unable to load settings file";
			return;
		}

		int lineNumber = 0;
		// split the file into seperate lines
		std::vector<std::string> lines = IOUtilities::split(text, '\n');
		// iterate through the lines
		for (const auto& line : lines)
		{
			++lineNumber;
			// split the line by the = delimiter
			std::vector<std::string> pair = IOUtilities::split(line, '=');
			// if there isn't a valid pair
			if (pair.size() < 2 || pair.size() > 2)
			{
				continue;
			}
			
			if (pair[0] == "ParticlesPerSecond")
			{
				particlesPerSecond = std::stoi(pair[1]);
			}
			else if (pair[0] == "LifetimeMin")
			{
				lifetimeMin = std::stof(pair[1]);
			}
			else if (pair[0] == "LifetimeMax")
			{
				lifetimeMax = std::stof(pair[1]);
			}
			else if (pair[0] == "GlobalWind")
			{
				std::vector<std::string> values = IOUtilities::split(pair[1], ',');
				if (values.size() < 3 || values.size() > 3)
				{
					LOG(LOG_WARNING) << "Line " << lineNumber << " contains incorrect number of arguments for Vec3";
					continue;
				}
				else
				{
					globalWind = glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
				}
			}
			else if (pair[0] == "InitialVelocity")
			{
				std::vector<std::string> values = IOUtilities::split(pair[1], ',');
				if (values.size() < 3 || values.size() > 3)
				{
					LOG(LOG_WARNING) << "Line " << lineNumber << " contains incorrect number of arguments for Vec3";
					continue;
				}
				else
				{
					initialVelocity = glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
				}
			}
			else if (pair[0] == "CollisionMultiplier")
			{
				collisionMultiplier = std::stoi(pair[1]);
			}
			else if (pair[0] == "DomainPosition")
			{
				std::vector<std::string> values = IOUtilities::split(pair[1], ',');
				if (values.size() < 3 || values.size() > 3)
				{
					LOG(LOG_WARNING) << "Line " << lineNumber << " contains incorrect number of arguments for Vec3";
					continue;
				}
				else
				{
					domainPosition = glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
				}
			}
			else if (pair[0] == "DomainSize")
			{
				std::vector<std::string> values = IOUtilities::split(pair[1], ',');
				if (values.size() < 3 || values.size() > 3)
				{
					LOG(LOG_WARNING) << "Line " << lineNumber << " contains incorrect number of arguments for Vec3";
					continue;
				}
				else
				{
					domainSize = glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
				}
			}
			else if (pair[0] == "DrawDomain")
			{
				drawDomain = std::stoi(pair[1]);
			}
			else if (pair[0] == "DrawPartition")
			{
				drawPartition = std::stoi(pair[1]);
			}
			else
			{
				LOG(LOG_WARNING) << "Parameter " << line << " not found";
			}
		}

		LOG(LOG_DEBUG) << "Loaded particle settings from file";
	}	
}
