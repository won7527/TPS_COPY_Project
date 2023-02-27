// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MIsson.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UUI_MIsson::NativeConstruct()
{
	Super::NativeConstruct();

	Misson1 = "Misson 1";
	MissonTxt = "Rescue Captive";
	TimerArr = { a, b, c, d, e, f, g, h, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z};
	TimerArr1 = { a1, b1, c1, d1, e1, f1, g1, h1, l1, m1, n1, o1, p1, q1, r1, s1, t1, u1, v1, w1, x1, y1, z1};
	i = 0;
	i1 = 0;
	for (i; i < Misson1.Len(); i++)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerArr[i], this, &UUI_MIsson::MissonSet, 1.0 + UKismetMathLibrary::Conv_IntToFloat(i) * 0.2f, false);
	}
	for (i; i < Misson1.Len() + MissonTxt.Len(); i++)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerArr[i], this, &UUI_MIsson::MissonTxtSet, 1.0 + UKismetMathLibrary::Conv_IntToFloat(i) * 0.2f, false);
	}
	for (i1; i1 < Misson1.Len(); i1++)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerArr1[i1], this, &UUI_MIsson::MissonDestroy, 1.0 + UKismetMathLibrary::Conv_IntToFloat(i) * 0.2f + UKismetMathLibrary::Conv_IntToFloat(i1) * 0.2f, false);
	}
	for (i1; i1 < Misson1.Len() + MissonTxt.Len(); i1++)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerArr1[i1], this, &UUI_MIsson::MissonTxtDestroy, 1.0 + UKismetMathLibrary::Conv_IntToFloat(i) * 0.2f + UKismetMathLibrary::Conv_IntToFloat(i1) * 0.2f, false);
	}
	UE_LOG(LogTemp, Warning, TEXT("%d %d"), i, i1);
}

void UUI_MIsson::MissonSet()
{
	Misson->SetText(UKismetTextLibrary::Conv_StringToText(UKismetTextLibrary::Conv_TextToString(Misson->GetText()) + Misson1[j]));
	j++;

}
void UUI_MIsson::MissonTxtSet()
{
	MissonText->SetText(UKismetTextLibrary::Conv_StringToText(UKismetTextLibrary::Conv_TextToString(MissonText->GetText()) + MissonTxt[k]));
	k++;
}

void UUI_MIsson::MissonDestroy()
{
	Misson1[j1] = ' ';
	Misson->SetText(UKismetTextLibrary::Conv_StringToText(Misson1));
	j1++;
	
}

void UUI_MIsson::MissonTxtDestroy() 
{
	MissonTxt[k1] = ' ';
	MissonText->SetText(UKismetTextLibrary::Conv_StringToText(MissonTxt));
	k1++;
	
}
