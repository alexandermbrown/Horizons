#include "pch.h"
#include "LocaleSegment.h"

#include "simpleini/simpleini.h"

#include <locale>
#include <codecvt>

namespace AssetBase
{
	LocaleSegment::LocaleSegment(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath, bool debugMode)
		: Segment(SegmentType::Locale), keysLength(0ULL), valuesLength(0ULL)
	{
		name[0] = '\0';
		for (rapidxml::xml_attribute<>* attr = shaderNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!strcmp(attr->name(), "name")) {
				strcpy_s(name, attr->value());
				break;
			}
		}
		if (strlen(name) == 0)
			throw "Attribute 'name' not found in locale.\n";

		std::cout << "Loading locale '" << name << "' ... ";

		std::filesystem::path filePath;
		for (rapidxml::xml_node<>* node = shaderNode->first_node(); node; node = node->next_sibling())
		{
			if (!strcmp(node->name(), "source"))
				filePath = node->value();
		}

		if (filePath.empty())
			throw "Missing <source> in locale.\n";


		CSimpleIniW ini;
		ini.SetUnicode();
		if (ini.LoadFile(filePath.c_str()) < 0)
			throw "Failed to open ini source.";

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wname = converter.from_bytes(name);

		CSimpleIniW::TNamesDepend iniKeys;
		ini.GetAllKeys(wname.c_str(), iniKeys);

		for (const auto& key : iniKeys)
		{
			const wchar_t* value = ini.GetValue(wname.c_str(), key.pItem);

			std::string charKey = converter.to_bytes(key.pItem);

			keysLength += charKey.length() + 1ULL;
			valuesLength += wcslen(value) + 1ULL;
		}

		keys = new char[keysLength];
		values = new wchar_t[valuesLength];

		size_t keysOffset = 0;
		size_t valuesOffset = 0;
		for (const auto& key : iniKeys)
		{
			const wchar_t* value = ini.GetValue(wname.c_str(), key.pItem);

			std::string charKey = converter.to_bytes(key.pItem);

			if (strcpy_s(&keys[keysOffset], keysLength - keysOffset, charKey.c_str()) != 0) {
				throw "Failed to write key string into buffer.";
			}

			if (wcscpy_s(&values[valuesOffset], valuesLength - valuesOffset, value) != 0) {
				throw "Failed to write key string into buffer.";
			}

			keysOffset += charKey.length() + 1ULL;
			valuesOffset += wcslen(value) + 1ULL;
		}

		std::cout << "done.\n";
	}

	LocaleSegment::~LocaleSegment()
	{
		if (keys)
			delete[] keys;
		if (values)
			delete[] values;
	}

	size_t LocaleSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) + 
			sizeof(keysLength) + keysLength * sizeof(char) + 
			sizeof(valuesLength) + valuesLength * sizeof(wchar_t);
	}

	std::ostream& operator<<(std::ostream& os, const LocaleSegment& s)
	{
		os.write((const char*)&s.type, sizeof(s.type));
		os.write(s.name, sizeof(s.name));

		os.write((const char*)&s.keysLength, sizeof(s.keysLength));
		os.write((const char*)s.keys, s.keysLength * sizeof(char));
		os.write((const char*)&s.valuesLength, sizeof(s.valuesLength));
		os.write((const char*)s.values, s.valuesLength * sizeof(wchar_t));

		return os;
	}
}
