//
// Created by Gaynelle Flores on 1/27/22.
//
#include "Server.hpp" //todo заменить

#ifndef SERVER_CLIENTSOCKET_H
#define SERVER_CLIENTSOCKET_H

//то, что отправляется Аделине
class ClientSocket {
private:
	char *request; //реквест в формате, который пришел ко мне от клиента
	Server *config; //параметры из config файла в соответствии с которыми нужно обрабатывать request
	char *response; //ответ клиенту
	//думаю, тут может лежать еще все что угодно
public:
	ClientSocket() { //todo перенеси!!!
		response = new char[500];
		response[0] = 'a';
		response[1] = 'b';
		response[2] = 'c';
		response[3] = 'd';
		response[4] = 'e';
	}
	const char *printSomething() {
		return (char *)"smth";
	}
	char returnIndex(int i) {
		return(response[i]);
	}
	void setServer(Server *newConfig) {
		this->config = newConfig;
	}
	bool checkConfiguration() {
		if (this->config == nullptr)
			return false;
		return true;
	}
};

#endif //SERVER_CLIENTSOCKET_H