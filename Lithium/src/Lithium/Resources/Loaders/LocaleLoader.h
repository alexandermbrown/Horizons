#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Localization/Localization.h"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace li
{
	class LocaleLoader
	{
	public:
		static Ref<Locale> Load(const Assets::Locale* locale);
	};
}