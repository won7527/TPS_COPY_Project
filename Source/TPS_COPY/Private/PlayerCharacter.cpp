// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBullet.h"
#include "GameFramework/Character.h"
#include <Math/UnrealMathUtility.h>
#include <Components/SceneCaptureComponent2D.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

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

		scopePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("scopePlane"));
		scopePlane->SetupAttachment(cameraComp);
		scopeCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("scopeCaptureComponent"));
		scopeCaptureComponent->SetupAttachment(scopePlane);

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
	OnActionZoomRelease();
	
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


	ThrowBack(DeltaTime);


}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerCharacter::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Look Up"), this, &APlayerCharacter::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right"), this, &APlayerCharacter::OnAxisTurnRight);

	// ZoomInAxis 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &APlayerCharacter::OnActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &APlayerCharacter::OnActionZoomOut);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::OnActionJump);
	// Crouch 이벤트 처리 함수 바인딩	
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacter::OnActionCrouch);
	// Crouch 이벤트 해제 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacter::OnActionCrouchRelease);
	// Zoom 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &APlayerCharacter::OnActionZoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &APlayerCharacter::OnActionZoomRelease);
	// Fire 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::OnActionFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayerCharacter::OnActionFireRelease);
	// Sniper 스왑 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Sniper"),  IE_Pressed, this, &APlayerCharacter::ChangeToSniper);
	// Rifle 스왑 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &APlayerCharacter::ChangeToRifle);
	// Dash 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &APlayerCharacter::OnActionDash);
	// DashReleased 이벤트 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Released, this, &APlayerCharacter::OnActionDashReleased);


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

void APlayerCharacter::OnActionZoomIn() {
	UE_LOG(LogTemp, Warning, TEXT("ZoomIn"))
		scopeCaptureComponent->FOVAngle -= 3;
		
}

void APlayerCharacter::OnActionZoomOut() {
	UE_LOG(LogTemp, Warning, TEXT("ZoomOut"))
		scopeCaptureComponent->FOVAngle += 3;
}

void APlayerCharacter::OnActionJump() {
	Jump();
}

void APlayerCharacter::OnActionZoom() {

	UE_LOG(LogTemp, Warning, TEXT("Zooming"))
		isZooming = true;
	scopeCaptureComponent->SetVisibility(true);
	scopePlane->SetVisibility(true);


}

void APlayerCharacter::OnActionZoomRelease() {
	UE_LOG(LogTemp, Warning, TEXT("NotZooming"))
	   isZooming = false;
	scopeCaptureComponent->SetVisibility(false);
	scopeCaptureComponent->FOVAngle = 90.0;
	scopePlane->SetVisibility(false);
}

void APlayerCharacter::OnActionCrouch() {
 	Crouch();
 }
 
 void APlayerCharacter::OnActionCrouchRelease() {
 	UnCrouch();
 }

 void APlayerCharacter::OnActionFire()
 {
	 if (bUsingSniper) {	

		 OneShot();
		 
			
	 }
	 else {
		 
		 GetWorld()->GetTimerManager().SetTimer(fireTimerHandle, this, &APlayerCharacter::OnFire, rifleFireInterval, true);
		 OnFire();

		// UE_LOG(LogTemp, Warning, TEXT("RifleFire"))
		 		 
	 }
	 	 	
	
 }


 void APlayerCharacter::OnActionFireRelease() {

	  GetWorldTimerManager().ClearTimer(fireTimerHandle);
	 // GetWorld()->GetTimerManager().SetTimer(sniperShot, this, &APlayerCharacter::ThrowBack, 5, true);
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

 void APlayerCharacter::OnActionDash() {

	GetCharacterMovement()->MaxWalkSpeed  = 1100.0f;

 }
 void APlayerCharacter::OnActionDashReleased() {

	 GetCharacterMovement()->MaxWalkSpeed = 600.0f;

 }

 void APlayerCharacter::OnFire() {

	 FTimerHandle dieTimerHandle;
	 FTransform t = sniperComp->GetSocketTransform(TEXT("FirePosition"));
	 FVector spawnLoc = t.GetLocation();

	 if (bUsingSniper)
	 {
		 if (isZooming == true)
		 {
			 GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnLoc, cameraComp->GetComponentRotation());
		 }
		 else {
			 int32 randomBulletYaw = FMath::RandRange(1, 20);
			 int32 randomBulletPitch = FMath::RandRange(1, 20);
			 FRotator randomBullet = FRotator(randomBulletPitch, randomBulletYaw, 0);

			 GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnLoc, cameraComp->GetComponentRotation() + randomBullet);
		 }
	 }
	 else {
		 int32 randomBulletYaw = FMath::RandRange(1, 10);
		 int32 randomBulletPitch = FMath::RandRange(1, 10);
		 FRotator randomBullet = FRotator(randomBulletPitch, randomBulletYaw, 0);

		 GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, spawnLoc, cameraComp->GetComponentRotation() + randomBullet);
	 }

		
 }

 void APlayerCharacter::OneShot() {
	 if (count == 1) {
		 UE_LOG(LogTemp, Warning, TEXT("SniperFire"))
			 OnFire();
			 count = 0;
	 }

 }

 void APlayerCharacter::ThrowBack(float deltaTime) {
	 if (count == 0) {
		 curTime += deltaTime;
		 if (curTime >= 5.0f)
		 {
			 count = 1;
			 UE_LOG(LogTemp, Warning, TEXT("Reload"))
				 curTime = 0;
		 }
	 }

 }
