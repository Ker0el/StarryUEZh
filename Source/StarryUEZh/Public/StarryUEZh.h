// Copyright 星空汉化. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogStarryUEZh, Log, All);

class FStarryUEZhModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FTSTicker::FDelegateHandle GapsTickerHandle;
	FTSTicker::FDelegateHandle DictionaryTickerHandle;
};
