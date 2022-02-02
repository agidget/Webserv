#ifndef WEBSERVER_REQUESTPARSER_HPP
#define WEBSERVER_REQUESTPARSER_HPP
#include <iostream>
#include "../ClientSocket.hpp"

class RequestParser
{
public:
	std::string getServerProtocol();
	std::string getServerPort();
	//std::string getPathInfo() {} //todo что возвращать???
	//the info below we get from config
	// std::string getPathTranslated() {}
	// std::string getScriptName() {}
	//TODO the part of URL after ? character (not so easy i think)
	std::string getQueryString();
	//unset for us coz we dont have domain name
	// std::string getRemoteHost() {}
	//127.0.0.1
	std::string getRemoteAddr();
	//we dont need authication, not needed
	// std::string getAuthType() {}
	// std::string getRemoteUser() {}
	// std::string getRemoteIdent() {}


	//Only in POST method (in Get and Delelte - NULL)
	std::string getContentType();
	//size of body (can return in int) NULL - if nothing
	std::string getContentLength();

	//FIXME add the below ones as well
	std::string getServerName();

	//TODO add everything what we have in a request
	std::string	getConnection();
	std::string	getUserAgent();
	std::string	getAccept();
	std::string	getAcceptEncoding();
	std::string	getAcceptLanguage();
	std::string	getSecFetchDest();
	std::string	getSecFetchMode();
	std::string	getSecFetchSite();
	std::string	getHost();
	std::string getRequestMethod();
	std::string getPathTranslated();
	RequestParser (ClientSocket *socket);
	RequestParser ();
	void setSocket(ClientSocket *socket);
private:
	ClientSocket *_socket;
	std::string method;
	std::string translatedPath;
	std::string request;
	std::string path;
	std::string serverProtocol;
	std::string serverName;
	std::string port;
	std::string acceptEncoding;
	std::string acceptLanguage;
	std::string connection;
	std::string SFMode;
	std::string SFDest;
	std::string SFSite;
	std::string contentType;
	std::string body;
	std::string contentLen;
	std::string accept;
	std::string userAgent;
	bool checkMethodIsAllowed();
	int findEndOfWord(int *offset);
	int findEndOfString(std::string str);
	std::string createSubstring();
	void parseFirstStr();
	std::string findParameter(std::string param);
	void parseHostPort();
	void parser();
};
#endif
