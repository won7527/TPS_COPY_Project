// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WeaponUI.h"
#include "MainWidget.h"
#include "EndGameUI.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AMyPlayerController();
	virtual void BeginPlay() override;	

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UWeaponUI> weaponUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMainWidget> mainUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEndGameUI> endUI;

	class UWeaponUI* UIWeapon;
	
	class UMainWidget* MainWid;

	class UEndGameUI* UIEndGame;


};
