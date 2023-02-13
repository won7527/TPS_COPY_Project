// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Components/WidgetComponent.h"
#include "CountProgressUI.h"
#include "Prisoner.h"
#include "Components/AudioComponent.h"


// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	SetRootComponent(BoxComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	MeshComp->SetupAttachment(RootComponent);
	CountWid = CreateDefaultSubobject<UWidgetComponent>(TEXT("countwid"));
	CountWid->SetupAttachment(RootComponent);
	ConstructorHelpers::FClassFinder<UUserWidget>CountProg(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TW_BP/UI_DoorInteraction.UI_DoorInteraction_C'"));
	if (CountProg.Succeeded())
	{
		CountWid->SetWidgetClass(CountProg.Class);
	}
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::Overlapped);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ADoor::EndOverlapped);

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
	savior = Cast<APlayerCharacter>(actor);

	auto actor1 = UGameplayStatics::GetActorOfClass(GetWorld(), APrisoner::StaticClass());
	prisoner = Cast<APrisoner>(actor1);

	CountProgress = Cast<UCountProgressUI>(CountWid->GetUserWidgetObject());
	
	CountProgress->owner = this;

	if (CountProgress != nullptr) {
		
	}
	else {
		FString s = CountWid->GetWidgetClass()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("NUll CountProgressBar : %s"),*s);
		UE_LOG(LogTemp, Warning, TEXT("NUll CountProgressBar"));
	}


}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (count > 500)
	{
		Open();
	}
	if (!IsIn || !savior->IsCount)
	{
		AccumulatedAngle = 0;
		if (DoorSound)
		{
			DoorSound->Stop();
		}
		
	}
	if (count >= 0)
	{
	AccumulatedAngle -= OpenAngle * count;
	MeshComp->SetRelativeRotation(FRotator(0, AccumulatedAngle, 0));

	}

}

void ADoor::Overlapped(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor == savior)
	{
	
		IsIn = true;
		CountWid->SetVisibility(true);
	}

}

void ADoor::EndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == savior)
	{
		IsIn = false;
		savior->IsCount = false;
		count = 0;
		CountWid->SetVisibility(false);
	}

}
void ADoor::Open()
{
	prisoner->StandUpAnimPlay();
	Destroy();
}

void ADoor::SoundStart()
{
	DoorSound = UGameplayStatics::SpawnSound2D(this, DoorOpenSound);

	if (DoorSound)
	{
		//DoorSound->Play();
	}
}