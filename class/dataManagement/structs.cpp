std::string alert;

// Definição da estrutura 'client' com campos para Nome, Email, Numero e Classificacao.
struct client
{
    std::string Nome;
    std::string Email;
    std::string Numero;
    std::string Classificacao;
};

// Declaração de uma string para armazenar a lista de clientes.
std::string listofClients;

struct product
{
    std::string Origem;
    std::vector<std::string> Descrição;
    std::string Foto;
    std::string Produto;
    std::string Preço;
    std::string Quantidade;
    std::string Tempo;
};

std::string listofProducts;

struct request
{
    std::string Cliente;
    std::vector<std::string> Produto;
    std::vector<std::string> Quantidade;
    std::vector<std::string> Observação;
    std::string Data;
    std::string Hora;
    std::string Preco;
    std::string Senha;
    std::string Atendimento;
};

std::string listofRequests;
std::string listofHistorys;
std::string searchValue;

// Definição de uma estrutura para representar uma fila
struct Fila
{
    std::size_t capacidade; // Capacidade máxima da fila
    std::vector<int> dados; // Vetor para armazenar os dados da fila
    std::size_t primeiro;   // Índice do primeiro elemento na fila
    std::size_t ultimo;     // Índice do último elemento na fila
    std::size_t nItens;     // Número atual de elementos na fila

    // Construtor padrão da fila, inicializando valores padrão
    Fila() : capacidade(200), dados(capacidade), primeiro(0), ultimo(-1), nItens(0) {}

    // Destrutor padrão da fila (o vetor é automaticamente liberado quando sai do escopo)
    ~Fila() {}
};

std::string senhaChamada;
int programSenha = 0;
int atualSenha = 0;

std::string createStringList(const std::vector<std::string> &observacaoList)
{
    std::string observacao;
    for (size_t i = 0; i < observacaoList.size(); i++)
    {
        if (i > 0)
        {
            observacao += ";";
        }
        observacao += observacaoList[i];
    }
    return observacao;
}

// Função auxiliar para dividir uma string com um delimitador
std::vector<std::string> splitString(const std::string &input, char delimiter)
{
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, delimiter))
    {
        result.push_back(token);
    }
    return result;
}