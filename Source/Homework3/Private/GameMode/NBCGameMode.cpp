// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NBCGameMode.h"
#include "Character/NBCCharacter.h"
#include "Controller/NBCPlayerController.h"
#include "HomeworkGameState.h"

ANBCGameMode::ANBCGameMode()
{
	DefaultPawnClass = ANBCCharacter::StaticClass();
	PlayerControllerClass = ANBCPlayerController::StaticClass();
	GameStateClass = AHomeworkGameState::StaticClass();
}