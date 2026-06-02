#include <iostream>
#include <chrono>
#include <string>
#include <windows.h>
#include "myClass/httpClient/httpClient.h"


std::wstring ConvertString(const std::string& s) {
    int len = MultiByteToWideChar(
        CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
    std::wstring ws(len, 0);
    MultiByteToWideChar(
        CP_UTF8, 0, s.data(), (int)s.size(), &ws[0], len);
    return ws;
}

void WriteUtf8(const std::string& s) {
    std::wstring ws = ConvertString(s);
    DWORD _;
    WriteConsoleW(GetStdHandle(
        STD_OUTPUT_HANDLE), ws.c_str(),
        (DWORD)ws.size(), &_, nullptr);
}

int main() {

    //  IDを指定してデータを更新
    int updateId = 4;
    std::string newName = "ゲームデザイン科";
    auto patchFuture = PatchFacultyAsync(updateId, newName);

    while (true) {
        // 非同期処理の完了チェック
        if (patchFuture.valid() &&
            patchFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            
            std::string res = patchFuture.get();
            WriteUtf8("更新結果:\n" + res + "\n\n");

            // データの全件表示
            auto getFuture = GetAllFacultiesAsync();
            std::string all = getFuture.get();
            WriteUtf8("現在の全学科一覧:\n" + all + "\n");

            break; // 終了
        }
    }

    return 0;
}
