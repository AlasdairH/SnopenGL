#include "PCH.h"
#include "SceneDump.h"

namespace SnowGL
{
	std::stringstream SceneDump::getDataSS()
	{
		unsigned int total = 0;

		std::stringstream ss;
		ss << "==========" << "\n";
		ss << "Total Frames: " << simulatedFrames << "\n";
		ss << "Triangle Count: " << triangleCount << "\n";
		ss << "Particle Count: " << particleCount << "\n";
		ss << "Avg Particle Sim Time: " << avgParticleSimTime << "\n";
		ss << "Avg Frames per Second: " << avgParticleSimTime << "\n";

		ss << "End Scene";
		return ss;
	}

	std::stringstream SceneDump::getCSVHeaders()
	{
		std::stringstream ss;
		ss << "Total Frames" << "," << "Triangle Count" << ", " << "Particle Count" << "," << "Avg Particle Sim Time" << "," << "Avg Frames Per Second";
		return ss;
	}

	std::stringstream SceneDump::getDataCSV()
	{
		std::stringstream ss;
		ss << simulatedFrames << "," << triangleCount << "," << particleCount << "," << avgParticleSimTime << "," << avgFPS;
		return ss;
	}
}