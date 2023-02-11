// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AEnemy* owner = Cast<AEnemy>(TryGetPawnOwner());

	if (!owner)
	{
		return;
	}

	HittedLocationY = FVector::DotProduct(owner->GetActorRightVector(), (owner->HittedLocation - owner->GetActorLocation()).GetSafeNormal());
	HittedLocationZ = FVector::DotProduct(owner->GetActorUpVector(), (owner->HittedLocation - owner->GetActorLocation()).GetSafeNormal());

	
	IsAttackAnim = owner->IsAttack;
	IsBackAnim = owner->IsBackAttack;
	
}