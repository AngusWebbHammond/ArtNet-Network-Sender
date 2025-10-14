# ArtNet Network Sender

**ArtNet Network Sender** is a lightweight C++ application for sending Art-Net DMX packets over a network.
Designed to work on Windows (using Winsock2).

This project is licensed under the MIT License, see [LICENSE](/LICENSE).

---

## Features

- Sends **ArtDMX packets** over UDP to broadcast or specific Art-Net nodes.
- Supports **configurable IP address and subnet mask**.
- Rate-limited sending at **40 Hz** to ensure smooth DMX output.
- Uses standard **C++20** and no external libraries beyond Winsock2 on Windows.

---

## Build Instructions

### Prerequisites

- **Windows**
- **C++20 - compatible compiler**
- **Premake 5**
- **Visual Studio**

### Generating Project Files

1. Install **Premake5** and ensure it is in your **PATH**.
1. Generate a Visual Studio solution in the project root directory (example for VS 2022):

   ```bash
   premake5 vs2022
   ```

   If you are not sure how to do the above, the same can be achieved by running **setupWindows.bat**, for windows, more OS will be supported soon.

1. Open the generated project in Visual Studio.
1. Build in **Debug** or **Release** mode.

---

## Usage

Example usage of ArtNetSocket:

```c++
#include "ArtNetSocket.h"
#include "Packets/Packets.h"

int main() {
    ArtNetSender::ArtNetSocket sock = ArtNetSender::ArtNetSocket("2.2.2.2", "255.0.0.0"); // IP, Subnet Mask.
    ArtNetSender::ArtNetDMXPacket packet;

    for (int i = 0; i < 512; ++i) {
        packet.data[i] = i % 256; // Used to change the data of the packet.
    }

    sock.sendPacketForDuration(packet, 10000); // Sends a packet on the socket for a duration of 10000ms or 10s at a rate limit of 40hz.
    return 0;
}
```

---

## Future Improvements

- Implement threaded sending to allow updating DMX data while sending continuously.

- Add cross-platform network support for Linux and macOS.

- Support multiple universes and advanced Art-Net features.
