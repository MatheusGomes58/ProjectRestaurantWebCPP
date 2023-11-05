void createConnection(int& sock) {
    int one = 1;
    struct sockaddr_in svr_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Can't open socket");
        exit(1);
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    int port = 3070;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) == -1) {
        close(sock);
        perror("Can't bind");
        exit(1);
    }

    listen(sock, 5);
}