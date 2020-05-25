#include "pch.h"
#include "AudioSegment.h"

namespace AssetBase
{
	AudioSegment::AudioSegment(rapidxml::xml_node<>* audioNode, const std::filesystem::path& basePath)
		: Segment(SegmentType::Audio)
	{
		name[0] = '\0';
		for (rapidxml::xml_attribute<>* attr = audioNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!strcmp(attr->name(), "name")) {
				strcpy_s(name, attr->value());
				break;
			}
		}
		if (strlen(name) == 0)
			throw "Attribute 'name' not found in audio.\n";

		std::cout << "Loading audio '" << name << "' ... ";

		std::filesystem::path audioPath;
		for (rapidxml::xml_node<>* node = audioNode->first_node(); node; node = node->next_sibling())
		{
			if (!strcmp(node->name(), "source"))
				audioPath = node->value();
		}

		if (audioPath.empty())
			throw "Missing <source> in audio.\n";

		std::ifstream audioFile(basePath.parent_path() / audioPath, std::ios::in | std::ios::binary);
		if (!audioFile.is_open()) {
			throw "Failed to open audio file.\n";
		}

		audioFile.ignore(std::numeric_limits<std::streamsize>::max());
		fileSize = audioFile.gcount();
		audioFile.clear();
		audioFile.seekg(0, std::ios_base::beg);

		fileData = new char[fileSize];

		audioFile.read((char*)&fileData[0], fileSize);
		audioFile.close();

		std::cout << "done.\n";
	}

	AudioSegment::~AudioSegment()
	{
		delete[] fileData;
	}

	size_t AudioSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) + sizeof(fileSize) + fileSize;
	}

	std::ostream& operator<<(std::ostream& os, const AudioSegment& a)
	{
		os.write((const char*)&a.type, sizeof(a.type));
		os.write(a.name, sizeof(a.name));
		os.write((char*)&a.fileSize, sizeof(a.fileSize));
		os.write(a.fileData, a.fileSize);
		return os;
	}
}
