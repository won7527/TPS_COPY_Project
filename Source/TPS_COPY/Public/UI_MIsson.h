// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_MIsson.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UUI_MIsson : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Misson;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MissonText;

	TArray<FTimerHandle> TimerArr;
	TArray<FTimerHandle> TimerArr1;
	FTimerHandle a, b, c, d, e, f, g, h, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
	FTimerHandle a1, b1, c1, d1, e1, f1, g1, h1, l1, m1, n1, o1, p1, q1, r1, s1, t1, u1, v1, w1, x1, y1, z1;
	
	int i;
	int i1;
	int j = 0;
	int j1 = 0;
	int k = 0;
	int k1 = 0;
	
	FString Misson1;
	FString MissonTxt;


	void MissonSet();
	void MissonTxtSet();
	void MissonDestroy();
	void MissonTxtDestroy();

};
