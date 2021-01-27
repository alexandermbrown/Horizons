#include "pch.h"
#include "ConfigLoader.h"

#include "ConfigConvert.h"
#include "Lithium.h"

bool StringToBool(const std::string& value, bool default)
{
	if (value == "true")
		return true;
	else if (value == "false")
		return false;

	Li::Log::Error("Invalid boolean {}. Must be either true or false.", value);
	return default;
}

int StringToInt(const std::string & value, int default)
{
	try {
		return std::stoi(value);
	}
	catch (std::invalid_argument) {
		Li::Log::Error("Invalid integer {}.", value);
		return default;
	}
	catch (std::out_of_range) {
		Li::Log::Error("Integer {} out of range.", value);
		return default;
	}
}

float StringToFloat(const std::string & value, float default)
{
	try {
		return std::stof(value);
	}
	catch (const std::invalid_argument&) {
		Li::Log::Error("Invalid float {}.", value);
		return default;
	}
	catch (const std::out_of_range&) {
		Li::Log::Error("Float {} out of range.", value);
		return default;
	}
}

void ConfigLoader::RegisterDefaults(const std::string& defaults_path)
{
	YAML::Node defaults = YAML::LoadFile(defaults_path);

	for (const std::pair<YAML::Node, YAML::Node>& kvp : defaults)
	{
		ConfigStore::DefaultVar var;
		var.Name = kvp.first.Scalar();

		const std::string& type = kvp.second["type"].Scalar();
		const std::string& default_value = kvp.second["default"].Scalar();
		if (type == "bool")
			var.Default = StringToBool(default_value, false);
		else if (type == "int")
			var.Default = StringToInt(default_value, 0);
		else if (type == "float")
			var.Default = StringToFloat(default_value, 0.0f);
		else if (type == "string")
			var.Default = default_value;

		m_Defaults.push_back(std::move(var));
	}
}

ConfigStore ConfigLoader::LoadStore(const std::filesystem::path& store_path) const
{
	ConfigStore store;
	std::ifstream file(store_path);

	if (file.is_open())
		LoadStoreStream(store, file);
	else
	{
		Li::Log::Warn("Config not found at {}", store_path);
		Li::Log::Warn("Setting all config values to default.");
		for (auto& default : m_Defaults)
			store.SetVariant(default.Name, default.Default);

		SaveStore(store, store_path);
	}
	return store;
}

void ConfigLoader::SaveStore(const ConfigStore& store, const std::filesystem::path& store_path) const
{
	std::filesystem::create_directories(store_path.parent_path());

	std::ofstream file(store_path, std::ios::out | std::ios::trunc);
	if (!file.is_open())
	{
		Li::Log::Error("Failed to open config file: {}", store_path);
		return;
	}

	YAML::Emitter emitter;
	emitter << YAML::BeginMap;

	for (auto& default: m_Defaults)
	{
		emitter << YAML::Key << default.Name;

		if (std::holds_alternative<bool>(default.Default))
			emitter << YAML::Value << BoolToString(store.Get<bool>(default.Name));
		if (std::holds_alternative<int>(default.Default))
			emitter << YAML::Value << std::to_string(store.Get<int>(default.Name));
		if (std::holds_alternative<float>(default.Default))
			emitter << YAML::Value << std::to_string(store.Get<float>(default.Name));
		if (std::holds_alternative<std::string>(default.Default))
			emitter << YAML::Value << store.GetRef<std::string>(default.Name);
	}
	emitter << YAML::EndMap;
	emitter << YAML::Newline;

	file << emitter.c_str();
}

void ConfigLoader::LoadStoreStream(ConfigStore& store, std::istream& stream) const
{
	LI_ASSERT(m_Defaults.size() > 0, "Default config not loaded.");

	YAML::Node config = YAML::Load(stream);

	for (auto& default : m_Defaults)
	{
		YAML::Node var = config[default.Name];
		if (var && var.IsScalar())
		{
			if (std::holds_alternative<bool>(default.Default))
				store.Set(default.Name, StringToBool(var.Scalar(), std::get<bool>(default.Default)));
			if (std::holds_alternative<int>(default.Default))
				store.Set(default.Name, StringToInt(var.Scalar(), std::get<int>(default.Default)));
			if (std::holds_alternative<float>(default.Default))
				store.Set(default.Name, StringToFloat(var.Scalar(), std::get<float>(default.Default)));
			if (std::holds_alternative<std::string>(default.Default))
				store.Set(default.Name, var.Scalar());
		}
		else
		{
			Li::Log::Warn("Config variable {} not found or invalid in user config. Setting value to default.");
			store.SetVariant(default.Name, default.Default);
		}
	}
}
