// Copyright 星空汉化. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Switches the editor's live localization table between official English and
 * official Simplified Chinese by loading the corresponding .locres files.
 * This never changes the editor language setting or any asset.
 */
namespace StarryLocale
{
	void Apply(bool bChinese);
}
