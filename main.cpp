#include "tempheader.hpp"
#define LISTEN_NUM 2 //todo замени это на что-нибудь!


bool isListening(int socket, int *listeningSockets) {
	for (int i = 0; i < LISTEN_NUM; i++) {
		//std::cout << "isListening " << listeningSockets[i] << std::endl;
		if (socket == listeningSockets[i]) {
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


t_pollSockets *createFdsStruct(std::vector<Server *> servers) {
	t_pollSockets *newFds;
	newFds = new t_pollSockets;
	newFds->fds = new pollfd[servers.size() + 1];
//	//std::cout << "\nnew address "  << newFds->fds << std::endl;
	//memset(newFds->fds, 0 , sizeof(&newFds->fds));
	for (int i = 0; i < servers.size(); i++) {
		newFds->fds[i].fd = servers[i]->getFd();
		newFds->fds[i].events = POLLIN;
	}
	newFds->listeningSockets = new int[servers.size()];

	for(int i = 0; i < servers.size(); i++) {
		newFds->listeningSockets[i] = servers.at(i)->getFd();
	}

	int size = (int)servers.size();
	newFds->fdsNum = size;
	newFds->validFdsNum = size;
	return newFds;
}

int createOneSocket(int port, std::string host) {
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


void createListeningSockets(std::vector<Server *> *servers) {
	for (int i = 0; i < servers->size(); i++) {
		servers->at(i)->setFd(createOneSocket(servers->at(i)->getPort(), servers->at(i)->getHost()));
		//std::cout << "servers fd: " << servers->at(i)->getFd() << std::endl;
	}
}

std::vector<Server *> createServers() {
	std::vector<Server *> servers;

	Server *tempServer1 = new Server();
	Server *tempServer2 = new Server();
	tempServer1->setHost((char *)"127.0.0.1");
	tempServer1->setPort(8090);
	tempServer2->setPort(8080);
	tempServer2->setHost((char *)"127.0.0.1");
	//std::cout << "In function " << servers[0]->getFd() <<  " " << servers[1]->getFd() << std::endl;
	servers.push_back(tempServer1);
	servers.push_back(tempServer2);
	//std::cout << "In function " << servers[0]->getPort() <<  " " << servers[1]->getPort() << std::endl;
	return servers;
}


//RequestParser *pseudoParser() {
//	RequestParser *request = new RequestParser(new ClientSocket());
//	request->method = "DELETE";
//	request->translatedPath = "/Users/gflores/webserver/deleteMe";
//	return request;
//}

bool acceptConnection(s_pollSockets *sockets, int i) {
	struct sockaddr_in clientAddress;
	int addrlen = sizeof(clientAddress);
	sockets->fds[sockets->fdsNum].fd = accept(sockets->fds[i].fd, (struct sockaddr *) &clientAddress, (socklen_t *) &addrlen);
	if (sockets->fds[sockets->fdsNum].fd == -1) {
		std::cout << "Accept failed!" << std::endl;
		return false;
	}
	fcntl(sockets->fds[sockets->fdsNum].fd, F_SETFL, O_NONBLOCK);
	//std::cout << "Accepted " << sockets->fds[sockets->fdsNum].fd << std::endl;
	sockets->fds[sockets->fdsNum].events = POLLIN;
	sockets->fdsNum++;
	sockets->validFdsNum++;
	return true;
}

Server *findServerConfig(std::vector<Server *> servers, int fd) {
	for (int i = 0; i < servers.size(); i++) {
		if (servers[i]->getFd() == fd) {
			return servers[i];
		}
	}
	return nullptr;
}

void sendResponse(s_pollSockets *sockets, std::map <int, ClientSocket *> clientsSockets, int i) {
	std::string response;
	response = clientsSockets.at(sockets->fds[i].fd)->getResponse();
	//std::cout << "\n++++++This is response+++++++" << std::endl;
	//std::cout << response << std::endl;
	//response = clientsSockets.at(sockets->fds[i].fd)->getResponse();  //todo удали
	//response = createResponse(readText("/Users/gflores/newWebServ/page.html"));
	//std::cout << response << std::endl;
	if (send(sockets->fds[i].fd, response.c_str(), response.length(), 0) == -1) {
		std::cout << "error." << std::endl;
		//todo определить корректное поведение!
	}
	close(sockets->fds[i].fd);
	sockets->fds[i].fd = -1;
	sockets->validFdsNum--;
}

//void receiveData() {
//	char *buffer = new char[3000];
//
//	int res = 0;
//	while (res <= 0) {
//
//		res = recv(sockets->fds[i].fd, buffer, 1024, 0);
//		if (res == 0) {
//			std::cout << "Connection was closed." << std::endl;
//			//todo определить корректное поведение!
//		} else if (res == -1) {
//			std::cout << "error." << std::endl;
//			//todo определить корректное поведение!
//		}
//		clientsSockets.at(sockets->fds[i].fd)->SetRequest(buffer);
//		requestParser->setSocket(clientsSockets.at(sockets->fds[i].fd));
//		responseCreator->setRequest(requestParser);
//		clientsSockets.at(sockets->fds[i].fd)->setResponse(responseCreator->getResponse());
//		sockets->fds[i].events = POLLOUT;
//		puts(reinterpret_cast<const char *>(buffer));
//	}
//	delete[]buffer;
//}


const char *findConfigFile(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cout << "Usage: ./webServ configFileName.config" << std::endl;
		exit(0); //todo code exit
	}
	return argv[1];
}

int main(int argc, char const *argv[]) {
	Config	config;
	config.readingFile(const_cast<char *>(findConfigFile(argc, argv)));
	//распарсили конфиг файл и создали соответствующие сервера


	RequestParser *requestParser = new RequestParser();

	std::map<int, ClientSocket *> clientsSockets;
	//std::map <int, Server *> serversSockets;
	ResponseCreator *responseCreator = new ResponseCreator();


	std::vector<Server *> servers = config.getServers();
	for (int i = 0; i < servers.size(); i++) {
		std::cout << "server port " << servers.at(i)->getPort() << std::endl;
		std::cout << "server port " << servers.at(i)->getBodySize() << std::endl;

	}
	//std::cout << "Check vector: " << servers[0]->getPort() << " " << servers[1]->getPort() << std::endl;
	createListeningSockets(&servers);

	char *buffer;
	s_pollSockets *sockets = createFdsStruct(servers);

	int i = 0;
	//todo сделай это нормально!!!
	std::string response = createResponse(readText("./page.html"));

	while (1) {
		int res = poll(sockets->fds, sockets->fdsNum, 3000);

		if (res > 0) {
			while (i < sockets->fdsNum) {
				if (!res) {
					std::cout << "timeout in poll\n";
				} else if (sockets->fds[i].revents == POLLIN) {
					std::cout << "Pollin\n";
					if (isListening(sockets->fds[i].fd, sockets->listeningSockets)) {
						if (!acceptConnection(sockets, i)) {
							break;
						}
						std::cout << "After accept: " << sockets->fdsNum << std::endl;

						if (clientsSockets.count(sockets->fds[sockets->fdsNum - 1].fd)) {
							delete clientsSockets.at(sockets->fds[sockets->fdsNum - 1].fd);
							clientsSockets.erase(sockets->fds[sockets->fdsNum - 1].fd);
						}

						clientsSockets.insert(
								std::make_pair(sockets->fds[sockets->fdsNum - 1].fd, new ClientSocket())); //добавили сокет в массив отслеживания для poll
								//нашли нужный конфиг от текущего сервера
						clientsSockets.at(sockets->fds[sockets->fdsNum - 1].fd)->setServer(
								findServerConfig(servers, sockets->fds[i].fd)); //может вернуть нулл, поэтому ниже его проверка
						//clientsSockets.at(sockets->fds[sockets->fdsNum - 1].fd)->
						if (!clientsSockets.at(sockets->fds[sockets->fdsNum - 1].fd)->checkConfiguration()) {
							//todo СДЕЛАЙ ЧТО-НИБУДЬ!
							std::cout << "CATASTROPHE!!!" << std::endl;
						}
					} else {
						std::cout << "Else pollin\n";
						res = 0;
						while (res <= 0) {
							buffer = new char[3000]; //todo выбери размер буфера!!!
							//memset(&buffer, ' ', 3000);
							res = recv(sockets->fds[i].fd, buffer, 1024, 0);
							if (res == 0) {
								std::cout << "Connection was closed." << std::endl;
								//todo определить корректное поведение!
							} else if (res == -1) {
								std::cout << "error." << std::endl;
								//todo определить корректное поведение!
							}
							//закидываем в мапу, где лежат клиентские сокеты и их фд, реквест в виде строки

							std::cout << "SetRequest." << std::endl;
							clientsSockets.at(sockets->fds[i].fd)->SetRequest(buffer, res);

							//этот сокет кидаем в реквест парсер, там устанавливается соответствующий сокет и сервер-конфиг для него, он парсит
							std::cout << "SetSocket." << std::endl;
							requestParser->setSocket(clientsSockets.at(sockets->fds[i].fd)); //todo нужен ли?
							//requestParser->setConfig(findServerConfig(servers, sockets->fds[i].fd));
							std::cout << "Start parsing." << std::endl;
							requestParser->startParsing();
							//в мой создатель ответов кладем парсер
							std::cout << "Set reqv." << std::endl;
							responseCreator->setRequest(requestParser);
							//responseCreator->setConfig(sockets->fds[i].fd->);
							//кладем в первую мапу строчку ответа и забываем о том, что лежало в парсере и респонс креаторе
							clientsSockets.at(sockets->fds[i].fd)->setResponse(responseCreator->getResponse());
							sockets->fds[i].events = POLLOUT;
							std::cout << "\nOriginal request: " << std::endl;
							delete[]buffer;
							//puts(reinterpret_cast<const char *>(buffer));

						}
					}
				} else if (sockets->fds[i].revents == POLLOUT) {
					//std::cout << "Response:\n" << std::endl;
					sendResponse(sockets, clientsSockets, i);
				}
					i++;
				}
//			for(std::map<int, ClientSocket *>::const_iterator it = clientsSockets.cbegin(); it != clientsSockets.cend(); ++it)
//			{
//				std::cout << "Map: " << it->first << " " << it->second->printSomething() << " " << "\n";
//			}
				i = 0;
				sockets = updateFdsStruct(sockets);
			}
		}

		//close(server_fd);
		return 0;

}
