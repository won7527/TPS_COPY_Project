// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Image.h"
#include "WeaponUI.h"

void UWeaponUI::SniperSet()
{
	Rifle->SetVisibility(ESlateVisibility::Hidden);
	Sniper->SetVisibility(ESlateVisibility::Visible);
}
void UWeaponUI::ZoomSet()
{
	Sniper->SetVisibility(ESlateVisibility::Hidden);
	Rifle->SetVisibility(ESlateVisibility::Hidden);
}
void UWeaponUI::RifleSet()
{
	Sniper->SetVisibility(ESlateVisibility::Hidden);
	Rifle->SetVisibility(ESlateVisibility::Visible);
}