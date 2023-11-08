struct client
{
    std::string Nome;
    std::string Email;
    std::string Numero;
};
std::string listofClients;

void addClient(const client &client)
{
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfClients.is_open())
    {
        databaseOfClients << client.Nome << "|" << client.Email << "|" << client.Numero << "\n";
        databaseOfClients.close();
    }else{
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listClients() {
    listofClients = "";
    std::fstream databaseOfClients("./database/clients.csv");

    if (databaseOfClients.is_open()) {
        std::string line;
        while (std::getline(databaseOfClients, line)) {
            std::istringstream iss(line);
            std::string Nome, Email, Numero;

            if (std::getline(iss, Nome, '|') &&
                std::getline(iss, Email, '|') &&
                (std::getline(iss, Numero, '\n')||"")) {
                if (!listofClients.empty()) {
                    listofClients += ",";
                }
                listofClients += "{\"Nome\":\"" + Nome + "\",\"Email\":\"" + Email + "\",\"Numero\":\"" + Numero + "\",\"Origem\":\"client\"}";
            }
        }
        databaseOfClients.close();
    }
}

void dellClient(const client &client) {
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfClients.is_open()) {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario) {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfClients, line)) {
            std::istringstream iss(line);
            std::string Nome, Email, Numero;

            if (std::getline(iss, Nome, '|') &&
                std::getline(iss, Email, '|') &&
                std::getline(iss, Numero, '\n')) {

                if (!(client.Nome == Nome &&
                      client.Email == Email &&
                      client.Numero == Numero)) {
                    temporario << Nome << "|" << Email << "|"
                               << Numero << "\n";
                } else {
                    encontrado = true;
                }
            }
        }

        temporario.close();
        databaseOfClients.close();
        remove("./database/clients.csv");
        rename("./database/temp.csv", "./database/clients.csv");

        if (!encontrado) {
            std::cout << "Cliente não encontrado." << std::endl;
        }
    }
}

void editClient(const client &oldClient, const client &newClient) {
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfClients.is_open()) {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario) {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfClients, line)) {
            std::istringstream iss(line);
            std::string Nome, Email, Numero;

            if (std::getline(iss, Nome, '|') &&
                std::getline(iss, Email, '|') &&
                std::getline(iss, Numero, '\n')) {

                if (oldClient.Nome == Nome &&
                    oldClient.Email == Email &&
                    oldClient.Numero == Numero) {
                    // Se o cliente atual for o que queremos editar, substitua pelos novos valores
                    temporario << newClient.Nome << "|" << newClient.Email << "|"
                               << newClient.Numero << "\n";
                    encontrado = true;
                } else {
                    // Caso contrário, mantenha o cliente Origemal
                    temporario << Nome << "|" << Email << "|"
                               << Numero << "\n";
                }
            }
        }

        temporario.close();
        databaseOfClients.close();
        remove("./database/clients.csv");
        rename("./database/temp.csv", "./database/clients.csv");

        if (!encontrado) {
            std::cout << "Cliente não encontrado para edição." << std::endl;
        }
    }
}



void saveClient(json jsonResponse) {
    client newClient;
    newClient.Nome = jsonResponse["Nome"];
    newClient.Email = jsonResponse["Email"];
    newClient.Numero = jsonResponse["Numero"];

    addClient(newClient);
}

void deleteClient(json jsonResponse) {
    client deleteClient;
    deleteClient.Nome = jsonResponse["Nome"];
    deleteClient.Email = jsonResponse["Email"];
    deleteClient.Numero = jsonResponse["Numero"];

    dellClient(deleteClient);
}

void updateClient(json jsonResponse) {
    client oldClient;
    oldClient.Nome = jsonResponse["old_Nome"];
    oldClient.Email = jsonResponse["old_Email"];
    oldClient.Numero = jsonResponse["old_Numero"];

    client newClient;
    newClient.Nome = jsonResponse["Nome"];
    newClient.Email = jsonResponse["Email"];
    newClient.Numero = jsonResponse["Numero"];

    editClient(oldClient, newClient);
}