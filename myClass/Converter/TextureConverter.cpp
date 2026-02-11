#include "TextureConverter.h"
#include <cassert>
#include <Windows.h>
#include <cstdio>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath, int numOptions, char* options[]) {

    LoadWICTextureFromFile(filePath);

    SaveDDSTextureToFile(numOptions, options);
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
    std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

    HRESULT hr = DirectX::LoadFromWICFile(
        wFilePath.c_str(),
        DirectX::WIC_FLAGS_NONE,
        &metadata_,
        scratchImage_);

    assert(SUCCEEDED(hr));
    hr;

    SeparateFilePath(wFilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
    int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

    // ワイド文字列
    std::wstring wString;
    wString.resize(filePathBufferSize);

    // ワイド文字列に変換
    MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

    return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {
    size_t pos1;
    std::wstring exceptExt;

    pos1 = filePath.rfind('.');
    if (pos1 != std::wstring::npos) {
        fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
        exceptExt = filePath.substr(0, pos1);
    } else {
        fileExt_ = L"";
        exceptExt = filePath;
    }

    pos1 = exceptExt.rfind('\\');
    if (pos1 != std::wstring::npos) {
        directoryPath_ = exceptExt.substr(0, pos1 + 1);
        fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
        return;
    }

    pos1 = exceptExt.rfind('/');
    if (pos1 != std::wstring::npos) {
        directoryPath_ = exceptExt.substr(0, pos1 + 1);
        fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
        return;
    }

    // 区切り文字がないのでファイル名のみ
    directoryPath_ = L"";
    fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[]) {
    HRESULT result;
    std::wstring filePath = directoryPath_ + fileName_ + L".dds";

    size_t mipLevel = 0;
    //ミップマップレベル指定を検索
    for (int i = 0; i < numOptions; ++i) {
        if (std::string(options[i]) == "-ml") {
            // ミップレベル指定
            mipLevel = std::stoi(options[i + 1]);
            break;
        }
    }

    DirectX::ScratchImage mipChain;
    // ミップマップ生成
    result = DirectX::GenerateMipMaps(
        scratchImage_.GetImages(), scratchImage_.GetImageCount(),
        scratchImage_.GetMetadata(),
        DirectX::TEX_FILTER_DEFAULT,
        mipLevel,
        mipChain);

    if (SUCCEEDED(result)) {
        scratchImage_ = std::move(mipChain);
        metadata_ = scratchImage_.GetMetadata();
    }

    DirectX::ScratchImage converted;
    result = DirectX::Compress(
        scratchImage_.GetImages(),
        scratchImage_.GetImageCount(),
        metadata_,
        DXGI_FORMAT_BC7_UNORM_SRGB,
        DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT | DirectX::TEX_COMPRESS_PARALLEL,
        1.0f,
        converted);

    if (SUCCEEDED(result)) {
        scratchImage_ = std::move(converted);
        metadata_ = scratchImage_.GetMetadata();
    }

    metadata_.format = DirectX::MakeSRGB(metadata_.format);

    // DDSファイル書き出し
    result = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(),
        metadata_,
        DirectX::DDS_FLAGS_NONE,
        filePath.c_str());

    assert(SUCCEEDED(result));
}


 void TextureConverter::OutputUsage() {
     printf("画像ファイルをWIC形式からDDS形式に変換します。\n");
     printf("/n");
     printf("TextureConverter[ドライブ:][パス][ファイル名]\n");
     printf("/n");
     printf("[ドライブ:][パス][ファイル名]: 変換したいWIC形式の画像ファイルを指定します。\n");
     printf("/n");
     printf("[-ml level] ミップマップレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します。\n");
}