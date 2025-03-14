// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalGun.generated.h"

UCLASS()
class PORTAL_API APortalGun : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalGun();

	UFUNCTION(BlueprintCallable)
	void ShootPortal();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* Portal;
};
