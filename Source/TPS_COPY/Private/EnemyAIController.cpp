// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{

	static ConstructorHelpers::FObjectFinder<UBlackboardData> bBData(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BT_BB.BT_BB'"));
	if (bBData.Succeeded())
	{
		BBData = bBData.Object;
	}

}

void AEnemyAIController::Printsomething()
{
	
}