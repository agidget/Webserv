#include "tempheader.h"
#include "cgi.hpp"
#define LISTEN_NUM 2 //todo замени это на что-нибудь!


bool isListening(int socket, int *listeningSockets) {
	for (int i = 0; i < LISTEN_NUM; i++) {
		//std::cout << "isListening " << listeningSockets[i] << std::endl;
		if (socket == listeningSockets[i]) {
			std::cout << " true " << std::endl;
			return true;
		}
	}
	return false;
}

std::string readText(std::string filename) {
	std::string str;
	std::stringstream buffer;
	//std::ofstream file("./text.txt");
	std::ifstream read(filename);
	if (!read.is_open())
	{
		printf("no open!\n");
	}
	buffer << read.rdbuf();
	str = buffer.str();

	read.close();
	return str;
}

std::string createResponse(std::string body) {
	std::stringstream response;
	response << "HTTP/1.1 200 OK\r\n"
			 << "Version: HTTP/1.1\r\n"
			 << "Content-Type: text/html; charset=utf-8\r\n"
			 << "Content-Length: " << body.length()
			 << "\r\n\r\n"
			 << body;
	return response.str();
}

t_pollSockets *updateFdsStruct(t_pollSockets *old) {
	int i = 0;
	int j = 0;
	t_pollSockets *newFds;
	newFds = new t_pollSockets;

	newFds->fds = new pollfd[(*old).validFdsNum + 1];
	while (i < (*old).fdsNum) {
		//std::cout << "Update struct cycle"  << std::endl;
		if ((*old).fds[i].fd != -1) {
			newFds->fds[j].fd = (*old).fds[i].fd;
			newFds->fds[j].events = (*old).fds[i].events;
			j++;
		}
		i++;
	}

	newFds->fdsNum = (*old).validFdsNum;
	newFds->validFdsNum = (*old).validFdsNum;

	newFds->listeningSockets = new int[LISTEN_NUM]; //todo где должен храниться этот размер?
	for (int i = 0; i < LISTEN_NUM; i++) {
		newFds->listeningSockets[i] = (*old).listeningSockets[i];
	}
	//std::cout << "listening "  << newFds->listeningSockets[0] << "listening " <<  newFds->listeningSockets[1] << std::endl;
	delete []old->fds;
	delete []old->listeningSockets;
	delete old;
	return newFds;
}


t_pollSockets *createFdsStruct(std::vector<Server> servers) {
	t_pollSockets *newFds;
	newFds = new t_pollSockets;
	newFds->fds = new pollfd[servers.size() + 1];
//	//std::cout << "\nnew address "  << newFds->fds << std::endl;
	//memset(newFds->fds, 0 , sizeof(&newFds->fds));
	for (int i = 0; i < servers.size(); i++) {
		newFds->fds[i].fd = servers[i].getFd();
		newFds->fds[i].events = POLLIN;
	}
	newFds->listeningSockets = new int[servers.size()];

	for(int i = 0; i < servers.size(); i++) {
		newFds->listeningSockets[i] = servers.at(i).getFd();
	}

	int size = (int)servers.size();
	newFds->fdsNum = size;
	newFds->validFdsNum = size;
	return newFds;
}

int createOneSocket(int port, char *host) {
	int server_fd;
	struct sockaddr_in address;

	//заполняем структуру
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(port);
	memset(address.sin_zero, '\0', sizeof address.sin_zero);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}
	//make socket reusable
	int option = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option)) < 0) {
		std::cout << "setsock failed";
	}
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, SOMAXCONN) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	//getsockname(server_fd, (struct sockaddr *)&address, reinterpret_cast<socklen_t *>(sizeof(address)));
	//printf("Listen on port: %d\n", address.sin_addr.s_addr);
	return server_fd;
}


void createListeningSockets(std::vector<Server> *servers) {
	for (int i = 0; i < servers->size(); i++) {
		servers->at(i).setFd(createOneSocket(servers->at(i).getPort(), servers->at(i).getHost()));
	}
}

std::vector<Server> createServers() {
	std::vector<Server> servers;

	Server tempServer1;
	Server tempServer2;
	tempServer1.setHost((char *)"127.0.0.1");
	tempServer1.setPort(8080);
	tempServer2.setPort(8090);
	tempServer2.setHost((char *)"127.0.0.1");
	servers.push_back(tempServer1);
	servers.push_back(tempServer2);
	return servers;
}


// RequestParser *pseudoParser() {
// 	RequestParser *request = new RequestParser(new ClientSocket());
// 	request->method = "DELETE";
// 	request->translatedPath = "/Users/gflores/webserver/deleteMe";
// 	return request;
// }

bool acceptConnection(s_pollSockets *sockets, int i) {
	struct sockaddr_in clientAddress;
	int addrlen = sizeof(clientAddress);
	sockets->fds[sockets->fdsNum].fd = accept(sockets->fds[i].fd, (struct sockaddr *) &clientAddress, (socklen_t *) &addrlen);
	if (sockets->fds[sockets->fdsNum].fd == -1) {
		std::cout << "Accept failed!" << std::endl;
		return false;
	}
	std::cout << "Accepted " << sockets->fds[sockets->fdsNum].fd << std::endl;
	sockets->fds[sockets->fdsNum].events = POLLIN;
	sockets->fdsNum++;
	sockets->validFdsNum++;
	return true;
}

Server *findServerConfig(std::vector<Server> servers, int fd) {
	for (int i = 0; i < servers.size(); i++) {
		if (servers[i].getFd() == fd) {
			return &servers[i];
		}
	}
	return nullptr;
}

int main(int argc, char const *argv[])
{
	//masha
	cgi a = cgi();
	a.execute();
	//a();

	std::map <int, ClientSocket *> clientsSockets;
	//std::map <int, Server *> serversSockets;
	//ResponseCreator *responseCreator = new ResponseCreator(pseudoParser());

	std::vector<Server> servers = createServers(); //todo здесь должен быть парсинг


	createListeningSockets(&servers);
	//std::cout << "Here 1"  << std::endl;
	struct sockaddr_in clientAddress;
	int addrlen = sizeof(clientAddress);

	//char buffer[30000] = {0};
	char *buffer = new char[3000];
	s_pollSockets *sockets =  createFdsStruct(servers);


	int i = 0;
	//todo сделай это нормально!!!
	std::string response = createResponse(readText("/Users/agidget/www/add.html"));


	while (1) {
		int res = poll(sockets->fds, sockets->fdsNum, 3000);

		if (res > 0) {
			while (i < sockets->fdsNum) {

				if (!res) {
					std::cout << "timeout in poll\n";
				} else if (sockets->fds[i].revents == POLLIN) {
					if (isListening(sockets->fds[i].fd, sockets->listeningSockets)) {
						if (!acceptConnection(sockets, i)) {
							break;
						}
						clientsSockets.insert(std::make_pair(sockets->fds[sockets->fdsNum].fd, new ClientSocket())); //todo MAP
						clientsSockets.at(sockets->fds[sockets->fdsNum].fd)->setServer(findServerConfig(servers, sockets->fds[i].fd));
						if (!clientsSockets.at(sockets->fds[sockets->fdsNum].fd)->checkConfiguration()) {
							//todo СДЕЛАЙ ЧТО-НИБУДЬ!
							std::cout << "CATASTROPHE!!!" << std::endl;
						}
					} else {
						int res = 0;
						while (res <= 0) {
						delete []buffer;
						buffer = new char[3000];
						res = recv(sockets->fds[i].fd, buffer, 1024, 0);
						sockets->fds[i].events = POLLOUT;
						puts(reinterpret_cast<const char *>(buffer));
					}
				}
				} else if (sockets->fds[i].revents == POLLOUT) {
					std::cout << "Response:\n" << std::endl;
					//response = responseCreator->getResponse();
					
					response = a.getOutput();
					std::cout << response << std::endl;
					send(sockets->fds[i].fd, response.c_str(), response.length(), 0);
					close(sockets->fds[i].fd);
					sockets->fds[i].fd = -1;
					sockets->validFdsNum--;
					std::cout << "end Response\n" << std::endl;
				}
				i++;
			}
			i = 0;
			sockets = updateFdsStruct(sockets);
		}
	}
	//close(server_fd);
	return 0;
}
