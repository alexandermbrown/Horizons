#pragma once

#include "ConfigStore.h"

#include <string>
#include <istream>
#include <filesystem>

class ConfigLoader
{
public:
	void RegisterDefaults(const std::string& defaults_path);

	ConfigStore LoadStore(const std::filesystem::path& store_path) const;
	void SaveStore(const ConfigStore& store, const std::filesystem::path& store_path) const;

private:
	void LoadStoreStream(ConfigStore& store, std::istream& stream) const;

	std::vector<ConfigStore::DefaultVar> m_Defaults;
};
