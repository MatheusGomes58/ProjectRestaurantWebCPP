struct request
{
    std::string client_request;
    std::string product_request;
    std::string quantity_request;
    std::string observations_request;
    std::string date_request;
    std::string hour_request;
};
std::string listofRequests;

void addRequest(const request &request)
{
    std::fstream databaseOfRequests("./database/requests.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfRequests.is_open())
    {
        databaseOfRequests << request.client_request << "," << request.product_request << ","
                           << request.quantity_request << "," << request.observations_request << ","
                           << request.date_request << "," << request.hour_request << "\n";
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
            std::string client_request, product_request, quantity_request, observations_request, date_request, hour_request;

            if (std::getline(iss, client_request, ',') &&
                std::getline(iss, product_request, ',') &&
                std::getline(iss, quantity_request, ',') &&
                std::getline(iss, observations_request, ',') &&
                std::getline(iss, date_request, ',') &&
                std::getline(iss, hour_request, ';'))
            {
                if (!listofRequests.empty())
                {
                    listofRequests += ",";
                }
                listofRequests += "{\"date_request\":\"" + date_request + "\",\"hour_request\":\"" + hour_request + "\",\"client_request\":\"" + client_request + "\",\"product_request\":\"" + product_request + "\",\"quantity_request\":\"" + quantity_request + "\",\"observations_request\":\"" + observations_request + "\",\"origin\":\"request\"}";
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

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
            std::string client_request, product_request, quantity_request, observations_request, date_request, hour_request;

            if (std::getline(iss, client_request, ',') &&
                std::getline(iss, product_request, ',') &&
                std::getline(iss, quantity_request, ',') &&
                std::getline(iss, observations_request, ',') &&
                std::getline(iss, date_request, ',') &&
                std::getline(iss, hour_request, ';'))
            {

                if (!(request.client_request == client_request &&
                      request.date_request == date_request &&
                      request.hour_request == hour_request &&
                      request.observations_request == observations_request &&
                      request.product_request == product_request &&
                      request.quantity_request == quantity_request))
                {
                    temporario << client_request << "," << product_request << ","
                               << quantity_request << "," << observations_request << ","
                               << date_request << "," << hour_request << "\n";
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
            std::cout << "Contato não encontrado." << std::endl;
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
            std::string client_request, product_request, quantity_request, observations_request, date_request, hour_request;

            if (std::getline(iss, client_request, ',') &&
                std::getline(iss, product_request, ',') &&
                std::getline(iss, quantity_request, ',') &&
                std::getline(iss, observations_request, ',') &&
                std::getline(iss, date_request, ',') &&
                std::getline(iss, hour_request, ';'))
            {

                if (oldRequest.client_request == client_request &&
                    oldRequest.date_request == date_request &&
                    oldRequest.hour_request == hour_request &&
                    oldRequest.observations_request == observations_request &&
                    oldRequest.product_request == product_request &&
                    oldRequest.quantity_request == quantity_request)
                {
                    // Se a entrada atual for a que queremos editar, substitua pelos novos valores
                    temporario << newRequest.client_request << "," << newRequest.product_request << ","
                               << newRequest.quantity_request << "," << newRequest.observations_request << ","
                               << oldRequest.date_request << "," << oldRequest.hour_request << "\n";
                    encontrado = true;
                }
                else
                {
                    // Caso contrário, mantenha a entrada original
                    temporario << client_request << "," << product_request << ","
                               << quantity_request << "," << observations_request << ","
                               << date_request << "," << hour_request << "\n";
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
    if (jsonResponse.contains(premissa + "client_request"))
    {
        request.client_request = jsonResponse[premissa + "client_request"];
    }
    if (jsonResponse.contains(premissa + "observations_request"))
    {
        request.observations_request = jsonResponse[premissa + "observations_request"];
    }
    if (jsonResponse.contains(premissa + "product_request"))
    {
        request.product_request = jsonResponse[premissa + "product_request"];
    }
    if (jsonResponse.contains(premissa + "date_request"))
    {
        request.date_request = jsonResponse[premissa + "date_request"];
    }
    if (jsonResponse.contains(premissa + "hour_request"))
    {
        request.hour_request = jsonResponse[premissa + "hour_request"];
    }
    if (jsonResponse.contains(premissa + "quantity_request"))
    {
        request.quantity_request = jsonResponse[premissa + "quantity_request"];
    }
    return request;
}

void saveRequest(json jsonResponse)
{
    request newRequest = requestMap(jsonResponse,"");

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    newRequest.date_request = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    newRequest.hour_request = timeBuffer;

    addRequest(newRequest);
    return;
}

void deleteRequest(json jsonResponse)
{
    request deleteRequest = requestMap(jsonResponse,"");
    dellRequest(deleteRequest);
    return;
}

void updateRequest(json jsonResponse)
{
    request oldRequest = requestMap(jsonResponse, "old_");
    request UpdateRequest = requestMap(jsonResponse, "");
    editRequest(oldRequest,UpdateRequest);
    return;
}