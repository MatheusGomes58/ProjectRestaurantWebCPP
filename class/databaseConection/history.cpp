struct history
{
    std::string client_history;
    std::string product_history;
    std::string quantity_history;
    std::string observations_history;
    std::string date_history;
    std::string hour_history;
};
std::string listofHistory;

void addHistory(const history &history)
{
    std::fstream databaseOfHistory("./database/history.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfHistory.is_open())
    {
        databaseOfHistory << history.client_history << "," << history.product_history << ","
                           << history.quantity_history << "," << history.observations_history << ","
                           << history.date_history << "," << history.hour_history << "\n";
        databaseOfHistory.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listHistory()
{
    std::fstream databaseOfHistory("./database/history.csv");
    listofHistory = "";
    if (databaseOfHistory.is_open())
    {
        std::string line;
        while (std::getline(databaseOfHistory, line))
        {
            std::istringstream iss(line);
            std::string client_history, product_history, quantity_history, observations_history, date_history, hour_history;

            if (std::getline(iss, client_history, ',') &&
                std::getline(iss, product_history, ',') &&
                std::getline(iss, quantity_history, ',') &&
                std::getline(iss, observations_history, ',') &&
                std::getline(iss, date_history, ',') &&
                std::getline(iss, hour_history, ';'))
            {
                if (!listofHistory.empty())
                {
                    listofHistory += ",";
                }
                listofHistory += "{\"date_history\":\"" + date_history + "\",\"hour_history\":\"" + hour_history + "\",\"client_history\":\"" + client_history + "\",\"product_history\":\"" + product_history + "\",\"quantity_history\":\"" + quantity_history + "\",\"observations_history\":\"" + observations_history + "\",\"origin\":\"history\"}";
            }
        }
        databaseOfHistory.close();
    }
    return;
}

void editHistory(const history &oldHistory, const history &newHistory)
{
    std::fstream databaseOfHistory("./database/history.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfHistory.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfHistory, line))
        {
            std::istringstream iss(line);
            std::string client_history, product_history, quantity_history, observations_history, date_history, hour_history;

            if (std::getline(iss, client_history, ',') &&
                std::getline(iss, product_history, ',') &&
                std::getline(iss, quantity_history, ',') &&
                std::getline(iss, observations_history, ',') &&
                std::getline(iss, date_history, ',') &&
                std::getline(iss, hour_history, ';'))
            {

                if (oldHistory.client_history == client_history &&
                    oldHistory.date_history == date_history &&
                    oldHistory.hour_history == hour_history &&
                    oldHistory.observations_history == observations_history &&
                    oldHistory.product_history == product_history &&
                    oldHistory.quantity_history == quantity_history)
                {
                    // Se a entrada atual for a que queremos editar, substitua pelos novos valores
                    temporario << newHistory.client_history << "," << newHistory.product_history << ","
                               << newHistory.quantity_history << "," << newHistory.observations_history << ","
                               << oldHistory.date_history << "," << oldHistory.hour_history << "\n";
                    encontrado = true;
                }
                else
                {
                    // Caso contrário, mantenha a entrada original
                    temporario << client_history << "," << product_history << ","
                               << quantity_history << "," << observations_history << ","
                               << date_history << "," << hour_history << "\n";
                }
            }
        }

        temporario.close();
        databaseOfHistory.close();
        remove("./database/history.csv");
        rename("./database/temp.csv", "./database/history.csv");

        if (!encontrado)
        {
            std::cout << "Solicitação não encontrada para edição." << std::endl;
        }
    }
}

history historyMap(json jsonResponse, std::string premissa)
{
    history history;
    if (jsonResponse.contains(premissa + "client_request"))
    {
        history.client_history = jsonResponse[premissa + "client_request"];
    }
    if (jsonResponse.contains(premissa + "observations_request"))
    {
        history.observations_history = jsonResponse[premissa + "observations_request"];
    }
    if (jsonResponse.contains(premissa + "product_request"))
    {
        history.product_history = jsonResponse[premissa + "product_request"];
    }
    if (jsonResponse.contains(premissa + "date_request"))
    {
        history.date_history = jsonResponse[premissa + "date_request"];
    }
    if (jsonResponse.contains(premissa + "hour_request"))
    {
        history.hour_history = jsonResponse[premissa + "hour_request"];
    }
    if (jsonResponse.contains(premissa + "quantity_request"))
    {
        history.quantity_history = jsonResponse[premissa + "quantity_request"];
    }
    return history;
}

void saveHistory(json jsonResponse)
{
    history newHistory = historyMap(jsonResponse,"");

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    newHistory.date_history = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    newHistory.hour_history = timeBuffer;

    addHistory(newHistory);
    return;
}

void updateHistory(json jsonResponse)
{
    history oldHistory = historyMap(jsonResponse, "old_");
    history UpdateHistory = historyMap(jsonResponse, "");
    editHistory(oldHistory,UpdateHistory);
    return;
}