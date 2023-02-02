// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponUI.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UWeaponUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UImage* Sniper;
	UPROPERTY(meta = (BindWidget))
	class UImage* Rifle;

	void SniperSet();
	void ZoomSet();
	void RifleSet();
};
