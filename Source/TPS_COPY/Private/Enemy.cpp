// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());

	target = Cast<APlayerCharacter>(actor);
	
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InRange && target)
	{
		FRotator HeadToTarget = (target->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(HeadToTarget);
		UE_LOG(LogTemp, Warning, TEXT("a"));
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Attack()
{
	FHitResult Hit;
	InRange = true;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, param);
	if (bHit)
	{
		
	
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("S"));
	}
	
}

int32 AEnemy::Patrol()
{

	int32 RandomPatrolPoint = FMath::RandRange(0, PatArr.Num() - 1);
	if (CurrentPoint == RandomPatrolPoint)
	{
		CurrentPoint = -1;
		return CurrentPoint;
	}
	CurrentPoint = RandomPatrolPoint;
	return RandomPatrolPoint;

}

void AEnemy::Killed()
{
	Destroy();
}

void AEnemy::HPDecreased()
{
	if (CurrentHp != 0)
	{
		CurrentHp -= 1;
	}
}