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
	char *response; //ответ клиенту
	//думаю, тут может лежать еще все что угодно
public:
	Server *config; //параметры из config файла в соответствии с которыми нужно обрабатывать request
	ClientSocket() { //todo перенеси!!!

	}
	void setResponse(char *str){
		this->response = str;
	}
	char *getResponse() {
		return response;
	}
	char *getRequest() {
		return request;
	}
//
	void setServer(Server *newConfig) {
		this->config = newConfig;
	}
	void SetRequest(char *request) {
		//std::cout << "Set request: " << request << std::endl;
		this->request = request;
	}
	bool checkConfiguration() {
		if (this->config == nullptr)
			return false;
		return true;
	}
//	const char *printSomething() {
//		std::cout << config->getPort() << std::endl;
//		if (config->getPort() == 8080)
//			return (char *)"hey, you are on 8080 port!";
//		return (char *)"hey, you are NOT on 8080 port!";
//	}
//	char returnIndex(int i) {
//		return(response[i]);
//	}
};

#endif //SERVER_CLIENTSOCKET_H
