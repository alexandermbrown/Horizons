#pragma once

#include "Lithium/Renderer/Texture.h"
#include "Lithium/Utility/Time.h"
#include "AudioQueue.h"
#include "AVReader.h"

#include "glm/glm.hpp"

#include <string>

struct AVFormatContext;
struct AVCodec;
struct AVCodecContext;
struct AVCodecParserContext;
struct AVFrame;
struct AVPacket;
struct AVRational;
struct SwsContext;
struct SwrContext;
enum AVSampleFormat;

namespace Li
{
	class AVPlayer
	{
	public:
		AVPlayer();
		AVPlayer(const AVPlayer&) = delete;
		virtual ~AVPlayer();

		bool Open(const std::string& path, FilterType min_filter = FilterType::Linear, FilterType mag_filter = FilterType::Linear);
		void Close();

		// Returns true while video is playing.
		bool UpdateFrame(Duration::us dt);

		inline const Ref<Texture2D>& GetTexture() const { return m_Video.Texture; }
		inline int GetWidth() const { return m_Video.Width; }
		inline int GetHeight() const { return m_Video.Height; }

	private:
		bool InitVideo(const std::string& path, FilterType min_filter, FilterType mag_filter);
		bool InitAudio(const std::string& path);
		void FreeInternal();
		void FreeAudioInternal();

		bool ProcessVideoPacket(AVPacket* packet);
		bool ProcessAudioPacket(AVPacket* packet);

		int64_t GetPTS(int64_t frame_pts, int64_t dts);

		bool m_Open;
		bool m_Playing;
		Duration::us m_Elapsed;

		AVFrame* m_Frame;

		Unique<AVReader> m_Reader;

		struct VideoData
		{
			AVCodec* Codec{ nullptr };
			AVCodecContext* CodecCtx{ nullptr };
			SwsContext* SwsContext{ nullptr };
			uint8_t* PixelBuffer{ nullptr };
			int Width{ 0 };
			int Height{ 0 };
			int64_t NextPTS{ 0 };
			float TimeBase{ 0 };
			Ref<Texture2D> Texture;

			constexpr static int NumOutChannels = 4;
		};
		VideoData m_Video;

		struct AudioData
		{
			bool HasAudio{ false };
			bool Started{ false };

			AVCodec* Codec{ nullptr };
			AVCodecContext* CodecCtx{ nullptr };

			SwrContext* SwrCtx{ nullptr };
			uint8_t** DstData{ nullptr };
			bool Resample;
			int Channels;
			int Frequency;
			int MaxNumSamples;
			int OutBytesPerSample;
			AVSampleFormat OutFormat;

			Unique<AudioQueue> Queue{ nullptr };

			// Debug info
			int DiscardCount{ 0 };
		};
		AudioData m_Audio;
	};
}
