//
// Created by Gaynelle Flores on 1/28/22.
//

#include "./request_parser/RequestParser.hpp"
#ifndef SERVER_RESPONSECREATOR_H
#define SERVER_RESPONSECREATOR_H

class ResponseCreator {

private:
	std::string response;
	t_location *location;
	RequestParser *request;
	std::string pathToFile;

	std::string statusCode;
	std::string contentType;
	std::string contentLen;
	std::string body;
	std::string version;
	Server			*_config;


	std::string readFile() {
		std::string str;
		std::stringstream buffer;
		//std::ofstream file("./text.txt");
		std::ifstream read(this->pathToFile);
		if (!read.is_open())
		{
			printf("no open!\n"); //todo what type of error??
			throw 404;
		}
		buffer << read.rdbuf();
		str = buffer.str();
		read.close();
		return str;
	}

	void getMethod() {
		this->statusCode = "200 OK";
		this->contentType = "text/html; charset=utf-8"; // type of content
		this->body = readFile();
	}

	void deleteMethod() {
		if (remove(this->pathToFile.c_str()) == 0) {
			this->statusCode = "200 OK";
			this->body = "<html>\n"
				   "  <body>\n"
				   "    <h1>File deleted.</h1>\n"
				   "  </body>\n"
				   "</html>";
		} else {
			this->statusCode = "404 Not Found";
			this->body = "<html>\n"
				   "  <body>\n"
				   "    <h1>File not found.</h1>\n"
				   "  </body>\n"
				   "</html>";
		}
		this->contentType += "text/html; charset=utf-8";
	}

	void createResponse(){
		this->response += this->statusCode + "\r\n" + this->version + "\r\n" + this->contentType + "\r\n" +  this->contentLen + "\r\n\n";
		this->response += body;
	}

	void checkMethod() {
		std::string method = request->getRequestMethod();
		std::vector<std::string>	methods = this->location->_methods;
		for (int i = 0; i < methods.size(); i++) {
			if (methods.at(i) == method) {
				return;
			}
		}
		throw 405;
	}

	void findPathToFile(){
		std::cout << "Root " << this->_config->getRoot() << std::endl;
		if (this->request->getPath() == this->location->_path) {
			this->pathToFile = this->_config->getRoot() + "/" + this->location->_index;
			std::cout << "PathToFile " << this->pathToFile << std::endl;
			return;
		}
		this->pathToFile = this->_config->getRoot() + this->request->getPath();
		std::cout << "PathToFile " << this->pathToFile << std::endl;
		//throw 404;
	}

	void chooseMethod() {
		//todo в случае с редиректом добавляется поле location???
		std::string method = request->getRequestMethod();
		if (method == "DELETE") {
			deleteMethod();
		}
		else if (method == "GET") {
			getMethod();
		}

	}
	void countContentLen() {
		std::ostringstream stream;
		stream << this->body.size();
		std::string converted = stream.str();
		this->contentLen += converted;
	}
	void findConfiguration() {
	//	this->location =
	}
	void checkBodySize() {
		char * pEnd;
		long body = std::strtol(request->getContentLength().c_str(),&pEnd,10);
		//std::cout << "_config->getBodySize() " << _config->getBodySize() << "\n";
		if (body > _config->getBodySize()) {
			throw 413;
		}
	}

public:
	void setConfig(Server *config) {
		this->_config = config;
	}

//	ResponseCreator(RequestParser *request) {
//		//проверить допустим ли метод!!! на этапе аделины?
//		this->request = request;
//		this->response = "HTTP/1.1 ";
//		this->version = "Version: HTTP/1.1";
//		this->contentLen = "Content-Length: ";
//		this->contentTy
//		pe = "Content-Type: ";
//		this->body = "";
//		findConfiguration();
//		chooseMethod();
//		countContentLen();
//		createResponse();
//
//	}

	ResponseCreator() { //todo доделай и сделай предохранение от нул!
		//проверить допустим ли метод!!! на этапе аделины?
		this->request = NULL;
		this->response = "HTTP/1.1 ";
		this->version = "Version: HTTP/1.1";
		this->contentLen = "Content-Length: ";
		this->contentType = "Content-Type: ";
		this->body = "";
	}

	void start() {
		try {
			checkBodySize();
			checkMethod();
			findPathToFile();
			chooseMethod();
		}
		catch (int code) {
			if (code == 405) {
				std::cout << "method is not allowed " << code << std::endl; //todo return error pages
			}
			if (code == 413) {
				std::cout << "Body size is too big " << code << std::endl; //todo return error pages
			}
			if (code == 404) {
				std::cout << "not found " << code << std::endl; //todo return error pages
			}
		}
		countContentLen();
		createResponse();
		std::cout << "Response = " + this->response << std::endl;
	}

	std::string findPathToLocation(std::string path) {
		char *temp = new char[path.size()];
		std::cout << path.size()  << std::endl;
		for (int i = 0; i < path.size(); i++) {
			temp[i] = path[i];
		}
		int end = 0;
		std::string res;
		strcpy( temp, path.c_str());
		for (int i = path.size(); i >= 0; i--) {
			if (temp[i] == '/') {
				end = i;
				break;
			}
		}
		res = std::string(temp);
		delete[] temp;
		res = res.substr(0,end + 1);
		//std::cout << "trimmed path " << res << std::endl;
		return res;
	}

	void setLocation(){
		std::string path = findPathToLocation(request->getPath());
		std::vector<t_location*>	*locations = this->_config->getLocation();
		for (int i = 0; i < locations->size(); i++) {
			if (locations->at(i)->_path == path) {
				this->location = locations->at(i);
				return ;
			}
		}
		for (int i = 0; i < locations->size(); i++) {
			if (locations->at(i)->_path == "/") {
				this->location = locations->at(i);
				return ;
			}
		}
	}

	void setRequest(RequestParser *request) {
		this->response = "HTTP/1.1 ";
		this->version = "Version: HTTP/1.1";
		this->contentLen = "Content-Length: ";
		this->contentType = "Content-Type: ";
		this->body = "";
		this->request = request;
		this->_config = request->getConfig();
		setLocation();
		//std::cout << "this location " << location->_path << std::endl; todo test with different locations
		//std::cout << "this->location " << this->location->_path << std::endl;
		start();
	}

	char *getResponse() {
//		return "HTTP/1.1 200 OK\n"
//			   "Version: HTTP/1.1\n"
//			   "Content-Type: text/html; charset=utf-8\n"
//			   "Content-Length: 577\n"
//			   "\n"
//			   "<!--<!DOCTYPE html>-->\n"
//			   "<!--<html>-->\n"
//			   "<!--<body style=\"background-color:aquamarine;\">-->\n"
//			   "<!--<h1 style=\"color:indigo;\"><center>GET RESPONSE!!!!</center></h1>-->\n"
//			   "<!--<p><center><b>Wow that's possible!</b></center></p>-->\n"
//			   "<!--</body>-->\n"
//			   "<!--</html>-->\n"
//			   "\n"
//			   "<html>\n"
//			   "<body>\n"
//			   "<form action=\"add.cgi\" method=\"POST\">\n"
//			   "<fieldset>\n"
//			   "    <legend>Enter two numbers to add</legend>\n"
//			   "    <label>First Number: <input type=\"number\" name=\"num1\"></label><br/>\n"
//			   "    <label>Second Number: <input type=\"number\" name=\"num2\"></label><br/>\n"
//			   "</fieldset>\n"
//			   "<button>Add</button>\n"
//			   "</form>\n"
//			   "</body>\n"
//			   "</html>";
		return const_cast<char *>(this->response.c_str());
	}
};

#endif //SERVER_RESPONSECREATOR_H
