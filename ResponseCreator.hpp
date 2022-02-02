//
// Created by Gaynelle Flores on 1/28/22.
//

#include "./request_parser/RequestParser.hpp"
#ifndef SERVER_RESPONSECREATOR_H
#define SERVER_RESPONSECREATOR_H

class ResponseCreator {

private:
	std::string response;
	RequestParser *request;
	std::string statusCode;
	std::string contentType;
	std::string contentLen;
	std::string body;
	std::string version;

	void tempMethod() {
		this->statusCode = "200 OK";
		this->contentType = "text/html; charset=utf-8";
		this->body = "<html>\n"
					 "  <body>\n"
					 "    <h1>This is a get method, isn't it?</h1>\n"
					 "  </body>\n"
					 "</html>";
	}
	void deleteMethod() {
		if (remove(request->getPathTranslated().c_str()) == 0) {
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
	void chooseMethod() {
		//в случае с редиректом добавляется поле location???
		std::string method = request->getRequestMethod();
		if (method == "DELETE") { //todo проверять на запрещенный метод???
			deleteMethod();
		}
		else if (method == "GET") {
			tempMethod();
		}

	}
	void countContentLen() {
		std::ostringstream stream;
		stream << this->body.size();
		std::string converted = stream.str();
		this->contentLen += converted;
	}
public:
	ResponseCreator(RequestParser *request) {
		//проверить допустим ли метод!!! на этапе аделины?
		this->request = request;
		this->response = "HTTP/1.1 ";
		this->version = "Version: HTTP/1.1";
		this->contentLen = "Content-Length: ";
		this->contentType = "Content-Type: ";
		this->body = "";
		chooseMethod();
		countContentLen();
		createResponse();
		std::cout << "this->contentLen str " << this->contentLen << std::endl;
	}
	ResponseCreator() { //todo доделай и сделай предохранение от нул!
		//проверить допустим ли метод!!! на этапе аделины?
		this->request = NULL;
		this->response = "HTTP/1.1 ";
		this->version = "Version: HTTP/1.1";
		this->contentLen = "Content-Length: ";
		this->contentType = "Content-Type: ";
		this->body = "";
		std::cout << "this->contentLen str " << this->contentLen << std::endl;
	}
	void start() {
		chooseMethod();
		countContentLen();
		createResponse();
	}
	void setRequest(RequestParser *request) {
		this->response = "HTTP/1.1 ";
		this->version = "Version: HTTP/1.1";
		this->contentLen = "Content-Length: ";
		this->contentType = "Content-Type: ";
		this->body = "";
		this->request = request;
		start();
	}
	char *getResponse() {
		//return "HTTP/1.1 200 OK\n"
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
