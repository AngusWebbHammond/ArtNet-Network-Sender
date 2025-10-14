#pragma once

#include <cstdint>
#include <iostream>

#ifdef PLATFORM_WINDOWS
#include <winsock2.h>
#endif

namespace ArtNetSender {
	struct Packet {
	};

#pragma pack(push, 1)
	struct ArtNetDMXPacket {
		char id[8] = "Art-Net";
		std::uint16_t opcode = 0x5000;
		std::uint16_t protocolVersion = htons(14);
		std::uint8_t sequence = 1;
		std::uint8_t physical = 0;
		std::uint16_t universe = 0;
		std::uint16_t DMXLength = htons(512);
		std::uint8_t data[512]{};

		friend std::ostream& operator<<(std::ostream& os, const ArtNetDMXPacket& packet) {
			os << "ArtNetDMXPacket:\n";
			os << "  ID: " << std::string(packet.id, 8) << "\n";
			os << "  Opcode: 0x" << std::hex << ntohs(packet.opcode) << std::dec << "\n";
			os << "  Protocol Version: " << ntohs(packet.protocolVersion) << "\n";
			os << "  Sequence: " << static_cast<int>(packet.sequence) << "\n";
			os << "  Physical: " << static_cast<int>(packet.physical) << "\n";
			os << "  Universe: " << ntohs(packet.universe) << "\n";
			os << "  DMX Length: " << ntohs(packet.DMXLength) << "\n";
			os << "  Data: [";
			for (int i = 0; i < ntohs(packet.DMXLength); ++i) {
				int j = i;
				os << static_cast<int>(packet.data[j]);
				if (i < ntohs(packet.DMXLength) - 1) {
					os << ", ";
				}
			}
			os << "]" << std::endl;
			return os;
		}
	};
#pragma pack(pop)
}
