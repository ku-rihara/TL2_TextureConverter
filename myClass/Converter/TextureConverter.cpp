#include"TextureConverter.h"
#include <Windows.h>




void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath) {

	LoadWICTextureFromFile(filePath);
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	std::wstring wFilePath = ConvertMultiByteStringToWideString(const_cast<std::string&>(filePath));

	HRESULT hr = DirectX::LoadFromWICFile(
		wFilePath.c_str(),
		DirectX::WIC_FLAGS_NONE,
		&metadata_,
		scratchImage_
	);

	assert(SUCCEEDED(hr));
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(std::string& mString) {
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	//���C�h������
	std::wstring wString;
	wString.resize(filePathBufferSize);

	//���C�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}