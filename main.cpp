
#include"myClass/Converter/TextureConverter.h"

#include<cstdio>
#include<cstdlib>
#include <combaseapi.h>
#include <cassert>
#include <Windows.h>

enum  Argument {
	kApplicationPath,
	kFilePath,
	NumArgument
};

int main(int argc, char* argv[]) {
	HRESULT hr= CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	if (argc < NumArgument) {
		// 使用方法を出力
		TextureConverter::OutputUsage();
		return 0;
	}

	// オプションの数
	int numOptions = argc - NumArgument;
	// オプション配列
	char** options = argv+NumArgument;

	TextureConverter converter;
	converter.ConvertTextureWICToDDS(argv[kFilePath],numOptions,options);

	// COM ライブラリの終了
	CoUninitialize();

	//system("pause");
	return 0;
}