struct product
{
    std::string Origem;
    std::string Descrição;
    std::string Foto;
    std::string Prato;
    std::string Preço;
    std::string Quantidade;
    std::string Tempo;
};

std::string listofProducts;

void addProduct(const product &product)
{
    std::fstream databaseOfProducts("./database/products.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfProducts.is_open())
    {
        databaseOfProducts << product.Origem << "|" << product.Descrição << "|" << product.Foto << "|" << product.Prato << "|"
                           << product.Preço << "|" << product.Quantidade << "|" << product.Tempo << "\n";
        databaseOfProducts.close();
    }
    else
    {
        std::cout << "falha ao estabelecer conexão com banco de dados" << std::endl;
    }
    return;
}

void listProducts()
{
    listofProducts = "";
    std::fstream databaseOfProducts("./database/products.csv");
    if (databaseOfProducts.is_open())
    {
        std::string line;
        while (std::getline(databaseOfProducts, line))
        {
            std::istringstream iss(line);
            std::string Origem, descricao, foto, name, preco, quantidade, tempo;

            if (std::getline(iss, Origem, '|') &&
                std::getline(iss, descricao, '|') &&
                std::getline(iss, foto, '|') &&
                std::getline(iss, name, '|') &&
                std::getline(iss, preco, '|') &&
                std::getline(iss, quantidade, '|') &&
                (std::getline(iss, tempo, '\n')||""))
            {
                json product;
                product["Origem"] = Origem;
                product["Product"] = name;
                product["Descrição"] = descricao;
                product["Foto"] = foto;
                product["Preço"] = preco;
                product["Quantidade"] = quantidade;
                product["Tempo"] = tempo;

                if (!listofProducts.empty())
                {
                    listofProducts += ",";
                }
                listofProducts += product.dump();
            }
        }
        databaseOfProducts.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo de produtos." << std::endl;
    }
}

void dellProduct(const product &product)
{
    std::fstream databaseOfProducts("./database/products.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfProducts.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfProducts, line))
        {
            std::istringstream iss(line);
            std::string descricao, foto, name, preco, quantidade, tempo;

            if (std::getline(iss, descricao, '|') &&
                std::getline(iss, foto, '|') &&
                std::getline(iss, name, '|') &&
                std::getline(iss, preco, '|') &&
                std::getline(iss, quantidade, '|') &&
                std::getline(iss, tempo, '\n'))
            {
                if (!(product.Descrição == descricao &&
                      product.Foto == foto &&
                      product.Prato == name &&
                      product.Preço == preco &&
                      product.Quantidade == quantidade &&
                      product.Tempo == tempo))
                {
                    temporario << descricao << "|" << foto << "|"
                               << name << "|" << preco << "|"
                               << quantidade << "|" << tempo << "\n";
                }
                else
                {
                    encontrado = true;
                }
            }
        }

        temporario.close();
        databaseOfProducts.close();
        remove("./database/products.csv");
        rename("./database/temp.csv", "./database/products.csv");

        if (!encontrado)
        {
            std::cout << "Prato não encontrado." << std::endl;
        }
    }
}

void editProduct(const product &oldProduct, const product &newProduct)
{
    std::fstream databaseOfProducts("./database/products.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfProducts.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario)
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfProducts, line))
        {
            std::istringstream iss(line);
            std::string descricao, foto, name, preco, quantidade, tempo;

            if (std::getline(iss, descricao, '|') &&
                std::getline(iss, foto, '|') &&
                std::getline(iss, name, '|') &&
                std::getline(iss, preco, '|') &&
                std::getline(iss, quantidade, '|') &&
                std::getline(iss, tempo, '\n'))
            {
                if (oldProduct.Descrição == descricao &&
                    oldProduct.Foto == foto &&
                    oldProduct.Prato == name &&
                    oldProduct.Preço == preco &&
                    oldProduct.Quantidade == quantidade &&
                    oldProduct.Tempo == tempo)
                {
                    temporario << newProduct.Descrição << "|" << newProduct.Foto << "|"
                               << newProduct.Prato << "|" << newProduct.Preço << "|"
                               << newProduct.Quantidade << "|" << newProduct.Tempo << "\n";
                    encontrado = true;
                }
                else
                {
                    temporario << descricao << "|" << foto << "|"
                               << name << "|" << preco << "|"
                               << quantidade << "|" << tempo << "\n";
                }
            }
        }

        temporario.close();
        databaseOfProducts.close();
        remove("./database/products.csv");
        rename("./database/temp.csv", "./database/products.csv");

        if (!encontrado)
        {
            std::cout << "Prato não encontrado para edição." << std::endl;
        }
    }
}

product productMap(json jsonResponse, std::string premissa)
{
    product product;
    if (jsonResponse.contains(premissa + "Origem"))
    {
        product.Origem = jsonResponse[premissa + "Origem"];
    }
    if (jsonResponse.contains(premissa + "Descrição"))
    {
        product.Descrição = jsonResponse[premissa + "Descrição"];
    }
    if (jsonResponse.contains(premissa + "Foto"))
    {
        product.Foto = jsonResponse[premissa + "Foto"];
    }
    if (jsonResponse.contains(premissa + "Produto"))
    {
        product.Prato = jsonResponse[premissa + "Produto"];
    }
    if (jsonResponse.contains(premissa + "Preço"))
    {
        product.Preço = jsonResponse[premissa + "Preço"];
    }
    if (jsonResponse.contains(premissa + "Quantidade"))
    {
        product.Quantidade = jsonResponse[premissa + "Quantidade"];
    }
    if (jsonResponse.contains(premissa + "Tempo"))
    {
        product.Tempo = jsonResponse[premissa + "Tempo"];
    }
    return product;
}

void saveProduct(json jsonResponse)
{
    product newProduct = productMap(jsonResponse, "");
    addProduct(newProduct);
    return;
}

void deleteProduct(json jsonResponse)
{
    product deleteProduct = productMap(jsonResponse, "");
    dellProduct(deleteProduct);
    return;
}

void updateProduct(json jsonResponse)
{
    product oldproduct = productMap(jsonResponse, "old_");
    product UpdateProduct = productMap(jsonResponse, "");
    editProduct(oldproduct, UpdateProduct);
    return;
}