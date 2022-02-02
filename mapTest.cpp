//
// Created by Gaynelle Flores on 1/27/22.
//

#include "Server.h"
#include "ClientSocket.h"
#include <map>
#include "tempheader.h"

int main() {
	//remove test
	std::cout << remove("./deletTest.txt") << std::endl;

	//map test
//	char *response = new char[500];
//	int *u = new int[900];
//	std::map<int, ClientSocket *> myMap;
//	ClientSocket *cl = new ClientSocket();
//	std::pair<int, ClientSocket *> lol = std::make_pair(1, cl);
//	myMap.insert(lol);
//	myMap.insert(std::make_pair(2, new ClientSocket()));
//	for(std::map<int, ClientSocket *>::const_iterator it = myMap.cbegin(); it != myMap.cend(); ++it)
//	{
//		std::cout << it->first << " " << it->second->returnIndex(2) << " " << "\n";
//	}
//	std::cout <<  myMap.at(2)->returnIndex(3) << " " << "\n";
//
//	delete []response;
//	delete []u;
//	while(1){}
}

