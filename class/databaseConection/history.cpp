struct history
{
    std::string Cliente;
    std::vector<std::string> Produto;
    std::vector<std::string> Quantidade;
    std::vector<std::string> Observação;
    std::string Data;
    std::string Hora;
};

std::string listofHistorys;

void addHistory(const history &history)
{
    std::fstream databaseOfHistorys("./database/historys.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfHistorys.is_open())
    {
        std::string Produto, Quantidade, Observação;
        Produto = createStringList(history.Produto);
        Quantidade = createStringList(history.Quantidade);
        Observação = createStringList(history.Observação);

        databaseOfHistorys << history.Cliente << "|"
                           << Produto << "|"
                           << Quantidade << "|"
                           << Observação << "|"
                           << history.Data << "|"
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

            if (std::getline(iss, Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, Data, '|') &&
                (std::getline(iss, Hora, '\n')||""))
            {
                // Criar um objeto JSON para representar a solicitação
                json historyJson;
                historyJson["Cliente"] = Cliente;
                historyJson["Data"] = Data;
                historyJson["Hora"] = Hora;
                std::vector<std::string> products = splitString(Produto, ';');
                std::vector<std::string> quantities = splitString(Quantidade, ';');
                std::vector<std::string> observations = splitString(Observação, ';');
                historyJson["Produto"] = products;
                historyJson["Quantidade"] = quantities;
                historyJson["Observação"] = observations;
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

void dellHistory(const history &history)
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

        std::string listProduto, listQuantidade, listObservação;
        listProduto = createStringList(history.Produto);
        listQuantidade = createStringList(history.Quantidade);
        listObservação = createStringList(history.Observação);

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfHistorys, line))
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
                if (!(history.Cliente == Cliente &&
                      history.Data == Data &&
                      history.Hora == Hora &&
                      listObservação == Observação &&
                      listProduto == Produto &&
                      listQuantidade == Quantidade))
                {
                    temporario << Cliente << "|"
                               << Produto << "|"
                               << Quantidade << "|"
                               << Observação << "|"
                               << Data << "|"
                               << Hora << "\n";
                }
                else
                {
                    encontrado = true;
                }
            }
        }

        temporario.close();
        databaseOfHistorys.close();
        remove("./database/historys.csv");
        rename("./database/temp.csv", "./database/historys.csv");

        if (!encontrado)
        {
            std::cout << "Pedido não encontrado." << std::endl;
        }
    }
}

void editHistory(const history &oldHistory, const history &newHistory)
{
    std::fstream databaseOfHistorys("./database/historys.csv", std::ios::in);
    if (!databaseOfHistorys.is_open())
    {
        std::cout << "Erro ao abrir o arquivo de solicitações." << std::endl;
        return;
    }

    std::fstream temporario("./database/temp.csv", std::ios::out);
    if (!temporario.is_open())
    {
        std::cout << "Erro ao criar arquivo temporário." << std::endl;
        databaseOfHistorys.close();
        return;
    }

    std::string listProduto, listQuantidade, listObservação, novoProduto, novaQuantidade, novaObservação;
    listProduto = createStringList(oldHistory.Produto);
    listQuantidade = createStringList(oldHistory.Quantidade);
    listObservação = createStringList(oldHistory.Observação);
    novoProduto = createStringList(newHistory.Produto);
    novaQuantidade = createStringList(newHistory.Quantidade);
    novaObservação = createStringList(newHistory.Observação);

    bool encontrado = false;
    std::string line;
    while (std::getline(databaseOfHistorys, line))
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
            if (oldHistory.Cliente == Cliente &&
                oldHistory.Data == Data &&
                oldHistory.Hora == Hora &&
                listObservação == Observação &&
                listProduto == Produto &&
                listQuantidade == Quantidade)
            {
                encontrado = true;
                temporario << newHistory.Cliente << "|"
                           << novoProduto << "|"
                           << novaQuantidade << "|"
                           << novaObservação << "|"
                           << oldHistory.Data << "|"
                           << oldHistory.Hora << "\n";
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

    databaseOfHistorys.close();
    temporario.close();

    if (!encontrado)
    {
        std::cout << "Solicitação não encontrada para edição." << std::endl;
        remove("./database/temp.csv"); // Remove o arquivo temporário, pois não foi usado.
    }
    else
    {
        remove("./database/historys.csv"); // Remove o arquivo original.
        rename("./database/temp.csv", "./database/historys.csv"); // Renomeia o arquivo temporário.
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

void deleteHistory(json jsonResponse)
{
    history deleteHistory = historyMap(jsonResponse, "");
    dellHistory(deleteHistory);
    return;
}

void updateHistory(json jsonResponse)
{
    history oldHistory = historyMap(jsonResponse, "old_");
    history UpdateHistory = historyMap(jsonResponse, "");
    editHistory(oldHistory, UpdateHistory);
    return;
}