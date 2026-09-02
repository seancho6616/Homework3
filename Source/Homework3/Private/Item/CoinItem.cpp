// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CoinItem.h"
#include "Engine/World.h"
#include "HomeworkGameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString(TEXT("Player Gained %d points!"), PointValue));
		if (UWorld* World = GetWorld())
		{
			if (AHomeworkGameState* GameState = World->GetGameState<AHomeworkGameState>())
			{
				GameState->AddScore(PointValue);
				UE_LOG(LogTemp, Warning, TEXT("Fuck1"))
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}