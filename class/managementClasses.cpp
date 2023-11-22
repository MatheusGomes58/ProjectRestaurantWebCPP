#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

#include "../libraries/json.hpp"
using json = nlohmann::json;
using namespace std;

/*****************************************************
 * Classes de controle e gestão de senhas
 *****************************************************/
#include "./queueOfCostumers/password.cpp"

/*****************************************************
 * Classes de configuração e conexão banco de dados
 *****************************************************/
#include "./databaseConection/requests.cpp"
#include "./databaseConection/products.cpp"
#include "./databaseConection/clients.cpp"
#include "./databaseConection/history.cpp"

/*****************************************************
 * Classes de configuração e conexão com host
 *****************************************************/
#include "./host/htmlReader.cpp"
#include "./host/configHostConectio.cpp"