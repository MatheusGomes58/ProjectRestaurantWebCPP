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

// Função para criar uma fila com capacidade predefinida
void criarFila(Fila &f)
{
    f.capacidade = 200;
    f.dados.resize(f.capacidade);
    f.primeiro = 0;
    f.ultimo = -1;
    f.nItens = 0;
}

// Função para inserir um elemento na fila
void inserirNaFila(Fila &f, int v)
{
    // Verifica se a fila está cheia e ajusta o índice do último elemento
    if (f.ultimo == f.capacidade - 1)
        f.ultimo = -1;

    // Incrementa o índice do último e insere o valor na posição correspondente
    f.ultimo++;
    f.dados[f.ultimo] = v;
    f.nItens++;
}

// Função para verificar se a fila está vazia
bool estaVazia(const Fila &f)
{
    return (f.nItens == 0);
}

// Função para verificar se a fila está cheia
bool estaCheia(const Fila &f)
{
    return (f.nItens == f.capacidade);
}

// Função para remover um elemento da fila
int removerDaFila(Fila &f)
{
    // Armazena temporariamente o valor do primeiro elemento
    int temp = f.dados[f.primeiro++];

    // Verifica se o índice do primeiro atingiu a capacidade e ajusta se necessário
    if (f.primeiro == f.capacidade)
        f.primeiro = 0;

    // Decrementa o número de itens na fila
    f.nItens--;

    // Retorna o valor removido
    return temp;
}

// Função para mostrar os elementos da fila
void mostrarFila(const Fila &f)
{
    std::size_t cont, i;
    for (cont = 0, i = f.primeiro; cont < f.nItens; cont++)
    {
        // Imprime os elementos da fila, considerando a circularidade do vetor
        std::cout << f.dados[i++] << "\t";
        if (i == f.capacidade)
            i = 0;
    }
    std::cout << "\n\n";
}