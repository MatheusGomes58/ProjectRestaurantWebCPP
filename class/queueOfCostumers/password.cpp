#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include <cstdlib>

#include "../../libraries/json.hpp"
using json = nlohmann::json;

std::queue<std::string> normalQueue;
std::queue<std::string> preferredQueue;
std::queue<std::string> historyQueue;
std::string lastDisplayedTicket;
int ticketNumber = 1;

void saveTicketToFile(const std::string &fileName, const std::queue<std::string> &queue) {
    std::ofstream file(fileName);
    if (file.is_open()) {
        std::queue<std::string> tempQueue = queue;
        while (!tempQueue.empty()) {
            file << tempQueue.front() << "\n";
            tempQueue.pop();
        }
        file.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo " << fileName << std::endl;
    }
}

void loadTicketsFromFile(const std::string &fileName, std::queue<std::string> &queue) {
    std::ifstream file(fileName);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            queue.push(line);
        }
        file.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo " << fileName << std::endl;
    }
}

void generateTicket(const std::string &customerName, bool isPreferred) {
    std::string queueType = isPreferred ? "Preferred" : "Normal";
    std::string ticket = "Ticket " + std::to_string(ticketNumber) + " (" + queueType + "): " + customerName;

    // Verifica se a senha já existe
    std::queue<std::string> &currentQueue = isPreferred ? preferredQueue : normalQueue;
    std::queue<std::string> tempQueue = currentQueue;

    bool ticketExists = false;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == ticket) {
            ticketExists = true;
            break;
        }
        tempQueue.pop();
    }

    // Verifica se a senha já existe no histórico
    tempQueue = historyQueue;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == ticket) {
            ticketExists = true;
            break;
        }
        tempQueue.pop();
    }

    if (ticketExists) {
        std::cout << "Senha repetida. Gerando nova senha.\n";
        ticketNumber++;
        generateTicket(customerName, isPreferred);
        return;
    }

    currentQueue.push(ticket);
    historyQueue.push(ticket);

    // Salva no arquivo correspondente
    std::string fileName = isPreferred ? "preferred_tickets.txt" : "normal_tickets.txt";
    saveTicketToFile(fileName, currentQueue);

    // Salva no arquivo de histórico
    saveTicketToFile("history_tickets.txt", historyQueue);

    lastDisplayedTicket = ticket;

    std::cout << "Generated: " << ticket << std::endl;
    ticketNumber++;
}

void callNext() {
    if (!preferredQueue.empty()) {
        std::string nextCustomer = preferredQueue.front();
        preferredQueue.pop();
        std::cout << "Calling Next (Preferred): " << nextCustomer << std::endl;
    } else if (!normalQueue.empty()) {
        std::string nextCustomer = normalQueue.front();
        normalQueue.pop();
        std::cout << "Calling Next (Normal): " << nextCustomer << std::endl;
    } else {
        std::cout << "No customers in the queue.\n";
    }
}

void displayQueueStatus() {
    std::cout << "Preferred Queue Size: " << preferredQueue.size() << std::endl;
    std::cout << "Normal Queue Size: " << normalQueue.size() << std::endl;
}

void displayAllTickets() {
    std::cout << "\nAll Tickets:\n";
    std::queue<std::string> tempPreferredQueue = preferredQueue;
    std::queue<std::string> tempNormalQueue = normalQueue;

    while (!tempPreferredQueue.empty()) {
        std::cout << "Preferred: " << tempPreferredQueue.front() << std::endl;
        tempPreferredQueue.pop();
    }

    while (!tempNormalQueue.empty()) {
        std::cout << "Normal: " << tempNormalQueue.front() << std::endl;
        tempNormalQueue.pop();
    }

    std::cout << "\nHistory Tickets:\n";
    std::queue<std::string> tempHistoryQueue = historyQueue;
    while (!tempHistoryQueue.empty()) {
        std::cout << "History: " << tempHistoryQueue.front() << std::endl;
        tempHistoryQueue.pop();
    }

    std::cout << "\nLast Displayed Ticket: " << lastDisplayedTicket << std::endl;
}

int main() {
    // Limpar o console
    std::system("clear");

    // Carregar senhas dos arquivos ao iniciar
    loadTicketsFromFile("preferred_tickets.txt", preferredQueue);
    loadTicketsFromFile("normal_tickets.txt", normalQueue);
    loadTicketsFromFile("history_tickets.txt", historyQueue);

    while (true) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Generate Ticket (Preferred)\n";
        std::cout << "2. Generate Ticket (Normal)\n";
        std::cout << "3. Call Next\n";
        std::cout << "4. Display Queue Status\n";
        std::cout << "5. Display All Tickets\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1 || choice == 2) {
            std::string customerName;
            std::cout << "Enter customer name: ";
            std::cin >> customerName;
            generateTicket(customerName, choice == 1);
        } else if (choice == 3) {
            callNext();
        } else if (choice == 4) {
            displayQueueStatus();
        } else if (choice == 5) {
            displayAllTickets();
        } else if (choice == 6) {
            break;
        } else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
