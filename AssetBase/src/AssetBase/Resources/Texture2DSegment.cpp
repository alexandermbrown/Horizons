#include "pch.h"
#include "Texture2DSegment.h"

#include "rapidxml/rapidxml.hpp"

namespace AssetBase
{
	li::FilterType ConvertFilterType(const char* type)
	{
		if (!strcmp(type, "NEAREST")) 
			return li::FilterType::Nearest;
		else if (!strcmp(type, "LINEAR"))
			return li::FilterType::Linear;
		else if (!strcmp(type, "NEAREST_MIPMAP_NEAREST"))
			return li::FilterType::NearestMipmapNearest;
		else if (!strcmp(type, "LINEAR_MIPMAP_NEAREST"))
			return li::FilterType::LinearMipmapNearest;
		else if (!strcmp(type, "NEAREST_MIPMAP_LINEAR"))
			return li::FilterType::NearestMipmapLinear;
		throw "Unknown filter type!";
	}

	li::WrapType ConvertWrapType(const char* type)
	{
		if (!strcmp(type, "REPEAT"))
			return li::WrapType::Repeat;
		else if (!strcmp(type, "MIRRORED_REPEAT"))
			return li::WrapType::MirroredRepeat;
		else if (!strcmp(type, "CLAMP_TO_EDGE"))
			return li::WrapType::ClampToEdge;
		else if (!strcmp(type, "CLAMP_TO_BORDER"))
			return li::WrapType::ClampToBorder;
		throw "Unknown wrap type!";
	}
	
	Texture2DSegment::Texture2DSegment(rapidxml::xml_node<>* textureNode, const std::filesystem::path& basePath, bool debugMode)
		: Segment(SegmentType::Texture2D), min_filter(li::FilterType::Error), mag_filter(li::FilterType::Error),
		wrap_s(li::WrapType::Error), wrap_t(li::WrapType::Error)
	{
		name[0] = '\0';
		for (rapidxml::xml_attribute<>* attr = textureNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!strcmp(attr->name(), "name")) {
				strcpy_s(name, attr->value());
				break;
			}
		}
		if (strlen(name) == 0)
			throw "Attribute 'name' not found in texture.";

		std::cout << "Loading texture '" << name << "' ... ";

		std::filesystem::path imagePath;
		for (rapidxml::xml_node<>* node = textureNode->first_node(); node; node = node->next_sibling())
		{
			const char* name = node->name();
			const char* value = node->value();
			if (!strcmp(name, "source"))
				imagePath = value;
			else if (!strcmp(name, "min_filter"))
				min_filter = ConvertFilterType(value);
			else if (!strcmp(name, "mag_filter"))
				mag_filter = ConvertFilterType(value);
			else if (!strcmp(name, "wrap_s"))
				wrap_s = ConvertWrapType(node->value());
			else if (!strcmp(name, "wrap_t"))
				wrap_t = ConvertWrapType(node->value());
		}

		if (imagePath.empty()) 
			throw "Missing <source> in texture.\n";
		if (min_filter == li::FilterType::Error)
			throw "Missing <min_filter> in texture.\n";
		if (mag_filter == li::FilterType::Error)
			throw "Missing <mag_filter> in texture.\n";
		if (wrap_s == li::WrapType::Error)
			throw "Missing <wrap_s> in texture.\n";
		if (wrap_t == li::WrapType::Error)
			throw "Missing <wrap_t> in texture.\n";

		std::ifstream imageFile(basePath.parent_path() / imagePath, std::ios::in | std::ios::binary);
		if (!imageFile.is_open()) {
			throw "Error opening texture source file.";
		}

		imageFile.ignore(std::numeric_limits<std::streamsize>::max());
		
		imageSize = imageFile.gcount();
		imageFile.clear();
		imageFile.seekg(0, std::ios_base::beg);

		imageData = new uint8_t[imageSize];

		imageFile.read((char *)&imageData[0], imageSize);
		imageFile.close();

		std::cout << "done.\n";
	}

	Texture2DSegment::~Texture2DSegment()
	{
		delete[] imageData;
	}
	size_t Texture2DSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) + sizeof(min_filter) + sizeof(mag_filter) + 
			sizeof(wrap_s) + sizeof(wrap_t) + sizeof(imageSize) + imageSize;
	}

	std::ostream& operator<<(std::ostream& os, const Texture2DSegment& t)
	{
		os.write((const char*)&t.type, sizeof(t.type));
		os.write(t.name, sizeof(t.name));
		os.write((const char*)&t.min_filter, sizeof(t.min_filter));
		os.write((const char*)&t.mag_filter, sizeof(t.mag_filter));
		os.write((const char*)&t.wrap_s, sizeof(t.wrap_s));
		os.write((const char*)&t.wrap_t, sizeof(t.wrap_t));
		os.write((const char*)&t.imageSize, sizeof(t.imageSize));
		os.write((const char*)&t.imageData[0], t.imageSize);
		return os;
	}
}