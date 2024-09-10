#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include<future>

using namespace std;

std::mutex socketMutex;
void receiveMessages(sf::TcpSocket* clientSocket) {
    sf::Packet packet;
    std::string message;
    while (true) {
        if (clientSocket->receive(packet) == sf::Socket::Done) {
            packet >> message;
            std::cout << "Received: " << message << std::endl;
            packet.clear();
        }
    }
}

void sendMessages(sf::TcpSocket* clientSocket) {
    sf::Packet packet;
    std::string message;
    while (true) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            std::cout << "Send: ";
            std::cin >> message;
            packet.clear();
            packet << message;
            if (clientSocket->send(packet) != sf::Socket::Done) {
                std::cout << "Error sending" << std::endl;
            }
        }
    }
}
void handleClient(sf::TcpSocket* clientSocket) {
    auto receiveFuture = std::async(std::launch::async, receiveMessages, clientSocket);
    auto sendFuture = std::async(std::launch::async, sendMessages, clientSocket);

    receiveFuture.get();
    sendFuture.get();
}

int main() {
    sf::IpAddress addr = sf::IpAddress::getLocalAddress();
    std::vector<std::thread> clientsThr;
    std::vector<sf::TcpSocket*> sockets;
    sf::TcpListener listener;

    if (listener.listen(50000) != sf::Socket::Done) {
        std::cout << "Error starting listener\n";
        return -1;
    }

    while (true) {
        sf::TcpSocket* newClient = new sf::TcpSocket;
        if (listener.accept(*newClient) == sf::Socket::Done) {
            {
                sockets.push_back(newClient);
            }
            clientsThr.push_back(std::thread(handleClient, newClient));
        }
        else {
            delete newClient;
        }
    }

    for (auto& thread : clientsThr) {
        if (thread.joinable())
            thread.join();
    }

    for (auto& socket : sockets) {
        delete socket;
    }

    return 0;
}



