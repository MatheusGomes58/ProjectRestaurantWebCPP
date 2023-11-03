struct client
{
    std::string name_cliente;
    std::string email_cliente;
    std::string number_client;
};
std::string listofClients;

void addClient(const client &client)
{
    std::fstream datebaseOfClients("./class/datebase/files/clients.csv", std::ios::app | std::ios::in | std::ios::out);
    if (datebaseOfClients.is_open())
    {
        datebaseOfClients << client.name_cliente << "," << client.email_cliente << "," << client.number_client << ";\n";
        datebaseOfClients.close();
    }
}

void listClients()
{
    listofClients = "";
    std::ifstream datebaseOfClients("./class/datebase/files/clients.csv");

    if (datebaseOfClients.is_open())
    {
        std::string line;
        while (std::getline(datebaseOfClients, line))
        {
            std::istringstream iss(line);
            std::string name_cliente, email_cliente, number_client;

            if (std::getline(iss, name_cliente, ',') &&
                std::getline(iss, email_cliente, ',') &&
                std::getline(iss, number_client, ';'))
            {
                if (listofClients != "")
                {
                    listofClients += ",{\"name_cliente\":\"" + name_cliente + "\",\"email_cliente\":\"" + email_cliente + "\",\"number_client\":\"" + number_client + "\"}";
                }
                else
                {
                    listofClients += "{\"name_cliente\":\"" + name_cliente + "\",\"email_cliente\":\"" + email_cliente + "\",\"number_client\":\"" + number_client + "\"}";
                }
            }
        }
        datebaseOfClients.close();
    }
}
