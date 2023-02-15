// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <Animation/AnimMontage.h>
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	



public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	bool isInAir = false;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float direction = 0;

	UPROPERTY(EditDefaultsOnly, Category=PlayerAnim)
	class UAnimMontage* attackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* reloadAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* swapAnimMontage;

	UFUNCTION(BlueprintCallable)
	void PlayAttackAnim();

	UFUNCTION(BlueprintCallable)
	void PlaySwapAnim();

	UFUNCTION(BlueprintCallable)
	void PlayProneSwapAnim();

	UFUNCTION(BlueprintCallable)
	void PlayProneReloadAnim();
	
	UFUNCTION(BlueprintCallable)
	void PlayReloadAnim(FName sectionName);

	UFUNCTION(BlueprintCallable)
		void OnSniperReload();



};
