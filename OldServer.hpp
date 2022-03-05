//
// Created by Gaynelle Flores on 1/26/22.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

class Server {

private:
	int		fd; //нужно только мне
	char	*host; //из конфиг файла
	int		port; //из конфиг файла
	//здесь будет еще всякое: допустимые команды, максимальный размер тела сообщения и тп
public:
	char *getHost(){
		return host;
	}
	int getPort(){
		return port;
	}
	int getFd(){
		return fd;
	}
	void setHost(char	*newHost){
		this->host = newHost;
	}
	void setPort(int newPort){
		this->port = newPort;
	}
	void setFd(int newFd){
		this->fd = newFd;
	}
};

#endif //SERVER_SERVER_H
