// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds) {

	// 플레이어의 이동 속도를 가져와 speed에 할당하고 싶다.
	// 소유 폰 얻어오기
	auto ownerPawn = TryGetPawnOwner();
	// 플레이어로 캐스팅
	auto player = Cast<APlayerCharacter>(ownerPawn);

	if (player != nullptr)
	{
		// 이동 속도가 필요
		FVector velocity = player->GetVelocity();
		// 플레이어의 전방 벡터가 필요
		FVector forwardVector = player->GetActorForwardVector();
		// speed에 값 할당하기
		speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
		// 플레이어가 현재 공중에 있는지 여부를 기억하고 싶다.
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}

	
}

void UPlayerAnim::PlayAttackAnim() {
	Montage_Play(attackAnimMontage);
}
