#include <iostream>      // Для std::cout и std::cerr
#include <fstream>      // Для std::ofstream
#include <string>       // Для std::string
#include <unistd.h>     // Для getuid()
#include <pwd.h>        // Для getpwuid()
#include <filesystem>   // Для std::filesystem
#include <vector>       // Для std::vector
#include <cerrno>       // Для errno
#include <cstring>      // Для strerror()

int main() {
    // Получаем информацию о пользователе
    struct passwd *pw = getpwuid(getuid());
    std::string homeDir = pw->pw_dir; // Директория домашнего пользователя

    // Проверяем наличие папки "Desktop" или "Рабочий стол"
    std::string desktopPath;
    std::vector<std::string> possibleNames = {"Desktop", "Рабочий стол"};

    for (const auto& name : possibleNames) {
        std::filesystem::path path = std::filesystem::path(homeDir) / name;
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            desktopPath = path.string();
            break; // Выходим из цикла, если нашли подходящую папку
        }
    }

    // Проверяем, нашли ли мы папку рабочего стола
    if (desktopPath.empty()) {
        std::cerr << "Не удалось найти папку рабочего стола." << std::endl;
        return 1; // Выход с ошибкой
    }

    // Имя файла
    std::string fileName = "hello_from_dummy.txt"; // Добавим расширение .txt для удобства

    // Полный путь к файлу
    std::string filePath = desktopPath + "/" + fileName;

    // Создаем пустой файл
    std::ofstream outFile(filePath);
    if (outFile) {
        std::cout << "Пустой файл '" << fileName << "' успешно создан на рабочем столе." << std::endl;
    } else {
        std::cerr << "Не удалось создать файл: " << filePath << std::endl;
        std::cerr << "Ошибка: " << strerror(errno) << std::endl; // Выводим причину ошибки
    }

    outFile.close();
    return 0;
}
