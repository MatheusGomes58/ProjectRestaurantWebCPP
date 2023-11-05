#include <iostream>
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

#include "libraries/json.hpp"
#include "./class/managementClasses.cpp"

using json = nlohmann::json;

void saveRequest(json jsonResponse)
{
    request newRequest;
    newRequest.client = jsonResponse["client"];
    newRequest.product = jsonResponse["product"];
    newRequest.quantity = jsonResponse["quantity"];
    newRequest.observations = jsonResponse["observations"];

    time_t now = time(0);
    tm *localTime = localtime(&now);
    char dateBuffer[11];
    strftime(dateBuffer, sizeof(dateBuffer), "%d/%m/%Y", localTime);
    newRequest.date = dateBuffer;
    char timeBuffer[9];
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", localTime);
    newRequest.hour = timeBuffer;

    addRequest(newRequest);
}

void savePlate(json jsonResponse)
{
    plate newPlate;
    newPlate.desciption_plate = jsonResponse["desciption_plate"];
    newPlate.picture_plate = jsonResponse["picture_plate"];
    newPlate.name_plate = jsonResponse["name_plate"];
    newPlate.price_plate = jsonResponse["price_plate"];
    newPlate.quantity_free = jsonResponse["quantity_free"];
    newPlate.time_ready = jsonResponse["time_ready"];

    addPlate(newPlate);
}

void saveClient(json jsonResponse)
{
    client newClient;
    newClient.name_cliente = jsonResponse["name_client"];
    newClient.email_cliente = jsonResponse["email_client"];
    newClient.number_client = jsonResponse["number_client"];

    addClient(newClient);
}

void processJson(json jsonResponse)
{
    std::cout << "Received Data:\n"
              << jsonResponse << std::endl;

    if (jsonResponse.contains("type"))
    {
        std::string type = jsonResponse["type"];

        if (type == "requests")
        {
            saveRequest(jsonResponse);
        }
        else if (type == "plates")
        {
            savePlate(jsonResponse);
        }
        else if (type == "clients")
        {
            saveClient(jsonResponse);
        }
        else
        {
            std::cout << "tipo de processo desconhecido" << std::endl;
        }
    }
}

int main()
{
    int sock;
    createConnection(sock);

    while (true)
    {
        int client_fd;
        struct sockaddr_in cli_addr;
        socklen_t sin_len = sizeof(cli_addr);

        client_fd = accept(sock, (struct sockaddr *)&cli_addr, &sin_len);

        if (client_fd == -1)
        {
            perror("Can't accept");
            continue;
        }

        char buffer[2048];
        ssize_t read_size = read(client_fd, buffer, sizeof(buffer));
        if (read_size > 0)
        {
            buffer[read_size] = '\0';
            char *request_start = strstr(buffer, "\r\n\r\n");
            std::string form_response;

            if (request_start)
            {
                try
                {
                    json jsonResponse = json::parse(request_start + 4);
                    processJson(jsonResponse);
                }
                catch (const std::exception &e)
                {
                }
            }

            // Se o JSON não for recebido ou "newPage" não for reconhecido, abra o arquivo "index" por padrão.
            if (form_response.empty())
            {
            }

            loadHtmlFileIntoString("./html/program.html", form_response);
            write(client_fd, form_response.c_str(), form_response.size());
        }

        close(client_fd);
    }

    close(sock);
    return 0;
}