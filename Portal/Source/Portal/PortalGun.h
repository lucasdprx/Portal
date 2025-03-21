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

	UFUNCTION(BlueprintCallable)
	AActor* ShootPortal(AActor* Portal, UCameraComponent* Camera, FHitResult HitResult) const;
	
private:
	static TArray<FVector> GetBottomPortal(const AActor* Portal);
	static TArray<FVector> GetPointCenter(const AActor* Portal, const FVector& HitPosition);
	static bool            AllBottomIsOnWall(const AActor* Portal, const UWorld*  World);
};
