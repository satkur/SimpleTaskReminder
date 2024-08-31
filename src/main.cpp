#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

int main() {
    // リマインダーメッセージを格納するファイルのパス
    const std::string filePath = "C:\\Users\\pathf\\Desktop\\reminder.txt";

    // ファイルが存在するか確認し、存在しない場合は新しいファイルを作成
    std::ifstream file(filePath);
    if (!file.good()) {
        // ファイルが存在しない場合、新しいファイルを作成
        std::ofstream outfile(filePath);
        outfile.close();
        file.open(filePath); // 新しく作成されたファイルを開く
    }

    // コンソール出力のエンコーディングをUTF8にする
    UINT previousCP = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);

    // ファイルからリマインダーメッセージを読み込む
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cout << "Unable to open file." << std::endl;
    }

    // Enterキーが押されるまで待機
    std::cin.get();

    SetConsoleOutputCP(previousCP);
    return 0;
}
