// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBullet.h"
#include "GameFramework/Character.h"
#include <Math/UnrealMathUtility.h>
#include <Components/SceneCaptureComponent2D.h>
#include "Blueprint/UserWidget.h"
#include "PlayerAnim.h"
#include "Components/StaticMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerController.h"
#include "Enemy.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(0);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/CharacterSkeletal/Ch15_nonPBR.Ch15_nonPBR'"));

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
	springArmComp->TargetArmLength = 110;

	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Sniper 컴포넌트 등록
	sniperComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperComp"));

	// 부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperComp->SetupAttachment(GetMesh(), TEXT("sniperSocket"));

	// Sniper Static Mesh Data Load
	ConstructorHelpers::FObjectFinder<UStaticMesh> sniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	
	// 데이터 로드가 성공했다면
	if (sniperMesh.Succeeded())
	{	// 스태틱 메시 데이터를 할당한다.
		if (sniperComp != nullptr)
		{
			sniperComp->SetStaticMesh(sniperMesh.Object);
			// Sniper 위치 조정
			sniperComp->SetRelativeLocationAndRotation(sniperLoc, FRotator(0, 0, 0));
			sniperComp->SetRelativeScale3D(FVector(0.2f));
		}

		//scopePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("scopePlane"));
		//scopePlane->SetupAttachment(cameraComp);
		//scopeCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("scopeCaptureComponent"));
		//scopeCaptureComponent->SetupAttachment(scopePlane);
		//scopeBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("scopeBack"));
		//scopeBack->SetupAttachment(scopePlane);


	}
	
	//Rifle 컴포넌트 등록
	rifleComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rifleComp"));

	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	rifleComp->SetupAttachment(GetMesh(), TEXT("rifleSocket"));

	// Rifle Static Mesh Data Load
	ConstructorHelpers::FObjectFinder<UStaticMesh> rifleMesh(TEXT("/Script/Engine.StaticMesh'/Game/M4A1/M4A1.M4A1'"));
	
	// 데이터 로드가 성공했다면
	if (rifleMesh.Succeeded())
	{
		if (rifleComp != nullptr)
		{
			// 스태틱 메시 데이터를 할당한다.
			rifleComp->SetStaticMesh(rifleMesh.Object);
			// Rifle 위치 조정
			//rifleComp->SetRelativeLocationAndRotation(FVector((-23.209405,  68.760827,  140.746834)), FRotator(0, 180, 0));

			rifleComp->SetRelativeRotation(FRotator(0, -90, 0));
			rifleComp->SetRelativeLocation(rrifleLoc);
			rifleComp->SetRelativeScale3D(FVector(1.5, 2.0, 1.5));
		}
	}




}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	crosshairUI = CreateWidget<UUserWidget>(GetWorld(), crosshairFactory);
	crosshairUI->AddToViewport();

	sniperUI = CreateWidget<UUserWidget>(GetWorld(), sniperFactory);
	
	ChangeToSniper();
	//OnActionZoomRelease();


	
	
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
	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Pressed, this, &APlayerCharacter::OnActionLookAroundPressed);
	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Released, this, &APlayerCharacter::OnActionLookAroundReleased);


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
		cameraComp->FieldOfView -= 3;
		
}

void APlayerCharacter::OnActionZoomOut() {
	UE_LOG(LogTemp, Warning, TEXT("ZoomOut"))
		cameraComp->FieldOfView += 3;
}

void APlayerCharacter::OnActionJump() {
	Jump();
}

void APlayerCharacter::OnActionZoom() {
	GetCharacterMovement()->MaxWalkSpeed = 70.0f;

	if (bUsingSniper == true)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), zoomInSound);
		sniperComp->SetVisibility(false);
		GetMesh()->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("Zooming"))
			isZooming = true;
		//scopeCaptureComponent->SetVisibility(true);
		//scopePlane->SetVisibility(true);
		//scopeBack->SetVisibility(true);
		sniperUI->AddToViewport();

		cameraComp->SetFieldOfView(20.0f);

		crosshairUI->RemoveFromParent();
				
	}
	else
	{
		
		cameraComp->FieldOfView = 50.0f;
	}


}

void APlayerCharacter::OnActionZoomRelease() {
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	if (bUsingSniper == true)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), zoomOutSound);
		sniperComp->SetVisibility(true);
		GetMesh()->SetVisibility(true);
		sniperUI->RemoveFromParent();
		crosshairUI->AddToViewport();
		cameraComp->SetFieldOfView(90.0f);
		UE_LOG(LogTemp, Warning, TEXT("NotZooming"))
			isZooming = false;
		//scopeCaptureComponent->SetVisibility(false);
		//scopeCaptureComponent->FOVAngle = 90.0;
		//scopePlane->SetVisibility(false);
		//scopeBack->SetVisibility(false);
	}
	else
	{
		GetMesh()->SetVisibility(true);
		sniperUI->RemoveFromParent();
		cameraComp->FieldOfView = 90.0f;
		isZooming = false;
		//scopeCaptureComponent->SetVisibility(false);
		//scopeCaptureComponent->FOVAngle = 90.0;
		//scopePlane->SetVisibility(false);
		//scopeBack->SetVisibility(false);
	}
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
		 auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		 anim->PlayAttackAnim();
		
			
	 }
	 else {
		 
		 /*GetWorld()->GetTimerManager().SetTimer(fireTimerHandle, this, &APlayerCharacter::OnFire, rifleFireInterval, true);
		 OnFire();

		 auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		 anim->PlayAttackAnim();*/

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

	GetCharacterMovement()->MaxWalkSpeed  = runSpeed;

 }
 void APlayerCharacter::OnActionDashReleased() {

	 GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

 }

void APlayerCharacter::OnActionLookAroundPressed()
{
	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = false;
	
}

void APlayerCharacter::OnActionLookAroundReleased()
{
	bUseControllerRotationYaw = true;
	//GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerCharacter::OnFire() {

	 FTimerHandle dieTimerHandle;
	 FTransform t = GetMesh()->GetSocketTransform(TEXT("sniperFire"));
	 FTransform s = GetMesh()->GetSocketTransform(TEXT("rifleFire"));
	 FVector sniperFireLoc = t.GetLocation();
	 FVector rifleFireLoc = s.GetLocation();
	 FHitResult hitInfo;
	 FVector start = cameraComp->GetComponentLocation();
	 FVector end = start + cameraComp->GetForwardVector() * 100000;
	 FCollisionQueryParams params;
	 params.AddIgnoredActor(this);
	
	 
	 if (bUsingSniper)
	 {
		 UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
		  if (isZooming == true)
		 {
			 bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params);
			 if (bHit)
			 {
				 auto hitComp = hitInfo.GetComponent();
				 
				 FTransform trans(hitInfo.ImpactPoint);
				 /*auto decalLoc = hitInfo.Location;
				 auto hitIN = hitInfo.ImpactNormal;
				 auto hitRot = UKismetMathLibrary::Conv_VectorToRotator(hitIN);
				 auto decalRot = UKismetMathLibrary::MakeTransform(hitInfo.Location, hitRot);
				
				 UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decalFactory, FVector(1, 1, 1), decalLoc, hitRot);*/
				 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, trans);
				 if (hitComp != nullptr && hitComp->IsSimulatingPhysics())
				 {
					 //FVector force = -hitInfo.ImpactNormal * hitComp->GetMass();
					// hitComp->AddForce(force);
					 UE_LOG(LogTemp, Warning, TEXT("ischased"))
					 FVector forceDir = (hitInfo.TraceEnd - hitInfo.TraceStart).GetSafeNormal();

					 FVector force = forceDir * 500000 * hitComp->GetMass();
					 hitComp->AddForce(force);
					//auto enemy = Cast<AEnemy>()

					 
					 
				 }
			 }
		 }
		 else {
			 int32 randomBulletYaw = FMath::RandRange(1, 20);
			 int32 randomBulletPitch = FMath::RandRange(1, 20);
			 FRotator randomBullet = FRotator(randomBulletPitch, randomBulletYaw, 0);

			 GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, sniperFireLoc, cameraComp->GetComponentRotation() + randomBullet);

			 auto controller = GetWorld()->GetFirstPlayerController();
			 controller->PlayerCameraManager->StartCameraShake(cameraShake);
		 }
	 }
	 else {
		 int32 randomBulletYaw = FMath::RandRange(1, 10);
		 int32 randomBulletPitch = FMath::RandRange(1, 10);
		 FRotator randomBullet = FRotator(randomBulletPitch, randomBulletYaw, 0);

		 GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, rifleFireLoc, cameraComp->GetComponentRotation() + randomBullet);
	 }

		
 }

 void APlayerCharacter::OneShot() {
	 if (count == 1) {
		 UE_LOG(LogTemp, Warning, TEXT("SniperFire"))
			 OnFire();
			 count = 0;
			 //카메라 셰이크 재생
			 auto controller = GetWorld()->GetFirstPlayerController();
			 controller->PlayerCameraManager->StartCameraShake(cameraShake);
	 }

 }

 void APlayerCharacter::ThrowBack(float deltaTime) {
	 if (count == 0) {
		 curTime += deltaTime;
		 if (curTime >= 2.0f)
		 {
			 count = 1;
			 UE_LOG(LogTemp, Warning, TEXT("Reload"))
				 curTime = 0;
		 }
	 }

 }

 