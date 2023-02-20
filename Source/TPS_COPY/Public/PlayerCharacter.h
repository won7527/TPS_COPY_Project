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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* cameraComp;

	// �������� ���̷�Ż�޽� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SniperMesh)
	class UStaticMeshComponent* sniperComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SniperMesh)
	class UStaticMeshComponent* sniperBack;

	// ������ ���̷�Ż�޽� �߰�
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

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> sniperDotFactory;

	UPROPERTY()
		class UUserWidget* sniperUI;
	UPROPERTY()
		class UUserWidget* sniperDotUI;





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

	UPROPERTY(EditDefaultsOnly, Category = SoundSetting)
		class USoundBase* sniperShellDropSound;



	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisLookUp(float value);
	void OnAxisTurnRight(float value);
	UFUNCTION(BlueprintCallable)
	void OnActionZoomIn();
	UFUNCTION(BlueprintCallable)
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
	UFUNCTION(BlueprintCallable)
	void OnActionReload();
	UFUNCTION(BlueprintCallable)
	void OnActionLaser();
	void OnFire();

	void OnActionInteraction();
	void OnActionInteractionEnd();
	
	FVector direction;

	// �������� ��� ���ΰ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Setting")
	bool bUsingSniper = true;

	// �������۷� ����
	UFUNCTION(BlueprintCallable)
	void ChangeToSniper();

	// �����÷� ����
	UFUNCTION(BlueprintCallable)
	void ChangeToRifle();

	UPROPERTY(EditAnywhere, Category="Factory Setting")
	TSubclassOf<class APlayerBullet> bulletFactory;
	
	UPROPERTY(EditAnywhere)		
	TSubclassOf<class UUserWidget> crosshairFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> hitAimFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> rifleAmmoFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> sniperAmmoFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> sniperBulletShellFactory;

	UPROPERTY(BlueprintReadOnly)
	class UUserWidget* crosshairUI;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* hitUI;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* rifleAmmoUI;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* sniperAmmoUI;


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
	float runSpeed = 450;
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
		int allSniperAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSetting)
	int maxSniperAmmo = 8;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSetting)
	int sniperAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSetting)
		int allRifleAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSetting)
		int maxRifleAmmo = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSetting)
		int rifleAmmo;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCount;

	bool IsFinal;
	float KeepCount;

	bool IsEndLev;
	bool IsRealEnd;
	bool IsZoomEnd;
	bool isRifleZooming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isProne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCrouching;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHanging;
};
