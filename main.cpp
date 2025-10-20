
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

	assert(argc >= NumArgument);

	TextureConverter converter;
	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	// COM ƒ‰ƒCƒuƒ‰ƒŠ‚ÌI—¹
	CoUninitialize();

	system("pause");
	return 0;
}