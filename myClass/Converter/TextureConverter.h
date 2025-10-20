#pragma once
#include <string>
#include "../../externals/DirectXTex/DirectXTex.h"

class TextureConverter {
public:
	TextureConverter() = default;
	~TextureConverter() = default;

	void ConvertTextureWICToDDS(const std::string& filePath);
private:
	void LoadWICTextureFromFile(const std::string& filePath);
	static std::wstring ConvertMultiByteStringToWideString(std::string& mString);

private:
	DirectX::TexMetadata metadata_;
	DirectX::ScratchImage scratchImage_;
};