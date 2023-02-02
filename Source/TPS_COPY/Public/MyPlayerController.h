// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WeaponUI.h"
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
	
	class UWeaponUI* UIWeapon;


};
