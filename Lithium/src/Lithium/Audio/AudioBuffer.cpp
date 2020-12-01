#include "lipch.h"
#include "AudioBuffer.h"

#include "AudioCore.h"

#define AL_LIBTYPE_STATIC
#include "AL/al.h"

#include "vorbis/vorbisfile.h"
#include <stdlib.h>

#define LI_AUDIO_BUFFER_SIZE 8192

namespace li
{
	static ALenum GetALFormat(int channels, int bytes_per_sample);
	static void ReadPcmData(std::vector<char>& pcm_data, OggVorbis_File& file);

	typedef struct
	{
		const void* buffer;
		size_t size;
		off_t read_pos;
	} OggMemoryFile;

	static size_t OggRead(void* buffer, size_t size, size_t nmemb, void* f);
	static int OggSeek(void* f, ogg_int64_t off, int whence);
	static int OggClose(void* f);
	static long OggTell(void* f);

	AudioBuffer::AudioBuffer()
	{
		ALCall(alGenBuffers(1, &m_BufferID));
	}

	AudioBuffer::~AudioBuffer()
	{
		ALCall( alDeleteBuffers(1, &m_BufferID) );
	}

	void AudioBuffer::SetData(const void* pcm_data, uint32_t size, int channels, int bytes_per_sample, int frequency)
	{
		ALCall(alBufferData(m_BufferID, GetALFormat(channels, bytes_per_sample), pcm_data, (int)size, frequency));
	}

	Ref<AudioBuffer> AudioBuffer::FromOggFile(const std::string& filename)
	{
		OggVorbis_File file;
		if (ov_fopen(filename.c_str(), &file))
		{
			LI_CORE_ERROR("Failed to load {}. Not a valid Ogg bitstream.\n", filename);
			return nullptr;
		}

		std::vector<char> pcm_data;
		ReadPcmData(pcm_data, file);

		vorbis_info* info = ov_info(&file, -1);
		Ref<AudioBuffer> buffer = CreateRef<AudioBuffer>();
		buffer->SetData(pcm_data.data(), (uint32_t)pcm_data.size(), info->channels, 2, info->rate);

		ov_clear(&file);
		return buffer;
	}

	Ref<AudioBuffer> AudioBuffer::FromOggMemory(const void* ogg_source, uint32_t size)
	{
		OggVorbis_File file;
		int eof = 0;
		int current_section = 0;

		OggMemoryFile file_handle;
		file_handle.buffer = ogg_source;
		file_handle.size = size;
		file_handle.read_pos = 0;

		ov_callbacks callbacks = {
			(size_t(*)(void*, size_t, size_t, void*))	OggRead,
			(int (*)(void*, ogg_int64_t, int))			OggSeek,
			(int (*)(void*))							OggClose,
			(long (*)(void*))							OggTell,
		};

		if (ov_open_callbacks(&file_handle, &file, nullptr, 0, callbacks) < 0)
		{
			LI_CORE_ERROR("Failed to load audio. Not a valid Ogg bitstream.\n");
			return nullptr;
		}

		std::vector<char> pcm_data;
		ReadPcmData(pcm_data, file);

		vorbis_info* info = ov_info(&file, -1);
		Ref<AudioBuffer> buffer = CreateRef<AudioBuffer>();
		buffer->SetData(pcm_data.data(), (uint32_t)pcm_data.size(), info->channels, 2, info->rate);

		ov_clear(&file);
		return buffer;
	}

	static void ReadPcmData(std::vector<char>& pcm_data, OggVorbis_File& file)
	{
		int bit_stream;
		long bytes;
		char buffer[LI_AUDIO_BUFFER_SIZE];
		do {
			bytes = ov_read(&file, buffer, LI_AUDIO_BUFFER_SIZE, 0, 2, 1, &bit_stream);
			pcm_data.insert(pcm_data.end(), buffer, buffer + bytes);
		} while (bytes > 0);
	}

	static ALenum GetALFormat(int channels, int bytes_per_sample)
	{
		switch (channels)
		{
		case 1:
			switch (bytes_per_sample)
			{
			case 1:
				return AL_FORMAT_MONO8;
			case 2:
				return AL_FORMAT_MONO16;
			default:
				LI_ASSERT(false, "Invalid number of bytes per sample in audio!");
			}
		case 2:
			switch (bytes_per_sample)
			{
			case 1:
				return AL_FORMAT_STEREO8;
			case 2:
				return AL_FORMAT_STEREO16;
			default:
				LI_ASSERT(false, "Invalid number of bytes per sample in audio!");
			}
		default:
			LI_ASSERT(false, "Invalid number of channels in audio!");
			return AL_FORMAT_MONO16;
		}
	}

	//////////////////////////////////
	// Memory Buffer Read Functions //
	//////////////////////////////////
	static size_t OggRead(void* buffer, size_t size, size_t nmemb, void* f)
	{
		size_t actual_size = size * nmemb;
		if (((OggMemoryFile*)f)->size - ((OggMemoryFile*)f)->read_pos < actual_size)
			actual_size = ((OggMemoryFile*)f)->size - ((OggMemoryFile*)f)->read_pos;
		if (actual_size != size * nmemb) {
			nmemb = actual_size / size;
		}
		memcpy(buffer,
			(uint8_t*)((OggMemoryFile*)f)->buffer + ((OggMemoryFile*)f)->read_pos,
			size * nmemb);
		((OggMemoryFile*)f)->read_pos += (off_t)(size * nmemb);
		return nmemb;
	}

	static int OggSeek(void* f, ogg_int64_t off, int whence)
	{
		switch (whence) {
		case SEEK_SET:
			((OggMemoryFile*)f)->read_pos = (off_t)off;
			break;
		case SEEK_CUR:
			((OggMemoryFile*)f)->read_pos += (off_t)off;
			break;
		case SEEK_END:
			((OggMemoryFile*)f)->read_pos = (off_t)(((OggMemoryFile*)f)->size) + (off_t)off;
			break;
		}
		return ((OggMemoryFile*)f)->read_pos;
	}

	static int OggClose(void* f)
	{
		return 0;
	}

	static long OggTell(void* f)
	{
		return ((OggMemoryFile*)f)->read_pos;
	}
}