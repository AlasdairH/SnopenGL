#include "PCH.h"
#include "LogFile.h"

namespace SnowGL
{
	LogFile::LogFile(const std::string &_filepath)
	{
		// open the file in append mode to add to it
		m_file.open(_filepath, std::ios::app);
		// get the current date and time
		auto time = std::chrono::system_clock::now();
		std::time_t startTime = std::chrono::system_clock::to_time_t(time);
		// write a start into the log
		m_file << "Log Startd " << std::ctime(&startTime);
	}

	LogFile::~LogFile()
	{
		// get the current date and time
		auto time = std::chrono::system_clock::now();
		std::time_t endTime = std::chrono::system_clock::to_time_t(time);
		// write a end into the log
		m_file << "Log Ended " << std::ctime(&endTime);
		// close the file
		m_file.close();
	}

	void LogFile::write(const std::string & _line)
	{
		m_file << _line << "\n";
	}
}