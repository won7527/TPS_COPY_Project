// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"




// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> defaultAnim(TEXT("/Script/Engine.AnimSequence'/Game/MyAnimation/Idle_Rifle_Hip.Idle_Rifle_Hip'"));
	if (defaultAnim.Succeeded())
	{
		DefaultAnim = defaultAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> hittedAnim(TEXT("/Script/Engine.AnimSequence'/Game/MyAnimation/Hit_React_1.Hit_React_1'"));
	if (hittedAnim.Succeeded())
	{
		HittedAnim = hittedAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> hittedBackAnim(TEXT("/Script/Engine.AnimSequence'/Game/MyAnimation/StandingDeathForward02_UE.StandingDeathForward02_UE'"));
	if (hittedBackAnim.Succeeded())
	{
		HittedBackAnim = hittedBackAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> deadAnim(TEXT("/Script/Engine.AnimSequence'/Game/MyAnimation/Death_Ironsights_1.Death_Ironsights_1'"));
	if (deadAnim.Succeeded())
	{
		DeadAnim = deadAnim.Object;
	}
	
	
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
	float IsBack = FVector::DotProduct(-GetActorForwardVector(), target->GetActorLocation() - GetActorLocation());

	if (InRange && target)
	{
		FRotator HeadToTarget = (target->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(HeadToTarget);
		//UE_LOG(LogTemp, Warning, TEXT("a"));
	}
	

	if (CurrentHp < RightBeforeHp && IsBack < 0)
	{
		this->GetMesh()->PlayAnimation(HittedAnim, false);
		RightBeforeHp = CurrentHp;
		IsAttack = true;

	}
	else if (CurrentHp < RightBeforeHp && IsBack >= 0)
	{
		this->GetMesh()->PlayAnimation(HittedBackAnim, false);
		RightBeforeHp = 0;
		UE_LOG(LogTemp, Warning, TEXT("a"));
		IsBackAttack = true;
		FTimerHandle DecreasedHpTime;
		FTimerHandle DestroyTime;
		GetWorldTimerManager().SetTimer(DecreasedHpTime, this, &AEnemy::HPDecreased, 0.01f, true);
		GetWorldTimerManager().SetTimer(DestroyTime, this, &AEnemy::Killed, 0.1f, false, 1.8f);
		
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
		//UE_LOG(LogTemp, Warning, TEXT("S"));
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

void AEnemy::RifleHit()
{
	CurrentHp -= 5;
}