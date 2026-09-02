// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK3_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
	
	virtual void ActivateItem(AActor* Activator) override;
};
