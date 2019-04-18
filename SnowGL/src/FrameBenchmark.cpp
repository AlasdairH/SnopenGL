#include "PCH.h"
#include "FrameBenchmark.h"

namespace SnowGL
{
	std::stringstream FrameBenchmark::getDataSS()
	{
		unsigned int total = 0;

		std::stringstream ss;
		ss << "==========" << "\n";
		ss << "Frame: " << frame << "\n";
		// shadow mapping
		if (shadowMapping.isRunning())
		{
			ss << "Shadow Mapping: " << "Still Running" << "\n";
		}
		else
		{
			ss << "Shadow Mapping:				" << shadowMapping.getDuration() << "ns" << "\n";
			total += shadowMapping.getDuration();
		}

		// collision detection transform feedback
		if (collisionDetectionTransformFeedback.isRunning())
		{
			ss << "Collision Detection Transform Feedback: " << "Still Running" << "\n";
		}
		else
		{
			ss << "Collision Detection Transform Feedback:	" << collisionDetectionTransformFeedback.getDuration() << "ns" << "\n";
			total += collisionDetectionTransformFeedback.getDuration();
		}

		// visuals
		if (visuals.isRunning())
		{
			ss << "Visuals: " << "Still Running" << "\n";
		}
		else
		{
			ss << "Visuals:				" << visuals.getDuration() << "ns" << "\n";
			total += visuals.getDuration();
		}

		// particle simulation
		if (particleSimulation.isRunning())
		{
			ss << "Particle Simulation: " << "Still Running" << "\n";
		}
		else
		{
			ss << "Particle Simulation:			" << particleSimulation.getDuration() << "ns" << "\n";
			total += particleSimulation.getDuration();
		}

		ss << "Total Time:				" << total << "ns" << "\n";

		ss << "End Frame";
		return ss;
	}

	std::stringstream FrameBenchmark::getCSVHeaders()
	{
		std::stringstream ss;
		ss << "Frame" << "," << "Shadow Mapping" << "," << "Collision Detection TFB" << ","
			<< "Visuals" << "," << "Particle Simulation";
		return ss;
	}

	std::stringstream FrameBenchmark::getDataCSV()
	{
		std::stringstream ss;
		ss << frame << "," << shadowMapping.getDuration() << "," << collisionDetectionTransformFeedback.getDuration() << ","
			<< visuals.getDuration() << "," << particleSimulation.getDuration();
		return ss;
	}
}