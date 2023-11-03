struct request
{
    std::string name;
    std::string email;
    std::string product;
    std::string quantidade;
    std::string observacoes;
    std::string date;
    std::string hour;
};
std::string listofRequests;

void addRequest(const request &request)
{
    std::fstream datebaseOfRequests("./class/datebase/files/requests.csv", std::ios::app | std::ios::in | std::ios::out);
    if (datebaseOfRequests.is_open())
    {
        std::string observacoes = request.observacoes;
        std::replace(observacoes.begin(), observacoes.end(), '\n', '*');
        datebaseOfRequests << request.product << "," << request.quantidade << "," << observacoes << "," << request.date << "," << request.hour << ";\n";
        datebaseOfRequests.close();
    }
}

void listRequests()
{
    listofRequests = "";
    std::ifstream datebaseOfRequests("./class/datebase/files/requests.csv");

    if (datebaseOfRequests.is_open())
    {
        std::string line;
        while (std::getline(datebaseOfRequests, line))
        {
            std::istringstream iss(line);
            std::string name, email, product, quantidade, observacoes, date, hour;

            if (std::getline(iss, name, ',') &&
                std::getline(iss, email, ',') &&
                std::getline(iss, product, ',') &&
                std::getline(iss, quantidade, ',') &&
                std::getline(iss, observacoes, ',') &&
                std::getline(iss, date, ',') &&
                std::getline(iss, hour, ';'))
            {
                if (listofRequests != "")
                {
                    listofRequests += ",{\"date\":\"" + date + "\",\"hour\":\"" + hour + "\",\"product\":\"" + product + "\",\"quantity\":\"" + quantidade + "\",\"observations\":\"" + observacoes + "\"}";
                }
                else
                {
                    listofRequests += "{\"date\":\"" + date + "\",\"hour\":\"" + hour + "\",\"product\":\"" + product + "\",\"quantity\":\"" + quantidade + "\",\"observations\":\"" + observacoes + "\"}";
                }
            }
        }
        datebaseOfRequests.close();
    }
}
