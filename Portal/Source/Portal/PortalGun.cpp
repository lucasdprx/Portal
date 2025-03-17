// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalGun.h"

#include "Camera/CameraComponent.h"

// Sets default values
APortalGun::APortalGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector APortalGun::ShootPortal(AActor *Portal, UCameraComponent *Camera)
{
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = StartLocation + Camera->GetForwardVector() * 100000.0f;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	UWorld*World = GetWorld();
	if (World == nullptr)
	{
		return FVector::ZeroVector;
	}
	GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(1.0f), QueryParams);
	
	if (HitResult.bBlockingHit && Portal != nullptr)
	{
		auto Rotation = HitResult.ImpactNormal.Rotation();// + FRotator(0, Camera->GetComponentRotation().Yaw, 0);
		auto HitRotation = HitResult.ImpactNormal.Rotation();

		FVector Normal = NormalizeVector(HitResult.ImpactNormal);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, HitResult.ImpactNormal.ToString());
		}
		Portal->SetActorLocation(HitResult.ImpactPoint);
		Portal->SetActorRotation(Rotation.Quaternion());
	}
	return StartLocation;
}



void APortalGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APortalGun::NormalizeVector(FVector Vec)
{
	double norm = std::sqrt(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
	if (norm > 0)
	{
		Vec.X /= norm;
		Vec.Y /= norm;
		Vec.Z /= norm;
	}
	return Vec;
}

