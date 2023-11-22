struct Fila
{
    std::size_t capacidade;
    std::vector<int> dados;
    std::size_t primeiro;
    std::size_t ultimo;
    std::size_t nItens;

    Fila() : capacidade(200), dados(capacidade), primeiro(0), ultimo(-1), nItens(0) {}

    ~Fila()
    {
        // O vetor é automaticamente liberado quando sai do escopo
    }
};

void criarFila(Fila &f)
{
    f.capacidade = 200;
    f.dados.resize(f.capacidade);
    f.primeiro = 0;
    f.ultimo = -1;
    f.nItens = 0;
}

void inserirNaFila(Fila &f, int v)
{
    if (f.ultimo == f.capacidade - 1)
        f.ultimo = -1;

    f.ultimo++;
    f.dados[f.ultimo] = v;
    f.nItens++;
}

bool estaVazia(const Fila &f)
{
    return (f.nItens == 0);
}

bool estaCheia(const Fila &f)
{
    return (f.nItens == f.capacidade);
}

int removerDaFila(Fila &f)
{
    int temp = f.dados[f.primeiro++];
    if (f.primeiro == f.capacidade)
        f.primeiro = 0;
    f.nItens--;

    return temp;
}

void mostrarFila(const Fila &f)
{
    std::size_t cont, i;
    for (cont = 0, i = f.primeiro; cont < f.nItens; cont++)
    {
        std::cout << f.dados[i++] << "\t";
        if (i == f.capacidade)
            i = 0;
    }
    std::cout << "\n\n";
}
/*/
int main()
{
    

    // Teste das funções
    inserirNaFila(umaFila, 1);
    inserirNaFila(umaFila, 2);
    inserirNaFila(umaFila, 3);

    mostrarFila(umaFila);

    removerDaFila(umaFila);

    mostrarFila(umaFila);

    return 0;
}*/