#include <iostream>
#include <string>
#include <utility>
#include <map>
// #include "Config.hpp"
#include <cstdlib>

typedef std::map<std::string, std::string>	cgiMap;

char**	mapToAr(cgiMap _metaVars)
{
	char**	arr;
	int		i;

	arr = new char* [_metaVars.size() + 1];
	i = 0;
	for (cgiMap::iterator it = _metaVars.begin(); it != _metaVars.end(); it++)
	{
		std::string str = it->first + "=" + it->second;
		arr[i] = new char [str.length() + 1];
		std::strcpy(arr[i], str.c_str());
		i++;
	}
	arr[i] = NULL;
	return arr;
}

int main()
{
	// Config	toto;
	// toto.readingFile((char *)"test.config");
	// toto.check();

	// int a = 400;
	// std::cout << "lol" + std::to_string(a) << std::endl;

	setenv("LOL", "lol", true);
	while (1);

}