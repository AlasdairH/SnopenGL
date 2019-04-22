#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

// TODO: Doxygen

namespace SnowGL
{
	enum LogType { LOG_TEXT, LOG_CSV };
	enum LogMethod { LOG_APPEND, LOG_FRESH };

	/*! @class LogFile
	*	@brief This class contains the functionality to write to a file, typically for logging purposes
	*
	*	This class contains the functionality to write to a file for logging purposes. This can either be in CSV or text format.
	*/
	class LogFile
	{
	public:
		/** @brief Opens (or creates if not already created) a file for writing
		*
		*	This constructor will open the file for modification. If the mode is CSV then the file will be overwritten, otherwise it is appended.
		*/
		LogFile(const std::string &_filepath, const LogType &_type, const LogMethod &_method);
		/** @brief Closes the file
		*
		*	This destructor closes the file.
		*/
		~LogFile();

		/** @brief Writes to the file
		*	@param _line The line to write to the log file. The '/n' character can be used to write multiple lines.
		*
		*	Writes a line to the file as given by the parameter
		*/
		void write(const std::string &_line);

	protected:
		std::ofstream	m_file;				/**< The log file to be modified */
		LogType			m_type = LOG_TEXT;	/**< The mode to use when opening and writing to the file */
	};
}