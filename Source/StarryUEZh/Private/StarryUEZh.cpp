// Copyright 星空汉化. All Rights Reserved.

#include "StarryUEZh.h"

#include "StarryDictionary.h"
#include "StarryHooks.h"
#include "StarryLocale.h"
#include "StarryUI.h"

#include "Containers/Ticker.h"
#include "Framework/Docking/TabManager.h"
#include "Misc/CommandLine.h"
#include "Misc/ConfigCacheIni.h"

DEFINE_LOG_CATEGORY(LogStarryUEZh);

void FStarryUEZhModule::StartupModule()
{
	UE_LOG(LogStarryUEZh, Log, TEXT("星空 UE 汉化启动"));

	const bool bLoaded = FStarryDictionary::Get().Load();

	bool bEnabled = true;
	if (!GConfig->GetBool(TEXT("StarryUEZh"), TEXT("Enabled"), bEnabled, GEditorPerProjectIni))
	{
		bEnabled = true;
	}
	FStarryDictionary::Get().SetEnabled(bEnabled);

	if (bLoaded)
	{
		StarryLocale::Apply(bEnabled);
		StarryHooks::Install();
		FStarryUI::Register();

		// Keep the untranslated list on disk without asking the user to click
		// anything, so the word tables can be refreshed from the real gaps.
		GapsTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float)
		{
			StarryHooks::DumpGaps();
			return true;
		}), 180.0f);

		if (FParse::Param(FCommandLine::Get(), TEXT("StarryPanel")))
		{
			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float)
			{
				FGlobalTabmanager::Get()->TryInvokeTab(FTabId(TEXT("StarryUEZh")));
				return false;
			}), 6.0f);
		}

		if (FParse::Param(FCommandLine::Get(), TEXT("StarryTestMaterial"))
			|| FParse::Param(FCommandLine::Get(), TEXT("StarryAutoDiag")))
		{
			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float)
			{
				FStarryUI::OpenTestMaterial();
				return false;
			}), 8.0f);
		}

		if (FParse::Param(FCommandLine::Get(), TEXT("StarryAutoDiag")))
		{
			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float)
			{
				FStarryUI::SetEnabled(false);
				return false;
			}), 12.0f);

			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float)
			{
				FStarryUI::ReopenTestMaterial();
				return false;
			}), 16.0f);

			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float)
			{
				FStarryUI::SetEnabled(true);
				return false;
			}), 40.0f);

			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float)
			{
				StarryHooks::DumpGaps();
				StarryHooks::HarvestStatic();
				return false;
			}), 45.0f);
		}
	}
	else
	{
		UE_LOG(LogStarryUEZh, Error, TEXT("词库不可用，插件保持关闭"));
	}

	UE_LOG(LogStarryUEZh, Log, TEXT("启动完成：词条 %d，显示 %s"), FStarryDictionary::Get().Num(),
		bEnabled ? TEXT("中文") : TEXT("English"));
}

void FStarryUEZhModule::ShutdownModule()
{
	if (GapsTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(GapsTickerHandle);
		GapsTickerHandle.Reset();
	}
	StarryHooks::DumpGaps();
	FStarryUI::Unregister();
	StarryHooks::Uninstall();
	UE_LOG(LogStarryUEZh, Log, TEXT("星空 UE 汉化已卸载"));
}

IMPLEMENT_MODULE(FStarryUEZhModule, StarryUEZh)
