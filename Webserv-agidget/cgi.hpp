#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <map>
# include <cstring>
# include <string>
# include <unistd.h>
# include <cstdlib>
# include <fcntl.h>
# include "request_parser/RequestParser.hpp"
# include "Config.hpp"

# define CGI_BUFSIZE 65536

class cgi
{
private:

	typedef std::map<std::string, std::string>	cgiMap;

	cgiMap		_metaVars;
	std::string	_bin;
	std::string	_path;
	std::string	_input; //body of request
	std::string	_output; //give to Olya as a response

public:

	//TODO change it ofc
	cgi()
	{
		_bin = "/usr/bin/python";
		_path = "/Users/agidget/www/add.cgi";
		_input = "num1=7&num2=8";
	}
	// ~cgi();
	// cgi(const cgi& copy);
	// cgi &operator=(const cgi& other);

	void	execute();
	char**	mapToAr();
	void	fillMetaVars(RequestParser req, Config con);

	std::string	getOutput();
};

#endif