// Definição da estrutura 'history' para armazenar informações históricas
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
std::string searchValue;

// Função para adicionar uma entrada histórica ao arquivo CSV
void addHistory(const history &history)
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
                           << history.Atendimento << "\n";

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
            history history;
            std::string Produto, Quantidade, Observação;

            // Extrair informações da linha CSV e preencher a estrutura 'history'
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

                // Verificar se a senha encontrada é igual ao valor de pesquisa
                if (history.Senha == searchValueData)
                {
                    senhaEncontrada = historyJson.dump();
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

        // Se uma senha específica foi encontrada, a lista é atualizada apenas com essa entrada
        if (senhaEncontrada != "")
        {
            listofHistorys = senhaEncontrada;
        }

        databaseOfHistorys.close();
    }
    return;
}

// Mapear um objeto JSON para a estrutura 'history'
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

// Função para salvar uma nova entrada histórica com base em um objeto JSON
void saveHistory(json jsonResponse)
{
    history newHistory = historyMap(jsonResponse, "");

    // Obter a data e hora atuais
    time_t now = time(0);
    tm *localTime = localtime(&now);

    // Formatar a data e hora e atribuir à nova entrada histórica
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    newHistory.Data = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", localTime);
    newHistory.Hora = timeBuffer;

    
    newHistory.Senha = std::to_string(programSenha + 1);

    newHistory.Senha = std::string(3 -  newHistory.Senha.length(), '0') + newHistory.Senha;

    // Adicionar a nova entrada histórica ao arquivo CSV
    addHistory(newHistory);
    return;
}