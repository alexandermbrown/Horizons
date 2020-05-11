#include "AssetSerial.h"

#include "rapidxml/rapidxml.hpp"

#include "Texture2DSegment.h"
#include "ShaderSegment.h"
#include "TextureAtlasSegment.h"
#include "FontSegment.h"
#include "AudioSegment.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <memory>

#define ASSETBASE_PUSH_SEGMENT(type, parentNode, path) for (rapidxml::xml_node<>* node = parentNode->first_node(); node; node = node->next_sibling())\
	m_Segments.push_back(std::make_shared<type>(node, path))

namespace AssetBase
{
	AssetSerial::AssetSerial(const std::filesystem::path& path)
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

		FontSegment::Init();

		ASSETBASE_PUSH_SEGMENT(Texture2DSegment,	root->first_node("textures"),			path);
		ASSETBASE_PUSH_SEGMENT(ShaderSegment,		root->first_node("shaders"),			path);
		ASSETBASE_PUSH_SEGMENT(TextureAtlasSegment, root->first_node("texture_atlases"),	path);
		ASSETBASE_PUSH_SEGMENT(FontSegment,			root->first_node("fonts"),				path);
		ASSETBASE_PUSH_SEGMENT(AudioSegment,		root->first_node("audio"),				path);

		FontSegment::Shutdown();

		m_HeaderSerial.infoTableOffset = HeaderSerial::GetSize();
		m_SegmentInfoTable = std::make_unique<SegmentInfoTable>(m_Segments);
		for (auto& segment : m_Segments)
		{
			m_HeaderSerial.infoTableOffset += segment->GetSize();
		}
		m_HeaderSerial.fileSize = m_HeaderSerial.infoTableOffset + sizeof(m_SegmentInfoTable->numEntries) + 
			m_SegmentInfoTable->numEntries * SegmentInfoTableEntry::GetSize();
	}

	std::ostream& operator<<(std::ostream& os, const AssetSerial& as)
	{
		os.write((char*)&as.GetHeaderSerial().signature, sizeof(uint32_t));
		os.write((char*)&as.GetHeaderSerial().fileSize, sizeof(size_t));
		os.write((char*)&as.GetHeaderSerial().infoTableOffset, sizeof(size_t));
		std::cout << "-";

		float i = 0.0f;
		int percentage = 0;
		for (const std::shared_ptr<Segment>& segment : as.GetSegments())
		{
			if (segment->type == SegmentType::Shader)
			{
				os << *std::dynamic_pointer_cast<ShaderSegment>(segment).get();
			}
			else if (segment->type == SegmentType::Texture2D)
			{
				os << *std::dynamic_pointer_cast<Texture2DSegment>(segment).get();
			}
			else if (segment->type == SegmentType::TextureAtlas)
			{
				os << *std::dynamic_pointer_cast<TextureAtlasSegment>(segment).get();
			}
			else if (segment->type == SegmentType::Font)
			{
				os << *std::dynamic_pointer_cast<FontSegment>(segment).get();
			}
			else if (segment->type == SegmentType::Audio)
			{
				os << *std::dynamic_pointer_cast<AudioSegment>(segment).get();
			}

			while (i / as.GetSegments().size() * 100 > percentage) {
				std::cout << "|";
				percentage++;
			}
			i++;
		}
		os << *as.GetSegmentInfoTable().get();
		std::cout << "-\n";
		return os;
	}
}
