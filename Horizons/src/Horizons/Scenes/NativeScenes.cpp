#include "pch.h"
#include "NativeScenes.h"

#include "GameScene.h"
#include "LevelEditorScene.h"
#include "VideoPlayerScene.h"

Li::Unique<Li::Scene> GetNativeScene(const std::string& name)
{
	if (name == "GameScene")
		return Li::MakeUnique<GameScene>();
#ifndef LI_DIST
	else if (name == "LevelEditorScene")
		return Li::MakeUnique<LevelEditorScene>();
	else if (name == "VideoPlayerScene")
		return Li::MakeUnique<VideoPlayerScene>();
#endif

	LI_ERROR("Unknown native scene {}", name);
	return nullptr;
}
