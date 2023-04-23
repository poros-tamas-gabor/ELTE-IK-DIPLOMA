#include "pch.h"
#include "CppUnitTest.h"
#include "../TerrainRender/Model/Persistence/FileDataAccessAsync.cpp"
#include "../TerrainRender/Model/Persistence/ReadSTLChunk.cpp"
#include "../TerrainRender/Model/Persistence/DataAccess.h"
#include "../TerrainRender/ErrorHandler.cpp"
#include "../TerrainRender/StringConverter.cpp"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TerrainRenderModelUnitTest
{
	TEST_CLASS(FileDataAccessAsyncUnitTest)
	{
	public:
		TEST_METHOD(LoadConfigFile)
		{
			IDataAccessPtr persistence = std::make_shared<BinaryFileDataAccessAsync>();
			ParameterFile file;
			auto func = [persistence, &file] { persistence->LoadConfigurationFile(L"invalid", file); };

			TCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);
			
			std::wstring path(buffer);
			path += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\testConfig.json";
			persistence->LoadConfigurationFile(path.c_str(), file);
			Assert::AreEqual(file.origo.latitude, 47.497913);
			Assert::AreEqual(file.origo.longitude, 19.040236);

			Assert::IsFalse(file.terrain.colors.empty());
		}
	};
}