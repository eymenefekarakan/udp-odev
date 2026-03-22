#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h> 
#include <ws2tcpip.h>  

#pragma comment(lib, "ws2_32.lib")

class UDPListener {
private:
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    bool running;

public:
    UDPListener(const std::string& ip, int port) : running(false) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Winsock baslatilamadi!" << std::endl;
            exit(EXIT_FAILURE);
        }

        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd == INVALID_SOCKET) {
            std::cerr << "Socket olusturulamadi! Hata: " << WSAGetLastError() << std::endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        servaddr.sin_addr.s_addr = inet_addr(ip.c_str());

        if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
            std::cerr << "Bind hatasi! Hata: " << WSAGetLastError() << std::endl;
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
    }

    void start() {
        running = true;
        char buffer[2048];
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);

        std::cout << "14550 portu dinleniyor" << std::endl;

        while (running) {
            int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&cliaddr, &len);
            if (n > 0) {
                buffer[n] = '\0';
                std::cout << "Gelen Veri: " << buffer << std::endl;
            }
        }
    }

    ~UDPListener() {
        closesocket(sockfd);
        WSACleanup();
    }
};

int main() {
    UDPListener listener("127.0.0.1", 14550);
    listener.start();
    return 0;
}