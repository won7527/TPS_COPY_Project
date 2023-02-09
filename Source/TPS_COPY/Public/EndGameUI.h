// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameUI.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UEndGameUI : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(meta = (BindWidget))
	class UImage* EndImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* BloodImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* Bullet1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Bullet2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Bullet3;
	UPROPERTY(meta = (BindWidget))
	class UImage* Bullet4;

	
	void EndImageSet();
	void BloodImageSet();
	void Bullet1Set();
	void Bullet2Set();
	void Bullet3Set();
	void Bullet4Set();

	void EndScreen();

	bool IsEndLevel;
};
