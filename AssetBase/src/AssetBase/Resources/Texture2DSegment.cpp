#include "pch.h"
#include "Texture2DSegment.h"

namespace AssetBase
{
	Assets::FilterType ConvertFilterType(const char* type)
	{
		if (!strcmp(type, "NEAREST")) 
			return Assets::FilterType_Nearest;
		else if (!strcmp(type, "LINEAR"))
			return Assets::FilterType_Linear;
		else if (!strcmp(type, "NEAREST_MIPMAP_NEAREST"))
			return Assets::FilterType_NearestMipmapNearest;
		else if (!strcmp(type, "LINEAR_MIPMAP_NEAREST"))
			return Assets::FilterType_LinearMipmapNearest;
		else if (!strcmp(type, "NEAREST_MIPMAP_LINEAR"))
			return Assets::FilterType_NearestMipmapLinear;
		throw "Unknown filter type!";
	}

	Assets::WrapType ConvertWrapType(const char* type)
	{
		if (!strcmp(type, "REPEAT"))
			return Assets::WrapType_Repeat;
		else if (!strcmp(type, "MIRRORED_REPEAT"))
			return Assets::WrapType_MirroredRepeat;
		else if (!strcmp(type, "CLAMP_TO_EDGE"))
			return Assets::WrapType_ClampToEdge;
		else if (!strcmp(type, "CLAMP_TO_BORDER"))
			return Assets::WrapType_ClampToBorder;
		throw "Unknown wrap type!";
	}

	flatbuffers::Offset<Assets::Texture2D> Texture2DSegment::Serialize(rapidxml::xml_node<>* textureNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode)
	{
		flatbuffers::Offset<flatbuffers::String> name = NULL;
		if (auto* name_attr = textureNode->first_attribute("name"))
		{
			name = builder.CreateString(name_attr->value());
			std::cout << "Loading texture '" << name_attr->value() << "' ... ";
		}
		else throw "Attribute 'name' not found in texture.";
		
		Assets::FilterType min_filter = Assets::FilterType_Error;
		Assets::FilterType mag_filter = Assets::FilterType_Error;
		Assets::WrapType wrap_s = Assets::WrapType_Error;
		Assets::WrapType wrap_t = Assets::WrapType_Error;

		std::filesystem::path imagePath;
		if (auto* node = textureNode->first_node("source"))
			imagePath = node->value();
		else throw "Missing <source> in texture.\n";

		if (auto* node = textureNode->first_node("min_filter"))
			min_filter = ConvertFilterType(node->value());
		else throw "Missing <min_filter> in texture.\n";

		if (auto* node = textureNode->first_node("mag_filter"))
			mag_filter = ConvertFilterType(node->value());
		else throw "Missing <mag_filter> in texture.\n";

		if (auto* node = textureNode->first_node("wrap_s"))
			wrap_s = ConvertWrapType(node->value());
		else throw "Missing <wrap_s> in texture.\n";

		if (auto* node = textureNode->first_node("wrap_t"))
			wrap_t = ConvertWrapType(node->value());
		else throw "Missing <wrap_t> in texture.\n";

		std::ifstream imageFile(basePath.parent_path() / imagePath, std::ios::in | std::ios::binary);
		if (!imageFile.is_open()) {
			throw "Error opening texture source file.";
		}

		imageFile.ignore(std::numeric_limits<std::streamsize>::max());

		size_t imageSize = imageFile.gcount();
		imageFile.clear();
		imageFile.seekg(0, std::ios_base::beg);

		uint8_t* imageData;
		auto data = builder.CreateUninitializedVector(imageSize, &imageData);

		imageFile.read((char*)&imageData[0], imageSize);
		imageFile.close();

		std::cout << "done.\n";
		return Assets::CreateTexture2D(builder, name, min_filter, mag_filter, wrap_s, wrap_t, data);
	}
}