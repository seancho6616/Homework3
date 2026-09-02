// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/HomeworkGameInstance.h"

UHomeworkGameInstance::UHomeworkGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UHomeworkGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated : %d"), TotalScore);
}