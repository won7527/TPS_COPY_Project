// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountProgressUI.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UCountProgressUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CountProgress;

	void UpdateCount(int32 count, int32 max);

	class ADoor* owner;
};
