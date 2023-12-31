void addProduct(const product &product)
{
    std::string descricao = createStringList(product.Descrição);
    std::fstream databaseOfProducts("./database/products.csv", std::ios::app | std::ios::in | std::ios::out);
    if (databaseOfProducts.is_open())
    {
        databaseOfProducts << product.Origem << "|" << descricao << "|" << product.Foto << "|" << product.Produto << "|"
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
            std::string origem, descricao, foto, produto, preco, quantidade, tempo;

            if (std::getline(iss, origem, '|') &&
                std::getline(iss, descricao, '|') &&
                std::getline(iss, foto, '|') &&
                std::getline(iss, produto, '|') &&
                std::getline(iss, preco, '|') &&
                std::getline(iss, quantidade, '|') &&
                std::getline(iss, tempo))
            {
                json product;
                product["Origem"] = origem;
                product["Produto"] = produto;
                std::vector<std::string> description = splitString(descricao, ';');
                product["Descrição"] = description;
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

product searchProducts(const std::string &produto)
{
    product foundProducts;
    std::fstream databaseOfProducts("./database/products.csv");
    if (databaseOfProducts.is_open())
    {
        std::string line;
        while (std::getline(databaseOfProducts, line))
        {
            std::istringstream iss(line);
            product currentProduct;
            std::string descricao;

            if (std::getline(iss, currentProduct.Origem, '|') &&
                std::getline(iss, descricao, '|') &&
                std::getline(iss, currentProduct.Foto, '|') &&
                std::getline(iss, currentProduct.Produto, '|') &&
                std::getline(iss, currentProduct.Preço, '|') &&
                std::getline(iss, currentProduct.Quantidade, '|') &&
                std::getline(iss, currentProduct.Tempo))
            {
                std::vector<std::string> description = splitString(descricao, ';');
                currentProduct.Descrição = description;
                if (currentProduct.Produto == produto)
                {
                    foundProducts = currentProduct;
                }
            }
        }
        databaseOfProducts.close();
    }
    else
    {
        std::cerr << "Erro ao abrir o arquivo de produtos." << std::endl;
    }
    return foundProducts;
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
            std::string origem, descricao, foto, produto, preco, quantidade, tempo;
            std::string description = createStringList(product.Descrição);

            if (std::getline(iss, origem, '|') &&
                std::getline(iss, descricao, '|') &&
                std::getline(iss, foto, '|') &&
                std::getline(iss, produto, '|') &&
                std::getline(iss, preco, '|') &&
                std::getline(iss, quantidade, '|') &&
                std::getline(iss, tempo))
            {
                if (!(product.Origem == origem &&
                      description == descricao &&
                      product.Foto == foto &&
                      product.Produto == produto &&
                      product.Preço == preco &&
                      product.Quantidade == quantidade &&
                      product.Tempo == tempo))
                {
                    temporario << origem << "|" << descricao << "|" << foto << "|"
                               << produto << "|" << preco << "|"
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
            std::cout << "Produto não encontrado." << std::endl;
        }
    }
}

void editProduct(const product &oldProduct, const product &newProduct)
{
    std::fstream databaseOfProducts("./database/products.csv", std::ios::in | std::ios::out);
    if (databaseOfProducts.is_open())
    {
        std::fstream temporario("./database/temp.csv", std::ios::out);
        if (!temporario.is_open())
        {
            std::cout << "Erro ao criar arquivo temporário." << std::endl;
            return;
        }

        bool encontrado = false;
        std::string line;
        while (std::getline(databaseOfProducts, line))
        {
            std::istringstream iss(line);
            std::string origem, descricao, foto, produto, preco, quantidade, tempo;
            std::string description = createStringList(oldProduct.Descrição);
            std::string descriptionnew = createStringList(newProduct.Descrição);

            if (std::getline(iss, origem, '|') &&
                std::getline(iss, descricao, '|') &&
                std::getline(iss, foto, '|') &&
                std::getline(iss, produto, '|') &&
                std::getline(iss, preco, '|') &&
                std::getline(iss, quantidade, '|') &&
                std::getline(iss, tempo))
            {
                if (oldProduct.Origem == origem &&
                    description == descricao &&
                    oldProduct.Foto == foto &&
                    oldProduct.Produto == produto &&
                    oldProduct.Preço == preco &&
                    oldProduct.Quantidade == quantidade &&
                    oldProduct.Tempo == tempo)
                {
                    temporario << newProduct.Origem << "|" << descriptionnew << "|" << newProduct.Foto << "|"
                               << newProduct.Produto << "|" << newProduct.Preço << "|"
                               << newProduct.Quantidade << "|" << newProduct.Tempo << "\n";
                    encontrado = true;
                }
                else
                {
                    temporario << origem << "|" << descricao << "|" << foto << "|"
                               << produto << "|" << preco << "|"
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
            std::cout << "Produto não encontrado para edição." << std::endl;
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
        product.Produto = jsonResponse[premissa + "Produto"];
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
    alert = "Falha ao criar o produto!";
    product newProduct = productMap(jsonResponse, "");
    addProduct(newProduct);
    alert = "Sucesso ao atualizar o produto!";
    return;
}

void deleteProduct(json jsonResponse)
{
    alert = "Falha ao deletar o produto!";
    product deleteProduct = productMap(jsonResponse, "");

    bool pedidoProduto = searchRequest(deleteProduct.Produto);

    if (pedidoProduto)
    {
        alert = "não é possivel deletar esse produto pois ele esta associado a um pedido em aberto!";
        return;
    }

    dellProduct(deleteProduct);
    alert = "Sucesso ao deletar o produto!";
    return;
}

void updateProduct(json jsonResponse)
{
    alert = "Falha ao atualizar o produto!";
    product oldproduct = productMap(jsonResponse, "old_");
    product UpdateProduct = productMap(jsonResponse, "");
    editProduct(oldproduct, UpdateProduct);
    alert = "Sucesso ao atualizar o produto!";
    return;
}