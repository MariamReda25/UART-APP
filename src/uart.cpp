#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>

#define SERIAL_PORT "/dev/ttyAMA2"
#define AI_FILE     "/home/root/main_app/ai.txt"

// Send a message to ESP32 via UART
void sendMessageToESP32(const std::string &message) {
    int serialPort = open(SERIAL_PORT, O_WRONLY | O_NOCTTY);
    if (serialPort == -1) {
        std::cerr << "❌ Error: Cannot open UART2!" << std::endl;
        return;
    }

     struct termios options;
    tcgetattr(serialPort, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag = CS8 | CLOCAL | CREAD;
    tcsetattr(serialPort, TCSANOW, &options);
    
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

        if (!aiMessage.empty()) {
             sendMessageToESP32(aiMessage);
      }
   
     

        if (aiMessage.empty()) {
            std::cout << "🔍 No alerts detected. Waiting..." << std::endl;
        }


    return 0;
}
