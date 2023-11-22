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
    std::string Preco;
    std::string Senha;
    std::string Atendimento;
};

std::string listofRequests;
std::string senhaChamada;
int programSenha = 0;
int atualSenha = 0;
int tamanhoDaFila = 1;

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
                           << request.Preco << "|"
                           << request.Senha << "|"
                           << request.Atendimento << "\n";
        databaseOfRequests.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listRequests(Fila &f)
{

    std::fstream databaseOfRequests("./database/requests.csv");
    listofRequests = "";
    atualSenha = programSenha;

    if (databaseOfRequests.is_open())
    {
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
            request request;
            std::string Produto, Quantidade, Observação;

            if (std::getline(iss, request.Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, request.Data, '|') &&
                std::getline(iss, request.Hora, '|') &&
                std::getline(iss, request.Preco, '|') &&
                std::getline(iss, request.Senha, '|') &&
                std::getline(iss, request.Atendimento, '\n'))
            {
                // Criar um objeto JSON para representar a solicitação
                json requestJson;
                requestJson["Cliente"] = request.Cliente;
                requestJson["Data"] = request.Data;
                requestJson["Hora"] = request.Hora;
                request.Produto = splitString(Produto, ';');
                request.Quantidade = splitString(Quantidade, ';');
                request.Observação = splitString(Observação, ';');
                requestJson["Produto"] = request.Produto;
                requestJson["Quantidade"] = request.Quantidade;
                requestJson["Observação"] = request.Observação;
                requestJson["Preço"] = request.Preco;
                requestJson["Senha"] = request.Senha;
                requestJson["Atendimento"] = request.Atendimento;
                requestJson["Origem"] = "request";

                // Converter o objeto JSON em uma string e adicionar à lista
                if (!listofRequests.empty())
                {
                    listofRequests += ",";
                }
                listofRequests += requestJson.dump();

                int senhaAtualValue = std::stoi(request.Senha);
                inserirNaFila(f, senhaAtualValue);
                if (senhaAtualValue < atualSenha)
                {
                    atualSenha = senhaAtualValue;
                }
                tamanhoDaFila++;
            }
        }
        databaseOfRequests.close();
    }
    return;
}

void dellRequest(const request &inputRequest)
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
        listProduto = createStringList(inputRequest.Produto);
        listQuantidade = createStringList(inputRequest.Quantidade);
        listObservação = createStringList(inputRequest.Observação);

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
            request consulrequest;
            std::string Produto, Quantidade, Observação;

            if (std::getline(iss, consulrequest.Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, consulrequest.Data, '|') &&
                std::getline(iss, consulrequest.Hora, '|') &&
                std::getline(iss, consulrequest.Preco, '|') &&
                std::getline(iss, consulrequest.Senha, '|') &&
                std::getline(iss, consulrequest.Atendimento, '\n'))
            {
                if (!(inputRequest.Cliente == consulrequest.Cliente &&
                      inputRequest.Data == consulrequest.Data &&
                      inputRequest.Hora == consulrequest.Hora &&
                      listObservação == Observação &&
                      listProduto == Produto &&
                      listQuantidade == Quantidade &&
                      inputRequest.Preco == consulrequest.Preco &&
                      inputRequest.Senha == consulrequest.Senha &&
                      inputRequest.Atendimento == consulrequest.Atendimento))
                {
                    temporario << consulrequest.Cliente << "|"
                               << Produto << "|"
                               << Quantidade << "|"
                               << Observação << "|"
                               << consulrequest.Data << "|"
                               << consulrequest.Hora << "|"
                               << consulrequest.Preco << "|"
                               << consulrequest.Senha << "|"
                               << consulrequest.Atendimento << "\n";
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
        request consulrequest;
        std::string Produto, Quantidade, Observação;

        if (std::getline(iss, consulrequest.Cliente, '|') &&
            std::getline(iss, Produto, '|') &&
            std::getline(iss, Quantidade, '|') &&
            std::getline(iss, Observação, '|') &&
            std::getline(iss, consulrequest.Data, '|') &&
            std::getline(iss, consulrequest.Hora, '|') &&
            std::getline(iss, consulrequest.Preco, '|') &&
            std::getline(iss, consulrequest.Senha, '|') &&
            std::getline(iss, consulrequest.Atendimento, '\n'))
        {
            if (!(oldRequest.Cliente == consulrequest.Cliente &&
                  oldRequest.Data == consulrequest.Data &&
                  oldRequest.Hora == consulrequest.Hora &&
                  listObservação == Observação &&
                  listProduto == Produto &&
                  listQuantidade == Quantidade &&
                  oldRequest.Preco == consulrequest.Preco &&
                  oldRequest.Senha == consulrequest.Senha &&
                  oldRequest.Atendimento == consulrequest.Atendimento))
            {
                temporario << consulrequest.Cliente << "|"
                           << Produto << "|"
                           << Quantidade << "|"
                           << Observação << "|"
                           << consulrequest.Data << "|"
                           << consulrequest.Hora << "|"
                           << consulrequest.Preco << "|"
                           << consulrequest.Senha << "|"
                           << consulrequest.Atendimento << "\n";
            }
            else
            {
                encontrado = true;
                temporario << newRequest.Cliente << "|"
                           << novoProduto << "|"
                           << novaQuantidade << "|"
                           << novaObservação << "|"
                           << newRequest.Data << "|"
                           << newRequest.Hora << "|"
                           << newRequest.Preco << "|"
                           << consulrequest.Senha << "|"
                           << newRequest.Atendimento << "\n";
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
        remove("./database/requests.csv");                        // Remove o arquivo original.
        rename("./database/temp.csv", "./database/requests.csv"); // Renomeia o arquivo temporário.
    }
}

void callRequest(std::string senhaChamada)
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
            request consulrequest;
            std::string Produto, Quantidade, Observação;

            if (std::getline(iss, consulrequest.Cliente, '|') &&
                std::getline(iss, Produto, '|') &&
                std::getline(iss, Quantidade, '|') &&
                std::getline(iss, Observação, '|') &&
                std::getline(iss, consulrequest.Data, '|') &&
                std::getline(iss, consulrequest.Hora, '|') &&
                std::getline(iss, consulrequest.Preco, '|') &&
                std::getline(iss, consulrequest.Senha, '|') &&
                std::getline(iss, consulrequest.Atendimento, '\n'))
            {

                if (!(consulrequest.Senha == senhaChamada))
                {
                    temporario << consulrequest.Cliente << "|"
                               << Produto << "|"
                               << Quantidade << "|"
                               << Observação << "|"
                               << consulrequest.Data << "|"
                               << consulrequest.Hora << "|"
                               << consulrequest.Preco << "|"
                               << consulrequest.Senha << "|"
                               << consulrequest.Atendimento << "\n";
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
    if (jsonResponse.contains(premissa + "Preço"))
    {
        request.Preco = jsonResponse[premissa + "Preço"];
    }
    if (jsonResponse.contains(premissa + "Senha"))
    {
        request.Senha = jsonResponse[premissa + "Senha"];
    }
    if (jsonResponse.contains(premissa + "Atendimento"))
    {
        request.Atendimento = jsonResponse[premissa + "Atendimento"];
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

    newRequest.Senha = std::to_string(programSenha + 1);

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

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    UpdateRequest.Data = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", localTime);
    UpdateRequest.Hora = timeBuffer;

    editRequest(oldRequest, UpdateRequest);
    return;
}

void salvarSenhaChamada(const std::string &senha)
{
    std::fstream databaseOfPassword("./database/senhaChamada.csv", std::ios::in | std::ios::out);
    if (databaseOfPassword.is_open())
    {
        databaseOfPassword.clear();
        databaseOfPassword << senha;
        databaseOfPassword.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo para salvar a senha chamada.\n";
    }
}

void lerSenhaChamada()
{
    std::fstream databaseOfPassword("./database/senhaChamada.csv");
    if (databaseOfPassword.is_open())
    {
        std::getline(databaseOfPassword, senhaChamada);
        databaseOfPassword.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo para ler a senha chamada.\n";
    }
}

void callPass(Fila &f)
{
    if (estaVazia(f))
    {
        int senhaChamadaValue = removerDaFila(f);

        std::ostringstream convert;
        convert << senhaChamadaValue;
        std::string atualSenhaStr = convert.str();

        salvarSenhaChamada(atualSenhaStr);

        std::cout << atualSenhaStr << std::endl;

        // Passe a string como parâmetro para a função callRequest
        callRequest(atualSenhaStr);
    }
}
