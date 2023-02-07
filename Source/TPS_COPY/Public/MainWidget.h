// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UImage* main1;
	UPROPERTY(meta = (BindWidget))
	class UImage* main2;
	UPROPERTY(meta = (BindWidget))
	class UButton* GameStart;



	void MainScreen();
	void Main1Set();
	void Main2Set();
	void MainReset();
	UFUNCTION()
	void OnClick();

	bool IsGlitch;
	bool IsMain = true;

};
