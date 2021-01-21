#include "pch.h"
#include "AudioSegment.h"

#include "OptionalField.h"

namespace AssetBase
{
	flatbuffers::Offset<Assets::Audio> SerializeAudio(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node audio, bool debug_mode)
	{
		flatbuffers::Offset<flatbuffers::String> name_offset = builder.CreateString(name);

		std::filesystem::path audio_path = base_path.parent_path() / GetOptionalString(audio, "path");

		std::ifstream audio_file(audio_path, std::ios::in | std::ios::binary);
		if (!audio_file.is_open())
			throw std::runtime_error("Failed to open audio file " + audio_path.string());

		audio_file.ignore(std::numeric_limits<std::streamsize>::max());
		size_t file_size = audio_file.gcount();
		audio_file.clear();
		audio_file.seekg(0, std::ios_base::beg);

		uint8_t* file_data;
		auto data = builder.CreateUninitializedVector(file_size, &file_data);

		audio_file.read((char*)file_data, file_size);
		audio_file.close();

		return Assets::CreateAudio(builder, name_offset, data);
	}
}
