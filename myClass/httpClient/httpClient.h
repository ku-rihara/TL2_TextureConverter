#pragma once
#include <string>
#include <future>

// 全件取得
std::future<std::string> GetAllFacultiesAsync();

// ID指定取得
std::future<std::string> GetFacultyByIdAsync(int id);

// 新規作成
std::future<std::string> PostFacultyAsync(const std::string& name);

// 更新
std::future<std::string> PatchFacultyAsync(int id, const std::string& newName);

// 削除
std::future<std::string> DeleteFacultyAsync(int id);
