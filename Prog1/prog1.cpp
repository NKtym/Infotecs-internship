#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket"

typedef void (*SortFunc)(std::string&);
typedef int (*SumFunc)(const std::string&);

SortFunc descendSortAndReplacement;
SumFunc sumNum;

void load_library() {
    void* handle = dlopen("../Lib/libmylib.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Ошибка загрузки библиотеки\n";
        exit(1);
    }
    descendSortAndReplacement = (SortFunc)dlsym(handle, "descendSortAndReplacement");
    sumNum = (SumFunc)dlsym(handle, "sumNum");
    if (!descendSortAndReplacement || !sumNum) {
        std::cerr << "Ошибка загрузки функций\n";
        dlclose(handle);
        exit(1);
    }
}

class Prog1 {
private:
    int client = socket(AF_UNIX, SOCK_STREAM, 0);
    std::queue<std::string> buf;
    std::mutex mtx;
    std::condition_variable cv;
    bool connected = false;

void connectProg2(){
    while (true) {
        if (client < 0) {
            std::cerr << "Ошибка: Cокет не был создан\n";
            break;
        }
        sockaddr_un serverAddress{};
        serverAddress.sun_family = AF_UNIX;
        strcpy(serverAddress.sun_path, SOCKET_PATH);
        if (connect(client, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            std::cerr << "Ошибка: Неудалось получить связь со второй программой, произвожу повтороную попытку подключение\n";
            close(client);
            sleep(10);
            client = socket(AF_UNIX, SOCK_STREAM, 0);
            continue;
        }
        connected = true;
        std::cout << "Успешное подключение ко второй программе\n";
        break;
    }    
}

    void sendData(const std::string& data) {
        if (!connected) {
            connectProg2();
        }
        if (send(client, data.c_str(), data.size(), 0) < 0) {
            std::cerr << "Ошибка: Отправка данных не удалась\n";
            connected = false;
            close(client);
        }
    }

public:

void thr1(){
    while (true) {
        std::string in;
        std::cout << "Введите строку (только цифры, кол-во символов не привышает 64): ";
        std::cin >> in;
        if (in.length() > 64){
            std::cerr << "Ошибка: Cтрока длиннее 64 символов\n";
            continue;
        }
        for (size_t i = 0; i < in.length(); i++) {
            char ch = in[i];
            if (ch < '0' || ch > '9') {
                std::cerr << "Ошибка: Cтрока содержать не только цифры\n";
                continue;
            }
        }
        descendSortAndReplacement(in);
        {
            std::unique_lock<std::mutex> lock(mtx);
            buf.push(in);
            cv.notify_one();
        }
        sleep(1);
    }
}

void thr2(){
    while (true) {
        std::string data;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !buf.empty(); });
            data = buf.front();
            buf.pop();
        }
        std::cout << "Обработанные данные: " << data << std::endl;
        int sum = sumNum(data);
        std::cout << "Сумма цифр: " << sum << std::endl;
        sendData(data);
    }
}
};

int main() {
    load_library();
    Prog1 prog1;
    std::thread t1(&Prog1::thr1, &prog1);
    std::thread t2(&Prog1::thr2, &prog1);
    t1.join();
    t2.join();
    return 0;
}