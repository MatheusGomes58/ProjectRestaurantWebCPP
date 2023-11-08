struct request
{
    std::string Cliente;
    std::vector<std::string> Produto;
    std::vector<std::string> Quantidade;
    std::string Observação;
    std::string Data;
    std::string Hora;
};

std::string listofRequests;

void addRequest(const request &request)
{
    std::fstream databaseOfRequests("./database/requests.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfRequests.is_open())
    {
        std::string Produto, Quantidade;

        for (size_t i = 0; i < request.Produto.size(); i++)
        {
            if (Produto.size() > 1)
            {
                Produto += ";";
            }
            if (Quantidade.size() >= 1)
            {
                Quantidade += ";";
            }
            Produto += request.Produto[i];
            Quantidade += request.Quantidade[i];
        }

        databaseOfRequests << request.Cliente << ","
                           << Produto << ","
                           << Quantidade << ","
                           << request.Observação << ","
                           << request.Data << ","
                           << request.Hora << "\n";

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
            std::string Cliente, Produto, Quantidade, Observação, Data, Hora;

            if (std::getline(iss, Cliente, ',') &&
                std::getline(iss, Produto, ',') &&
                std::getline(iss, Quantidade, ',') &&
                std::getline(iss, Observação, ',') &&
                std::getline(iss, Data, ',') &&
                std::getline(iss, Hora, '\n'))
            {
                // Criar um objeto JSON para representar a solicitação
                json requestJson;
                requestJson["Cliente"] = Cliente;
                requestJson["Data"] = Data;
                requestJson["Hora"] = Hora;
                std::vector<std::string> products = splitString(Produto, ';');
                std::vector<std::string> quantities = splitString(Quantidade, ';');
                requestJson["Produto"] = products;
                requestJson["Quantidade"] = quantities;
                requestJson["Observação"] = Observação;
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

        std::string Produto_requisicao, Quantidade_requisicao;

        for (size_t i = 0; i < request.Produto.size(); i++)
        {
            if (Produto_requisicao.size() > 1)
            {
                Produto_requisicao += ";";
            }
            if (Quantidade_requisicao.size() >= 1)
            {
                Quantidade_requisicao += ";";
            }
            Produto_requisicao += request.Produto[i];
            Quantidade_requisicao += request.Quantidade[i];
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
            std::string Cliente, Produto, Quantidade, Observação, Data, Hora;

            if (std::getline(iss, Cliente, ',') &&
                std::getline(iss, Produto, ',') &&
                std::getline(iss, Quantidade, ',') &&
                std::getline(iss, Observação, ',') &&
                std::getline(iss, Data, ',') &&
                std::getline(iss, Hora, ';'))
            {
                if (!(request.Cliente == Cliente &&
                      request.Data == Data &&
                      request.Hora == Hora &&
                      request.Observação == Observação &&
                      Produto_requisicao == Produto &&
                      Quantidade_requisicao == Quantidade))
                {
                    temporario << Cliente << ","
                                       << Produto << ","
                                       << Quantidade << ","
                                       << Observação << ","
                                       << Data << ","
                                       << Hora << "\n";
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
    std::fstream databaseOfRequests("./database/requests.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfRequests.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
            std::string Cliente, Produto, Quantidade, Observação, Data, Hora;

            if (std::getline(iss, Cliente, ',') &&
                std::getline(iss, Produto, ',') &&
                std::getline(iss, Quantidade, ',') &&
                std::getline(iss, Observação, ',') &&
                std::getline(iss, Data, ',') &&
                std::getline(iss, Hora, ';'))
            {

                bool match = true;

                if (oldRequest.Cliente == Cliente &&
                    oldRequest.Data == Data &&
                    oldRequest.Hora == Hora &&
                    oldRequest.Observação == Observação)
                {
                    if (oldRequest.Produto.size() == Produto.size() &&
                        oldRequest.Quantidade.size() == Quantidade.size())
                    {
                        for (size_t i = 0; i < oldRequest.Produto.size(); i++)
                        {
                            if (oldRequest.Produto[i] != Produto ||
                                oldRequest.Quantidade[i] != Quantidade)
                            {
                                match = false;
                                break;
                            }
                        }
                    }
                    else
                    {
                        match = false;
                    }
                }
                else
                {
                    match = false;
                }

                if (match)
                {
                    // Se a entrada atual for a que queremos editar, substitua pelos novos valores
                    temporario << newRequest.Cliente << ",";
                    for (size_t i = 0; i < newRequest.Produto.size(); i++)
                    {
                        temporario << newRequest.Produto[i] << ",";
                        temporario << newRequest.Quantidade[i] << ",";
                    }
                    temporario << newRequest.Observação << ","
                               << oldRequest.Data << ","
                               << oldRequest.Hora << "\n";
                    encontrado = true;
                }
                else
                {
                    // Caso contrário, mantenha a entrada Origemal
                    temporario << Cliente << ",";
                    for (size_t i = 0; i < Produto.size(); i++)
                    {
                        temporario << Produto[i] << ",";
                        temporario << Quantidade[i] << ",";
                    }
                    temporario << Observação << ","
                               << Data << ","
                               << Hora << "\n";
                }
            }
        }

        temporario.close();
        databaseOfRequests.close();
        remove("./database/requests.csv");
        rename("./database/temp.csv", "./database/requests.csv");

        if (!encontrado)
        {
            std::cout << "Solicitação não encontrada para edição." << std::endl;
        }
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
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
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