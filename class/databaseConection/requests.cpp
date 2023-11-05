struct request
{
    std::string client;
    std::string product;
    int quantity;
    std::string observations;
    std::string date;
    std::string hour;
};
std::string listofRequests;

void addRequest(const request &request)
{
    std::fstream datebaseOfRequests("./database/requests.csv", std::ios::app | std::ios::in | std::ios::out);
    if (datebaseOfRequests.is_open())
    {
        datebaseOfRequests << request.client << "," << request.product << "," << request.quantity << "," << request.observations << "," << request.date << "," << request.hour << ";\n";
        datebaseOfRequests.close();
    }
}

void listRequests()
{
    listofRequests = "";
    std::fstream datebaseOfRequests("./database/requests.csv");

    if (datebaseOfRequests.is_open())
    {
        std::string line;
        while (std::getline(datebaseOfRequests, line))
        {
            std::istringstream iss(line);
            std::string client, product, quantity, observations, date, hour;

            if (std::getline(iss, client, ',') &&
                std::getline(iss, product, ',') &&
                std::getline(iss, quantity, ',') &&
                std::getline(iss, observations, ',') &&
                std::getline(iss, date, ',') &&
                std::getline(iss, hour, ';'))
            {
                if (listofRequests != "")
                {
                    listofRequests += ",{\"date\":\"" + date + "\",\"hour\":\"" + hour + "\",\"product\":\"" + product + "\",\"quantity\":\"" + quantity + "\",\"observations\":\"" + observations + "\"}";
                }
                else
                {
                    listofRequests += "{\"date\":\"" + date + "\",\"hour\":\"" + hour + "\",\"product\":\"" + product + "\",\"quantity\":\"" + quantity + "\",\"observations\":\"" + observations + "\"}";
                }
            }
        }
        datebaseOfRequests.close();
    }
}
