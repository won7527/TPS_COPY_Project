// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HittedLocationY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HittedLocationZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttackAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBackAnim;

};
