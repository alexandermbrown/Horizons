#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Localization/Localization.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace Li::Loaders
{
	Ref<Locale> LoadLocale(const Assets::Locale* locale);
}
