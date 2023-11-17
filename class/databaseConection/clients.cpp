struct client
{
    std::string Nome;
    std::string Email;
    std::string Numero;
    std::string Classificacao;
};
std::string listofClients;

void addClient(const client &client)
{
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfClients.is_open())
    {
        databaseOfClients << client.Nome << "|" << client.Email << "|" << client.Numero << "|" << client.Classificacao << "\n";
        databaseOfClients.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listClients()
{
    listofClients = "";

    std::ifstream databaseOfClients("./database/clients.csv");
    if (!databaseOfClients.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de clientes." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(databaseOfClients, line))
    {
        std::istringstream iss(line);
        std::string Nome, Email, Numero, Classificacao;

        if (std::getline(iss, Nome, '|') &&
            std::getline(iss, Email, '|') &&
            std::getline(iss, Numero, '|') &&
            std::getline(iss, Classificacao, '\n'))
        {
            json client{
                {"Nome", Nome},
                {"Email", Email},
                {"Numero", Numero},
                {"Classificação", Classificacao},
                {"Origem", "client"}};

            // Converter o objeto JSON em uma string e adicionar à lista
            if (!listofClients.empty())
            {
                listofClients += ",";
            }
            listofClients += client.dump();
        }
    }

    databaseOfClients.close();
}

void dellClient(const client &client)
{
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfClients.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfClients, line))
        {
            std::istringstream iss(line);
            std::string Nome, Email, Numero, Classificacao;

            if (std::getline(iss, Nome, '|') &&
                std::getline(iss, Email, '|') &&
                std::getline(iss, Numero, '|') &&
                std::getline(iss, Classificacao, '\n'))
            {

                if (!(client.Nome == Nome &&
                      client.Email == Email &&
                      client.Numero == Numero &&
                      client.Classificacao == Classificacao))
                {
                    temporario << Nome << "|" << Email << "|"
                               << Numero << "|" << Classificacao << "\n";
                }
                else
                {
                    encontrado = true;
                }
            }
        }

        temporario.close();
        databaseOfClients.close();
        remove("./database/clients.csv");
        rename("./database/temp.csv", "./database/clients.csv");

        if (!encontrado)
        {
            std::cout << "Cliente não encontrado." << std::endl;
        }
    }
}

void editClient(const client &oldClient, const client &newClient)
{
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfClients.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfClients, line))
        {
            std::istringstream iss(line);
            std::string Nome, Email, Numero, Classificacao;

            if (std::getline(iss, Nome, '|') &&
                std::getline(iss, Email, '|') &&
                std::getline(iss, Numero, '|') &&
                std::getline(iss, Classificacao, '\n'))
            {

                if (oldClient.Nome == Nome &&
                    oldClient.Email == Email &&
                    oldClient.Numero == Numero &&
                    oldClient.Classificacao == Classificacao)
                {
                    // Se o cliente atual for o que queremos editar, substitua pelos novos valores
                    temporario << newClient.Nome << "|" << newClient.Email << "|"
                               << newClient.Numero << "|" << newClient.Classificacao << "\n";
                    encontrado = true;
                }
                else
                {
                    // Caso contrário, mantenha o cliente Origemal
                    temporario << Nome << "|" << Email << "|"
                               << Numero << "|" << Classificacao << "\n";
                }
            }
        }

        temporario.close();
        databaseOfClients.close();
        remove("./database/clients.csv");
        rename("./database/temp.csv", "./database/clients.csv");

        if (!encontrado)
        {
            std::cout << "Cliente não encontrado para edição." << std::endl;
        }
    }
}

void saveClient(json jsonResponse)
{
    client newClient;
    newClient.Nome = jsonResponse["Nome"];
    newClient.Email = jsonResponse["Email"];
    newClient.Numero = jsonResponse["Numero"];
    newClient.Classificacao = jsonResponse["Classificação"];

    addClient(newClient);
}

void deleteClient(json jsonResponse)
{
    client deleteClient;
    deleteClient.Nome = jsonResponse["Nome"];
    deleteClient.Email = jsonResponse["Email"];
    deleteClient.Numero = jsonResponse["Numero"];
    deleteClient.Classificacao = jsonResponse["Classificação"];

    dellClient(deleteClient);
}

void updateClient(json jsonResponse)
{
    client oldClient;
    oldClient.Nome = jsonResponse["old_Nome"];
    oldClient.Email = jsonResponse["old_Email"];
    oldClient.Numero = jsonResponse["old_Numero"];
    oldClient.Classificacao = jsonResponse["old_Classificação"];

    client newClient;
    newClient.Nome = jsonResponse["Nome"];
    newClient.Email = jsonResponse["Email"];
    newClient.Numero = jsonResponse["Numero"];
    newClient.Classificacao = jsonResponse["Classificação"];

    editClient(oldClient, newClient);
}