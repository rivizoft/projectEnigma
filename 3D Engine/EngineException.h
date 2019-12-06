#pragma once

#include <exception>
#include <string>

class EngineException : public std::exception
{
public: 
	EngineException(int line, const char* file);
	const char* what() const override;
	virtual const char* getType() const;
	int getLine() const;
	const std::string& getFile() const;
	std::string getOriginString() const;

private: 
	int _line;
	std::string _file;

protected:
	mutable std::string buffer;
};

