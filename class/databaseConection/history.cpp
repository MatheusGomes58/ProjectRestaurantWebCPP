// Função para adicionar uma entrada histórica ao arquivo CSV
void addHistory(const request &history, std::string action)
{
    std::fstream databaseOfHistorys("./database/historys.csv", std::ios::app | std::ios::in | std::ios::out);

    if (databaseOfHistorys.is_open())
    {
        std::string Produto, Quantidade, Observação;
        Produto = createStringList(history.Produto);
        Quantidade = createStringList(history.Quantidade);
        Observação = createStringList(history.Observação);

        // Escrever a entrada histórica no arquivo CSV
        databaseOfHistorys << history.Cliente << "|"
                           << Produto << "|"
                           << Quantidade << "|"
                           << Observação << "|"
                           << history.Data << "|"
                           << history.Hora << "|"
                           << history.Preco << "|"
                           << history.Senha << "|"
                           << history.Atendimento << "|"
                           << action << "\n";

        databaseOfHistorys.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

// Função para listar as entradas históricas com base em um valor de pesquisa
void listHistorys(std::string &searchValueData)
{
    std::fstream databaseOfHistorys("./database/historys.csv");
    listofHistorys = "";
    std::string senhaEncontrada;

    if (databaseOfHistorys.is_open())
    {
        std::string line;
        while (std::getline(databaseOfHistorys, line))
        {
            std::istringstream iss(line);
            request history;
            std::string Produto, Quantidade, Observação, action;

            // Extrair informações da linha CSV e preencher a estrutura 'history'
            if (std::getline(iss, history.Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, history.Data, '|') &&
                std::getline(iss, history.Hora, '|') &&
                std::getline(iss, history.Preco, '|') &&
                std::getline(iss, history.Senha, '|') &&
                std::getline(iss, history.Atendimento, '|') &&
                std::getline(iss, action, '\n'))
            {
                // Criar um objeto JSON para representar a entrada histórica
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
                historyJson["Ação"] = action;
                historyJson["Origem"] = "history";

                // Verifique se a string 'history.Senha' é válida antes de converter para int
                if (!history.Senha.empty())
                {
                    int senhaAtualValue = std::stoi(history.Senha);
                    if (senhaAtualValue > programSenha)
                    {
                        programSenha = senhaAtualValue;
                    }
                }

                // Verificar se a entrada atende aos critérios de pesquisa e adicionar à lista
                if (searchValueData == "" ||
                    history.Cliente == searchValueData ||
                    history.Data.find(searchValueData) != std::string::npos ||
                    history.Hora.find(searchValueData) != std::string::npos ||
                    Produto.find(searchValueData) != std::string::npos ||
                    Observação.find(searchValueData) != std::string::npos ||
                    history.Preco == searchValueData ||
                    history.Senha == searchValueData ||
                    history.Atendimento.find(searchValueData) != std::string::npos)
                {
                    if (!listofHistorys.empty())
                    {
                        listofHistorys += ",";
                    }
                    listofHistorys += historyJson.dump();
                }
            }
        }
        databaseOfHistorys.close();
    }
    return;
}