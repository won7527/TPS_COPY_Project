// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnemyHp.h"
#include "EnemyDistWidget.h"
#include "Components/WidgetComponent.h"





// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimSequence> DeathAnim1(TEXT("/Script/Engine.AnimSequence'/Game/Characters/EnemyMesh/Animation/Death_From_Front_Headshot.Death_From_Front_Headshot'"));
	if (DeathAnim1.Succeeded())
	{
		DeadAnim1 = DeathAnim1.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimSequence> DeathAnim2(TEXT("/Script/Engine.AnimSequence'/Game/Characters/EnemyMesh/Animation/Death_From_The_Front.Death_From_The_Front'"));
	if (DeathAnim2.Succeeded())
	{
		DeadAnim2 = DeathAnim2.Object;
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/TW_BP/ABP_Enemy.ABP_Enemy_C'"));
	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGun(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (TempGun.Succeeded())
	{
		GunMeshComp->SetSkeletalMesh(TempGun.Object);
		GunMeshComp->SetRelativeLocationAndRotation(FVector(-2.0f, -5.0f, 0.0f), FRotator(65.0f, 216.0f, 36.0f));
	}
	
	EnemyHPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	EnemyHPBar->SetupAttachment(GetMesh());
	EnemyHPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	EnemyHPBar->SetRelativeScale3D(FVector(0.5f));
	EnemyHPBar->SetWidgetSpace(EWidgetSpace::World);
	
	ConstructorHelpers::FClassFinder<UUserWidget> HPBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW_BP/UI_EnemyHP.UI_EnemyHP_C'"));
	if (HPBar.Succeeded())
	{
		EnemyHPBar->SetWidgetClass(HPBar.Class);
	
	}

	EnemyDist = CreateDefaultSubobject<UWidgetComponent>(TEXT("Distance"));
	EnemyDist->SetupAttachment(GetMesh());
	EnemyDist->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	EnemyDist->SetRelativeScale3D(FVector(1.5f));
	EnemyDist->SetWidgetSpace(EWidgetSpace::Screen);

	ConstructorHelpers::FClassFinder<UUserWidget> Distance(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW_BP/UI_EnemyDist.UI_EnemyDist_C'"));
	if (Distance.Succeeded())
	{
		EnemyDist->SetWidgetClass(Distance.Class);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());

	target = Cast<APlayerCharacter>(actor);
	
	EnemyDistWidg = Cast<UEnemyDistWidget>(EnemyDist->GetUserWidgetObject());

	AIController = Cast<AEnemyAIController>(GetController());
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float IsBack = FVector::DotProduct(-GetActorForwardVector(), target->GetActorLocation() - GetActorLocation());
	FRotator HPBarRot = (target->GetActorLocation() - GetActorLocation()).Rotation();
	EnemyHPBar->SetWorldRotation(FRotator(0, HPBarRot.Yaw, 0));
	int32 Dist = (target->GetActorLocation() - GetActorLocation()).Size()/100;

	EnemyDistWidg->Point->SetText(FText::FromString(FString::Printf(TEXT("%dm"), Dist)));
	InRange = AIController->IsRange;
	if (Dist > 50 || Dist <= 3)
	{
		EnemyDist->SetVisibility(false);
	}
	else
	{
		EnemyDist->SetVisibility(true);
	}

	if (InRange && target)
	{
		FRotator HeadToTarget = (target->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(HeadToTarget);
		
	}
	else if (!InRange)
	{
		
	}
	

	if (CurrentHp < RightBeforeHp && IsBack < 0 && CurrentHp > 0)
	{

		RightBeforeHp = CurrentHp;
		IsAttack = true;
		IsPastAttack = true;

	}
	else if (CurrentHp < RightBeforeHp && IsBack >= 0)
	{
		
		RightBeforeHp = 0;
		UE_LOG(LogTemp, Warning, TEXT("a"));
		IsBackAttack = true;
		FTimerHandle DecreasedHpTime;
		FTimerHandle DestroyTime;
		GetWorldTimerManager().SetTimer(DecreasedHpTime, this, &AEnemy::HPDecreased, 0.01f, true);
		GetWorldTimerManager().SetTimer(DestroyTime, this, &AEnemy::Killed, 0.1f, false, 5.0f);
		
	}
	else if (CurrentHp <= 0 && IsBack < 0 && !DuringDie)
	{
		AIController->StopMovement();
		DuringDie = true;
		int32 RandDead = FMath::RandRange(0, 1);
		if (RandDead == 0)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			GetMesh()->PlayAnimation(DeadAnim1, false);
		}
		else if (RandDead == 1)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			GetMesh()->PlayAnimation(DeadAnim2, false);
		}
		FTimerHandle DestroyTime;
		GetWorldTimerManager().SetTimer(DestroyTime, this, &AEnemy::Killed, 0.1f, false, 5.0f);
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
	StartTrace = GetActorLocation() + GetActorForwardVector() * 100;
	if ((target->GetActorLocation() - GetActorLocation()).Size() < FireRange)
	{
		EndTrace = target->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("asd"));
	}
	else
	{
		EndTrace = GetActorLocation() + GetActorForwardVector() * FireRange;
		UE_LOG(LogTemp, Warning, TEXT("asda"));
	}

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

void AEnemy::SniperHit()
{
	CurrentHp -= 70;
}
