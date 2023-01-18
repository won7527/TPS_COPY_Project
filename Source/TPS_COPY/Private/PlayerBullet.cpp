// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Components/StaticMeshComponent.h"
#include <Components/SphereComponent.h>

// Sets default values
APlayerBullet::APlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(10.0f);

	playerbulletComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("playerBulletComp"));
	playerbulletComp->SetupAttachment(sphereComp);
	playerbulletComp->SetRelativeScale3D(FVector(0.18f));
	playerbulletComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	movementComp->SetUpdatedComponent(sphereComp);
	movementComp->InitialSpeed = 30000.0f;
	movementComp->MaxSpeed = 30000.0f;
	movementComp->ProjectileGravityScale = 0.1f;
	movementComp->bShouldBounce = false;

	
}

// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle dieTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(dieTimerHandle, FTimerDelegate::CreateLambda([this]()->void {this->Destroy(); }), 0.3f, false);

}

// Called every frame
void APlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

