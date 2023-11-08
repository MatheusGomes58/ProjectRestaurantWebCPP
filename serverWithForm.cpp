#include "./class/managementClasses.cpp"

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
            saveHistory(jsonResponse);
        }
        else if (type == "requestDelete")
        {
            deleteRequest(jsonResponse);
        }
        else if (type == "requestUpdate")
        {
            updateRequest(jsonResponse);
            updateHistory(jsonResponse);
        }
        else if (type == "plates")
        {
            savePlate(jsonResponse);
        }
        else if (type == "plateDelete")
        {
            deletePlate(jsonResponse);
        }
        else if (type == "plateUpdate")
        {
            updatePlate(jsonResponse);
        }
        else if (type == "clients")
        {
            saveClient(jsonResponse);
        }
        else if (type == "clientDelete")
        {
            deleteClient(jsonResponse);
        }
        else if (type == "clientUpdate")
        {
            updateClient(jsonResponse);
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
            loadHtmlFileIntoString("./html/program.html", form_response);
            write(client_fd, form_response.c_str(), form_response.size());
        }

        close(client_fd);
    }

    close(sock);
    return 0;
}