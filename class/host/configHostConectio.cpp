// Função para criar uma conexão de servidor
void createConnection(int &sock)
{
    // Variável para configurar a opção SO_REUSEADDR
    int one = 1;

    // Estrutura para armazenar informações sobre o endereço do servidor
    struct sockaddr_in svr_addr;

    // Cria um novo socket usando o protocolo TCP (SOCK_STREAM)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Can't open socket");
        exit(1);
    }

    // Configura a opção SO_REUSEADDR para permitir a reutilização do endereço local
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    // Define o número da porta a ser usada
    int port = 3080;

    // Configura a estrutura do endereço do servidor
    svr_addr.sin_family = AF_INET;           // Família de endereços IPv4
    svr_addr.sin_addr.s_addr = INADDR_ANY;   // Aceita conexões em qualquer endereço disponível na máquina
    svr_addr.sin_port = htons(port);         // Converte o número da porta para o formato de rede (big-endian)

    // Associa o socket a um endereço e porta específicos
    if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) == -1)
    {
        // Fecha o socket em caso de erro ao associar
        close(sock);
        perror("Can't bind");
        exit(1);
    }

    // Coloca o socket em modo de escuta, permitindo que aceite conexões de clientes
    listen(sock, 5);
}