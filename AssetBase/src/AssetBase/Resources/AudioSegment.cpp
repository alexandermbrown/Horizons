#include "pch.h"
#include "AudioSegment.h"

namespace AssetBase
{
	flatbuffers::Offset<Assets::Audio> AudioSegment::Serialize(rapidxml::xml_node<>* audioNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode)
	{
		flatbuffers::Offset<flatbuffers::String> name = NULL;
		if (auto* name_attr = audioNode->first_attribute("name"))
		{
			name = builder.CreateString(name_attr->value());
			std::cout << "Loading audio '" << name_attr->value() << "' ... ";
		}
		else throw "Attribute 'name' not found in audio.";

		std::filesystem::path audioPath;
		if (auto* node = audioNode->first_node("source"))
			audioPath = node->value();
		else throw "Missing <source> in audio.\n";

		std::ifstream audioFile(basePath.parent_path() / audioPath, std::ios::in | std::ios::binary);
		if (!audioFile.is_open()) {
			throw "Failed to open audio file.\n";
		}

		audioFile.ignore(std::numeric_limits<std::streamsize>::max());
		size_t fileSize = audioFile.gcount();
		audioFile.clear();
		audioFile.seekg(0, std::ios_base::beg);

		uint8_t* fileData;
		auto data = builder.CreateUninitializedVector(fileSize, &fileData);

		audioFile.read((char*)fileData, fileSize);
		audioFile.close();

		std::cout << "done.\n";
		return Assets::CreateAudio(builder, name, data);
	}
}
