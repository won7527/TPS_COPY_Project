// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHp.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UEnemyHp : public UUserWidget
{
	GENERATED_BODY()


public:

	void HPUpdate();

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;
};
