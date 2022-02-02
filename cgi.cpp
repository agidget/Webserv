#include "cgi.hpp"

void	errorMsg()
{
	std::cerr << "Error with the operation\n";
	exit(EXIT_FAILURE);
}

char**	cgi::mapToAr()
{
	char**	arr;
	int		i;

	arr = new char* [_metaVars.size() + 1];
	i = 0;
	for (cgiMap::iterator it = _metaVars.begin(); it != _metaVars.end(); it++)
	{
		std::string str = it->first + "=" + it->second;
		arr[i] = new char [str.length() + 1];
		std::strcpy(arr[i], str.c_str());
		i++;
	}
	arr[i] = NULL;
	return arr;
}

void	cgi::execute()
{
	// int		savedFd[2];
	pid_t	pid;
	int		inFd;
	int		outFd;
	int		ret;
	char**	args;
	char	buf[CGI_BUFSIZE];

	if ((inFd = open("in", O_RDWR | O_CREAT, 0644)) == -1)
		errorMsg(); 
	if ((outFd = open("out", O_RDWR | O_CREAT, 0644)) == -1)
		errorMsg();
	write(inFd, _input.c_str(), _input.length());
	// savedFd[0] = dup(0);
	// savedFd[1] = dup(1);
	pid = fork();
	if (pid < 0)
		errorMsg();
	if (pid == 0)
	{
		dup2(inFd, 0);
		dup2(outFd, 1);
		args = new char* [3];
		args[0] = new char [_bin.length() + 1];
		args[1] = new char [_path.length() + 1];
		std::strcpy(args[0], _bin.c_str());
		std::strcpy(args[1], _path.c_str());
		args[2] = NULL;
		if (execve(_bin.c_str(), args, mapToAr()) == -1)
		{
			for (int i = 0; i < 3; i++)
				delete args[i];
			delete args;
			errorMsg();
		}
	}
	waitpid(pid, NULL, 0);
	ret = 1;
	while (ret)
	{
		bzero(buf, CGI_BUFSIZE);
		ret = read(outFd, buf, CGI_BUFSIZE);
		_output += buf;
	}
	close(inFd);
	close(outFd);
	unlink("in");
	unlink("out");
}

// void	cgi::fillMetaVars(RequestParser req, Config con)
// {
// 	_metaVars["SERVER_SOFTWARE"] = "webserv";
// 	_metaVars["SERVER_NAME"] = "127.0.0.1";//req.getServerName();
// 	_metaVars["GATEWAY_INTERFACE"] = "CGI/1.1";
// 	_metaVars["SERVER_PROTOCOL"] = "HTTP/1.1";//req.getServerProtocol();
// 	_metaVars["SERVER_PORT"] = "8080";//req.getServerPort();
// 	_metaVars["REQUEST_METHOD"] = "POST";req.getRequestMethod();
// 	// _metaVars["PATH_INFO"] = "/add.cgi";//req.getPathInfo();
// 	//a filesystem path - root from config file (absolute path)
// 	_metaVars["PATH_TRANSLATED"] = "/Users/agidget/www";//con.getRoot();
// 	//relative path to my program (php or py) (from root)
// 	_metaVars["SCRIPT_NAME"] = "/add.cgi";//con.getPath();
// 	_metaVars["QUERY_STRING"] = "num1=7&num2=8";//req.getQueryString();
// 	_metaVars["REMOTE_ADDR"] = "127.0.0.1";//req.getRemoteAddr();
// 	// _metaVars["REMOTE_USER"] = ;
// 	// _metaVars["REMOTE_IDENT"] = ;
// 	_metaVars["CONTENT_TYPE"] = "application/x-www-form-urlencoded";//req.getContentType();
// 	_metaVars["CONTENT_LENGTH"] = "13";//std::to_string(req.getContentLength());
// 	_metaVars["HTTP_ACCEPT"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9";//req.getAccept();
// 	_metaVars["HTTP_ACCEPT_ENCODING"] = "gzip, deflate, br";//req.getAcceptEncoding();
// 	_metaVars["HTTP_ACCEPT_LANGUAGE"] = "en-US,en;q=0.9";//req.getAcceptLanguage();
// 	_metaVars["HTTP_CONNECTION"] = "keep-alive";//req.getConnection();
// 	_metaVars["HTTP_USER_AGENT"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36";//req.getUserAgent();
// 	// _metaVars["HTTP_HOST"] = //req.getHost();
// 	_metaVars["HTTP_SEC_FETCH_DEST"] = "document";//req.getSecFetchDest();
// 	_metaVars["HTTP_SEC_FETCH_MODE"] = "navigate";//req.getSecFetchMode();
// 	_metaVars["HTTP_SEC_FETCH_SITE"] = "same-origin";//req.getSecFetchSite();
// }

std::string	cgi::getOutput()
{
	return _output;
}