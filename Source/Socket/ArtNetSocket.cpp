#include "ArtNetSocket.h"
#include "Packets/Packets.h"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

// Platform-specific includes
#ifdef PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
static_assert(false, "Network implementation missing for this platform, in file ArtNetSocket.cpp");
#endif // PLATFORM_WINDOWS

namespace ArtNetSender {
	ArtNetSocket::ArtNetSocket()
	{
		m_ip = "";
		m_subnetMask = "";
	}

	ArtNetSocket::ArtNetSocket(std::string ip, std::string subnetMask)
	{
		m_ip = ip;
		m_subnetMask = subnetMask;
		m_port = 6454; // Art-Net default

#ifdef PLATFORM_WINDOWS
		m_socket = INVALID_SOCKET;

		m_iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (m_iResult != 0) {
			std::cerr << "WSAStartup failed with error: " << m_iResult << std::endl;
			return;
		}

		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_socket == INVALID_SOCKET) {
			std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return;
		}

		BOOL broadcast = TRUE;
		setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast));

		// Bind to your local network interface
		sockaddr_in localAddr = {};
		localAddr.sin_family = AF_INET;
		localAddr.sin_port = htons(0); // any port
		inet_pton(AF_INET, m_ip.c_str(), &localAddr.sin_addr);

		if (bind(m_socket, (sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
			std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		}

		// Destination: broadcast address for your subnet
		m_destAddr.sin_family = AF_INET;
		m_destAddr.sin_port = htons(m_port);
		inet_pton(AF_INET, "2.255.255.255", &m_destAddr.sin_addr);

#else
		static_assert(false, "Network implementation missing for this platform");
#endif

		std::cout << "ArtNetSocket created on " << m_ip
			<< ", subnet mask " << m_subnetMask
			<< ", port " << m_port << std::endl;
	}

	ArtNetSocket::~ArtNetSocket()
	{
		closesocket(m_socket);
		WSACleanup();
	}

	void ArtNetSocket::sendPacket(ArtNetDMXPacket& packet)
	{
#ifdef DEBUG
		std::cout << "Sending Packet..." << std::endl;
#endif // DEBUG

#ifdef PLATFORM_WINDOWS
		int sendResult = sendto(m_socket, reinterpret_cast<const char*>(&packet), sizeof(ArtNetDMXPacket), 0, (sockaddr*)&m_destAddr, sizeof(m_destAddr));
		if (sendResult == SOCKET_ERROR) {
			std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
			return;
		}
#else
		static_assert(false, "Network implementation missing for this platform, in file ArtNetSocket.cpp, function sendPacket");
#endif // PLATFORM_WINDOWS

		m_lastSend = std::chrono::steady_clock::now();

#ifdef DEBUG
		//std::cout << packet << std::endl;
		std::cout << "Packet sent successfully at " << duration_cast<std::chrono::milliseconds>(m_lastSend.time_since_epoch()).count() << std::endl;
#endif // DEBUG
	}

	void ArtNetSocket::sendPacketForDuration(ArtNetDMXPacket& packet, std::uint32_t durationMilliseconds)
	{
		auto startTime = std::chrono::steady_clock::now();
		auto endTime = startTime + std::chrono::milliseconds(durationMilliseconds);

		while (std::chrono::steady_clock::now() < endTime) {
			// Rate limiting (reuse sendPacket logic)
			auto now = std::chrono::steady_clock::now();
			auto elapsed = duration_cast<std::chrono::milliseconds>(now - m_lastSend);
			if (elapsed < m_minInterval) {
				std::this_thread::sleep_for(m_minInterval - elapsed);
			}

			sendPacket(packet);
		}
	}
}

int main() {
	ArtNetSender::ArtNetSocket sock = ArtNetSender::ArtNetSocket("2.2.2.2", "255.0.0.0");

	ArtNetSender::ArtNetDMXPacket packet;

	for (int i = 0; i < 512; ++i)
		packet.data[i] = i % 256;

	sock.sendPacketForDuration(packet, 10000);

	return 0;
}
