#include "lipch.h"
#include "Audio.h"

#include "AudioCore.h"

#define AL_LIBTYPE_STATIC
#include "AL/al.h"

#include "vorbis/vorbisfile.h"
#include <stdlib.h>

#define LI_AUDIO_BUFFER_SIZE 8192

namespace li
{
	typedef struct
	{
		void* buffer;
		size_t size;
		off_t read_pos;
	} ogg_handle;

	static size_t audio_read(void* buffer, size_t size, size_t nmemb, void* f)
	{
		size_t actual_size = size * nmemb;
		if (((ogg_handle*)f)->size - ((ogg_handle*)f)->read_pos < actual_size)
			actual_size = ((ogg_handle*)f)->size - ((ogg_handle*)f)->read_pos;
		if (actual_size != size * nmemb) {
			nmemb = actual_size / size;
		}
		memcpy(buffer,
			(uint8_t*)((ogg_handle*)f)->buffer + ((ogg_handle*)f)->read_pos,
			size * nmemb);
		((ogg_handle*)f)->read_pos += (off_t)(size * nmemb);
		return nmemb;
	}
	static int audio_seek(void* f, ogg_int64_t off, int whence)
	{
		switch (whence) {
		case SEEK_SET:
			((ogg_handle*)f)->read_pos = (off_t)off;
			break;
		case SEEK_CUR:
			((ogg_handle*)f)->read_pos += (off_t)off;
			break;
		case SEEK_END:
			((ogg_handle*)f)->read_pos = (off_t)(((ogg_handle*)f)->size) + (off_t)off;
			break;
		}
		return ((ogg_handle*)f)->read_pos;
	}

	static int audio_close(void* f)
	{
		return 0;
	}
	static long audio_tell(void* f)
	{
		return ((ogg_handle*)f)->read_pos;
	}

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


	Audio::Audio(const void* source, uint32_t length)
	{
		OggVorbis_File file;
		int eof = 0;
		int current_section = 0;

		ogg_handle oh;
		oh.buffer = const_cast<void*>(source);
		oh.size = length;
		oh.read_pos = 0;

		ov_callbacks callbacks = {
			(size_t(*)(void*, size_t, size_t, void*))		audio_read,
			(int (*)(void*, ogg_int64_t, int))				audio_seek,
			(int (*)(void*))								audio_close,
			(long (*)(void*))								audio_tell,
		};

		if (ov_open_callbacks(&oh, &file, NULL, 0, callbacks) < 0)
		{
			LI_CORE_ERROR("Failed to load audio. Not a valid Ogg bitstream.\n");
			return;
		}

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

	Audio::~Audio()
	{
		ALCall( alDeleteBuffers(1, &m_BufferID) );
	}
}