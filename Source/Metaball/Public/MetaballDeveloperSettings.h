// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MetaballDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Game Settings"))
class METABALL_API UMetaballDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMetaballDeveloperSettings();
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	int32 MinChildCount;
};
