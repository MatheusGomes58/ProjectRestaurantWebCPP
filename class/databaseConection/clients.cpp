// Definição da estrutura 'client' com campos para Nome, Email, Numero e Classificacao.
struct client
{
    std::string Nome;
    std::string Email;
    std::string Numero;
    std::string Classificacao;
};

// Declaração de uma string para armazenar a lista de clientes.
std::string listofClients;

// Função para adicionar um cliente ao arquivo CSV.
void addClient(const client &client)
{
    // Abertura do arquivo de clientes em modo de apêndice e leitura/escrita.
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);

    // Verificação se o arquivo foi aberto com sucesso.
    if (databaseOfClients.is_open())
    {
        // Escrita dos dados do cliente no arquivo CSV, separados por '|'.
        databaseOfClients << client.Nome << "|" << client.Email << "|" << client.Numero << "|" << client.Classificacao << "\n";

        // Fechamento do arquivo.
        databaseOfClients.close();
    }
    else
    {
        // Exibição de uma mensagem de erro se a conexão com o banco de dados falhar.
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

// Função para listar os clientes do arquivo CSV.
void listClients()
{
    // Inicialização da string que armazenará a lista de clientes.
    listofClients = "";

    // Abertura do arquivo de clientes em modo de leitura.
    std::ifstream databaseOfClients("./database/clients.csv");

    // Verificação se o arquivo foi aberto com sucesso.
    if (!databaseOfClients.is_open())
    {
        // Exibição de uma mensagem de erro se a abertura do arquivo falhar.
        std::cerr << "Erro ao abrir o arquivo de clientes." << std::endl;
        return;
    }

    // Leitura do arquivo linha por linha.
    std::string line;
    while (std::getline(databaseOfClients, line))
    {
        // Extração dos dados da linha usando o delimitador '|'.
        std::istringstream iss(line);
        std::string Nome, Email, Numero, Classificacao;

        // Atribuição dos valores extraídos aos campos correspondentes da estrutura 'client'.
        if (std::getline(iss, Nome, '|') &&
            std::getline(iss, Email, '|') &&
            std::getline(iss, Numero, '|') &&
            std::getline(iss, Classificacao, '\n'))
        {
            // Criação de um objeto JSON com os dados do cliente e adição à lista.
            json client{
                {"Nome", Nome},
                {"Email", Email},
                {"Numero", Numero},
                {"Classificação", Classificacao},
                {"Origem", "client"}};

            // Conversão do objeto JSON em uma string e adição à lista de clientes.
            if (!listofClients.empty())
            {
                listofClients += ",";
            }
            listofClients += client.dump();
        }
    }

    // Fechamento do arquivo.
    databaseOfClients.close();
}

// Função para excluir um cliente do arquivo CSV.
void dellClient(const client &client)
{
    // Abertura do arquivo de clientes em modo de apêndice e leitura/escrita.
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);

    // Verificação se o arquivo foi aberto com sucesso.
    if (databaseOfClients.is_open())
    {
        // Criação de um arquivo temporário para armazenar os dados após a exclusão.
        std::fstream temporario("./database/temp.csv", std::ios::out);

        // Verificação se o arquivo temporário foi criado com sucesso.
        if (!temporario)
        {
            // Exibição de uma mensagem de erro se a criação do arquivo temporário falhar.
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        // Variável para indicar se o cliente a ser excluído foi encontrado.
        bool encontrado = false;
        std::string line;

        // Leitura do arquivo de clientes linha por linha.
        while (std::getline(databaseOfClients, line))
        {
            // Extração dos dados da linha usando o delimitador '|'.
            std::istringstream iss(line);
            std::string Nome, Email, Numero, Classificacao;

            // Atribuição dos valores extraídos aos campos correspondentes da estrutura 'client'.
            if (std::getline(iss, Nome, '|') &&
                std::getline(iss, Email, '|') &&
                std::getline(iss, Numero, '|') &&
                std::getline(iss, Classificacao, '\n'))
            {
                // Verificação se o cliente atual é o que queremos excluir.
                if (!(client.Nome == Nome &&
                      client.Email == Email &&
                      client.Numero == Numero &&
                      client.Classificacao == Classificacao))
                {
                    // Se não for o cliente a ser excluído, os dados são escritos no arquivo temporário.
                    temporario << Nome << "|" << Email << "|"
                               << Numero << "|" << Classificacao << "\n";
                }
                else
                {
                    // Se for o cliente a ser excluído, marcamos como encontrado.
                    encontrado = true;
                }
            }
        }

        // Fechamento dos arquivos.
        temporario.close();
        databaseOfClients.close();

        // Exclusão do arquivo original e renomeação do arquivo temporário para o nome original.
        remove("./database/clients.csv");
        rename("./database/temp.csv", "./database/clients.csv");

        // Exibição de uma mensagem se o cliente não foi encontrado.
        if (!encontrado)
        {
            std::cout << "Cliente não encontrado." << std::endl;
        }
    }
}

// Função para editar um cliente no arquivo CSV.
void editClient(const client &oldClient, const client &newClient)
{
    // Abertura do arquivo de clientes em modo de apêndice e leitura/escrita.
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);

    // Verificação se o arquivo foi aberto com sucesso.
    if (databaseOfClients.is_open())
    {
        // Criação de um arquivo temporário para armazenar os dados após a edição.
        std::fstream temporario("./database/temp.csv", std::ios::out);

        // Verificação se o arquivo temporário foi criado com sucesso.
        if (!temporario)
        {
            // Exibição de uma mensagem de erro se a criação do arquivo temporário falhar.
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        // Variável para indicar se o cliente a ser editado foi encontrado.
        bool encontrado = false;
        std

::string line;

        // Leitura do arquivo de clientes linha por linha.
        while (std::getline(databaseOfClients, line))
        {
            // Extração dos dados da linha usando o delimitador '|'.
            std::istringstream iss(line);
            std::string Nome, Email, Numero, Classificacao;

            // Atribuição dos valores extraídos aos campos correspondentes da estrutura 'client'.
            if (std::getline(iss, Nome, '|') &&
                std::getline(iss, Email, '|') &&
                std::getline(iss, Numero, '|') &&
                std::getline(iss, Classificacao, '\n'))
            {
                // Verificação se o cliente atual é o que queremos editar.
                if (oldClient.Nome == Nome &&
                    oldClient.Email == Email &&
                    oldClient.Numero == Numero &&
                    oldClient.Classificacao == Classificacao)
                {
                    // Se for o cliente a ser editado, substituímos pelos novos valores.
                    temporario << newClient.Nome << "|" << newClient.Email << "|"
                               << newClient.Numero << "|" << newClient.Classificacao << "\n";
                    encontrado = true;
                }
                else
                {
                    // Se não for o cliente a ser editado, os dados são escritos no arquivo temporário.
                    temporario << Nome << "|" << Email << "|"
                               << Numero << "|" << Classificacao << "\n";
                }
            }
        }

        // Fechamento dos arquivos.
        temporario.close();
        databaseOfClients.close();

        // Exclusão do arquivo original e renomeação do arquivo temporário para o nome original.
        remove("./database/clients.csv");
        rename("./database/temp.csv", "./database/clients.csv");

        // Exibição de uma mensagem se o cliente não foi encontrado para edição.
        if (!encontrado)
        {
            std::cout << "Cliente não encontrado para edição." << std::endl;
        }
    }
}

// Função para salvar um novo cliente no arquivo CSV a partir de um objeto JSON.
void saveClient(json jsonResponse)
{
    // Criação de um objeto 'client' a partir dos dados fornecidos no objeto JSON.
    client newClient;
    newClient.Nome = jsonResponse["Nome"];
    newClient.Email = jsonResponse["Email"];
    newClient.Numero = jsonResponse["Numero"];
    newClient.Classificacao = jsonResponse["Classificação"];

    // Adição do novo cliente ao arquivo CSV.
    addClient(newClient);
}

// Função para excluir um cliente do arquivo CSV a partir de um objeto JSON.
void deleteClient(json jsonResponse)
{
    // Criação de um objeto 'client' a partir dos dados fornecidos no objeto JSON.
    client deleteClient;
    deleteClient.Nome = jsonResponse["Nome"];
    deleteClient.Email = jsonResponse["Email"];
    deleteClient.Numero = jsonResponse["Numero"];
    deleteClient.Classificacao = jsonResponse["Classificação"];

    // Exclusão do cliente do arquivo CSV.
    dellClient(deleteClient);
}

// Função para atualizar um cliente no arquivo CSV a partir de um objeto JSON contendo os dados antigos e novos.
void updateClient(json jsonResponse)
{
    // Criação de objetos 'client' representando os dados antigos e novos do cliente.
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

    // Edição do cliente no arquivo CSV.
    editClient(oldClient, newClient);
}