// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBullet.h"

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

	// Sniper 컴포넌트 등록
	sniperComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("sniperComp"));

	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperComp->SetupAttachment(GetMesh());

	// Sniper Static Mesh Data Load
	ConstructorHelpers::FObjectFinder<USkeletalMesh> sniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MyResources/FPWeapon/Mesh/Sniper.Sniper'"));
	
	// 데이터 로드가 성공했다면
	if (sniperMesh.Succeeded())
	{	// 스태틱 메시 데이터를 할당한다.
		if (sniperComp != nullptr)
		{
			sniperComp->SetSkeletalMesh(sniperMesh.Object);
			// Sniper 위치 조정
			sniperComp->SetRelativeLocation(FVector(-14, 52, 120));
		}
	}
	
	//Rifle 컴포넌트 등록
	rifleComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("rifleComp"));

	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	rifleComp->SetupAttachment(GetMesh());

	// Rifle Static Mesh Data Load
	ConstructorHelpers::FObjectFinder<USkeletalMesh> rifleMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MyResources/FPWeapon/Mesh/Rifle.Rifle'"));

	// 데이터 로드가 성공했다면
	if (rifleMesh.Succeeded())
	{
		if (rifleComp != nullptr)
		{
			// 스태틱 메시 데이터를 할당한다.
			rifleComp->SetSkeletalMesh(rifleMesh.Object);
			// Rifle 위치 조정
			rifleComp->SetRelativeLocation(FVector(-14, 52, 120));
		}
	}




}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ChangeToSniper();
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
 	// Crouch 이벤트 처리 함수 바인딩	
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacter::OnActionCrouch);
	// Crouch 이벤트 해제 함수 바인딩
 	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacter::OnActionCrouchRelease);
	// Zoom 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &APlayerCharacter::OnActionZoom);
	// Fire 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::OnActionFire);
	// Sniper 스왑 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &APlayerCharacter::ChangeToSniper);
	// Rifle 스왑 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &APlayerCharacter::ChangeToRifle);
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

void APlayerCharacter::OnActionZoom() {

	UE_LOG(LogTemp, Warning, TEXT("Zoom"))
}

 void APlayerCharacter::OnActionCrouch() {
 	Crouch();
 }
 
 void APlayerCharacter::OnActionCrouchRelease() {
 	UnCrouch();
 }

 void APlayerCharacter::OnActionFire() {
	 	 
	 FTransform t = sniperComp->GetSocketTransform(TEXT("FirePosition"));
	 FVector spawnLoc = t.GetLocation();

	 GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnLoc, cameraComp->GetComponentRotation());
	
	 UE_LOG(LogTemp, Warning, TEXT("Fire"))
 }


 void APlayerCharacter::ChangeToSniper() {
	 bUsingSniper = true;
	 sniperComp->SetVisibility(true);
	 rifleComp->SetVisibility(false);

	 UE_LOG(LogTemp, Warning, TEXT("Sniper"))
 }
 
 void APlayerCharacter::ChangeToRifle() {
	 bUsingSniper = false;
	 sniperComp->SetVisibility(false);
	 rifleComp->SetVisibility(true);

	 UE_LOG(LogTemp, Warning, TEXT("Rifle"))

 }