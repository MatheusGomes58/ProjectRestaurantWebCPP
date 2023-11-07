struct plate
{
    std::string desciption_plate;
    std::string picture_plate;
    std::string name_plate;
    std::string price_plate;
    std::string quantity_free;
    std::string time_ready;
};

std::string listofPlates;

void addPlate(const plate &plate)
{
    std::fstream databaseOfPlates("./database/plates.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfPlates.is_open())
    {
        databaseOfPlates << plate.desciption_plate << "," << plate.picture_plate << "," << plate.name_plate << ","
                         << plate.price_plate << "," << plate.quantity_free << "," << plate.time_ready << ";\n";
        databaseOfPlates.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listPlates()
{
    listofPlates = "";
    std::fstream databaseOfPlates("./database/plates.csv");

    if (databaseOfPlates.is_open())
    {
        std::string line;
        while (std::getline(databaseOfPlates, line))
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
                listofPlates += "{\"name_plate\":\"" + name + "\",\"desciption_plate\":\"" + descricao + "\",\"picture_plate\":\"" + foto + "\",\"price_plate\":\"" + preco + "\",\"quantity_free\":\"" + quantidade +
                                "\",\"time_ready\":\"" + tempo + "\",\"origin\":\"plate\"}";
            }
        }
        databaseOfPlates.close();
    }
    return;
}

void dellPlate(const plate &plate)
{
    std::fstream databaseOfPlates("./database/plates.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfPlates.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfPlates, line))
        {
            std::istringstream iss(line);
            std::string descricao, foto, name, preco, quantidade, tempo;

            if (std::getline(iss, descricao, ',') &&
                std::getline(iss, foto, ',') &&
                std::getline(iss, name, ',') &&
                std::getline(iss, preco, ',') &&
                std::getline(iss, quantidade, ',') &&
                std::getline(iss, tempo, ';'))
            {
                if (!(plate.desciption_plate == descricao &&
                      plate.picture_plate == foto &&
                      plate.name_plate == name &&
                      plate.price_plate == preco &&
                      plate.quantity_free == quantidade &&
                      plate.time_ready == tempo))
                {
                    temporario << descricao << "," << foto << ","
                               << name << "," << preco << ","
                               << quantidade << "," << tempo << "\n";
                }
                else
                {
                    encontrado = true;
                }
            }
        }

        temporario.close();
        databaseOfPlates.close();
        remove("./database/plates.csv");
        rename("./database/temp.csv", "./database/plates.csv");

        if (!encontrado)
        {
            std::cout << "Prato não encontrado." << std::endl;
        }
    }
}

void editPlate(const plate &oldPlate, const plate &newPlate)
{
    std::fstream databaseOfPlates("./database/plates.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfPlates.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfPlates, line))
        {
            std::istringstream iss(line);
            std::string descricao, foto, name, preco, quantidade, tempo;

            if (std::getline(iss, descricao, ',') &&
                std::getline(iss, foto, ',') &&
                std::getline(iss, name, ',') &&
                std::getline(iss, preco, ',') &&
                std::getline(iss, quantidade, ',') &&
                std::getline(iss, tempo, ';'))
            {
                if (oldPlate.desciption_plate == descricao &&
                    oldPlate.picture_plate == foto &&
                    oldPlate.name_plate == name &&
                    oldPlate.price_plate == preco &&
                    oldPlate.quantity_free == quantidade &&
                    oldPlate.time_ready == tempo)
                {
                    temporario << newPlate.desciption_plate << "," << newPlate.picture_plate << ","
                               << newPlate.name_plate << "," << newPlate.price_plate << ","
                               << newPlate.quantity_free << "," << newPlate.time_ready << "\n";
                    encontrado = true;
                }
                else
                {
                    temporario << descricao << "," << foto << ","
                               << name << "," << preco << ","
                               << quantidade << "," << tempo << "\n";
                }
            }
        }

        temporario.close();
        databaseOfPlates.close();
        remove("./database/plates.csv");
        rename("./database/temp.csv", "./database/plates.csv");

        if (!encontrado)
        {
            std::cout << "Prato não encontrado para edição." << std::endl;
        }
    }
}


plate plateMap(json jsonResponse, std::string premissa)
{
    plate plate;
    if (jsonResponse.contains(premissa + "desciption_plate"))
    {
        plate.desciption_plate = jsonResponse[premissa + "desciption_plate"];
    }
    if (jsonResponse.contains(premissa + "picture_plate"))
    {
        plate.picture_plate = jsonResponse[premissa + "picture_plate"];
    }
    if (jsonResponse.contains(premissa + "name_plate"))
    {
        plate.name_plate = jsonResponse[premissa + "name_plate"];
    }
    if (jsonResponse.contains(premissa + "price_plate"))
    {
        plate.price_plate = jsonResponse[premissa + "price_plate"];
    }
    if (jsonResponse.contains(premissa + "quantity_free"))
    {
        plate.quantity_free = jsonResponse[premissa + "quantity_free"];
    }
    if (jsonResponse.contains(premissa + "time_ready"))
    {
        plate.time_ready = jsonResponse[premissa + "time_ready"];
    }
    return plate;
}

void savePlate(json jsonResponse)
{
    plate newPlate = plateMap(jsonResponse, "");
    addPlate(newPlate);
    return;
}

void deletePlate(json jsonResponse)
{
    plate deletePlate = plateMap(jsonResponse, "");
    dellPlate(deletePlate);
    return;
}

void updatePlate(json jsonResponse)
{
    plate oldplate = plateMap(jsonResponse, "old_");
    plate UpdatePlate = plateMap(jsonResponse, "");
    editPlate(oldplate,UpdatePlate);
    return;
}