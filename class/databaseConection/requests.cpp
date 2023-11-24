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

void listRequests(Fila &filaComum, Fila &filaPreferencial)
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
                if (request.Atendimento == "Comum")
                {
                    inserirNaFila(filaComum, senhaAtualValue);
                    if (senhaAtualValue < atualSenha)
                    {
                        atualSenha = senhaAtualValue;
                    }
                }
                else
                {
                    inserirNaFila(filaPreferencial, senhaAtualValue);
                    if (senhaAtualValue < atualSenha)
                    {
                        atualSenha = senhaAtualValue;
                    }
                }
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
    alert = "Falha ao salvar dados do pedido!";
    request newRequest = requestMap(jsonResponse, "");

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    newRequest.Data = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    newRequest.Hora = timeBuffer;

    newRequest.Senha = std::to_string(programSenha + 1);

    newRequest.Senha = std::string(3 - newRequest.Senha.length(), '0') + newRequest.Senha;

    for (int i = 0; i < newRequest.Produto.size(); i++)
    {
        product produto = searchProducts(newRequest.Produto[i]);

        // Convert string to numerical values
        int newRequestQuantity = std::stoi(newRequest.Quantidade[i]);
        int produtoQuantity = std::stoi(produto.Quantidade);

        if (newRequestQuantity > produtoQuantity)
        {
            alert = "não foi possivel cadastrar o produto, pois a quantidade informada é maior que a quantidade disponível!";
            return;
        }
        else
        {
            product produtoNovo = produto;
            produtoNovo.Quantidade = std::to_string(produtoQuantity - newRequestQuantity);
            editProduct(produto, produtoNovo);
        }
    }
    addRequest(newRequest);
    addHistory(newRequest, "Inserido");
    alert = "Sucesso ao inserir dados do Pedido!";
    return;
}

void deleteRequest(json jsonResponse)
{
    alert = "Falha ao deletar dados do Pedido!";
    request deleteRequest = requestMap(jsonResponse, "");
    request delRequestValue = deleteRequest;

    for (int i = 0; i < delRequestValue.Produto.size(); i++)
    {
        product produto = searchProducts(delRequestValue.Produto[i]);

        // Convert string to numerical values
        int newRequestQuantity = -(std::stoi(delRequestValue.Quantidade[i]));
        int produtoQuantity = std::stoi(produto.Quantidade);

        if (newRequestQuantity > 0)
        {
            alert = "não foi possivel deletar o produto, pois a quantidade informada é maior que a quantidade disponível!";
            return;
        }
        else
        {
            product produtoNovo = produto;
            produtoNovo.Quantidade = std::to_string(produtoQuantity - newRequestQuantity);
            editProduct(produto, produtoNovo);
        }
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    delRequestValue.Data = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    delRequestValue.Hora = timeBuffer;

    addHistory(delRequestValue, "Deletado");
    dellRequest(deleteRequest);

    alert = "Sucesso ao deletar dados do pedido!";
    return;
}

void updateRequest(json jsonResponse)
{
    alert = "Falha ao atualizar dados do pedido!";
    request oldRequest = requestMap(jsonResponse, "old_");
    request UpdateRequest = requestMap(jsonResponse, "");

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dataBuffer[11];
    strftime(dataBuffer, sizeof(dataBuffer), "%d/%m/%Y", localTime);
    UpdateRequest.Data = dataBuffer;

    char timeBuffer[10];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    UpdateRequest.Hora = timeBuffer;

    UpdateRequest.Senha = oldRequest.Senha;

    for (int i = 0; i < UpdateRequest.Produto.size(); i++)
    {
        product produto = searchProducts(UpdateRequest.Produto[i]);

        // Convert string to numerical values
        int newRequestQuantity = (std::stoi(UpdateRequest.Quantidade[i]) - std::stoi(oldRequest.Quantidade[i]));
        int produtoQuantity = std::stoi(produto.Quantidade);

        if (newRequestQuantity > produtoQuantity)
        {
            alert = "não foi possivel editar o produto, pois a quantidade informada é maior que a quantidade disponível!";
            return;
        }
        else
        {
            product produtoNovo = produto;
            produtoNovo.Quantidade = std::to_string(produtoQuantity - newRequestQuantity);
            editProduct(produto, produtoNovo);
        }
    }

    editRequest(oldRequest, UpdateRequest);
    addHistory(UpdateRequest, "Cancelado");
    alert = "Sucesso ao atualizar dados do pedido!";
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

void callPass(Fila &filaComum, Fila &filaPreferencial)
{
    int senhaChamadaValue = 0;

    // Inicializa a semente do gerador de números aleatórios
    std::srand(std::time(0));

    // Gera um número aleatório de 0 a 10
    int numeroAleatorio = std::rand() % 11;

    if (numeroAleatorio > 7 && estaVazia(filaComum))
    {
        senhaChamadaValue = removerDaFila(filaComum);
    }
    else if (estaVazia(filaPreferencial))
    {
        senhaChamadaValue = removerDaFila(filaPreferencial);
    }

    std::string atualSenhaStr = std::to_string(senhaChamadaValue);

    atualSenhaStr = std::string(3 - atualSenhaStr.length(), '0') + atualSenhaStr;

    salvarSenhaChamada(atualSenhaStr);

    std::cout << atualSenhaStr << std::endl;

    // Passe a string como parâmetro para a função callRequest
    callRequest(atualSenhaStr);
}