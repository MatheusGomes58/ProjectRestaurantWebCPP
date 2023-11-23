#include <iostream>
#include <iomanip>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

#include "./class/managementClasses.cpp"

// Função para processar um objeto JSON recebido, realizando operações com base no campo "type"
void processJson(json jsonResponse, Fila &filaComum, Fila &filaPreferencial)
{
    std::cout << "Received Data:\n"
              << jsonResponse << std::endl;

    // Verifica se o objeto JSON contém o campo "type"
    if (jsonResponse.contains("type"))
    {
        // Obtém o valor do campo "type"
        std::string type = jsonResponse["type"];

        // Realiza operações com base no valor do campo "type"
        if (type == "requests")
        {
            // Salva uma nova solicitação e registra no histórico
            saveRequest(jsonResponse);
        }
        else if (type == "requestDelete")
        {
            // Exclui uma solicitação
            deleteRequest(jsonResponse);
        }
        else if (type == "requestUpdate")
        {
            // Atualiza uma solicitação e registra no histórico
            updateRequest(jsonResponse);
        }
        else if (type == "products")
        {
            // Salva um novo produto
            saveProduct(jsonResponse);
        }
        else if (type == "productDelete")
        {
            // Exclui um produto
            deleteProduct(jsonResponse);
        }
        else if (type == "productUpdate")
        {
            // Atualiza um produto
            updateProduct(jsonResponse);
        }
        else if (type == "clients")
        {
            // Salva um novo cliente
            saveClient(jsonResponse);
        }
        else if (type == "clientDelete")
        {
            // Exclui um cliente
            deleteClient(jsonResponse);
        }
        else if (type == "clientUpdate")
        {
            // Atualiza um cliente
            updateClient(jsonResponse);
        }
        else if (type == "callPass")
        {
            // Chama o próximo cliente nas filas comum e preferencial
            callPass(filaComum, filaPreferencial);
        }
        else if (type == "search")
        {
            // Atualiza o valor de pesquisa
            searchValue = jsonResponse["valor"];
        }
        else if (type == "cleanSearch")
        {
            // Limpa o valor de pesquisa
            searchValue = "";
        }
        else if (type == "alert")
        {
            // Limpa o valor da mensagem de alerta
            alert = "";
        }
        else
        {
            // Mensagem de aviso para tipos de processo desconhecidos
            std::cout << "Tipo de processo desconhecido" << std::endl;
        }
    }
}

int main()
{
    // Criação de filas para clientes comuns e preferenciais
    Fila filaComum;
    Fila filaPreferencial;

    // Criação e configuração do socket do servidor
    int sock;
    createConnection(sock);

    // Loop principal do servidor
    while (true)
    {
        // Criação de novas filas a cada iteração do loop
        criarFila(filaComum);
        criarFila(filaPreferencial);

        // Configuração de variáveis para aceitar uma conexão do cliente
        int client_fd;
        struct sockaddr_in cli_addr;
        socklen_t sin_len = sizeof(cli_addr);

        // Aceita uma conexão do cliente
        client_fd = accept(sock, (struct sockaddr *)&cli_addr, &sin_len);

        if (client_fd == -1)
        {
            // Mensagem de erro em caso de falha ao aceitar a conexão
            perror("Can't accept");
            continue;
        }

        // Buffer para armazenar os dados recebidos do cliente
        char buffer[2048];
        ssize_t read_size = read(client_fd, buffer, sizeof(buffer));

        if (read_size > 0)
        {
            // Adiciona um caractere nulo ao final do buffer
            buffer[read_size] = '\0';

            // Localiza o início da requisição HTTP após os cabeçalhos
            char *request_start = strstr(buffer, "\r\n\r\n");
            std::string form_response;

            if (request_start)
            {
                try
                {
                    // Parseia o JSON a partir da requisição HTTP
                    json jsonResponse = json::parse(request_start + 4);

                    // Processa o JSON, realizando operações específicas com base no campo "type"
                    processJson(jsonResponse, filaComum, filaPreferencial);
                }
                catch (const std::exception &e)
                {
                    // Tratamento de exceção em caso de erro ao parsear o JSON
                }
            }

            // Carrega o conteúdo de um arquivo HTML em uma string e inclui dados dinâmicos
            loadHtmlFileIntoString("./html/program.html", form_response, filaComum, filaPreferencial);

            // Envia a resposta HTTP de volta ao cliente
            write(client_fd, form_response.c_str(), form_response.size());
        }

        // Fecha o socket do cliente após o processamento da requisição
        close(client_fd);
    }

    // Fecha o socket do servidor
    close(sock);

    // Encerra o programa
    return 0;
}