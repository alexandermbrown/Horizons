#pragma once

#include "Lithium/Core/Assert.h"
#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/Shader.h"
#include "Lithium/Renderer/TextureAtlas.h"
#include "Lithium/Renderer/Font.h"
#include "Lithium/Audio/AudioBuffer.h"

#include <unordered_map>

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace Li
{
	class ResourceManager
	{
	public:
		static void Shutdown();

		static void Load(const std::string& labFilePath);

		static void BeginStaggeredLoad(const std::string& labFilePath);
		static bool UpdateStaggeredLoad();

		static void PrintInfo();

		static Ref<Texture2D> GetTexture2D(const std::string& name)
		{
			return GetResource(s_Data->Textures, name, "texture_default");
		}

		static Ref<Shader> GetShader(const std::string& name)
		{
			return GetResource(s_Data->Shaders, name, nullptr);
		}

		static Ref<TextureAtlas> GetTextureAtlas(const std::string& name)
		{
			return GetResource(s_Data->TextureAtlases, name, "atlas_default");
		}

		static Ref<Font> GetFont(const std::string& name)
		{
			return GetResource(s_Data->Fonts, name, "Lato-Regular");
		}

		static Ref<AudioBuffer> GetAudioBuffer(const std::string& name)
		{
			return GetResource(s_Data->Audio, name, nullptr);
		}

	private:
		template<typename T>
		using ResourceMap = std::unordered_map<std::string, Ref<T>>;

		template<typename T>
		static Ref<T> GetResource(const ResourceMap<T>& map, const std::string& name, const char* default)
		{
			auto it = map.find(name);
			if (it == map.end())
			{
				Log::CoreError("Resource {} not found.", name);

				if (default)
				{
					LI_CORE_ASSERT(map.find(default) != map.end(), "Default does not exist!");
					return map.at(default);
				}
				else return nullptr;
			}
			return it->second;
		}

		template<typename T>
		using VecIterator = flatbuffers::VectorIterator<flatbuffers::Offset<T>, const T*>;

		struct StaggeredLoadData
		{
			uint8_t* Buffer = nullptr;
			const Assets::AssetBundle* Bundle = nullptr;

			VecIterator<Assets::Texture2D> TextureIt;
			VecIterator<Assets::Shader> ShaderIt;
			VecIterator<Assets::TextureAtlas> TextureAtlasIt;
			VecIterator<Assets::Font> FontIt;
			VecIterator<Assets::Audio> AudioIt;
			VecIterator<Assets::Locale> LocaleIt;
		};

		struct ResourceData
		{
			ResourceMap<Texture2D> Textures;
			ResourceMap<Shader> Shaders;
			ResourceMap<TextureAtlas> TextureAtlases;
			ResourceMap<Font> Fonts;
			ResourceMap<AudioBuffer> Audio;

			StaggeredLoadData LoadData;
		};
		
		static Unique<ResourceData> s_Data;
	};
}
