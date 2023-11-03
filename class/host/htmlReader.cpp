#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void loadHtmlFileIntoString(const std::string &filename, std::string &targetString)
{
    listRequests();
    listPlates();
    listClients();

    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> fileContents(static_cast<size_t>(fileSize));
        file.read(&fileContents[0], fileSize);

        file.close();

        // Construa a resposta HTTP com o conteúdo do arquivo
        targetString = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html; charset=UTF-8\r\n\r\n";
        targetString.append(fileContents.begin(), fileContents.end());
        targetString.append("\n<script>renderRequests('" + listofRequests + "');</script>");
        targetString.append("\n<script>renderPlates('" + listofPlates + "');</script>");
        targetString.append("\n<script>renderPlates('" + listofClients + "');</script>");
    }
    else
    {
        std::cerr << "Unable to open the file: " << filename << std::endl;
        // Você pode adicionar código de tratamento de erro aqui, como retornar um código de erro.
    }
}
