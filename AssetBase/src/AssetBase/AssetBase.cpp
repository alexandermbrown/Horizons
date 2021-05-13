#include "pch.h"
#include "AssetBase.h"

#include "AssetPack.h"

AssetBase* AssetBase::s_Instance = nullptr;

AssetBase::AssetBase()
{
	if (s_Instance)
		throw "AssetBase singleton already instanitated.";
	s_Instance = this;

	std::cout << "-------------------\n AssetBase v0.4.0 \n-------------------\n";

	YAML::Node config = YAML::LoadFile("config.yaml");
	if (!config || !config.IsMap())
		throw "YAML file 'config.yaml' does not exist or is invalid.";

	YAML::Node packs = config["packs"];
	if (!packs || !packs.IsSequence())
		throw "config.yaml: cannot find sequence 'packs'.";

	
	YAML::Node includes = config["includes"];
	if (includes)
	{
		if (!includes.IsSequence())
			throw "config.yaml: includes must be a sequence of strings.";

		for (const YAML::Node& include : includes)
		{
			if (!include || !include.IsScalar())
				throw "config.yaml: include item must be scalar.";
			m_IncludePaths.push_back(include.Scalar());
		}
	}

	for (const YAML::Node& pack : packs)
	{
		YAML::Node yaml_path_node = pack[0];
		if (!yaml_path_node || !yaml_path_node.IsScalar())
			throw "config.yaml: invalid yaml path in pack.";
		std::string yaml_path = yaml_path_node.Scalar();

		YAML::Node out_path_node = pack[1];
		if (!out_path_node || !out_path_node.IsScalar())
			throw "config.yaml: invalid out path in pack.";
		std::string out_path = pack[1].Scalar();

		try {
			std::filesystem::path output_path = out_path;
			std::filesystem::create_directories(output_path.parent_path());
			CompileAssetPack(yaml_path, out_path + "-debug", true);
			CompileAssetPack(yaml_path, out_path, false);
		}
		catch (const char* msg) {
			std::cout << msg << std::endl;
			break;
		}
		catch (YAML::Exception error) {
			std::cout << "[YAML Exception] " << error.what() << std::endl;
			break;
		}
		catch (std::runtime_error error) {
			std::cout << error.what() << std::endl;
			break;
		}
	}
}

void AssetBase::CompileAssetPack(const std::string& yaml_path, const std::string& out_path, bool debug_mode)
{
	std::cout << std::string(32, '-') << "\nLoading " << yaml_path << (debug_mode ? " in debug mode" : " in release mode") << "...\n";
	AssetPack serial(yaml_path, debug_mode);

	std::ofstream out_file(out_path, std::ios::out | std::ios::trunc | std::ios::binary);

	std::cout << "\nWriting " << serial.GetBufferSize() << " bytes to file " << out_path << "...\n";
	out_file << serial;
	out_file.close();
}
