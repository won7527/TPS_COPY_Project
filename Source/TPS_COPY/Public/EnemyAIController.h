// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TPS_COPY_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		SetGenericTeamId(FGenericTeamId(1));
	}
public:

	AEnemyAIController();


	UPROPERTY()
	class UBlackboardData* BBData;

	UFUNCTION(BlueprintCallable)
	void Printsomething();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRange = false;
};

