#include <curl/curl.h>
#include <json.hpp>
#include <Windows.h>
#include <string>
#include <format>
#include <iostream>

using json = nlohmann::json;

// [STEP.1.5] Shift-JIS環境での日本語対応用ヘルパー
std::wstring ConvertString(const std::string& s) {
    int len = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
    std::wstring ws(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), &ws[0], len);
    return ws;
}

void WriteUtf8(const std::string& s) {
    std::wstring ws = ConvertString(s);
    DWORD _;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), ws.c_str(), (DWORD)ws.size(), &_, nullptr);
}

// [STEP.3] レスポンスをstd::stringに格納するためのコールバック関数
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t totalSize = size * nmemb;
    s->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    // [STEP.1] curlの初期化
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();

    if (curl) {
        WriteUtf8("curl 初期化成功\n");

        // [STEP.2] URLを設定しGET通信を送る
        // 課題内容: WebAPIからデータを1件取得 (例: faculties/2)
        std::string url = "http://localhost:3000/faculties/2";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // [STEP.3] レスポンスをstd::stringに格納
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // 通信実行
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            try {
                // [STEP.4] JSONのパース
                json data = json::parse(response);

                // [STEP.5] データを表示
                if (data.is_object()) {
                    int id = data["id"].get<int>();
                    std::string name = data["name"].get<std::string>();
                    
                    // std::formatを用いた出力
                    WriteUtf8(std::format("ID: {}, Name: {}\n", id, name));
                } else {
                    WriteUtf8("取得したデータがオブジェクト形式ではありません。\n");
                    WriteUtf8("レスポンス内容: " + response + "\n");
                }

            } catch (const std::exception& e) {
                WriteUtf8("JSON解析エラー: " + std::string(e.what()) + "\n");
                WriteUtf8("レスポンス内容: " + response + "\n");
            }
        } else {
            WriteUtf8("通信失敗: " + std::string(curl_easy_strerror(res)) + "\n");
        }

        // [STEP.1] クリーンアップ
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
