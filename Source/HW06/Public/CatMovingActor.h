#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"	// 오디오 관련 헤더
#include "CatMovingActor.generated.h"

UCLASS()
class HW06_API ACatMovingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACatMovingActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components");
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components");
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Audios");
	UAudioComponent* AudioComp;

	float CurrentYawOffset = 0.f;
	float RotationDirection = 1.f;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float RotationSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement");
	float RotationMaxAngle = 45.f;

	UPROPERTY(EditAnywhere, Category = "CatMovement")
	float MoveSpeed = 200.0f;          // 이동 속도
	UPROPERTY(EditAnywhere, Category = "CatMovement")
	float MoveMaxDistance = 500.0f;    // 이동할 최대 거리

	// 시작할 때 부여할 오프셋 (초 단위 또는 거리 단위)
	UPROPERTY(EditAnywhere, Category = "CatMovement")
	float StartOffset = 0.0f;

	// 인스턴스마다 무작위로 시작할지 여부
	UPROPERTY(EditAnywhere, Category = "CatMovement")
	bool bRandomStart = true;

	float CurrentDistance = 0.0f;      // 현재 이동한 거리
	int32 MoveDirection = 1;           // 1: 전진, -1: 후진
	bool bIsMovingForward = true;      // 전진 중인지 여부

	FRotator BaseRotation;

	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

};
