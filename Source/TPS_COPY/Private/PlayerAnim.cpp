// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds) {

	// �÷��̾��� �̵� �ӵ��� ������ speed�� �Ҵ��ϰ� �ʹ�.
	// ���� �� ������
	auto ownerPawn = TryGetPawnOwner();
	// �÷��̾�� ĳ����
	auto player = Cast<APlayerCharacter>(ownerPawn);

	if (player != nullptr)
	{
		// �̵� �ӵ��� �ʿ�
		FVector velocity = player->GetVelocity();
		// �÷��̾��� ���� ���Ͱ� �ʿ�
		FVector forwardVector = player->GetActorForwardVector();
		// speed�� �� �Ҵ��ϱ�
		speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
		// �÷��̾ ���� ���߿� �ִ��� ���θ� ����ϰ� �ʹ�.
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
		isRifleZoomingAB = player->isRifleZooming;
		isProneAB = player->isProne;
		isTiltingLeftAB = player->isTiltingLeft;
		isTiltingRightAB = player->isTiltingRight;
		isZoomingAB = player->isZooming;

	}

	
}

void UPlayerAnim::PlayAttackAnim() {
	
	APlayerCharacter* owner = Cast<APlayerCharacter>(TryGetPawnOwner());
	owner->PlayAnimMontage(attackAnimMontage, 1, TEXT("Default"));
}

void UPlayerAnim::PlaySwapAnim()
{
	APlayerCharacter* owner = Cast<APlayerCharacter>(TryGetPawnOwner());
	owner->PlayAnimMontage(attackAnimMontage, 1, TEXT("Swap"));
}

void UPlayerAnim::PlayProneSwapAnim() {
	APlayerCharacter* owner = Cast<APlayerCharacter>(TryGetPawnOwner());
	owner->PlayAnimMontage(attackAnimMontage, 1, TEXT("ProneSwap"));
}

void UPlayerAnim::PlayProneReloadAnim() {
	APlayerCharacter* owner = Cast<APlayerCharacter>(TryGetPawnOwner());
	owner->PlayAnimMontage(attackAnimMontage, 1, TEXT("ProneReload"));
}

void UPlayerAnim::PlayReloadAnim(FName sectionName)
{
	APlayerCharacter* owner = Cast<APlayerCharacter>(TryGetPawnOwner());
	owner->PlayAnimMontage(attackAnimMontage, 1, sectionName);
}

void UPlayerAnim::OnSniperReload()
{
	APlayerCharacter* owner = Cast<APlayerCharacter>(TryGetPawnOwner());
	owner->OnMySniperReload();
}
