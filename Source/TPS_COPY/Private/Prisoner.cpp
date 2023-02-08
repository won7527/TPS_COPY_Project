// Fill out your copyright notice in the Description page of Project Settings.

#include "PrisonerAnimInstance.h"
#include "Prisoner.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
APrisoner::APrisoner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UAnimInstance> PrisonerAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/TW_BP/ABP_Prisoner.ABP_Prisoner_C'"));
	if (PrisonerAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PrisonerAnim.Class);
		
	}
	
	
	
}
void APrisoner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PrisonerAnimInst = Cast<UPrisonerAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == PrisonerAnimInst)
	{
		return;
	}
}

// Called when the game starts or when spawned
void APrisoner::BeginPlay()
{
	Super::BeginPlay();

	

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	savior = Cast<APlayerCharacter>(actor);
	
	DefaultLocation = GetActorLocation();
}



// Called every frame
void APrisoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsWalk)
	{
		KeepTime += DeltaTime;
		SetActorLocation(FMath::Lerp(DefaultLocation, savior->GetActorLocation(), 0.2f * KeepTime));
		SetActorRotation((savior->GetActorLocation() - GetActorLocation()).Rotation());
		if (0.2f * KeepTime > 0.7f)
		{
			IsWalk = false;
			PrisonerAnimInst->SaluteAnim();
		}
	}

}

// Called to bind functionality to input
void APrisoner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APrisoner::StandUpAnimPlay()
{
	
	PrisonerAnimInst->StandUpAnim();
}

void APrisoner::WalkAnimPlay()
{
	PrisonerAnimInst->WalkAnim();
	IsWalk = true;
}