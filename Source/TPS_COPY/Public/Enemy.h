// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "EnemyAIController.h"
#include "Enemy.generated.h"

UCLASS()
class TPS_COPY_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightBeforeHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartTrace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndTrace;

	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	int32 Patrol();
	UFUNCTION(BlueprintCallable)
	void Killed();
	UFUNCTION(BlueprintCallable)
	void HPDecreased();
	UFUNCTION(BlueprintCallable)
	void RifleHit();


	UPROPERTY(EditAnywhere)
	class APlayerCharacter* target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoint")
	TArray<ATargetPoint*> PatArr;

	int32 CurrentPoint = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* DefaultAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* HittedAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* HittedBackAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* DeadAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBackAttack;
	

	
};
