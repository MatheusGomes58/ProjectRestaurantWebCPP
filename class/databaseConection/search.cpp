// Função para pesquisar os pedidos com base em um valor de pesquisa
bool searchRequest(std::string &searchValueData)
{
    std::fstream databaseOfRequests("./database/requests.csv");
    request request;

    if (databaseOfRequests.is_open())
    {
        std::string line;
        while (std::getline(databaseOfRequests, line))
        {
            std::istringstream iss(line);
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
                // Verificar se a entrada atende aos critérios de pesquisa e adicionar à lista
                if (request.Cliente == searchValueData ||
                    Produto.find(searchValueData) != std::string::npos)
                {
                    databaseOfRequests.close();
                    return true;
                }
            }
        }
        databaseOfRequests.close();
    }
    return false;
}