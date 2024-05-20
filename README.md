# ProjectRestaurantWebCPP

## Autor

MatheusGomes58

## Turma

Engenharia da Computação

## Turno

Segundo semestre

## Disciplina

Algoritmos e Programação II – Prof. Celso

---

## Descrição do Projeto

O **ProjectRestaurantWebCPP** é um programa desenvolvido em linguagem C para gerenciar um restaurante/fast food. Este sistema tem como objetivo principal automatizar o processo de pedidos, gerenciamento de estoque e registro de histórico de vendas, proporcionando uma experiência eficiente tanto para os clientes quanto para os funcionários do restaurante.

## Requisitos do Sistema

### Funcionalidades Básicas (0,5)

- **Pratos/Lanches:**
  - 5 opções de pratos/lanches.
  
- **Bebidas:**
  - 5 opções de bebidas.
  
- **Sobremesas:**
  - 5 opções de sobremesas.

### Banco de Dados (Arquivos) (0,5)

- **Produtos:**
  - Informações armazenadas: item, quantidade, preço.
  
- **Histórico dos Produtos:**
  - Informações armazenadas: senha, data, hora do pedido, descrição, valor.

### Funcionalidades do Programa e Requisitos Mínimos (0,5)

- **Menu Principal:**
  - Apresentação de um menu interativo para o usuário.
  
- **Pedido de Cliente:**
  - Exibição dos itens selecionados.
  - Cálculo do valor total do pedido.
  - Geração de senha (normal ou preferencial).
  - Alteração da quantidade de produtos disponíveis (diferencial 0,5).

- **Chamada de Senhas:**
  - Sistema de chamada de senhas para organizar a entrega dos pedidos.

- **Edição do Banco de Dados de Produtos:**
  - Capacidade de editar as informações dos produtos: item, quantidade, preço.

- **Consulta ao Histórico:**
  - Visualização do histórico de pedidos.
  - Consulta por data (diferencial 0,5).

---

## Estrutura de Arquivos

- **main.c:** Código fonte principal do programa.
- **produtos.txt:** Banco de dados de produtos, contendo item, quantidade e preço.
- **historico.txt:** Banco de dados do histórico de produtos, contendo senha, data, hora do pedido, descrição e valor.
- **README.md:** Documento de descrição do projeto.

---

## Como Executar

1. Clone este repositório:
   ```
   git clone https://github.com/MatheusGomes58/ProjectRestaurantWebCPP.git
   ```

2. Navegue até o diretório do projeto:
   ```
   cd ProjectRestaurantWebCPP
   ```

3. Compile o programa:
   ```
   gcc main.c -o restaurante
   ```

4. Execute o programa:
   ```
   ./restaurante
   ```

---

## Utilização

1. **Menu Principal:**
   - Escolha entre fazer um pedido, chamar uma senha, editar o banco de dados ou consultar o histórico.

2. **Fazer um Pedido:**
   - Selecione os itens desejados (pratos/lanches, bebidas, sobremesas).
   - O programa exibirá o valor total e gerará uma senha.

3. **Chamada de Senhas:**
   - O sistema chamará as senhas na ordem correta.

4. **Editar Banco de Dados:**
   - Altere as informações dos produtos, incluindo quantidade e preço.

5. **Consulta ao Histórico:**
   - Visualize o histórico de pedidos realizados, com a opção de filtragem por data.

---

## Contribuições

Para contribuir com este projeto, por favor siga os passos abaixo:

1. Faça um fork deste repositório.
2. Crie uma branch com a sua feature: `git checkout -b minha-feature`.
3. Commit suas mudanças: `git commit -m 'Minha nova feature'`.
4. Faça um push para a branch: `git push origin minha-feature`.
5. Envie um Pull Request.

---

## Licença

Este projeto está licenciado sob a licença MIT. Para mais detalhes, consulte o arquivo LICENSE.

---

## Contato

Para mais informações, entre em contato com MatheusGomes58 através do [GitHub](https://github.com/MatheusGomes58).

---
