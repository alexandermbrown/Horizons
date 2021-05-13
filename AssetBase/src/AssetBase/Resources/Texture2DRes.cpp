#include "pch.h"
#include "Texture2DRes.h"

#include "AssetBase/Util/OptionalField.h"

Assets::FilterType ConvertFilterType(const std::string& type)
{
	if (type == "NEAREST")
		return Assets::FilterType_Nearest;
	else if (type == "LINEAR")
		return Assets::FilterType_Linear;
	else if (type == "NEAREST_MIPMAP_NEAREST")
		return Assets::FilterType_NearestMipmapNearest;
	else if (type == "LINEAR_MIPMAP_NEAREST")
		return Assets::FilterType_LinearMipmapNearest;
	else if (type == "NEAREST_MIPMAP_LINEAR")
		return Assets::FilterType_NearestMipmapLinear;
	throw std::runtime_error("Unknown filter type " + type);
}

Assets::WrapType ConvertWrapType(const std::string& type)
{
	if (type == "REPEAT")
		return Assets::WrapType_Repeat;
	else if (type == "MIRRORED_REPEAT")
		return Assets::WrapType_MirroredRepeat;
	else if (type == "CLAMP_TO_EDGE")
		return Assets::WrapType_ClampToEdge;
	else if (type == "CLAMP_TO_BORDER")
		return Assets::WrapType_ClampToBorder;
	throw std::runtime_error("Unknown wrap type " + type);
}

flatbuffers::Offset<Assets::Texture2D> SerializeTexture2D(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node texture, bool debug_mode)
{
	flatbuffers::Offset<flatbuffers::String> name_offset = builder.CreateString(name);

	std::filesystem::path image_path(base_path.parent_path() / GetOptionalString(texture, "path"));
	Assets::FilterType min_filter = ConvertFilterType(GetOptionalString(texture, "min_filter"));
	Assets::FilterType mag_filter = ConvertFilterType(GetOptionalString(texture, "mag_filter"));
	Assets::WrapType wrap_s = ConvertWrapType(GetOptionalString(texture, "wrap_s"));
	Assets::WrapType wrap_t = ConvertWrapType(GetOptionalString(texture, "wrap_t"));
		
	std::ifstream image_file(image_path, std::ios::in | std::ios::binary);
	if (!image_file.is_open())
		throw std::runtime_error("Error opening texture source file.");

	image_file.ignore(std::numeric_limits<std::streamsize>::max());

	size_t image_size = image_file.gcount();
	image_file.clear();
	image_file.seekg(0, std::ios_base::beg);

	uint8_t* image_data;
	auto data = builder.CreateUninitializedVector(image_size, &image_data);

	image_file.read((char*)&image_data[0], image_size);
	image_file.close();

	return Assets::CreateTexture2D(builder, name_offset, min_filter, mag_filter, wrap_s, wrap_t, data);
}
