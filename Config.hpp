#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Server.hpp"

class Config
{
private:

	std::vector<Server*>	servers;

public:

	Config() {}
	~Config()
	{
		for (int i = 0; i < servers.size(); i++)
			delete servers[i];
	}
	Config(const Config &copy)
	{
		for (int i = 0; i < copy.servers.size(); i++)
		{
			servers.push_back(new Server());
			servers[i] = copy.servers[i];
		}
	}
	
	void	errorMsg()
	{
		std::cerr << "Error with the config file\n";
		for (int i = 0; i < servers.size(); i++)
			delete servers[i];
		exit(1);
	}
	void	readingFile(char* file)
	{
		std::ifstream				infile;
		std::stringstream			buf;
		std::string					line;

		infile.open(file, std::ios::in);
		if (!infile.good())
			errorMsg();
		buf << infile.rdbuf();
		line = buf.str();
		int i = 0;
		int j = 0;
		while (line[i])
		{
			while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
				i++;
			if (line.find("server", i) != -1)
			{
				i += 7;
				while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
					i++;
				if (line[i] == '{')
				{
					i++;
					servers.push_back(new Server());
					i = servers[j]->fillServer(line, i);
					j++;
				}
				else
					errorMsg();
			}
			else
				errorMsg();
			i++;
		}
	}
	//FIXME delete later
	void	check()
	{
		std::cout << "\n----------\n";
		for (int j = 0; j < servers.size(); j++)
		{
			std::cout << servers[j]->getHost() << std::endl;
			std::cout << servers[j]->getPort() << std::endl;
			for (int i = 0; i < servers[j]->getServerNames().size(); i++)
				std::cout << servers[j]->getServerNames()[i] << " ";
			std::cout << std::endl;
			std::cout << servers[j]->getRoot() << std::endl;
			std::cout << servers[j]->getBodySize() << std::endl;
			std::map<int, std::string>::iterator it;
			for (it = servers[0]->getErrors()->begin(); it != servers[0]->getErrors()->end(); it++)
				std::cout << it->first << " " << it->second << std::endl;
			for (int k = 0; k < (*servers[j]->getLocation()).size(); k++)
			{
				std::cout << (*servers[j]->getLocation())[k]->_path << std::endl;
				for (int i = 0; i < (*servers[j]->getLocation())[k]->_methods.size(); i++)
					std::cout << (*servers[j]->getLocation())[k]->_methods[i] << " ";
				std::cout << std::endl;
				std::cout << (*servers[j]->getLocation())[k]->_index << std::endl;
				std::cout << (*servers[j]->getLocation())[k]->_autoindex << std::endl;
				std::cout << (*servers[j]->getLocation())[k]->_enable_uploading << std::endl;
				std::cout << (*servers[j]->getLocation())[k]->_index << std::endl;
				std::cout << (*servers[j]->getLocation())[k]->_upload_route << std::endl;
				std::cout << (*servers[j]->getLocation())[k]->_redirection << std::endl;
			}
			std::cout << "----------\n";
		}
	}
};

#endif