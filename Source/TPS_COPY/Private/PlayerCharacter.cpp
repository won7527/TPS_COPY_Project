// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "GameFramework/Character.h"
#include <Math/UnrealMathUtility.h>
#include "Blueprint/UserWidget.h"
#include "PlayerAnim.h"
#include "Components/StaticMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy.h"
#include "MyPlayerController.h"
#include "Animation/AnimInstance.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Door.h"
#include "Particles/ParticleSystemComponent.h"


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

	// Sniper ������Ʈ ���
	sniperComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperComp"));
	sniperBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperBack"));

	// �θ� ������Ʈ�� Mesh ������Ʈ�� ����
	sniperComp->SetupAttachment(GetMesh(), TEXT("sniperSocket"));
	sniperBack->SetupAttachment(GetMesh(), TEXT("sniperSocket"));

	// Sniper Static Mesh Data Load
	ConstructorHelpers::FObjectFinder<UStaticMesh> sniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	
	// ������ �ε尡 �����ߴٸ�
	if (sniperMesh.Succeeded())
	{	// ����ƽ �޽� �����͸� �Ҵ��Ѵ�.
		if (sniperComp != nullptr)
		{
			sniperComp->SetStaticMesh(sniperMesh.Object);
			// Sniper ��ġ ����
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
	
	//Rifle ������Ʈ ���
	rifleComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rifleComp"));
	rifleBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rifleBack"));

	//�θ� ������Ʈ�� Mesh ������Ʈ�� ����
	rifleComp->SetupAttachment(GetMesh(), TEXT("rifleSocket"));
	rifleBack->SetupAttachment(GetMesh(), TEXT("rifleSocket"));

	// Rifle Static Mesh Data Load
	ConstructorHelpers::FObjectFinder<UStaticMesh> rifleMesh(TEXT("/Script/Engine.StaticMesh'/Game/M4A1/M4A1.M4A1'"));
	
	// ������ �ε尡 �����ߴٸ�
	if (rifleMesh.Succeeded())
	{
		if (rifleComp != nullptr)
		{
			// ����ƽ �޽� �����͸� �Ҵ��Ѵ�.
			rifleComp->SetStaticMesh(rifleMesh.Object);
			// Rifle ��ġ ����
			//rifleComp->SetRelativeLocationAndRotation(FVector((-23.209405,  68.760827,  140.746834)), FRotator(0, 180, 0));

			rifleComp->SetRelativeRotation(FRotator(0, -90, 0));
			rifleComp->SetRelativeLocation(rrifleLoc);
			rifleComp->SetRelativeScale3D(FVector(1.5, 2.0, 1.5));
		}
	}

	ScopeCamLocMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScopeCamLocMesh"));
	ScopeCamLocMesh->SetupAttachment(rifleComp);



}

void APlayerCharacter::OnMySniperReload()
{
	if (bUsingSniper) {
		allSniperAmmo -= (maxSniperAmmo - sniperAmmo);
		sniperAmmo = maxSniperAmmo;
	}
	else
	{
		allRifleAmmo -= (maxRifleAmmo - rifleAmmo);
		rifleAmmo = maxRifleAmmo;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	sniperAmmo = maxSniperAmmo;
	rifleAmmo = maxRifleAmmo;
	allSniperAmmo = 24;
	allRifleAmmo = 120;
	

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	crosshairUI = CreateWidget<UUserWidget>(GetWorld(), crosshairFactory);
	hitUI = CreateWidget<UUserWidget>(GetWorld(), hitAimFactory);
	rifleAmmoUI = CreateWidget<UUserWidget>(GetWorld(), rifleAmmoFactory);
	sniperAmmoUI = CreateWidget<UUserWidget>(GetWorld(), sniperAmmoFactory);
	crosshairUI->AddToViewport();
	sniperUI = CreateWidget<UUserWidget>(GetWorld(), sniperFactory);
	sniperDotUI = CreateWidget<UUserWidget>(GetWorld(), sniperDotFactory);
	if (!sniperAmmoUI->IsInViewport())
	{
		sniperAmmoUI->AddToViewport();
	
	}
	
	
	ChangeToSniper();
	rifleComp->SetVisibility(false);
	//OnActionZoomRelease();

		PlayerController = Cast<AMyPlayerController>(GetController());

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ADoor::StaticClass());

	PrisonDoor = Cast<ADoor>(actor);
	


	
	
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

	if (isZooming)
	{
		PlayerController->UIWeapon->ZoomSet();
		
	}
	else if (IsZoomEnd && bUsingSniper)
	{
		PlayerController->UIWeapon->ZoomOut();
	}

	else if (!bUsingSniper)
	{
		PlayerController->UIWeapon->RifleSet();
	}
	else if (bUsingSniper)
	{
		PlayerController->UIWeapon->SniperSet();
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 라이플을 장착하고 있다면
	/*if(bUsingSniper==false)
	{
		// 사격 가능한 상태이면서 왼쪽 마우스 버튼이 눌려져 있다면
		if(CanShoot&&isLMBC)
		{
			// 라이플 줌을 하고 있는 상태라면
			if(isRifleZooming)
			{
				FVector startLoc = ScopeCamLocMesh->K2_GetComponentLocation();
				FVector EndLoc = startLoc + ScopeCamLocMesh->GetForwardVector()*10000.0f;
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // LineTrace로 히트 가능한 오브젝트 유형들.
				TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
				TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
				TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
				TEnumAsByte<EObjectTypeQuery> PhysicsBody = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
				ObjectTypes.Add(WorldStatic);
				ObjectTypes.Add(WorldDynamic);
				ObjectTypes.Add(Pawn);
				ObjectTypes.Add(PhysicsBody);
				TArray<AActor*> ActorsToIgnore;
				isZoomingLineTraceHitted = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),startLoc, EndLoc, ObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::None, zoomingHitResult, true);
			}
			// 라이플 줌을 하고 있지 않은 상태라면
			else
			{
				FVector startLoc = cameraComp->K2_GetComponentLocation();
				FVector EndLoc = startLoc + cameraComp->GetForwardVector()*10000.0f;
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // LineTrace로 히트 가능한 오브젝트 유형들.
				TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
				TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
				TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
				TEnumAsByte<EObjectTypeQuery> PhysicsBody = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
				ObjectTypes.Add(WorldStatic);
				ObjectTypes.Add(WorldDynamic);
				ObjectTypes.Add(Pawn);
				ObjectTypes.Add(PhysicsBody);
				TArray<AActor*> ActorsToIgnore;
				isNotZoomingLineTraceHitted = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),startLoc, EndLoc, ObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::None, notZoomingHitResult, true);
			}
			// Clamp를 통한 탄약 수 차감
			rifleAmmo = FMath::Clamp(rifleAmmo-1, 0, 30);
			// 라이플 탄약 수가 0발 이하라면
			if(rifleAmmo<=0)
			{
				// 사격할 수 없는 상태로 전환
				isRifleShootableC=false;
			}
			// 라이플을 사격할 수 있는 상태이면서
			if(isRifleShootableC)
			{
				// 라이플 줌을 하고 있는 상태라면
				if(isRifleZooming)
				{
					// 줌 상태의 라인 트레이스가 적중했다면
					if(isZoomingLineTraceHitted)
					{
						// AddControllerInput와 RandomFloat을 통한 랜덤 반동 구현
						auto randF = UKismetMathLibrary::RandomFloatInRange(-0.4, -1.5);
						auto randF2 = UKismetMathLibrary::RandomFloatInRange(-0.5, 0.5);
						AddControllerPitchInput(randF);
						AddControllerYawInput(randF2);
						auto soundLoc = rifleComp->GetSocketTransform(FName("RifleFirePosition")).GetLocation();
						auto bulSoundLoc = GetActorLocation()*FVector(0, 0, -80);
						// 사격 사운드와 탄피 드랍 사운드 재생
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RifleFireSound, soundLoc);
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RifleBulletShellDropSound, bulSoundLoc, FRotator::ZeroRotator, 0.4, 1, 0);
						// 애니메이션 인스턴스 캐스팅
						auto animInst=Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
						// 캐스팅이 성공했다면
						if(animInst)
						{
							// 사격 애니메이션 몽타주 재생
							animInst->PlayAttackAnim();
							// 라이플 줌을 하고 있는 상태라면
							if(isRifleZooming)
							{
								auto fireLoc = rifleComp->GetSocketTransform(FName("RifleFirePosition")).GetLocation();
								UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTrailSystem, zoomingHitResult.Location, FRotator::ZeroRotator,FVector(1), true, true, ENCPoolMethod::AutoRelease);
								UFXSystemComponent fxSys;
								fxSys.SetVectorParameter(FName("EndPoint"), fireLoc);
								if(isRifleZooming)
								{
									if(isRifleZooming)
									{
										// enemy 캐스팅
										auto enemy = Cast<AEnemy>(zoomingHitResult.GetActor());
										// 라인트레이스가 enemy에 적중했다면
										if(enemy)
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											// X Vector 값으로부터 Rotator 값 추출
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(zoomingHitResult.ImpactNormal);
											auto decalLoc = zoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											// 트랜스폼 값을 바탕으로 탄흔 액터 스폰
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											// 트랜스폼 값을 바탕으로 피가 튀는 파티클 스폰
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodParticle, decalLoc, decalRot);
											// enemy 적중할 시 나타나는 UI를 띄우고
											hitUI->AddToViewport();
											//사격 불가 상태로 전환
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											// 사격 가능 상태 전환을 위한 타이머 설정
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												// 1/BulletsPerSec 초 후에 사격 가능 상태로 전환
												CanShoot=true;
											}), 1/BulletsPerSec, false);
											enemy->RifleHit();
										}
										// 라인트레이스가 enemy에 적중하지 않았다면(지형지물에 적중했다면)
										else
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											// 줌 상태의 라인트레이스 적중 결과 노말의 X Vector 값으로부터 Rotator 값 추출
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(zoomingHitResult.ImpactNormal);
											// 라인트레이스 적중 결과의 위치값
											auto decalLoc = zoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											// 트랜스폼 값을 바탕으로 탄흔 액터 스폰
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											// 트랜스폼 값을 바탕으로 벽이나 바닥 잔해가 튀는 파티클 스폰
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletMarksParticle, decalLoc, decalRot);
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											// 사격 가능 상태 전환을 위한 타이머 설정
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												// 1/BulletsPerSec 초 후에 사격 가능 상태로 전환
												CanShoot=true;
											}), 1/BulletsPerSec, false);
										}
									}
									// 라이플 줌을 하고 있지 않은 상태라면
									else
									{
										// enemy 캐스팅
										auto enemy = Cast<AEnemy>(notZoomingHitResult.GetActor());
										// 라인트레이스가 enemy에 적중했다면
										if(enemy)
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											// 줌이 아닌 상태의 라인트레이스 적중 결과 노말의 X Vector 값으로부터 Rotator 값 추출
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(notZoomingHitResult.ImpactNormal);
											// 라인트레이스 적중 결과의 위치값
											auto decalLoc = notZoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											// 트랜스폼 값을 바탕으로 탄흔 액터 스폰
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											// 트랜스폼 값을 바탕으로 피가 튀는 파티클 스폰
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodParticle, decalLoc, decalRot);
											// enemy 적중할 시 나타나는 UI를 띄우고
											hitUI->AddToViewport();
											// 사격 불가능 상태로 전환
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											// 사격 가능 상태 전환을 위한 타이머 설정
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												// 1/BulletsPerSec 초 후에 사격 가능 상태로 전환
												CanShoot=true;
											}), 1/BulletsPerSec, false);
											enemy->RifleHit();
										}
										// 라인트레이스가 enemy에 적중하지 않았다면
										else
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(notZoomingHitResult.ImpactNormal);
											auto decalLoc = notZoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletMarksParticle, decalLoc, decalRot);
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												CanShoot=true;
											}), 1/BulletsPerSec, false);
										}
									}
								}
								else
								{
									auto fireSocketTrans = rifleComp->GetSocketTransform(FName("RifleFirePosition"));
									UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fireParticle, fireSocketTrans);
									if(isRifleZooming)
									{
										auto enemy = Cast<AEnemy>(zoomingHitResult.GetActor());
										if(enemy)
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(zoomingHitResult.ImpactNormal);
											auto decalLoc = zoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodParticle, decalLoc, decalRot);
											hitUI->AddToViewport();
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												CanShoot=true;
											}), 1/BulletsPerSec, false);
											enemy->RifleHit();
										}
										else
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(zoomingHitResult.ImpactNormal);
											auto decalLoc = zoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletMarksParticle, decalLoc, decalRot);
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												CanShoot=true;
											}), 1/BulletsPerSec, false);
										}
									}
									else
									{
										auto enemy = Cast<AEnemy>(notZoomingHitResult.GetActor());
										if(enemy)
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(notZoomingHitResult.ImpactNormal);
											auto decalLoc = notZoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodParticle, decalLoc, decalRot);
											hitUI->AddToViewport();
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												CanShoot=true;
											}), 1/BulletsPerSec, false);
											enemy->RifleHit();
										}
										else
										{
											FActorSpawnParameters param;
											param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
											auto decalRot = UKismetMathLibrary::Conv_VectorToRotator(notZoomingHitResult.ImpactNormal);
											auto decalLoc = notZoomingHitResult.Location;
											auto decalTrans = UKismetMathLibrary::MakeTransform(decalLoc, decalRot);
											GetWorld()->SpawnActor<AActor>(ShotDecalFactory, decalTrans);
											UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletMarksParticle, decalLoc, decalRot);
											CanShoot=false;
											FTimerHandle shootEnableHandle;
											GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
											{
												CanShoot=true;
											}), 1/BulletsPerSec, false);
										}
									}
								}
							}
							else
							{
								auto fireLoc = rifleComp->GetSocketTransform(FName("RifleFirePosition")).GetLocation();
								UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTrailSystem, notZoomingHitResult.Location, FRotator::ZeroRotator,FVector(1), true, true, ENCPoolMethod::AutoRelease);
								UFXSystemComponent fxSys;
								fxSys.SetVectorParameter(FName("EndPoint"), fireLoc);
							}
						}
						
						FActorSpawnParameters param;
						param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						auto spawnTrans = rifleComp->GetSocketTransform(FName("BulletShell"));
						auto bulletShell = GetWorld()->SpawnActor<AActor>(BulletShellFactory, spawnTrans);
						bulletShell->SetLifeSpan(5.0f);
					}
					else
					{
						auto randF = UKismetMathLibrary::RandomFloatInRange(-0.4, -1.5);
						auto randF2 = UKismetMathLibrary::RandomFloatInRange(-0.5, 0.5);
						AddControllerPitchInput(randF);
						AddControllerYawInput(randF2);
						auto soundLoc = rifleComp->GetSocketTransform(FName("RifleFirePosition")).GetLocation();
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RifleFireSound, soundLoc);
						if(isRifleZooming)
						{
							FVector startLoc = ScopeCamLocMesh->K2_GetComponentLocation();
							FVector ForwardLoc = startLoc + ScopeCamLocMesh->GetForwardVector()*10000.0f;
							auto FireLoc = rifleComp->GetSocketTransform(FName("RifleFirePosition")).GetLocation();
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTrailSystem, ForwardLoc, FRotator::ZeroRotator, FVector(1), true, true, ENCPoolMethod::AutoRelease);
							UFXSystemComponent fxSys;
							fxSys.SetVectorParameter(FName("EndPoint"), FireLoc);
						}
						else
						{
							FVector startLoc = cameraComp->K2_GetComponentLocation();
							FVector ForwardLoc = startLoc + cameraComp->GetForwardVector()*10000.0f;
							auto FireLoc = rifleComp->GetSocketTransform(FName("RifleFirePosition")).GetLocation();
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTrailSystem, ForwardLoc, FRotator::ZeroRotator, FVector(1), true, true, ENCPoolMethod::AutoRelease);
							UFXSystemComponent fxSys;
							fxSys.SetVectorParameter(FName("EndPoint"), FireLoc);
						}
						if(isRifleZooming)
						{
							CanShoot=false;
							FTimerHandle shootEnableHandle;
							GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
							{
								CanShoot=true;
							}), 1/BulletsPerSec, false);
						}
						else
						{
							auto particleTrans = rifleComp->GetSocketTransform(FName("RifleFirePosition"));
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), fireParticle, particleTrans);
							CanShoot=false;
							FTimerHandle shootEnableHandle;
							GetWorldTimerManager().SetTimer(shootEnableHandle, FTimerDelegate::CreateLambda([this]()->void
							{
								CanShoot=true;
							}), 1/BulletsPerSec, false);
						}
					}
				}
				else
				{
					if(isNotZoomingLineTraceHitted)
					{
						auto randF = UKismetMathLibrary::RandomFloatInRange(-0.4, -1.5);
						auto randF2 = UKismetMathLibrary::RandomFloatInRange(-0.5, 0.5);
						AddControllerPitchInput(randF);
						AddControllerYawInput(randF2);
					}
					else
					{
						auto randF = UKismetMathLibrary::RandomFloatInRange(-0.4, -1.5);
						auto randF2 = UKismetMathLibrary::RandomFloatInRange(-0.5, 0.5);
						AddControllerPitchInput(randF);
						AddControllerYawInput(randF2);
					}
				}
			}
		}
		
	}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











	
	/*
	if (!(PlayerController->MainWid->IsGlitch) && GetWorld()->GetName() == FString("MainLevel"))
	{
		PlayerController->MainWid->MainScreen();
	}

	if (GetWorld()->GetName() == FString("MainLevel"))
	{

		SetActorHiddenInGame(true);
		DisableInput(PlayerController);
		PlayerController->bShowMouseCursor = true;
	


	}

	if (GetWorld()->GetName() == FString("FinalLevel") && !IsRemove)
	{
		PlayerController->MainWid->RemoveFromParent();
		IsRemove = true;

	}
	
	if (IsCount)
	{
		PrisonDoor->count += 1;
		PrisonDoor->CountProgress->UpdateCount(PrisonDoor->count, PrisonDoor->max);
		if (PrisonDoor->count >= PrisonDoor->max)
		{
			
			DisableInput(PlayerController);
			UWidgetLayoutLibrary::RemoveAllWidgets(this);

		}
	}
	if (IsFinal)
	{
		KeepCount += DeltaTime;
		cameraComp->SetFieldOfView(FMath::Lerp(90.0f, 170.0f, KeepCount * 0.2f));
		if (KeepCount * 0.2f >= 1)
		{
			IsFinal = false;
			IsEndLev = true;
			PlayerController->UIEndGame->IsEndLevel = true;
			UGameplayStatics::OpenLevel(GetWorld(), FName("EndLevel"));
		}
	}
	if (GetWorld()->GetName() == FString("EndLevel") && !IsRealEnd)
	{
		PlayerController->MainWid->RemoveFromParent();
		SetActorHiddenInGame(true);
		PlayerController->UIEndGame->AddToViewport();
		PlayerController->UIEndGame->EndScreen();
		IsRealEnd = true;

	}
	
	*/
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerCharacter::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerCharacter::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Look Up"), this, &APlayerCharacter::OnAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right"), this, &APlayerCharacter::OnAxisTurnRight);

	// ZoomInAxis �̺�Ʈ ó�� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &APlayerCharacter::OnActionZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &APlayerCharacter::OnActionZoomOut);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::OnActionJump);
	// Crouch �̺�Ʈ ó�� �Լ� ���ε�	
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayerCharacter::OnActionCrouch);
	// Crouch �̺�Ʈ ���� �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayerCharacter::OnActionCrouchRelease);
	// Zoom �̺�Ʈ �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &APlayerCharacter::OnActionZoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &APlayerCharacter::OnActionZoomRelease);
	// Fire �̺�Ʈ �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::OnActionFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayerCharacter::OnActionFireRelease);
	// Sniper ���� �̺�Ʈ �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Sniper"),  IE_Pressed, this, &APlayerCharacter::ChangeToSniper);
	// Rifle ���� �̺�Ʈ �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &APlayerCharacter::ChangeToRifle);
	// Dash �̺�Ʈ �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &APlayerCharacter::OnActionDash);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &APlayerCharacter::OnActionReload);
	// DashReleased �̺�Ʈ �Լ� ���ε�
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Released, this, &APlayerCharacter::OnActionDashReleased);
	PlayerInputComponent->BindAction(TEXT("LaserSwitch"), IE_Released, this, &APlayerCharacter::OnActionLaser);
	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Pressed, this, &APlayerCharacter::OnActionLookAroundPressed);
	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Released, this, &APlayerCharacter::OnActionLookAroundReleased);
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &APlayerCharacter::OnActionInteraction);
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Released, this, &APlayerCharacter::OnActionInteractionEnd);


}
	
	
void APlayerCharacter::OnAxisHorizontal(float value) {
	direction.Y = value;
}

void APlayerCharacter::OnAxisVertical(float value) {
	if (isHanging == true)
	{
		return;
	}
	direction.X = value;
}

void APlayerCharacter::OnAxisLookUp(float value) {
	AddControllerPitchInput(value);
}

void APlayerCharacter::OnAxisTurnRight(float value) {
	AddControllerYawInput(value);
}

void APlayerCharacter::OnActionZoomIn() {
	if (bUsingSniper)
	{
		if (isZooming)
		{
			cameraComp->FieldOfView -= 3;		
		}
	}
		
}

void APlayerCharacter::OnActionZoomOut() {
	if (bUsingSniper)
	{
		if (isZooming)
		{			
			cameraComp->FieldOfView += 3;			
		}
	}
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
		isZooming = true;	
		
		sniperUI->AddToViewport();
		sniperDotUI->AddToViewport();
		cameraComp->SetFieldOfView(20.0f);

		crosshairUI->RemoveFromParent();
		IsZoomEnd = false;
	
	}
	else
	{		
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
		sniperDotUI->RemoveFromParent();
		crosshairUI->AddToViewport();
		cameraComp->SetFieldOfView(90.0f);
		isZooming = false;
		//scopeCaptureComponent->SetVisibility(false);
		//scopeCaptureComponent->FOVAngle = 90.0;
		//scopePlane->SetVisibility(false);
		//scopeBack->SetVisibility(false);
		IsZoomEnd = true;
	}
	else
	{
		GetMesh()->SetVisibility(true);
		sniperUI->RemoveFromParent();
		sniperDotUI->RemoveFromParent();
		//cameraComp->FieldOfView = 90.0f;
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

	 // GetWorldTimerManager().ClearTimer(fireTimerHandle);
	 // GetWorld()->GetTimerManager().SetTimer(sniperShot, this, &APlayerCharacter::ThrowBack, 5, true);
 }

 void APlayerCharacter::ChangeToSniper() {

	  auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	 
	 bool  isMontagePlaying = anim->Montage_IsPlaying(anim->swapAnimMontage);
	 if (isMontagePlaying || bUsingSniper==true)
	 {
		 return;
	 }
	 if (isProne == true)
	 {
		 anim->PlayProneSwapAnim();
	 }
	 else {
		 anim->PlaySwapAnim();
	 }
	 
	 bUsingSniper = true;
	 //sniperComp->SetVisibility(true);
	 //rifleComp->SetVisibility(false);
	 if (!sniperAmmoUI->IsInViewport())
	 {
		sniperAmmoUI->AddToViewport();

	 }
	 rifleAmmoUI->RemoveFromParent();
	//sniperBack->SetVisibility(false);
	 //rifleBack->SetVisibility(true);

	 //UE_LOG(LogTemp, Warning, TEXT("Sniper"))
 }
 
 void APlayerCharacter::ChangeToRifle() {
	 auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	 bool  isMontagePlaying = anim->Montage_IsPlaying(anim->swapAnimMontage);
	 OnActionZoomRelease();
	 if (isMontagePlaying || bUsingSniper==false)
	 {
		 return;
	 }
	 if (isProne == true)
	 {
		 anim->PlayProneSwapAnim();
	 }
	 else {
		 anim->PlaySwapAnim();
	 }
	 
	 bUsingSniper = false;
	 //sniperComp->SetVisibility(false);
	 //rifleComp->SetVisibility(true);
	 sniperAmmoUI->RemoveFromParent();
	 rifleAmmoUI->AddToViewport();
	 //sniperBack->SetVisibility(true);
	 //rifleBack->SetVisibility(false);

	 //UE_LOG(LogTemp, Warning, TEXT("Rifle"))

 }

 void APlayerCharacter::OnActionDash() {
	 
	 if (isCrouching || isProne || isRifleZooming || isZooming ==true) {
		 return;
	 }

	GetCharacterMovement()->MaxWalkSpeed  = runSpeed;

 }
 void APlayerCharacter::OnActionDashReleased() {

	 if (isCrouching || isProne || isRifleZooming || isZooming ==true) {
		 return;
	 }

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

void APlayerCharacter::OnActionReload()
{
	if(bUsingSniper == true)
	{
		if(sniperAmmo==maxSniperAmmo || allSniperAmmo<=0)
		{
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("%d"), sniperAmmo)
		auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		bool  isMontagePlaying = anim->Montage_IsPlaying(anim->attackAnimMontage);
		if(isMontagePlaying)
		{
			return;
		}
		if (isProne == true)
		{
			anim->PlayProneReloadAnim();
					}
		else {
			anim->PlayReloadAnim(TEXT("SniperReload"));
		}
		UGameplayStatics::PlaySound2D(GetWorld(), reloadSound);

	}
	else
	{
		if (rifleAmmo == maxRifleAmmo || allRifleAmmo <= 0)
		{
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("%d"), rifleAmmo)
			auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		bool  isMontagePlaying = anim->Montage_IsPlaying(anim->attackAnimMontage);
		if (isMontagePlaying)
		{
			return;
		}
		if (isProne == true)
		{
			anim->PlayProneReloadAnim();
		}
		else {
			anim->PlayReloadAnim(TEXT("SniperReload"));
		}
		UGameplayStatics::PlaySound2D(GetWorld(), reloadSound);
	}

}

void APlayerCharacter::OnActionLaser()
{
}

void APlayerCharacter::OnFire() {

	 FTimerHandle dieTimerHandle;
	 FTransform t = GetMesh()->GetSocketTransform(TEXT("sniperFire"));
	 FTransform s = GetMesh()->GetSocketTransform(TEXT("rifleFire"));
	 FVector sniperFireLoc = t.GetLocation();
	 FVector rifleFireLoc = s.GetLocation();
	 FHitResult hitInfo;
	 FVector start = cameraComp->GetComponentLocation();
	 end = start + cameraComp->GetForwardVector() * 100000;
	 FCollisionQueryParams params;
	 params.AddIgnoredActor(this);
	 // 스나이퍼를 사용 중이라면
	 if (bUsingSniper)
	 {
		// 스나이퍼 탄약이 1발 이상이라면
		if(sniperAmmo>0)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
			GetWorld()->SpawnActor<AActor>(sniperBulletShellFactory, sniperComp->GetSocketTransform(TEXT("sniperBulletShell")));
			sniperAmmo--;
		}
		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(), emptySound);
			return;
		}
		 auto controller = GetWorld()->GetFirstPlayerController();
		 controller->PlayerCameraManager->StartCameraShake(cameraShake);
		 bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params);
			 AEnemy* hittedActor = Cast<AEnemy>(hitInfo.GetActor());
			 sniperImpactPoint = hitInfo.ImpactPoint;
			 sniperTraceEnd = hitInfo.TraceEnd;
		  	FTransform trans(hitInfo.ImpactPoint);
			 if (bHit)
			 {

				 auto hitComp = hitInfo.GetComponent();
				 AEnemy* HittedActor = Cast<AEnemy>(hitInfo.GetActor());
				 if(HittedActor)
				 {
					 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodImpactFactory, trans);
					 HittedActor->SniperHit();
					 SniperHitTrail();
					 hitUI->AddToViewport();
				 }
				 else
				 {
					 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, trans);					 
				 }
			 //int32 randomBulletYaw = FMath::RandRange(1, 20);
			 //int32 randomBulletPitch = FMath::RandRange(1, 20);
			 //FRotator randomBullet = FRotator(randomBulletPitch, randomBulletYaw, 0);

			 //GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, sniperFireLoc, cameraComp->GetComponentRotation() + randomBullet);

			 
		 
				 /*auto decalLoc = hitInfo.Location;

				
				 //auto hitComp = hitInfo.GetComponent();
				 //auto hitEnemy = hitInfo.GetActor();
			 	;
				 FTransform trans(hitInfo.ImpactPoint);				
			
				 if(hittedActor)
				//if(hitInfo.GetActor()->GetClass()->GetName() == "BP_Enemy_C")
				{
					
				
					hittedActor->SniperHit();
				}
				else
				{
					
				}

				// bool isActor = Cast<AEnemy>(Get)

			 	/*auto decalLoc = hitInfo.Location;

				 auto hitIN = hitInfo.ImpactNormal;
				 auto hitRot = UKismetMathLibrary::Conv_VectorToRotator(hitIN);
				 auto decalRot = UKismetMathLibrary::MakeTransform(hitInfo.Location, hitRot);
				
				 UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decalFactory, FVector(1, 1, 1), decalLoc, hitRot);*/
				// if(hitInfo==)			 	
				 //if (hitComp != nullptr && hitComp->IsSimulatingPhysics())
				 //{
					 //FVector force = -hitInfo.ImpactNormal * hitComp->GetMass();
					// hitComp->AddForce(force);
					 //UE_LOG(LogTemp, Warning, TEXT("ischased"))
					 //FVector forceDir = (hitInfo.TraceEnd - hitInfo.TraceStart).GetSafeNormal();

					 //FVector force = forceDir * 500000 * hitComp->GetMass();
					 //hitComp->AddForce(force);
					//auto enemy = Cast<AEnemy>()

					 
					 
				 //}
			 }
			 else
			 {		
				 SniperNotHitTrail();
			 }
		 }
	 else
	 {
	 return;
	 }
	 }
	 //int32 randomBulletYaw = FMath::RandRange(1, 10);
		 //int32 randomBulletPitch = FMath::RandRange(1, 10);
		 //FRotator randomBullet = FRotator(randomBulletPitch, randomBulletYaw, 0);

		 //GetWorld()->SpawnActor<APlayerBullet>(bulletFactory, rifleFireLoc, cameraComp->GetComponentRotation() + randomBullet);
	 

		
 

 void APlayerCharacter::OneShot() {
	 if (count == 1) {
		 //UE_LOG(LogTemp, Warning, TEXT("SniperFire"))
			 OnFire();
			 count = 0;
			 //ī�޶� ����ũ ���
			 
	 }

 }

 void APlayerCharacter::ThrowBack(float deltaTime)
 {
	// 카운트 변수값이 0일 때
	 if (count == 0)
	 {
	 	 // 현재 시간에 델타타임을 누적시킨다.
		 curTime += deltaTime;
	 	 // 누적된 시간이 1.2초 이상일 때
		 if (curTime >= 1.2f)
		 {
		 	// 스나이퍼 장전 소리와 탄피 드랍 사운드 재생
			 UGameplayStatics::PlaySound2D(GetWorld(), reloadBulletSound);
		 	 UGameplayStatics::PlaySound2D(GetWorld(), sniperShellDropSound);
			// 카운트 1로 초기화
		 	count = 1;
		 	// 누적된 시간 초기화
		 	curTime = 0;			 
		 }
	 }
 }

 
 void APlayerCharacter::OnActionInteraction()
 {
	 if (PrisonDoor->IsIn)
	 {
		 IsCount = true;
		 PrisonDoor->SoundStart();
	 }
 }

 void APlayerCharacter::OnActionInteractionEnd()
 {
	 PrisonDoor->count = 0;
	 IsCount = false;
	 
 }