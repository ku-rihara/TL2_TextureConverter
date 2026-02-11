#pragma once
#include <string>
#include "../../externals/DirectXTex/DirectXTex.h"

class TextureConverter {
public:
	TextureConverter() = default;
	~TextureConverter() = default;

	/// <summary>
	/// 使用方法を出力する
	/// </summary>
	static void OutputUsage();

	/// <summary>
	/// テクスチャをWIC形式からDDSに変換する
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="numOptions"></param>
	/// <param name="options"></param>
	void ConvertTextureWICToDDS(const std::string& filePath,int numOptions=0,char* options[]=nullptr);

private:
	void LoadWICTextureFromFile(const std::string& filePath);
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	void SaveDDSTextureToFile(int numOptions, char* options[]);
	void SeparateFilePath(const std::wstring& filePath);

private:
	DirectX::TexMetadata metadata_;
	DirectX::ScratchImage scratchImage_;

	std::wstring directoryPath_;
	std::wstring fileName_;
	std::wstring fileExt_;
};