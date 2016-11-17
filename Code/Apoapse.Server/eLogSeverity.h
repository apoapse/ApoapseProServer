#pragma once
#pragma once
enum LogSeverity
{
	normal,
	verbose,
	debug,
	warning,
	error,
	fatalError	// Warning: this will only log a message as fatal error, use FatalError() to actually end the program
};