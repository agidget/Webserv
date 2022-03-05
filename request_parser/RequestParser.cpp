#include "RequestParser.hpp"

Server	*RequestParser::getConfig() {
	return this->_config;
}

std::string RequestParser::getServerProtocol() {
	return serverProtocol;
}
std::string RequestParser::getServerPort(){
	return port;
}

std::string RequestParser::getBody() {
	return body;
}

std::string RequestParser::getRemoteAddr() {
	return "127.0.0.1";
}

std::string RequestParser::getContentType() {
	return contentType;
}

std::string RequestParser::getContentLength() {
	return contentLen;
}

std::string RequestParser::getServerName() {
	return serverName;
}

std::string	RequestParser::getConnection() {
	return connection;
}
std::string	RequestParser::getUserAgent() {
	return userAgent;
}
std::string	RequestParser::getAccept() {
	return accept;
}
std::string	RequestParser::getAcceptEncoding() {
	return acceptEncoding;
}
std::string	RequestParser::getAcceptLanguage() {
	return acceptLanguage;
}
std::string	RequestParser::getSecFetchDest() {
	return SFDest;
}
std::string	RequestParser::getSecFetchMode() {
	return SFMode;
}
std::string	RequestParser::getSecFetchSite(){
	return SFSite;
}
std::string	RequestParser::getHost() {
	return serverName;
}
std::string RequestParser::getRequestMethod() {
	return method;
}
std::string RequestParser::getPath() {
	return path;
}

RequestParser:: RequestParser(ClientSocket *socket) {
this->_socket = socket;
this->request = this->_socket->getRequest();
parser();
};
RequestParser::RequestParser () {
	this->_socket = nullptr; //todo проверка на нул?
};

void RequestParser::setSocket(ClientSocket *socket) {
	this->_socket = socket;
	this->_config = _socket->getServer();
	std::cout << "I am a config file for port " << _config->getPort() << std::endl;
	this->request = this->_socket->getRequest();
}

void RequestParser::setConfig(Server *config) {
	//this->_config = config;
	//std::cout << "I am a config file for port " << _config->getPort() << std::endl;
	//this->request = this->_socket->getRequest();
	//parser();
}

void RequestParser::startParsing() {
	parser();
}


bool RequestParser::checkMethodIsAllowed() {
	if (this->method != "DELETE" && this->method != "GET" && this->method != "POST") {
		return false;
	}
	return true;
}
int RequestParser::findEndOfWord(int *offset){
	int find;
	find = (int) request.find(' ', 0);
	*offset = 1;
	if (request.find("\r\n", 0) < find) {
		find = (int) request.find("\r\n", 0);
		*offset = 2;
	}
	return find;
}
int RequestParser::findEndOfString(std::string str){
	int find;
	find = (int) str.find("\r\n", 0);
	return find;
}
std::string RequestParser::createSubstring() {
	int find;
	int offset;
	find = findEndOfWord(&offset);
	std::string substr = request.substr(0, find);
	request = request.substr(find + offset, request.size());
	return substr;
}
void RequestParser::parseFirstStr() {
	this->method = createSubstring();
	this->path = createSubstring();
	this->serverProtocol = createSubstring();
}
std::string RequestParser::findParameter(std::string param) {
	int find;
	int offset;

	offset = (int)param.size();
	find = (int)request.find(param, 0);
	if (find == -1) {
		return "";
	}
	std::string substr = request.substr(find + offset, request.size());
	return substr.substr(0, findEndOfString(substr));
}
void RequestParser::parseHostPort() {
	std::string hostAndPort = findParameter("Host: ");
	this->serverName = hostAndPort.substr(0, hostAndPort.find(':'));
	this->port = hostAndPort.substr(hostAndPort.find(':') + 1, hostAndPort.size());
}

//	this->location = this->_config->findLocation(this->path); // todo корень по умолчанию??? what is it?????

void RequestParser::parser() {


	parseFirstStr();
	parseHostPort();
	this->acceptEncoding = findParameter("Accept-Encoding: ");
	this->acceptLanguage = findParameter("Accept-Language: ");
	this->connection = findParameter("Connection: ");
	this->SFDest = findParameter("Sec-Fetch-Dest: ");
	this->SFMode = findParameter("Sec-Fetch-Mode: ");
	this->SFSite = findParameter("Sec-Fetch-Site: ");
	this->userAgent = findParameter("User-Agent: ");
	this->contentType = findParameter("Content-Type: ");
	this->contentLen = findParameter("Content-Length: ");
	this->body = findParameter("\n\r\n");
	this->accept = findParameter("Accept: ");



	//std::cout << "This is a current location path : " << location->_path << std::endl;

	//std::cout << "Is empty body: " << this->body.empty()  << std::endl;
	//todo чекаем допустим ли метод, если нет, сразу же возвращаем ошибку

	std::cout << "*******This is a parsing result*******\nMethod: " << this->method << std::endl;
	std::cout << "Path: " << this->path << std::endl;
	std::cout << "serverProtocol: " << this->serverProtocol << std::endl;
	std::cout << "serverName: " << this->serverName << std::endl;
	std::cout << "port: " << this->port << std::endl;
	std::cout << "acceptEncoding: " << this->acceptEncoding << std::endl;
	std::cout << "acceptLanguage: " << this->acceptLanguage << std::endl;
	std::cout << "connection: " << this->connection << std::endl;
	std::cout << "Sec-Fetch-Dest: " << this->SFDest << std::endl;
	std::cout << "Sec-Fetch-Mode: " << this->SFMode << std::endl;
	std::cout << "Sec-Fetch-Site: " << this->SFSite << std::endl;
	std::cout << "ContentType: " << this->contentType << std::endl;
	std::cout << "ContentLen: " << this->contentLen << std::endl;
	std::cout << "Accept: " << this->accept << std::endl;
	std::cout << "Body: " << this->body << std::endl;
	std::cout << "User Agent: " << this->userAgent << std::endl;
};