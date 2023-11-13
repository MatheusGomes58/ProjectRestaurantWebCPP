std::string createStringList(const std::vector<std::string> &observacaoList)
{
    std::string observacao;
    for (size_t i = 0; i < observacaoList.size(); i++)
    {
        if (i > 0)
        {
            observacao += ";";
        }
        observacao += observacaoList[i];
    }
    return observacao;
}

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

struct request
{
    std::string Cliente;
    std::vector<std::string> Produto;
    std::vector<std::string> Quantidade;
    std::vector<std::string> Observação;
    std::string Data;
    std::string Hora;
    std::string Preço;
    std::string Senha;
};

std::string listofRequests;

void addRequest(const request &request)
{
    std::fstream databaseOfRequests("./database/requests.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfRequests.is_open())
    {
        std::string Produto, Quantidade, Observação;
        Produto = createStringList(request.Produto);
        Quantidade = createStringList(request.Quantidade);
        Observação = createStringList(request.Observação);

        databaseOfRequests << request.Cliente << "|"
                           << Produto << "|"
                           << Quantidade << "|"
                           << Observação << "|"
                           << request.Data << "|"
                           << request.Hora << "|"
                           << request.Preço << "|"
                           << request.Senha << "\n";

        databaseOfRequests.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listRequests()
{
    std::fstream databaseOfRequests("./database/requests.csv");
    listofRequests = "";

    if (databaseOfRequests.is_open())
    {
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
            std::string Cliente, Produto, Quantidade, Observação, Data, Hora, Preco, Senha;

            if (std::getline(iss, Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, Data, '|') &&
                std::getline(iss, Hora, '|') &&
                std::getline(iss, Preco, '|') &&
                std::getline(iss, Senha, '\n'))
            {
                // Criar um objeto JSON para representar a solicitação
                json requestJson;
                requestJson["Cliente"] = Cliente;
                requestJson["Data"] = Data;
                requestJson["Hora"] = Hora;
                std::vector<std::string> products = splitString(Produto, ';');
                std::vector<std::string> quantities = splitString(Quantidade, ';');
                std::vector<std::string> observations = splitString(Observação, ';');
                requestJson["Produto"] = products;
                requestJson["Quantidade"] = quantities;
                requestJson["Observação"] = observations;
                requestJson["Preço"] = Preco;
                requestJson["Senha"] = Senha;
                requestJson["Origem"] = "request";

                // Converter o objeto JSON em uma string e adicionar à lista
                if (!listofRequests.empty())
                {
                    listofRequests += ",";
                }
                listofRequests += requestJson.dump();
            }
        }

        databaseOfRequests.close();
    }
    return;
}

void dellRequest(const request &request)
{
    std::fstream databaseOfRequests("./database/requests.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfRequests.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        std::string listProduto, listQuantidade, listObservação;
        listProduto = createStringList(request.Produto);
        listQuantidade = createStringList(request.Quantidade);
        listObservação = createStringList(request.Observação);

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
            std::string Cliente, Produto, Quantidade, Observação, Data, Hora, Preco, Senha;

            if (std::getline(iss, Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, Data, '|') &&
                std::getline(iss, Hora, '|') &&
                std::getline(iss, Preco, '|') &&
                std::getline(iss, Senha, '\n'))
            {
                if (!(request.Cliente == Cliente &&
                      request.Data == Data &&
                      request.Hora == Hora &&
                      listObservação == Observação &&
                      listProduto == Produto &&
                      listQuantidade == Quantidade &&
                      request.Preço == Preco))
                {
                    temporario << request.Cliente << "|"
                           << Produto << "|"
                           << Quantidade << "|"
                           << Observação << "|"
                           << Data << "|"
                           << Hora << "|"
                           << Preco << "|"
                           << Senha << "\n";
                }
                else
                {
                    encontrado = true;
                }
            }
        }

        temporario.close();
        databaseOfRequests.close();
        remove("./database/requests.csv");
        rename("./database/temp.csv", "./database/requests.csv");

        if (!encontrado)
        {
            std::cout << "Pedido não encontrado." << std::endl;
        }
    }
}

void editRequest(const request &oldRequest, const request &newRequest)
{
    std::fstream databaseOfRequests("./database/requests.csv", std::ios::in);
    if (!databaseOfRequests.is_open())
    {
        std::cout << "Erro ao abrir o arquivo de solicitações." << std::endl;
        return;
    }

    std::fstream temporario("./database/temp.csv", std::ios::out);
    if (!temporario.is_open())
    {
        std::cout << "Erro ao criar arquivo temporário." << std::endl;
        databaseOfRequests.close();
        return;
    }

    std::string listProduto, listQuantidade, listObservação, novoProduto, novaQuantidade, novaObservação;
    listProduto = createStringList(oldRequest.Produto);
    listQuantidade = createStringList(oldRequest.Quantidade);
    listObservação = createStringList(oldRequest.Observação);
    novoProduto = createStringList(newRequest.Produto);
    novaQuantidade = createStringList(newRequest.Quantidade);
    novaObservação = createStringList(newRequest.Observação);

    bool encontrado = false;
    std::string line;
    while (std::getline(databaseOfRequests, line))
    {
        std::istringstream iss(line);
        std::string Cliente, Produto, Quantidade, Observação, Data, Hora;

        if (std::getline(iss, Cliente, '|') &&
            std::getline(iss, Produto, '|') &&
            std::getline(iss, Quantidade, '|') &&
            std::getline(iss, Observação, '|') &&
            std::getline(iss, Data, '|') &&
            std::getline(iss, Hora, '\n'))
        {
            if (oldRequest.Cliente == Cliente &&
                oldRequest.Data == Data &&
                oldRequest.Hora == Hora &&
                listObservação == Observação &&
                listProduto == Produto &&
                listQuantidade == Quantidade)
            {
                encontrado = true;
                temporario << newRequest.Cliente << "|"
                           << novoProduto << "|"
                           << novaQuantidade << "|"
                           << novaObservação << "|"
                           << oldRequest.Data << "|"
                           << oldRequest.Hora << "\n";
            }
            else
            {
                temporario << Cliente << "|"
                           << Produto << "|"
                           << Quantidade << "|"
                           << Observação << "|"
                           << Data << "|"
                           << Hora << "\n";
            }
        }
    }

    databaseOfRequests.close();
    temporario.close();

    if (!encontrado)
    {
        std::cout << "Solicitação não encontrada para edição." << std::endl;
        remove("./database/temp.csv"); // Remove o arquivo temporário, pois não foi usado.
    }
    else
    {
        remove("./database/requests.csv"); // Remove o arquivo original.
        rename("./database/temp.csv", "./database/requests.csv"); // Renomeia o arquivo temporário.
    }
}


request requestMap(json jsonResponse, std::string premissa)
{
    request request;
    if (jsonResponse.contains(premissa + "Cliente"))
    {
        request.Cliente = jsonResponse[premissa + "Cliente"];
    }
    if (jsonResponse.contains(premissa + "Observação"))
    {
        request.Observação = jsonResponse[premissa + "Observação"];
    }
    if (jsonResponse.contains(premissa + "Produto"))
    {
        request.Produto = jsonResponse[premissa + "Produto"];
    }
    if (jsonResponse.contains(premissa + "Data"))
    {
        request.Data = jsonResponse[premissa + "Data"];
    }
    if (jsonResponse.contains(premissa + "Hora"))
    {
        request.Hora = jsonResponse[premissa + "Hora"];
    }
    if (jsonResponse.contains(premissa + "Quantidade"))
    {
        request.Quantidade = jsonResponse[premissa + "Quantidade"];
    }
    return request;
}

void saveRequest(json jsonResponse)
{
    request newRequest = requestMap(jsonResponse, "");

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    newRequest.Data = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", localTime);
    newRequest.Hora = timeBuffer;

    addRequest(newRequest);
    return;
}

void deleteRequest(json jsonResponse)
{
    request deleteRequest = requestMap(jsonResponse, "");
    dellRequest(deleteRequest);
    return;
}

void updateRequest(json jsonResponse)
{
    request oldRequest = requestMap(jsonResponse, "old_");
    request UpdateRequest = requestMap(jsonResponse, "");
    editRequest(oldRequest, UpdateRequest);
    return;
}