#include "pch.h"
#include "LocaleRes.h"

#include "AssetBase/Util/OptionalField.h"

flatbuffers::Offset<Assets::Locale> SerializeLocale(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node locale, bool debug_mode)
{
	std::filesystem::path locale_path(base_path.parent_path() / GetOptionalString(locale, "path"));
	YAML::Node locale_file = YAML::LoadFile(locale_path.string());

	std::vector<flatbuffers::Offset<Assets::LocaleEntry>> entries;
	for (const std::pair<YAML::Node, YAML::Node>& entry : locale_file)
	{
		auto key = builder.CreateString(entry.first.Scalar());
		auto value = builder.CreateString(entry.second.as<std::string>());
		entries.push_back(Assets::CreateLocaleEntry(builder, key, value));
	}

	return Assets::CreateLocaleDirect(builder, name.c_str(), &entries);
}
