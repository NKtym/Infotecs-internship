#include <iostream>
#include <cstring>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../Lib/lib.hpp"

#define SOCKET_PATH "/tmp/socket"

class Program2 {
private:
    int server = socket(AF_UNIX, SOCK_STREAM, 0);
    void connectProg1() {
        if (server < 0) {
            std::cerr << "Ошибка: Cокет не был создан\n";
            exit(1);
        }
        sockaddr_un serverAddress{};
        serverAddress.sun_family = AF_UNIX;
        strcpy(serverAddress.sun_path, SOCKET_PATH);
        unlink(SOCKET_PATH);
        if (bind(server, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            std::cerr << "Ошибка: Cокет не был привязан\n";
            close(server);
            exit(1);
        }
        listen(server, 1);
        std::cout << "Ожидание подключения к первой программе...\n";
    }

public:
    void start() {
        connectProg1();
        while (true) {
            sockaddr_un clientAddress{};
            socklen_t clientAddressSize = sizeof(clientAddress);
            int client = accept(server, (sockaddr*)&clientAddress, &clientAddressSize);
            if (client < 0) {
                std::cerr << "Ошибка: Неудачное подключение\n";
                continue;
            }
            std::cout << "Успешное подключение к первой программе\n";
            char buf[128];
            while (true) {
                memset(buf, 0, sizeof(buf));
                int bytesReceived = recv(client, buf, sizeof(buf), 0);
                if (bytesReceived <= 0) {
                    std::cerr << "Отключенение от первой программы\n";
                    close(client);
                    break;
                }
                std::string data(buf, bytesReceived);
                if (strCheck(data)) {
                    std::cout << "Данные успешно получены: " << data << '\n';
                } else {
                    std::cerr << "Ошибка: Данные не прошли проверку, некорректные данные\n";
                }
            }
        }
    }
};

int main() {
    Program2 program2;
    program2.start();
    return 0;
}