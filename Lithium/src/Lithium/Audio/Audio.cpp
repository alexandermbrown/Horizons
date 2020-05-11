#include "lipch.h"
#include "Audio.h"

#include "AudioCore.h"

#define AL_LIBTYPE_STATIC
#include "AL/al.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#define LI_AUDIO_BUFFER_SIZE 32768

namespace li
{
	Audio::Audio(const std::string& filename)
	{
		OggVorbis_File file;
#ifdef _WIN32
		if (ov_fopen(filename.c_str(), &file))
		{
			LI_CORE_ERROR("Failed to load {}. Not a valid Ogg bitstream.\n", filename);
			return;
		}
#else
		FILE* f;
		f = fopen(filename.c_str(), "rb");
		if (!f) {
			LI_CORE_ERROR("Failed to open file {}", filename);
			return;
		}
		if (ov_open(f, &file, NULL, 0) < 0) 
		{
			LI_CORE_ERROR("Failed to load {}. Not a valid Ogg bitstream.\n", filename);
			return;
		}
#endif
		
		vorbis_info* info = ov_info(&file, -1);

		uint32_t format;
		if (info->channels == 1)
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_STEREO16;

		long freq = info->rate;

		std::vector<char> pcmData = std::vector<char>();
		int bitStream;
		long bytes;
		char buffer[LI_AUDIO_BUFFER_SIZE];
		do 
		{
			bytes = ov_read(&file, buffer, LI_AUDIO_BUFFER_SIZE, 0, 2, 1, &bitStream);
			pcmData.insert(pcmData.end(), buffer, buffer + bytes);

		} while (bytes > 0);

		ov_clear(&file);

		ALCall( alGenBuffers(1, &m_BufferID) );
		ALCall( alBufferData(m_BufferID, format, &pcmData[0], (int)pcmData.size(), freq) );
	}

	Audio::Audio(void* source, uint32_t length)
	{
		//OggVorbis_File file;
		//int eof = 0;
		//int current_section = 0;

		//if (ov_open_callbacks(source, &file, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
		//{
		//	LI_CORE_ERROR("Failed to load audio. Not a valid Ogg bitstream.\n");
		//	return;
		//}

		//vorbis_info* info = ov_info(&file, -1);

		//uint32_t format;
		//if (info->channels == 1)
		//	format = AL_FORMAT_MONO16;
		//else
		//	format = AL_FORMAT_STEREO16;

		//long freq = info->rate;

		//std::vector<char> pcmData = std::vector<char>();
		//int bitStream;
		//long bytes;
		//char buffer[LI_AUDIO_BUFFER_SIZE];
		//do
		//{
		//	bytes = ov_read(&file, buffer, LI_AUDIO_BUFFER_SIZE, 0, 2, 1, &bitStream);
		//	pcmData.insert(pcmData.end(), buffer, buffer + bytes);

		//} while (bytes > 0);

		//ov_clear(&file);

		//ALCall(alGenBuffers(1, &m_BufferID));
		//ALCall(alBufferData(m_BufferID, format, &pcmData[0], (int)pcmData.size(), freq));

	}

	Audio::~Audio()
	{
		ALCall( alDeleteBuffers(1, &m_BufferID) );
	}
}