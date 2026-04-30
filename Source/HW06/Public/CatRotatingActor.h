#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"	// 오디오 관련 헤더
#include "CatRotatingActor.generated.h"

UCLASS()
class HW06_API ACatRotatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACatRotatingActor();

protected:
	virtual void BeginPlay() override;

	// 1초에 몇 번 왕복할지 결정하는 속도
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunningTime;

	// --- 애니메이션 타이밍 관련 ---
	UPROPERTY(EditAnywhere, Category = "Cat|Timing")
	float AnimDuration = 6.958f;       // 애니메이션 전체 길이 (초)

	UPROPERTY(EditAnywhere, Category = "Cat|Timing")
	float TransformStartTime = 0.04f; // 변형이 시작되는 시점 (초)

	UPROPERTY(EditAnywhere, Category = "Cat|Timing")
	float TransformEndTime = 4.92f; // 변형이 끝나는 시점 (초)

	// --- 변형 시 동작 수치 ---
	UPROPERTY(EditAnywhere, Category = "Cat|Movement")
	float RotationSpeed = 720.0f;    // 초당 회전 각도 (공회전용, 아주 빠르게)

	UPROPERTY(EditAnywhere, Category = "Cat|Movement")
	float FloatAmplitude = 30.0f;    // 위아래 이동 범위

	UPROPERTY(EditAnywhere, Category = "Cat|Movement")
	float FloatSpeed = 5.0f;         // 부드러운 이동 속도

	UPROPERTY(EditAnywhere, Category = "Cat|Movement")
	float StartPos = 50.0f;         // z축 기본 위치값

public:	
	virtual void Tick(float DeltaTime) override;

	// 1. 고양이 몸체를 담을 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cat|Components")
	class USkeletalMeshComponent* SkeletalMeshComp;

	// 2. 재생할 애니메이션 에셋 (에디터에서 선택 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat|Animation")
	class UAnimSequence* TargetAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cat|Components")
	UAudioComponent* AudioComp;

};
