struct client
{
    std::string name_client;
    std::string email_client;
    std::string number_client;
};
std::string listofClients;

void addClient(const client &client)
{
    std::fstream databaseOfClients("./database/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfClients.is_open())
    {
        databaseOfClients << client.name_client << "," << client.email_client << "," << client.number_client << ";\n";
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
            std::string name_client, email_client, number_client;

            if (std::getline(iss, name_client, ',') &&
                std::getline(iss, email_client, ',') &&
                std::getline(iss, number_client, ';')) {
                if (!listofClients.empty()) {
                    listofClients += ",";
                }
                listofClients += "{\"name_client\":\"" + name_client + "\",\"email_client\":\"" + email_client + "\",\"number_client\":\"" + number_client + "\",\"origin\":\"client\"}";
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
            std::string name_client, email_client, number_client;

            if (std::getline(iss, name_client, ',') &&
                std::getline(iss, email_client, ',') &&
                std::getline(iss, number_client, ';')) {

                if (!(client.name_client == name_client &&
                      client.email_client == email_client &&
                      client.number_client == number_client)) {
                    temporario << name_client << "," << email_client << ","
                               << number_client << "\n";
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
            std::string name_client, email_client, number_client;

            if (std::getline(iss, name_client, ',') &&
                std::getline(iss, email_client, ',') &&
                std::getline(iss, number_client, ';')) {

                if (oldClient.name_client == name_client &&
                    oldClient.email_client == email_client &&
                    oldClient.number_client == number_client) {
                    // Se o cliente atual for o que queremos editar, substitua pelos novos valores
                    temporario << newClient.name_client << "," << newClient.email_client << ","
                               << newClient.number_client << "\n";
                    encontrado = true;
                } else {
                    // Caso contrário, mantenha o cliente original
                    temporario << name_client << "," << email_client << ","
                               << number_client << "\n";
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
    newClient.name_client = jsonResponse["name_client"];
    newClient.email_client = jsonResponse["email_client"];
    newClient.number_client = jsonResponse["number_client"];

    addClient(newClient);
}

void deleteClient(json jsonResponse) {
    client deleteClient;
    deleteClient.name_client = jsonResponse["name_client"];
    deleteClient.email_client = jsonResponse["email_client"];
    deleteClient.number_client = jsonResponse["number_client"];

    dellClient(deleteClient);
}

void updateClient(json jsonResponse) {
    client oldClient;
    oldClient.name_client = jsonResponse["old_name_client"];
    oldClient.email_client = jsonResponse["old_email_client"];
    oldClient.number_client = jsonResponse["old_number_client"];

    client newClient;
    newClient.name_client = jsonResponse["name_client"];
    newClient.email_client = jsonResponse["email_client"];
    newClient.number_client = jsonResponse["number_client"];

    editClient(oldClient, newClient);
}