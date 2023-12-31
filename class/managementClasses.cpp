#include "../libraries/json.hpp"
using json = nlohmann::json;
using namespace std;

/*****************************************************
 * Classes de controle das estruturas
 *****************************************************/
#include "./dataManagement/structs.cpp"

/*****************************************************
 * Classes de controle e gestão de senhas
 *****************************************************/
#include "./queueOfCostumers/password.cpp"

/*****************************************************
 * Classes de configuração e conexão banco de dados
 *****************************************************/
#include "./databaseConection/search.cpp"
#include "./databaseConection/products.cpp"
#include "./databaseConection/clients.cpp"
#include "./databaseConection/history.cpp"
#include "./databaseConection/requests.cpp"

/*****************************************************
 * Classes de configuração e conexão com host
 *****************************************************/
#include "./host/htmlReader.cpp"
#include "./host/configHostConectio.cpp"