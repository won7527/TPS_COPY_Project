// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Image.h"
#include "Components/Button.h"
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameStart = Cast<UButton>(GetWidgetFromName(TEXT("GameStart")));
	if (GameStart)
	{
		GameStart->OnClicked.AddDynamic(this, &UMainWidget::OnClick);
	}
	
}



void UMainWidget::Main1Set()
{
	main1->SetVisibility(ESlateVisibility::Visible);
	main2->SetVisibility(ESlateVisibility::Hidden);

}

void UMainWidget::Main2Set()
{
	main1->SetVisibility(ESlateVisibility::Hidden);
	main2->SetVisibility(ESlateVisibility::Visible);
}

void UMainWidget::MainScreen()
{
	IsGlitch = true;
	
	Main1Set();
	FTimerHandle main1start;
	FTimerHandle main2start;
	FTimerHandle mainreset;
	GetWorld()->GetTimerManager().SetTimer(main2start, this, &UMainWidget::Main2Set, 1.0f, false);
	GetWorld()->GetTimerManager().SetTimer(main1start, this, &UMainWidget::Main1Set, 1.2f, false);
	GetWorld()->GetTimerManager().SetTimer(mainreset, this, &UMainWidget::MainReset, 1.4f, false);
}

void UMainWidget::MainReset()
{
	IsGlitch = false;
}

void UMainWidget::OnClick()
{
	IsMain = false;
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("T_Lev"));
	//main1->SetVisibility(ESlateVisibility::Hidden);
	//main2->SetVisibility(ESlateVisibility::Hidden);
	RemoveFromParent();
	
}