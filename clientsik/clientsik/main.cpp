#include<iostream>
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<thread>

using namespace sf;

void receiveMessages(sf::TcpSocket* socket) {
	sf::Packet packet;
	std::string message;
	while (true) {
		if (socket->receive(packet) == sf::Socket::Done) {
			packet >> message;
			std::cout << "Recived: " << message << std::endl;
			packet.clear();
		}
	}
}

int main() {

	sf::TcpSocket socket;
	sf::IpAddress addr = sf::IpAddress::getLocalAddress();
	if (socket.connect(addr, 50000) != sf::Socket::Done) {
		std::cout << "Error connection!" << std::endl;
	}
	sf::Packet packet;
	std::string massage;
	socket.setBlocking(false);
	std::thread receiveThread(receiveMessages, &socket);

	while (true) {
		if (sf::Keyboard::isKeyPressed(Keyboard::Enter))
			packet.clear();
		std::string newMassage;
		std::cout << "Send: ";
			std::cin >> newMassage;
			massage += newMassage;
		    packet << massage;
		if (socket.send(packet) != sf::Socket::Done) {
			std::cout << "Error sending" << std::endl;
		}
		massage = "";
	}
}