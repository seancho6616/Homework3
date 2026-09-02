// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeworkGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "Item/CoinItem.h"
#include "GameInstance/HomeworkGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Controller/NBCPlayerController.h"

AHomeworkGameState::AHomeworkGameState()
{
	Score = 0;
	SpawnCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AHomeworkGameState::BeginPlay()
{
	Super::BeginPlay();
	//UpdateHUD();
	StartLevel();
	int32 num = 0;
	for (FName name : LevelMapNames)
	{
		if (!name.IsNone())
		{
			num++;
		}
	}
	MaxLevels = num;

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AHomeworkGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 AHomeworkGameState::GetScore() const
{
	return Score;
}

void AHomeworkGameState::AddScore(int32 Amount)
{
	/*Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score : %d"), Score);*/

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UE_LOG(LogTemp, Warning, TEXT("Fuck2"))

			UHomeworkGameInstance* HomeworkInstance = Cast<UHomeworkGameInstance>(GameInstance);
		if (HomeworkInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Fuck3"))

			HomeworkInstance->AddToScore(Amount);
		}
	}
}

void AHomeworkGameState::StartLevel()
{
	/*if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHomeworkGameInstance* HomeworkGameInstance = Cast<UHomeworkGameInstance>(GameInstance);
		if (HomeworkGameInstance)
		{
			CurrentLevelIndex = HomeworkGameInstance->CurrentLevelIndex;
		}
	}*/
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ANBCPlayerController* NBCPlayerController = Cast<ANBCPlayerController>(PlayerController))
		{
			NBCPlayerController->ShowGameHUD();
		}
	}
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHomeworkGameInstance* HomeworkGameInstance = Cast<UHomeworkGameInstance>(GameInstance);
		if (HomeworkGameInstance)
		{
			CurrentLevelIndex = HomeworkGameInstance->CurrentLevelIndex;
		}
	}
	SpawnCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	
	const int32 ItemToSpawn = 20;

	/*for (int32 i = 0; i < ItemToSpawn; ++i)
	{*/
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				for (int32 i = 0; i < ItemToSpawn; ++i)
				{
					AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
					if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnCoinCount++;
					}
				}
			}
		}
	/*}*/

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AHomeworkGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!!"), CurrentLevelIndex + 1);
	UE_LOG(LogTemp, Warning, TEXT("Level %d Start! Spawned %d coin"), CurrentLevelIndex + 1, SpawnCoinCount);
}

void AHomeworkGameState::OnLevelTimeUp()
{
	EndLevel();
}

void AHomeworkGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected : %d / %d"), CollectedCoinCount, SpawnCoinCount);

	if (SpawnCoinCount > 0 && CollectedCoinCount >= SpawnCoinCount)
	{
		EndLevel();
	}
}


void AHomeworkGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHomeworkGameInstance* HomeworkGameInstance = Cast<UHomeworkGameInstance>(GameInstance);
		if (HomeworkGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			HomeworkGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			if (CurrentLevelIndex >= MaxLevels)
			{
				OnGameOver();
				return;
			}
			if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
			{
				UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
			}
			else
			{
				OnGameOver();
			}
		}
	}
}

void AHomeworkGameState::OnGameOver()
{
	//UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ANBCPlayerController* NBCPlayerController = Cast<ANBCPlayerController>(PlayerController))
		{
			NBCPlayerController->SetPause(true);
			NBCPlayerController->ShowMainMenu(true);
		}
	}
}

void AHomeworkGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ANBCPlayerController* NBCCotroller = Cast<ANBCPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = NBCCotroller->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingTime)));
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UHomeworkGameInstance* HomeworkGameInstance = Cast<UHomeworkGameInstance>(GameInstance);
						if (HomeworkGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), HomeworkGameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* Level = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LevelText"))))
				{
					Level->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurrentLevelIndex+1)));
				}
				if (UTextBlock* Item = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ItemText"))))
				{
					Item->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CollectedCoinCount, SpawnCoinCount)));
				}
			}

		}
	}
}