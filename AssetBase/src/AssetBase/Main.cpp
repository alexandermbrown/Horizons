#include "pch.h"

#include "Resources/AssetSerial.h"
#include "Resources/FontSegment.h"
#include "yaml-cpp/yaml.h"

#include <stdlib.h>

void LoadAssetBase(const std::string& xml_path, const std::string& lab_path, bool debug_mode);

int main(int argc, char* argv[])
{
	std::cout << "AssetBase v0.3.0\n";

	std::ifstream resfiles("./resfiles.txt");
	if (!resfiles.is_open())
	{
		std::cout << "Error: Make sure you have resfiles.txt in the current working directory." << std::endl;
		return EXIT_FAILURE;
	}

	AssetBase::InitFreetype();

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
		catch (std::runtime_error error) {
			std::cout << error.what() << std::endl;
			break;
		}
		catch (YAML::Exception error) {
			std::cout << "[YAML] " << error.what() << std::endl;
			break;
		}
	}

	AssetBase::ShutdownFreetype();

	return EXIT_SUCCESS;
}

void LoadAssetBase(const std::string& xml_path, const std::string& lab_path, bool debug_mode)
{
	std::cout << std::string(32, '-') << "\nLoading " << xml_path << (debug_mode ? " in debug mode" : " in release mode") << "...\n";
	AssetBase::AssetSerial serial(xml_path, debug_mode);

	std::ofstream out_file(std::string(lab_path), std::ios::out | std::ios::trunc | std::ios::binary);

	std::cout << "\nWriting " << serial.GetBufferSize() << " bytes to file " << lab_path << "...\n";
	out_file << serial;
	out_file.close();
}
