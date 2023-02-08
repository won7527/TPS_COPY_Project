// Fill out your copyright notice in the Description page of Project Settings.

#include "CountProgressUI.h"
#include "Components/ProgressBar.h"
#include "Door.h"




void UCountProgressUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CountProgress->SetPercent(owner->count/owner->max);
}

void UCountProgressUI::UpdateCount(int32 count, int32 max)
{
	CountProgress->SetPercent(count / max);
	UE_LOG(LogTemp, Warning, TEXT("IS PROGRESS"));
}