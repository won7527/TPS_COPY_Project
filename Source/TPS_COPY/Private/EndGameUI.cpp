// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Image.h"
#include "EndGameUI.h"

void UEndGameUI::EndImageSet()
{
	EndImage->SetVisibility(ESlateVisibility::Visible);
}
void UEndGameUI::BloodImageSet()
{
	BloodImage->SetVisibility(ESlateVisibility::Visible);
}
void UEndGameUI::Bullet1Set()
{
	Bullet1->SetVisibility(ESlateVisibility::Visible);
}
void UEndGameUI::Bullet2Set()
{
	Bullet2->SetVisibility(ESlateVisibility::Visible);
}
void UEndGameUI::Bullet3Set()
{
	Bullet3->SetVisibility(ESlateVisibility::Visible);
}
void UEndGameUI::Bullet4Set()
{
	Bullet4->SetVisibility(ESlateVisibility::Visible);
}

void UEndGameUI::EndScreen()
{
	EndImage->SetVisibility(ESlateVisibility::Hidden);
	BloodImage->SetVisibility(ESlateVisibility::Hidden);
	Bullet1->SetVisibility(ESlateVisibility::Hidden);
	Bullet2->SetVisibility(ESlateVisibility::Hidden);
	Bullet3->SetVisibility(ESlateVisibility::Hidden);
	Bullet4->SetVisibility(ESlateVisibility::Hidden);
	EndImageSet();
	FTimerHandle ScreenBullet1;
	FTimerHandle ScreenBullet2;
	FTimerHandle ScreenBullet3;
	FTimerHandle ScreenBullet4;
	FTimerHandle ScreenBlood;
	GetWorld()->GetTimerManager().SetTimer(ScreenBullet1, this, &UEndGameUI::Bullet1Set, 2.0f, false);
	GetWorld()->GetTimerManager().SetTimer(ScreenBullet2, this, &UEndGameUI::Bullet2Set, 2.2f, false);
	GetWorld()->GetTimerManager().SetTimer(ScreenBullet3, this, &UEndGameUI::Bullet3Set, 2.4f, false);
	GetWorld()->GetTimerManager().SetTimer(ScreenBullet4, this, &UEndGameUI::Bullet4Set, 2.6f, false);
	GetWorld()->GetTimerManager().SetTimer(ScreenBlood, this, &UEndGameUI::BloodImageSet, 2.85f, false);

}