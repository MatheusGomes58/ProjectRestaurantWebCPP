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

#include "../libraries/json.hpp"
using json = nlohmann::json;

// Função auxiliar para dividir uma string com um delimitador
std::vector<std::string> splitString(const std::string &input, char delimiter)
{
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, delimiter))
    {
        result.push_back(token);
    }
    return result;
}

/*****************************************************
 * Classes de configuração e conexão banco de dados
 *****************************************************/
#include "./databaseConection/requests.cpp"
#include "./databaseConection/plates.cpp"
#include "./databaseConection/clients.cpp"
#include "./databaseConection/history.cpp"


/*****************************************************
 * Classes de configuração e conexão com host
 *****************************************************/
#include "./host/htmlReader.cpp"
#include "./host/configHostConectio.cpp"