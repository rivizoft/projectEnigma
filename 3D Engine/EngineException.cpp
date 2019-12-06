#include "EngineException.h"
#include <sstream>

EngineException::EngineException(int line, const char* file)
{
	this->_line = line;
	this->_file = file;
}

const char* EngineException::what() const
{
	std::ostringstream oss;
	oss << getType() << std::endl << getOriginString();

	buffer = oss.str();
	
	return buffer.c_str();
}

const char* EngineException::getType() const
{
	return "Enigma Engine Exception";
}

int EngineException::getLine() const
{
	return this->_line;
}

const std::string& EngineException::getFile() const
{
	return this->_file;
}

std::string EngineException::getOriginString() const
{
	std::ostringstream oss;
	oss << "File: " << getFile() << std::endl
		<< "Line: " << getLine();

	return oss.str();
}