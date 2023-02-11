// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class TPS_COPY_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* DoorOpenSound;
	class UAudioComponent* DoorSound;
	void SoundStart();

	UFUNCTION()
	void Overlapped(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere)
	class APlayerCharacter* savior;
	UPROPERTY(EditAnywhere)
	class APrisoner* prisoner;

	void Open();
	bool IsIn;
	float count;
	float max = 500;
	float OpenAngle = 180 / (max * (1 + max));
	float AccumulatedAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* CountWid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCountProgressUI* CountProgress;
};
