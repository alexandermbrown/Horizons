#include "pch.h"
#include "AssetPack.h"

#include "yaml-cpp/yaml.h"

#include "Resources/Texture2DRes.h"
#include "Resources/ShaderRes.h"
#include "Resources/TextureAtlasRes.h"
#include "Resources/FontSerializer.h"
#include "Resources/AudioRes.h"
#include "Resources/LocaleRes.h"

#include "Util/OptionalField.h"
#include "AssetBase.h"

AssetPack::AssetPack(const std::filesystem::path& path, bool debug_mode)
{
	std::cout << "Building from resource definition file " << path.string() << "\n";
		
	YAML::Node resources = YAML::LoadFile(path.string());
	if (!resources || !resources.IsMap())
		throw "Invalid YAML file.";

	std::vector<flatbuffers::Offset<Assets::Texture2D>> textures;
	std::vector<flatbuffers::Offset<Assets::Shader>> shaders;
	std::vector<flatbuffers::Offset<Assets::TextureAtlas>> atlases;
	std::vector<flatbuffers::Offset<Assets::Font>> fonts;
	std::vector<flatbuffers::Offset<Assets::Audio>> audio;
	std::vector<flatbuffers::Offset<Assets::Locale>> locales;

	for (const std::pair<YAML::Node, YAML::Node>& resource : resources)
	{
		const std::string& name = resource.first.Scalar();

		std::string type = GetOptionalString(resource.second, "type");
		std::cout << "Loading " << type << " " << name << " ... ";

		if (type == "texture2d")
			textures.push_back(SerializeTexture2D(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "shader")
			shaders.push_back(SerializeShader(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "texture_atlas")
			atlases.push_back(SerializeTextureAtlas(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "font")
			fonts.push_back(AssetBase::Get()->GetFontSerializer().Serialize(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "audio")
			audio.push_back(SerializeAudio(m_Builder, path, name, resource.second, debug_mode));
		else if (type == "locale")
			locales.push_back(SerializeLocale(m_Builder, path, name, resource.second, debug_mode));
		else
		{
			std::cout << "Resource " << name << ": invalid type "<< type << "\n";
			continue;
		}

		std::cout << "done.         \n";
	}

	auto bundle = Assets::CreateAssetBundleDirect(m_Builder, &textures, &shaders, &atlases, &fonts, &audio, &locales);
	m_Builder.Finish(bundle, "LAB+");
}

std::ostream& operator<<(std::ostream& os, const AssetPack& as)
{
	return os.write((const char*)as.GetBufferPointer(), as.GetBufferSize());
}

