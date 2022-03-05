//
// Created by Gaynelle Flores on 1/27/22.
//
#include "Server.hpp" //todo заменить

#ifndef SERVER_CLIENTSOCKET_H
#define SERVER_CLIENTSOCKET_H


class ClientSocket {
private:
	std::string request; //реквест в формате, который пришел ко мне от клиента
	char *response; //ответ клиенту
	//думаю, тут может лежать еще все что угодно
public:
	Server *config; //параметры из config файла в соответствии с которыми нужно обрабатывать request

	ClientSocket() {

	}

	~ClientSocket() {
		std::cout << "DESTRUCTOR " << std::endl;
		//delete this->response;
	};

	void setResponse(char *str){
		this->response = str;
	}
	char *getResponse() {
		return response;
	}
	char *getRequest() {
		return const_cast<char *>(request.c_str());
	}
//
	void setServer(Server *newConfig) {
		this->config = newConfig;
		//std::cout << "I am a config file for port " << config->getPort() << std::endl;
	}
	Server *getServer() {
		//std::cout << "Set request: " << request << std::endl;
		return this->config;
	}

	void SetRequest(char *request, size_t res) {
		//std::cout << "Set request: " << request << std::endl;
		this->request = request;
//		std::string trimmed( this->request, //Смещаемся на 100 символов вперед от начала строки
//							 res);
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
