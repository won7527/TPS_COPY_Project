// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TCharacter.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "Math/Vector.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATCharacter::StaticClass());

	target = Cast<ATCharacter>(actor);

	me = Cast<AEnemy>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	/*enemySightS = FRotator(0, -60, 0).RotateVector(me->GetActorForwardVector());
	
	FVector enemySightD = enemySightS;

	startTrace = me->GetActorLocation();

	for (int i = 0; i < 120; i++)
	{
		enemySightD = enemyRot.RotateVector(enemySightD);
		endTrace = startTrace  + enemySightD * setDist;

		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, startTrace, endTrace, ECollisionChannel::ECC_Visibility))
		{
			DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Red);
			
			hitDist.Add(hit.Distance);

		}
		
	
	}*/
	

	FVector direction1 = me->GetActorForwardVector();
	me->SetActorLocation(me->GetActorLocation() + direction1* 1000 * DeltaTime);

	FVector enemySightL = FRotator(0, -30, 0).RotateVector(me->GetActorForwardVector());
	FVector enemySightR = FRotator(0, 30, 0).RotateVector(me->GetActorForwardVector());
	FHitResult hitL;
	FHitResult hitR;
	bool leftSensor = false;
	bool rightSensor = false;
	
	
	leftSensor = GetWorld()->LineTraceSingleByChannel(hitL, me->GetActorLocation(), me->GetActorLocation() + enemySightL * 150, ECollisionChannel::ECC_Visibility); 	
	rightSensor = GetWorld()->LineTraceSingleByChannel(hitR, me->GetActorLocation(), me->GetActorLocation() + enemySightR * 150, ECollisionChannel::ECC_Visibility);
	
	
	DrawDebugLine(GetWorld(), me->GetActorLocation(), me->GetActorLocation() + enemySightL * 150, FColor::Red);
	DrawDebugLine(GetWorld(), me->GetActorLocation(), me->GetActorLocation() + enemySightR * 150, FColor::Red);

	
	if (leftSensor == true)
	{
		if (rightSensor == true)
		{
			me->SetActorRotation(me->GetActorRotation() + FRotator(0, 90, 0));
		
		}
		else
		{
			me->SetActorRotation(me->GetActorRotation() + FRotator(0, 30, 0));
			
		}
	}
	else if (rightSensor == true)
	{
		me->SetActorRotation(me->GetActorRotation() + FRotator(0, -30, 0));
	}
	


	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;

	case EEnemyState::Move:
		MoveState();
		break;

	case EEnemyState::Attack:
		AttackState();
		break;

	case EEnemyState::Damage:
		DamageState();
		break;

	case EEnemyState::Die:
		DieState();
		break;

	}
}

void UEnemyFSM::IdleState()

{
	

	FVector toTarget = target->GetActorLocation() - me->GetActorLocation();
	toTarget.Normalize();
	float targetCos = FVector::DotProduct(me->GetActorForwardVector(), toTarget);

	if (enemyCos < targetCos)
	{
		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, me->GetActorLocation(), target->GetActorLocation(), ECollisionChannel::ECC_Visibility))
		{
			if (hit.GetActor() == target)
			{
				UE_LOG(LogTemp, Warning, TEXT("ss"));
			}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("ee"));
			//}
		}
	}
	

}
void UEnemyFSM::MoveState(){}
void UEnemyFSM::AttackState(){}
void UEnemyFSM::DamageState(){}
void UEnemyFSM::DieState(){}

