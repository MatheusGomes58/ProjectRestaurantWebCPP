#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Contato
{
    std::string name;
    std::string email;
    std::string telefone;
};

void adicionarContato(std::fstream &arquivo, const Contato &contato)
{
    arquivo << contato.name << "," << contato.email << "," << contato.telefone << "\n";
}

void listarContatos(std::fstream &arquivo)
{
    arquivo.clear();
    arquivo.seekg(0, std::ios::beg);
    Contato contato;
    while (std::getline(arquivo, contato.name, ',') &&
           std::getline(arquivo, contato.email, ',') &&
           std::getline(arquivo, contato.telefone))
    {
        std::cout << "Nome: " << contato.name << "\nEmail: " << contato.email << "\nTelefone: " << contato.telefone << "\n\n";
    }
}

void excluirContato(std::fstream &arquivo, const std::string &name)
{
    std::fstream temporario("temp.csv", std::ios::out);
    if (!temporario)
    {
        std::cout << "Erro ao criar arquivo temporário." << std::endl;
        return;
    }

    Contato contato;
    bool encontrado = false;
    arquivo.clear();
    arquivo.seekg(0, std::ios::beg);

    while (std::getline(arquivo, contato.name, ',') &&
           std::getline(arquivo, contato.email, ',') &&
           std::getline(arquivo, contato.telefone))
    {
        if (contato.name != name)
        {
            temporario << contato.name << "," << contato.email << "," << contato.telefone << "\n";
        }
        else
        {
            encontrado = true;
        }
    }

    temporario.close();
    arquivo.close();
    remove("contatos.csv");
    rename("temp.csv", "contatos.csv");

    if (!encontrado)
    {
        std::cout << "Contato não encontrado." << std::endl;
    }

    arquivo.open("contatos.csv", std::ios::app | std::ios::in | std::ios::out);
    if (!arquivo)
    {
        std::cout << "Não foi possível abrir o arquivo." << std::endl;
        return;
    }
}

int main()
{
    std::fstream arquivo("contatos.csv", std::ios::app | std::ios::in | std::ios::out);
    if (!arquivo)
    {
        std::cout << "Não foi possível abrir o arquivo." << std::endl;
        return 1;
    }

    Contato novoContato;
    int opcao;
    std::string nameExcluir; // Mova a declaração para fora do switch

    do
    {
        std::cout << "1. Adicionar Contato\n2. Listar Contatos\n3. Excluir Contato\n0. Sair\nEscolha uma opção: ";
        std::cin >> opcao;

        switch (opcao)
        {
        case 1:
            std::cout << "Nome: ";
            std::cin.ignore();
            std::getline(std::cin, novoContato.name);
            std::cout << "Email: ";
            std::getline(std::cin, novoContato.email);
            std::cout << "Telefone: ";
            std::getline(std::cin, novoContato.telefone);
            adicionarContato(arquivo, novoContato);
            break;
        case 2:
            listarContatos(arquivo);
            break;
        case 3:
            std::cout << "Nome do contato a ser excluído: ";
            std::cin >> nameExcluir;
            excluirContato(arquivo, nameExcluir);
            break;
        case 0:
            break;
        default:
            std::cout << "Opção inválida." << std::endl;
        }
    } while (opcao != 0);

    arquivo.close();
    return 0;
}
