// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	springArmComp->SetRelativeLocation(FVector(0, 40, 120));
	springArmComp->TargetArmLength = 130;

	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform trans(GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);

	resultDirection.Z = 0;
	resultDirection.Normalize();

	AddMovementInput(resultDirection);
	direction = FVector::ZeroVector;


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerCharacter::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Look Up"), this, &APlayerCharacter::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right"), this, &APlayerCharacter::OnAxisTurnRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::OnActionJump);

}

void APlayerCharacter::OnAxisHorizontal(float value) {
	direction.Y = value;
}

void APlayerCharacter::OnAxisVertical(float value) {
	direction.X = value;
}

void APlayerCharacter::OnAxisLookUp(float value) {
	AddControllerPitchInput(value);
}

void APlayerCharacter::OnAxisTurnRight(float value) {
	AddControllerYawInput(value);
}

void APlayerCharacter::OnActionJump() {
	Jump();
}

