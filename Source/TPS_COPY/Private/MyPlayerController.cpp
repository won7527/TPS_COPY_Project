// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"


AMyPlayerController::AMyPlayerController()
{
	ConstructorHelpers::FClassFinder<UWeaponUI> weapon(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW_BP/UI_Weapon.UI_Weapon_C'"));
	if (weapon.Succeeded())
	{
		weaponUI = weapon.Class;
	}
	
}

void AMyPlayerController::BeginPlay()
{
	UIWeapon = CreateWidget<UWeaponUI>(this, weaponUI);
	UIWeapon->AddToViewport();

	
}

