// Fill out your copyright notice in the Description page of Project Settings.

#include "Prisoner.h"
#include "PrisonerAnimInstance.h"
#include <Kismet/GameplayStatics.h>

UPrisonerAnimInstance::UPrisonerAnimInstance()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> StandUpMont(TEXT("/Script/Engine.AnimMontage'/Game/TW_BP/AM_StandUp.AM_StandUp'"));
	if (StandUpMont.Succeeded())
	{
		StandUp = StandUpMont.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> SaluteMont(TEXT("/Script/Engine.AnimMontage'/Game/TW_BP/AM_Salute.AM_Salute'"));
	if (SaluteMont.Succeeded())
	{
		Salute = SaluteMont.Object;
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> PrisonerWalkMont(TEXT("/Script/Engine.AnimMontage'/Game/Characters/EnemyMesh/Animation/Walking__1__Montage.Walking__1__Montage'"));
	if (PrisonerWalkMont.Succeeded())
	{
		PrisonerWalk = PrisonerWalkMont.Object;
	}
}

void UPrisonerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APrisoner::StaticClass());

	target = Cast<APrisoner>(actor);

}

void UPrisonerAnimInstance::StandUpAnim()
{
	Montage_Play(StandUp, 1.0f);
}
void UPrisonerAnimInstance::WalkAnim()
{
	Montage_Play(PrisonerWalk, 1.0f);
}
void UPrisonerAnimInstance::SaluteAnim()
{
	Montage_Play(Salute, 1.0f);
}


void UPrisonerAnimInstance::AnimNotify_StandUpEnd()
{
	target->WalkAnimPlay();

}