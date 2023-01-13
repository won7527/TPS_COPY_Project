// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_COPY_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;

	void IdleState();

	void MoveState();

	void AttackState();

	void DamageState();

	void DieState();

	UPROPERTY(VisibleAnywhere, Category = FSM)
		class ATCharacter* target;

	UPROPERTY()
		class AEnemy* me;

	// 전체 시야각의 1/2
	UPROPERTY(EditAnywhere)
		float enemyTheta = 60;
	float enemyCos = cos(FMath::DegreesToRadians(enemyTheta));


	FRotator enemyRot = FRotator(0, 1, 0);
	FVector enemySightS;

	FVector startTrace;
	FVector endTrace;
	float setDist = 1000;


};
