// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PrisonerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UPrisonerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPrisonerAnimInstance();
	
	virtual void NativeBeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* StandUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Salute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* PrisonerWalk;
	
	UPROPERTY(EditAnywhere)
	class APrisoner* target;
	UFUNCTION(BlueprintCallable)
	void StandUpAnim();
	UFUNCTION(BlueprintCallable)
	void WalkAnim();
	UFUNCTION(BlueprintCallable)
	void SaluteAnim();

	UFUNCTION()
	void AnimNotify_StandUpEnd();
	

};
