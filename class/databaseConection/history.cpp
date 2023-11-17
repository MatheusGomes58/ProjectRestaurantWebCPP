struct history
{
    std::string Cliente;
    std::vector<std::string> Produto;
    std::vector<std::string> Quantidade;
    std::vector<std::string> Observação;
    std::string Data;
    std::string Hora;
    std::string Preco;
    std::string Senha;
    std::string Atendimento;
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
                           << history.Hora << "|"
                           << history.Preco << "|"
                           << history.Senha << "|"
                           << history.Atendimento << "\n";

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
            history history;
            std::string Produto, Quantidade, Observação;

            if (std::getline(iss, history.Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, history.Data, '|') &&
                std::getline(iss, history.Hora, '|') &&
                std::getline(iss, history.Preco, '|') &&
                std::getline(iss, history.Senha, '|') &&
                std::getline(iss, history.Atendimento, '\n'))
            {
                // Criar um objeto JSON para representar a solicitação
                json historyJson;
                historyJson["Cliente"] = history.Cliente;
                historyJson["Data"] = history.Data;
                historyJson["Hora"] = history.Hora;
                history.Produto = splitString(Produto, ';');
                history.Quantidade = splitString(Quantidade, ';');
                history.Observação = splitString(Observação, ';');
                historyJson["Produto"] = history.Produto;
                historyJson["Quantidade"] = history.Quantidade;
                historyJson["Observação"] = history.Observação;
                historyJson["Preço"] = history.Preco;
                historyJson["Senha"] = history.Senha;
                historyJson["Atendimento"] = history.Atendimento;
                historyJson["Origem"] = "history";
                

                // Verifique se a string 'history.Senha' é válida antes de converter para int
                if (!history.Senha.empty()) {
                    try {
                        int senhaAtualValue = std::stoi(history.Senha);
                        if (senhaAtualValue > programSenha) {
                            programSenha = senhaAtualValue;
                        }
                    } catch (const std::invalid_argument& ia) {
                        std::cerr << "Erro de argumento inválido: " << ia.what() << std::endl;
                    } catch (const std::out_of_range& oor) {
                        std::cerr << "Erro de estouro: " << oor.what() << std::endl;
                    }
                } 

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
    if (jsonResponse.contains(premissa + "Preço"))
    {
        history.Preco = jsonResponse[premissa + "Preço"];
    }
    if (jsonResponse.contains(premissa + "Senha"))
    {
        history.Senha = jsonResponse[premissa + "Senha"];
    }
    if (jsonResponse.contains(premissa + "Atendimento"))
    {
        history.Atendimento = jsonResponse[premissa + "Atendimento"];
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
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", localTime);
    newHistory.Hora = timeBuffer;

    newHistory.Senha = std::to_string(programSenha + 1);

    addHistory(newHistory);
    return;
}