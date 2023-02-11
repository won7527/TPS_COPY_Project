// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"


AMyPlayerController::AMyPlayerController()
{
	ConstructorHelpers::FClassFinder<UWeaponUI> weapon(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW_BP/UI_Weapon.UI_Weapon_C'"));
	if (weapon.Succeeded())
	{
		weaponUI = weapon.Class;
	}
	ConstructorHelpers::FClassFinder<UMainWidget> main(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW_BP/BP_Main.BP_Main_C'"));
	if (main.Succeeded())
	{
		mainUI = main.Class;
	}
	ConstructorHelpers::FClassFinder<UEndGameUI> end(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW_BP/UI_EndScreen.UI_EndScreen_C'"));
	if (end.Succeeded())
	{
		endUI = end.Class;
	}
}

void AMyPlayerController::BeginPlay()
{
	UIWeapon = CreateWidget<UWeaponUI>(this, weaponUI);
	UIWeapon->AddToViewport();

	MainWid = CreateWidget<UMainWidget>(this, mainUI);
	if (GetWorld()->GetName() == FString("MainLevel"))
	{
		MainWid->AddToViewport();
		
	}
	UIEndGame = CreateWidget<UEndGameUI>(this, endUI);
	//if (UIEndGame->IsEndLevel)
	//{
	//	UIEndGame->AddToViewport();
	//}
	//if (GetWorld()->GetName() == FString("MainLevel"))
	//{
	//	DisableInput(this);
	//
	//}
	//
}

