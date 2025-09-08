// Fill out your copyright notice in the Description page of Project Settings.


#include "SequenceManager.h"
#include "Components/BoxComponent.h"

// Sets default values
ASequenceManager::ASequenceManager()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	// Taille de la box
	BoxComponent->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	// Collision
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASequenceManager::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASequenceManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASequenceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASequenceManager::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap détecté avec %s"), *OtherActor->GetName());

	if (OtherActor != this && OtherActor == ActorActive && bHasTriggered == false)
	{
		bHasTriggered = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlap avec ActorActive !"));
		if (!LevelSequence) return; // Vérifie que la séquence est assignée

		FMovieSceneSequencePlaybackSettings Settings; // Paramètres de lecture
		ALevelSequenceActor* OutActor; // Actor créé pour la séquence
		Settings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;

		// Crée le player et l’actor dans le monde
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),LevelSequence, Settings, OutActor);

		if (SequencePlayer)
		{
			SequencePlayer->Play(); // Lancer la séquence
		}
	}
}

