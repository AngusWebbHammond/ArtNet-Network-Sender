#pragma once

#include "../Packets/Packets.h"

#include <string>
#include <chrono>

// Platform-specific includes
#ifdef PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif // PLATFORM_WINDOWS

#ifdef PLATFORM_LINUX
#endif // PLATFORM_LINUX

#ifdef PLATFORM_MAC
#endif // PLATFORM_MAC

namespace ArtNetSender {
	class ArtNetSocket {
	public:
		ArtNetSocket();
		ArtNetSocket(std::string ip, std::string subnetMask);
		~ArtNetSocket();

		ArtNetSocket(ArtNetSocket&) = delete;
		ArtNetSocket(ArtNetSocket&&) = delete;

		void sendPacket(ArtNetDMXPacket& packet);
		void sendPacketForDuration(ArtNetDMXPacket& packet, std::uint32_t durationMilliseconds);

	private:
		std::string m_ip;
		int m_port = 6454;
		std::string m_subnetMask;

		std::chrono::steady_clock::time_point m_lastSend = std::chrono::steady_clock::now();
		const std::chrono::milliseconds m_minInterval{ 25 };

#ifdef PLATFORM_WINDOWS
		WSADATA m_wsaData;
		int m_iResult;

		SOCKET m_socket;

		sockaddr_in m_destAddr = {};
#else
		static_assert(false, "Network implementation missing for this platform, in file ArtNetSocket.h");
#endif // PLATFORM_WINDOWS
	};
}
