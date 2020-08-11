#include "pch.h"
#include "AssetSerial.h"

#include "rapidxml/rapidxml.hpp"

#include "Texture2DSegment.h"
#include "ShaderSegment.h"
#include "TextureAtlasSegment.h"
#include "FontSegment.h"
#include "AudioSegment.h"
#include "LocaleSegment.h"

#define ASSETBASE_PUSH_SEGMENT(type, parentNode, path, debug) if (parentNode)\
for (rapidxml::xml_node<>* node = parentNode->first_node(); node; node = node->next_sibling())\
	m_Segments.push_back(std::make_shared<type>(node, path, debug))

namespace AssetBase
{
	AssetSerial::AssetSerial(const std::filesystem::path& path, bool debugMode)
		: m_Builder(1024)
	{
		std::cout << "Building from resource definition file " << path.string() << "\n";
		std::ifstream xmlFile(path.string());

		if (!xmlFile.is_open()) {
			throw "Error opening xml file.";
		}

		std::string line = "";
		std::string rawXML;
		while (std::getline(xmlFile, line)) {
			rawXML += line + "\n";
		}
		xmlFile.close();

		rapidxml::xml_document<> doc;
		doc.parse<0>((char*)rawXML.c_str());

		rapidxml::xml_node<>* root = doc.first_node();

		std::vector<flatbuffers::Offset<Assets::Texture2D>> textures;
		if (auto parent = root->first_node("textures"))
		{
			for (rapidxml::xml_node<>* node = parent->first_node(); node; node = node->next_sibling())
				textures.push_back(Texture2DSegment::Serialize(node, path, m_Builder, debugMode));
		}

		std::vector<flatbuffers::Offset<Assets::Shader>> shaders;
		if (auto parent = root->first_node("shaders"))
		{
			for (rapidxml::xml_node<>* node = parent->first_node(); node; node = node->next_sibling())
				shaders.push_back(ShaderSegment::Serialize(node, path, m_Builder, debugMode));
		}

		std::vector<flatbuffers::Offset<Assets::TextureAtlas>> atlases;
		if (auto parent = root->first_node("texture_atlases"))
		{
			for (rapidxml::xml_node<>* node = parent->first_node(); node; node = node->next_sibling())
				atlases.push_back(TextureAtlasSegment::Serialize(node, path, m_Builder, debugMode));
		}

		std::vector<flatbuffers::Offset<Assets::Font>> fonts;
		if (auto parent = root->first_node("fonts"))
		{
			for (rapidxml::xml_node<>* node = parent->first_node(); node; node = node->next_sibling())
				fonts.push_back(FontSegment::Serialize(node, path, m_Builder, debugMode));
		}

		std::vector<flatbuffers::Offset<Assets::Audio>> audio;
		if (auto parent = root->first_node("audio"))
		{
			for (rapidxml::xml_node<>* node = parent->first_node(); node; node = node->next_sibling())
				audio.push_back(AudioSegment::Serialize(node, path, m_Builder, debugMode));
		}

		std::vector<flatbuffers::Offset<Assets::Locale>> locales;
		if (auto parent = root->first_node("locales"))
		{
			for (rapidxml::xml_node<>* node = parent->first_node(); node; node = node->next_sibling())
				locales.push_back(LocaleSegment::Serialize(node, path, m_Builder, debugMode));
		}

		auto bundle = Assets::CreateAssetBundleDirect(m_Builder, &textures, &shaders, &atlases, &fonts, &audio, &locales);
		m_Builder.Finish(bundle, "LAB+");
	}

	std::ostream& operator<<(std::ostream& os, const AssetSerial& as)
	{
		return os.write((const char*)as.GetBufferPointer(), as.GetBufferSize());
	}
}
