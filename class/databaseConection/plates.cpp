struct plate
{
    std::string desciption_plate;
    std::string picture_plate;
    std::string name_plate;
    double price_plate;
    int quantity_free;
    int time_ready;
};

std::string listofPlates;

void addPlate(const plate &plate)
{
    std::fstream datebaseOfPlates("./database/plates.csv", std::ios::app | std::ios::in | std::ios::out);
    if (datebaseOfPlates.is_open())
    {
        datebaseOfPlates << plate.desciption_plate << "," << plate.picture_plate << "," << plate.name_plate << ","
                << plate.price_plate << "," << plate.quantity_free << "," << plate.time_ready << ";\n";
        datebaseOfPlates.close();
    }
}

void listPlates()
{
    listofPlates = "";
    std::fstream datebaseOfPlates("./database/plates.csv");

    if (datebaseOfPlates.is_open())
    {
        std::string line;
        while (std::getline(datebaseOfPlates, line))
        {
            std::istringstream iss(line);
            std::string descricao, foto, name, preco, quantidade, tempo;

            if (std::getline(iss, descricao, ',') && std::getline(iss, foto, ',') &&
                std::getline(iss, name, ',') && std::getline(iss, preco, ',') &&
                std::getline(iss, quantidade, ',') && std::getline(iss, tempo, ';'))
            {
                if (!listofPlates.empty())
                {
                    listofPlates += ",";
                }
                listofPlates += "{\"name_plate\":\"" + name + "\",\"desciption_plate\":\"" + descricao + "\",\"picture_plate\":\"" + foto + "\",\"price_plate\":" + preco + ",\"quantity_free\":" + quantidade +
                                ",\"time_ready\":" + tempo + "}";
            }
        }
        datebaseOfPlates.close();
    }
}
