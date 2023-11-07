#include <iostream>
#include <queue>
#include <string>

std::queue<std::string> normalQueue;
std::queue<std::string> preferredQueue;
int ticketNumber = 1;

void generateTicket(const std::string &customerName, bool isPreferred) {
    std::string queueType = isPreferred ? "Preferred" : "Normal";
    std::string ticket = "Ticket " + std::to_string(ticketNumber) + " (" + queueType + "): " + customerName;
    if (isPreferred) {
        preferredQueue.push(ticket);
    } else {
        normalQueue.push(ticket);
    }
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
}

int main() {
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
