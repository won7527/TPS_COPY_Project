// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponUI.h"
#include "Components/Image.h"

void UWeaponUI::SniperSet()
{
	if ((Rifle->GetVisibility() != ESlateVisibility::Hidden) && (Sniper->GetVisibility() != ESlateVisibility::Visible))
	{
		Rifle->SetVisibility(ESlateVisibility::Hidden);
		Sniper->SetVisibility(ESlateVisibility::Visible);
	}
}
void UWeaponUI::ZoomSet()
{
	Sniper->SetVisibility(ESlateVisibility::Hidden);
	Rifle->SetVisibility(ESlateVisibility::Hidden);
}
void UWeaponUI::ZoomOut()
{
	Rifle->SetVisibility(ESlateVisibility::Hidden);
	Sniper->SetVisibility(ESlateVisibility::Visible);
}
void UWeaponUI::RifleSet()
{
	Sniper->SetVisibility(ESlateVisibility::Hidden);
	Rifle->SetVisibility(ESlateVisibility::Visible);
}