#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <iterator>

typedef struct s_location
{
	std::string					_path;
	std::vector<std::string>	_methods;
	std::string					_index;
	bool						_autoindex;
	bool						_enable_uploading;
	std::string					_upload_route;
	std::string					_redirection;
} t_location;

class	Server
{
private:
	int							fd;
	std::string					_host;
	int							_port;
	std::vector<std::string>	_server_names;
	std::string					_root;
	std::map<int, std::string>	_errors;
	long						_client_max_body_size;
	std::vector<t_location*>	_locations;

public:

	Server() : _port(0), _errors(errorCats()), _client_max_body_size(1048576) {}
	~Server()
	{
		for (int i = 0; i < _locations.size(); i++)
			delete _locations[i];
	}
	Server(const Server &copy)
	{
		_host = copy._host;
		_port = copy._port;
		_server_names = copy._server_names;
		_root = copy._root;
		_errors = copy._errors;
		_client_max_body_size = copy._client_max_body_size;
		for (int i = 0; i < copy._locations.size(); i++)
		{
			_locations.push_back(new t_location);
			_locations[i] = copy._locations[i];
		}
	}
	Server& operator=(const Server &copy)
	{
		for (int i = 0; i < _locations.size(); i++)
			delete _locations[i];
		_host = copy._host;
		_port = copy._port;
		_server_names = copy._server_names;
		_root = copy._root;
		_errors = copy._errors;
		_client_max_body_size = copy._client_max_body_size;
		for (int i = 0; i < copy._locations.size(); i++)
		{
			_locations.push_back(new t_location);
			_locations[i] = copy._locations[i];
		}

		return *this;
	}

	std::map<int, std::string>	errorCats()
	{
		std::map<int, std::string>	errors;

		for (int i = 400; i < 419; i++)
			errors[i] = "https://http.cat/" + std::to_string(i);
		for (int i = 497; i < 511; i++)
			errors[i] = "https://http.cat/" + std::to_string(i);
		return errors;
	}
	void	errorMsg()
	{
		std::cerr << "Error with the config file\n";
		for (int i = 0; i < _locations.size(); i++)
			delete _locations[i];
		exit(1);
	}
	void	initLocation(t_location* route)
	{
		route->_autoindex = false;
		route->_enable_uploading = false;
	}
	int	fillLocation(t_location* route, std::string line, int i)
	{
		initLocation(route);
		while (line[i] && line[i] != '}')
		{
			while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
				i++;
			if (route->_methods.empty() && line.find("allowed_methods", i) != -1
				&& line.find("allowed_methods", i) == line.rfind("allowed_methods", i))
			{
				i += 15;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				std::string allMethods = line.substr(i, line.find(';', i) - i);
				std::istringstream input;
				input.str(allMethods);
				for (std::string line; std::getline(input, line, ' ');)
					route->_methods.push_back(line);
				if (route->_methods.size() > 3)
					errorMsg();
				for (int j = 0; j < route->_methods.size(); j++)
				{
					if (route->_methods[j] != "GET" && route->_methods[j] != "POST"
						&& route->_methods[j] != "DELETE")
						errorMsg();
				}
				i += allMethods.length();
			}
			else if (route->_index.empty() && line.find("index", i) != -1
				&& line.find("index", i) == line.rfind("index", i))
			{
				i += 5;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				route->_index = line.substr(i, line.find(';', i) - i);
				i += route->_index.length();
			}
			else if (line.find("autoindex", i) != -1
					&& line.find("autoindex", i) == line.rfind("autoindex", i))
			{
				i += 9;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				std::string autoIndex = line.substr(i, line.find(';', i) - i);
				if (autoIndex == "on")
					route->_autoindex = true;
				else if (autoIndex == "off")
					route->_autoindex = false;
				else
					errorMsg();
				i += autoIndex.length();
			}
			else if (line.find("enable_upload", i) != -1
					&& line.find("enable_upload", i) == line.rfind("enable_upload", i))
			{
				i += 13;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				std::string enable = line.substr(i, line.find(';', i) - i);
				if (enable == "on")
					route->_enable_uploading = true;
				else if (enable == "off")
					route->_enable_uploading = false;
				else
					errorMsg();
				i += enable.length();
			}
			else if (route->_enable_uploading && line.find("upload_route", i) != -1
				&& line.find("upload_route", i) == line.rfind("upload_route", i))
			{
				i += 12;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				route->_upload_route = line.substr(i, line.find(';', i) - i);
				i += route->_upload_route.length();
			}
			else if (route->_redirection.empty() && line.find("return 301", i) != -1
				&& line.find("return 301", i) == line.rfind("return 301", i))
			{
				i += 10;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				route->_redirection = line.substr(i, line.find(';', i) - i);
				i += route->_redirection.length();
			}
			else if (line[i] == ']')
			{
				if (route->_path.empty() || route->_methods.empty()
					|| route->_index.empty())
					errorMsg();
				break ;
			}
			else
				errorMsg();
			i++;
		}
		return i;
	}
	int	fillServer(std::string line, int i)
	{
		int j = 0;

		while (line[i] && line[i] != '}')
		{
			while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
				i++;
			if (_host.empty() && line.find("host", i) != -1
				&& line.find("host", i) == line.rfind("host", i))
			{
				i += 4;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				_host = line.substr(i, line.find(';', i) - i);
				if (_host.find_first_not_of("0123456789.") != std::string::npos)
					errorMsg();
				i += _host.length();
			}
			else if (!_port && line.find("listen", i) != -1
				&& line.find("listen", i) == line.rfind("listen", i))
			{
				i += 6;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				std::string	portStr = line.substr(i, line.find(';', i) - i);
				if (portStr.find_first_not_of("0123456789") != std::string::npos)
					errorMsg();
				_port = stoi(portStr);
				if (_port < 1 || _port > 65535)
					errorMsg();
				i += portStr.length();
			}
			else if (_server_names.empty() && line.find("server_name", i) != -1
				&& line.find("server_name", i) == line.rfind("server_name", i))
			{
				i += 11;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				std::string allNames = line.substr(i, line.find(';', i) - i);
				std::istringstream input;
				input.str(allNames);
				for (std::string line; std::getline(input, line, ' ');)
					_server_names.push_back(line);
				i += allNames.length();
			}
			else if (_root.empty() && line.find("root", i) != -1
				&& line.find("root", i) == line.rfind("root", i))
			{
				i += 4;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				_root = line.substr(i, line.find(';', i) - i);
				i += _root.length();
			}
			else if (line.find("client_max_body_size", i) != -1
				&& (line.find("client_max_body_size", i)
				== line.rfind("client_max_body_size", i)))
			{
				i += 20;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				std::string	bodyStr = line.substr(i, line.find(';', i) - i);
				if (bodyStr.find_first_not_of("0123456789") != std::string::npos)
					errorMsg();
				_client_max_body_size = std::stol(bodyStr);
				i += bodyStr.length();
			}
			else if (line.find("error_page", i) != -1)
			{
				i += 10;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				int errorNum = stoi(line.substr(i, 3));
				_errors.erase(errorNum);
				i += 3;
				while (line[i] == ' ' || line[i] == '\t')
					i++;
				std::string errorPath = line.substr(i, line.find(';', i) - i);
				_errors.insert(_errors.begin(), std::pair<int, std::string>(errorNum, errorPath));
				i += errorPath.length();
			}
			else if (line.find("location", i) != -1 && line[i] != '}')
			{
				i += 8;
				while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
					i++;
				if (line[i] == '/')
				{
					_locations.push_back(new t_location);
					for (; line[i] != ' ' && line[i] != '\t' && line[i] != '\n'; i++)
						_locations[j]->_path += line[i];
					while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
						i++;
					if (line[i] == '[')
					{
						i++;
						i = fillLocation(_locations[j], line, i);
						j++;
					}
					else
						errorMsg();
				}
				else
					errorMsg();
			}
			else if (line[i] == '}')
			{
				if (_host.empty() || _port == 0 || _server_names.empty() || _root.empty()
					|| _locations.empty())
					errorMsg();
				i++;
				break ;
			}
			else
				errorMsg();
			i++;
		}
		return i;
	}
	std::string	getHost()
	{
		return _host;
	}
	int	getPort()
	{
		return _port;
	}
	std::vector<std::string>	getServerNames()
	{
		return _server_names;
	}
	std::string	getRoot()
	{
		return _root;
	}
	long	getBodySize()
	{
		return _client_max_body_size;
	}
	std::map<int, std::string>*	getErrors()
	{
		return &_errors;
	}
	std::vector<t_location*>*	getLocation()
	{
		return &_locations;
	}
};

#endif