#include "pch.h"
#include "Main.h"

#include "Resources/AssetSerial.h"
#include "Resources/FontSegment.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	std::cout << "AssetBase v0.2.1\n";

	std::ifstream resfiles("./resfiles.txt");
	if (!resfiles.is_open())
	{
		std::cout << "Error: Make sure you have resfiles.txt in the current working directory." << std::endl;
		return EXIT_FAILURE;
	}

	AssetBase::FontSegment::Init();

	std::string xml_path, lab_path;
	while (resfiles >> xml_path >> lab_path)
	{
		try {
			LoadAssetBase(xml_path, lab_path + "-debug", true);
			LoadAssetBase(xml_path, lab_path, false);
		}
		catch (const char* msg) {
			std::cout << msg << std::endl;
			break;
		}
	}

	AssetBase::FontSegment::Shutdown();

	return EXIT_SUCCESS;
}

void LoadAssetBase(const std::string& xml_path, const std::string& lab_path, bool debugMode)
{
	std::cout << std::string(32, '-') << "\nLoading " << xml_path << (debugMode ? " in debug mode" : " in release mode") << "...\n";
	AssetBase::AssetSerial serial(xml_path, debugMode);

	std::ofstream outFile(std::string(lab_path), std::ios::out | std::ios::trunc | std::ios::binary);

	std::cout << "\nWriting " << serial.GetBufferSize() << " bytes to file " << lab_path << "...\n";
	outFile << serial;
	outFile.close();
}