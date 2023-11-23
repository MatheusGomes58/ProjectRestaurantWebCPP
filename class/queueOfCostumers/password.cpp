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