// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EnemyDistWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UEnemyDistWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Point;
	UPROPERTY(meta = (BindWidget))
	class UImage* DistImage;

};
