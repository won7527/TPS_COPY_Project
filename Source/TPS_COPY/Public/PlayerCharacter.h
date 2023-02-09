// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TPS_COPY_API APlayerCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	FGenericTeamId TeamId;

	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return TeamId;
	}

	void OnMySniperReload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* cameraComp;

	// 스나이퍼 스켈레탈메시 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SniperMesh)
	class UStaticMeshComponent* sniperComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SniperMesh)
	class UStaticMeshComponent* sniperBack;

	// 라이플 스켈레탈메시 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RifleMesh)
	class UStaticMeshComponent* rifleComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RifleMesh)
	class UStaticMeshComponent* rifleBack;

	//UPROPERTY(EditAnywhere, Category = SniperMesh)
	//class USceneCaptureComponent2D* scopeCaptureComponent;

	//UPROPERTY(EditAnywhere, Category = SniperMesh)
	//class UStaticMeshComponent* scopePlane;

	//UPROPERTY(EditAnywhere, Category = SniperMesh)
	//class UStaticMeshComponent* scopeBack;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* bloodImpactFactory;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* fireSmokeFactory;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* shotFireFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> sniperFactory;

	UPROPERTY()
		class UUserWidget* sniperUI;



	//UPROPERTY(EditAnywhere)
	//class UMaterialInterface* decalFactory;

	UPROPERTY(EditDefaultsOnly, Category=CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* fireMontageFactory;

	UPROPERTY(EditDefaultsOnly, Category = SoundSetting)
		class USoundBase* fireSound;

	UPROPERTY(EditDefaultsOnly, Category = SoundSetting)
		class USoundBase* zoomInSound;

	UPROPERTY(EditDefaultsOnly, Category = SoundSetting)
		class USoundBase* zoomOutSound;;

	UPROPERTY(EditDefaultsOnly, Category = SoundSetting)
		class USoundBase* emptySound;

	UPROPERTY(EditDefaultsOnly, Category = SoundSetting)
		class USoundBase* reloadSound;

	UPROPERTY(EditDefaultsOnly, Category = SoundSetting)
		class USoundBase* reloadBulletSound;



	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookUp(float value);
	void OnAxisTurnRight(float value);
	void OnActionZoomIn();
	void OnActionZoomOut();
	void OnActionJump();
	void OnActionZoom();
	void OnActionZoomRelease();
 	void OnActionCrouch();
 	void OnActionCrouchRelease();
	void OnActionFire();
	void OnActionFireRelease();
	void OnActionDash();
	void OnActionDashReleased();
	void OnActionLookAroundPressed();
	void OnActionLookAroundReleased();
	void OnActionReload();
	void OnFire();

	void OnActionInteraction();
	void OnActionInteractionEnd();
	
	FVector direction;

	// 스나이퍼 사용 중인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Setting")
	bool bUsingSniper = true;

	// 스나이퍼로 변경
	void ChangeToSniper();

	// 라이플로 변경
	void ChangeToRifle();

	UPROPERTY(EditAnywhere, Category="Factory Setting")
	TSubclassOf<class APlayerBullet> bulletFactory;
	
	UPROPERTY(EditAnywhere)		
	TSubclassOf<class UUserWidget> crosshairFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> hitAimFactory;

	UPROPERTY()
	class UUserWidget* crosshairUI;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* hitUI;


	float sniperFireInterval = 3.1f;
	float rifleFireInterval = 0.1f;

	FTimerHandle fireTimerHandle;

	float curTime = 0;
	float coolTime = 3.0f;

	int32 count = 1;
	FTimerHandle sniperShot;

	void OneShot();

	void ThrowBack(float deltaTime);

	UPROPERTY(BlueprintReadOnly)
	bool isZooming = false;
//	UNiagaraSystem* niagaraTemplate;
	
	UPROPERTY(EditAnywhere, Category=PlayerSetting)
	float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float runSpeed = 600;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	FVector sniperLoc = FVector (40, 0, 30);
	FVector rifleLoc = FVector(-42, 7, 1);
	FVector fireLoc;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		FVector rrifleLoc = FVector(3.790621, 53.665231, 0.000000);

	class AMyPlayerController* PlayerController;


	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSetting)
	//int maxRifleAmmo = 20;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSetting)
	//int rifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSetting)
	int maxSniperAmmo = 8;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSetting)
	int sniperAmmo;
	UFUNCTION(BlueprintImplementableEvent)
	void SniperHitTrail();
	UFUNCTION(BlueprintImplementableEvent)
	void SniperNotHitTrail();
	UPROPERTY(BlueprintReadOnly)
	FVector_NetQuantize sniperImpactPoint;
	UPROPERTY(BlueprintReadOnly)
	FVector_NetQuantize	sniperTraceEnd;
	UPROPERTY(BlueprintReadOnly)
	FVector end;

	bool IsRemove = false;

	class ADoor* PrisonDoor;
	bool IsCount;

	bool IsFinal;
	float KeepCount;

	bool IsEndLev;
	bool IsRealEnd;

};
