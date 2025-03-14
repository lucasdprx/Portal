// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalGun.generated.h"

class UCameraComponent;

UCLASS()
class PORTAL_API APortalGun : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalGun();

	UFUNCTION(BlueprintCallable)
	FVector ShootPortal(AActor* Portal, UCameraComponent* Camera, AActor* Player);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	AActor* Portal1;
	AActor* Portal2;
};
