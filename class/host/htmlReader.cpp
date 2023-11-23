// Função para carregar o conteúdo de um arquivo HTML em uma string
void loadHtmlFileIntoString(const std::string &filename, std::string &targetString, Fila &filaComum, Fila &filaPreferencial)
{
    // Chamadas para listar produtos, clientes, históricos e pedidos, e carregar a senha chamada
    listProducts();
    listClients();
    listHistorys(searchValue);
    listRequests(filaComum, filaPreferencial);
    lerSenhaChamada();

    // Abre o arquivo HTML em modo de leitura binária
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (file.is_open())
    {
        // Obtém o tamanho do arquivo
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Cria um vetor para armazenar o conteúdo do arquivo
        std::vector<char> fileContents(static_cast<size_t>(fileSize));
        file.read(&fileContents[0], fileSize);

        // Fecha o arquivo após a leitura
        file.close();

        // Constrói a resposta HTTP com o conteúdo do arquivo HTML
        targetString = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html; charset=UTF-8\r\n\r\n";
        targetString.append(fileContents.begin(), fileContents.end());
        // Adiciona scripts JavaScript para renderizar dinamicamente os dados na página HTML
        targetString.append("<script>renderRequests('" + listofRequests + "')</script>");
        targetString.append("<script>renderProducts('" + listofProducts + "')</script>");
        targetString.append("<script>renderUsers('" + listofClients + "')</script>");
        targetString.append("<script>renderHistory('" + listofHistorys + "','" + searchValue +"')</script>");
        targetString.append("<script>renderPassword('" + senhaChamada + "')</script>");
        targetString.append("<script>renderAlert('" + alert + "')</script>");

        // Desabilita a solicitação se não houver clientes ou produtos listados
        if (!(listofClients.length() > 0 && listofProducts.length() > 0))
        {
            targetString.append("<script>disableRequest()</script>");
        }
    }
    else
    {
        // Imprime uma mensagem de erro se não for possível abrir o arquivo
        std::cerr << "Unable to open the file: " << filename << std::endl;
        // Você pode adicionar código de tratamento de erro aqui, como retornar um código de erro.
    }
}