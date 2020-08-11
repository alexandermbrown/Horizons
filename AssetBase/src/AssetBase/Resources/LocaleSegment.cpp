#include "pch.h"
#include "LocaleSegment.h"

#include "simpleini/simpleini.h"

#include <locale>
#include <codecvt>

namespace AssetBase
{
	flatbuffers::Offset<Assets::Locale> LocaleSegment::Serialize(rapidxml::xml_node<>* localeNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode)
	{
		flatbuffers::Offset<flatbuffers::String> name_offset = NULL;
		char* name = nullptr;
		if (auto* name_attr = localeNode->first_attribute("name"))
		{
			name = name_attr->value();
			name_offset = builder.CreateString(name);
			std::cout << "Loading locale '" << name << "' ... ";
		}
		else throw "Attribute 'name' not found in locale.";

		std::filesystem::path filePath;
		if (auto* node = localeNode->first_node("source"))
			filePath = node->value();
		else throw "Missing <source> in locale.\n";

		CSimpleIniW ini;
		ini.SetUnicode();
		if (ini.LoadFile(filePath.c_str()) < 0)
			throw "Failed to open ini source.";

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wname = converter.from_bytes(name);

		CSimpleIniW::TNamesDepend iniKeys;
		ini.GetAllKeys(wname.c_str(), iniKeys);

		std::vector<flatbuffers::Offset<Assets::LocaleEntry>> entries;
		for (const auto& key : iniKeys)
		{
			const wchar_t* value = ini.GetValue(wname.c_str(), key.pItem);
			
			auto key_offset = builder.CreateString(converter.to_bytes(key.pItem));

			static_assert(sizeof(wchar_t) == sizeof(int16_t));
			auto value_offset = builder.CreateVector((int16_t*)value, wcslen(value) + 1);

			entries.push_back(Assets::CreateLocaleEntry(builder, key_offset, value_offset));
		}
		auto entries_offset = builder.CreateVector(entries);

		std::cout << "done.\n";
		return Assets::CreateLocale(builder, name_offset, entries_offset);
	}
}
