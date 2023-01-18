// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBullet.generated.h"

UCLASS()
class TPS_COPY_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class UStaticMeshComponent* playerbulletComp;

	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "Bullet Settings")
	class UProjectileMovementComponent* movementComp;

};
