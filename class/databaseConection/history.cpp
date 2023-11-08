struct history
{
    std::string Cliente;
    std::vector<std::string> Produto;
    std::vector<std::string> Quantidade;
    std::string Observação;
    std::string Data;
    std::string Hora;
};

std::string listofHistorys;

void addHistory(const history &history)
{
    std::fstream databaseOfHistorys("./database/historys.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfHistorys.is_open())
    {
        std::string Produto, Quantidade;

        for (size_t i = 0; i < history.Produto.size(); i++)
        {
            if (Produto.size() > 1)
            {
                Produto += ";";
            }
            if (Quantidade.size() >= 1)
            {
                Quantidade += ";";
            }
            Produto += history.Produto[i];
            Quantidade += history.Quantidade[i];
        }

        databaseOfHistorys << history.Cliente << ","
                           << Produto << ","
                           << Quantidade << ","
                           << history.Observação << ","
                           << history.Data << ","
                           << history.Hora << "\n";

        databaseOfHistorys.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listHistorys()
{
    std::fstream databaseOfHistorys("./database/historys.csv");
    listofHistorys = "";

    if (databaseOfHistorys.is_open())
    {
        std::string line;
        while (std::getline(databaseOfHistorys, line))
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
                json historyJson;
                historyJson["Cliente"] = Cliente;
                historyJson["Data"] = Data;
                historyJson["Hora"] = Hora;
                std::vector<std::string> products = splitString(Produto, ';');
                std::vector<std::string> quantities = splitString(Quantidade, ';');
                historyJson["Produto"] = products;
                historyJson["Quantidade"] = quantities;
                historyJson["Observação"] = Observação;
                historyJson["Origem"] = "history";

                // Converter o objeto JSON em uma string e adicionar à lista
                if (!listofHistorys.empty())
                {
                    listofHistorys += ",";
                }
                listofHistorys += historyJson.dump();
            }
        }

        databaseOfHistorys.close();
    }
    return;
}

void editHistory(const history &oldHistory, const history &newHistory)
{
    std::fstream databaseOfHistorys("./database/historys.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfHistorys.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfHistorys, line))
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

                if (oldHistory.Cliente == Cliente &&
                    oldHistory.Data == Data &&
                    oldHistory.Hora == Hora &&
                    oldHistory.Observação == Observação)
                {
                    if (oldHistory.Produto.size() == Produto.size() &&
                        oldHistory.Quantidade.size() == Quantidade.size())
                    {
                        for (size_t i = 0; i < oldHistory.Produto.size(); i++)
                        {
                            if (oldHistory.Produto[i] != Produto ||
                                oldHistory.Quantidade[i] != Quantidade)
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
                    temporario << newHistory.Cliente << ",";
                    for (size_t i = 0; i < newHistory.Produto.size(); i++)
                    {
                        temporario << newHistory.Produto[i] << ",";
                        temporario << newHistory.Quantidade[i] << ",";
                    }
                    temporario << newHistory.Observação << ","
                               << oldHistory.Data << ","
                               << oldHistory.Hora << "\n";
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
        databaseOfHistorys.close();
        remove("./database/historys.csv");
        rename("./database/temp.csv", "./database/historys.csv");

        if (!encontrado)
        {
            std::cout << "Solicitação não encontrada para edição." << std::endl;
        }
    }
}

history historyMap(json jsonResponse, std::string premissa)
{
    history history;
    if (jsonResponse.contains(premissa + "Cliente"))
    {
        history.Cliente = jsonResponse[premissa + "Cliente"];
    }
    if (jsonResponse.contains(premissa + "Observação"))
    {
        history.Observação = jsonResponse[premissa + "Observação"];
    }
    if (jsonResponse.contains(premissa + "Produto"))
    {
        history.Produto = jsonResponse[premissa + "Produto"];
    }
    if (jsonResponse.contains(premissa + "Data"))
    {
        history.Data = jsonResponse[premissa + "Data"];
    }
    if (jsonResponse.contains(premissa + "Hora"))
    {
        history.Hora = jsonResponse[premissa + "Hora"];
    }
    if (jsonResponse.contains(premissa + "Quantidade"))
    {
        history.Quantidade = jsonResponse[premissa + "Quantidade"];
    }
    return history;
}

void saveHistory(json jsonResponse)
{
    history newHistory = historyMap(jsonResponse, "");

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    newHistory.Data = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    newHistory.Hora = timeBuffer;

    addHistory(newHistory);
    return;
}

void updateHistory(json jsonResponse)
{
    history oldHistory = historyMap(jsonResponse, "old_");
    history UpdateHistory = historyMap(jsonResponse, "");
    editHistory(oldHistory, UpdateHistory);
    return;
}