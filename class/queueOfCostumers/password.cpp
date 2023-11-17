struct Fila {
    int capacidade;
    int *dados;
    int primeiro;
    int ultimo;
    int nItens;
};
Fila umaFila;

void criarFila(Fila &f) {
    f.capacidade = 200;
    f.dados = new int[f.capacidade];
    f.primeiro = 0;
    f.ultimo = -1;
    f.nItens = 0;
}

void inserirNaFila(Fila &f, int v) {
    if (f.ultimo == f.capacidade - 1)
        f.ultimo = -1;

    f.ultimo++;
    f.dados[f.ultimo] = v;
    f.nItens++;
}

bool estaVazia(const Fila &f) {
    return (f.nItens == 0);
}

bool estaCheia(const Fila &f) {
    return (f.nItens == f.capacidade);
}


int removerDaFila(Fila &f) {
    int temp = f.dados[f.primeiro++];
    if (f.primeiro == f.capacidade)
        f.primeiro = 0;
    f.nItens--;

    return temp;
}


void mostrarFila(const Fila &f) {
    int cont, i;
    for (cont = 0, i = f.primeiro; cont < f.nItens; cont++) {
        std::cout << f.dados[i++] << "\t";
        if (i == f.capacidade)
            i = 0;
    }
    std::cout << "\n\n";
}

/*/
void displayMenu(Fila &umaFila) {
    system("clear");  // Modificado de "cls" para "clear"
    if (estaVazia(umaFila)) {
        std::cout << "\nFila vazia!!!\n\n";
    } else {
        std::cout << "\nConteudo da fila => ";
        mostrarFila(umaFila);
        std::cout << "\n\n";
    }

    std::cout << "1- Gerar senha normal\n";
    std::cout << "2- Gerar senha preferencial\n";
    std::cout << "3- Chamar\n";
    std::cout << "9- FIM\n";
}


int main() {
    do {
        displayMenu(umaFila);
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                if (estaCheia(umaFila)) {
                    std::cout << "\nFila Cheia!!!\n\n";
                    std::cout << "Tecle Enter";
                    std::cin.ignore();
                    std::cin.get();
                } else {
                    valor++;
                    enqueue(umaFila, valor);
                }
                break;
            case 2:
                // Implemente a lógica para gerar senhas preferenciais
                break;
            case 3:
                if (estaVazia(umaFila)) {
                    std::cout << "\nFila vazia!!!\n\n";
                } else {
                    dequeue(umaFila);
                    std::cout << "\n" << valor << " removido com sucesso\n\n";
                }
                break;
        }
    } while (opcao != EXIT_OPTION);

    delete[] umaFila.dados; // Não se esqueça de liberar a memória alocada
    return 0;
}
*/