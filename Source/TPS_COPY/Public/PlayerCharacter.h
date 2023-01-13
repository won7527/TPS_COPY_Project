// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TPS_COPY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;

	// 스나이퍼 스켈레탈메시 추가
	UPROPERTY(EditAnywhere, Category = SniperMesh)
	class USkeletalMeshComponent* sniperComp;

	// 라이플 스켈레탈메시 추가
	UPROPERTY(EditAnywhere, Category = RifleMesh)
	class USkeletalMeshComponent* rifleComp;

	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookUp(float value);
	void OnAxisTurnRight(float value);
	void OnActionJump();
	void OnActionZoom();
 	void OnActionCrouch();
 	void OnActionCrouchRelease();
	void OnActionFire();
	
	FVector direction;

	// 스나이퍼 사용 중인가
	bool bUsingSniper = true;

	// 스나이퍼로 변경
	void ChangeToSniper();

	// 라이플로 변경
	void ChangeToRifle();

	UPROPERTY(EditAnywhere, Category="Factory Setting")
	TSubclassOf<class APlayerBullet> bulletFactory;
	




};
