// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Prisoner.generated.h"

UCLASS()
class TPS_COPY_API APrisoner : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APrisoner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

public:

	

	void StandUpAnimPlay();
	void WalkAnimPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPrisonerAnimInstance* PrisonerAnimInst;

	bool IsWalk;

	float KeepTime;

	UPROPERTY(EditAnywhere)
	class APlayerCharacter* savior;

	FVector DefaultLocation;

};
