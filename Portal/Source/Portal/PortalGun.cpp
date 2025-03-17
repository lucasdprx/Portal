// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGun.h"

#include "Camera/CameraComponent.h"

// Sets default values
APortalGun::APortalGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AActor* APortalGun::ShootPortal(AActor *Portal, UCameraComponent *Camera)
{
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = StartLocation + Camera->GetForwardVector() * 100000.0f;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	UWorld*World = GetWorld();
	if (World == nullptr)
	{
		return nullptr;
	}
	GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(1.0f), QueryParams);
	
	if (HitResult.bBlockingHit && Portal != nullptr)
	{
		auto HitRotation = HitResult.ImpactNormal.Rotation();

		if (HitResult.ImpactNormal.Z >= 0.99f)
		{
			HitRotation += FRotator(0.0f, Camera->GetComponentRotation().Yaw, 0);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, HitResult.GetActor()->GetName());
		}
		Portal->SetActorLocation(HitResult.ImpactPoint);
		Portal->SetActorRotation(HitRotation.Quaternion());
	}
	return HitResult.GetActor();
}



void APortalGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

