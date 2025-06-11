#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>

#define SERIAL_PORT "/dev/ttyAMA2"
#define AI_FILE     "/home/root/main_app/ai.txt"
#define GPS_FILE    "/home/root/main_app/gps.txt"

// Send a message to ESP32 via UART
void sendMessageToESP32(const std::string &message) {
    int serialPort = open(SERIAL_PORT, O_WRONLY | O_NOCTTY);
    if (serialPort == -1) {
        std::cerr << "❌ Error: Cannot open UART2!" << std::endl;
        return;
    }
    write(serialPort, message.c_str(), message.size());
    write(serialPort, "\n", 1);

    std::cout << "📡 Sent to ESP32: " << message << std::endl;
    close(serialPort);
}

// Read the entire file content (single-line or multi-line)
std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath);
    std::string content;

    if (file.is_open()) {
        std::getline(file, content);  // Only first line
        file.close();
    } else {
        std::cerr << "⚠️ Error: Cannot open file " << filePath << std::endl;
    }

    return content;
}

int main() {
    std::cout << "✅ AI & GPS Notification Monitor Started..." << std::endl;

        std::string aiMessage = readFile(AI_FILE);
        std::string gpsMessage = readFile(GPS_FILE);

        if (!aiMessage.empty()) {
             size_t commaPos = aiMessage.find(',');
             std::string aiShortMessage = (commaPos != std::string::npos) ? aiMessage.substr(0, commaPos) : aiMessage;
             sendMessageToESP32(aiShortMessage);
      }
   
        if (!gpsMessage.empty()) {
            sendMessageToESP32(gpsMessage);
        }

        if (aiMessage.empty() && gpsMessage.empty()) {
            std::cout << "🔍 No alerts detected. Waiting..." << std::endl;
        }


    return 0;
}
