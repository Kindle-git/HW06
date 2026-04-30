#include "CatMovingActor.h"
#include "Components/SkeletalMeshComponent.h"

ACatMovingActor::ACatMovingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	SkeletalMeshComp->SetupAttachment(SceneRoot);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(SkeletalMeshComp);

}

void ACatMovingActor::BeginPlay()
{
	Super::BeginPlay();
	BaseRotation = SkeletalMeshComp->GetRelativeRotation();

	// 1. 무작위 시작 설정
	if (bRandomStart)
	{
		// 0부터 최대 이동 거리 사이의 무작위 값을 시작 지점으로 설정
		StartOffset = FMath::RandRange(0.0f, MoveMaxDistance);
	}
	// 2. 초기 위치/거리 설정
	CurrentDistance = StartOffset;
}

void ACatMovingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 프레임 이동할 벡터 계산
	//FVector DeltaLocation(MoveSpeed * MoveDirection * DeltaTime, 0.f, 0.f);
	FVector ForwardDirection = GetActorForwardVector();
	FVector DeltaLocation = ForwardDirection * MoveSpeed * MoveDirection * DeltaTime;
	AddActorWorldOffset(DeltaLocation);

	// 현재 이동 거리 누적
	CurrentDistance += MoveSpeed * DeltaTime;

	// 방향 반전 및 상태 전환
	if (CurrentDistance >= MoveMaxDistance)
	{
		MoveDirection *= -1;     // 이동 방향 반전
		CurrentDistance = 0.0f;  // 거리 초기화

		// 전진 중이었다면 후진으로, 후진 중이었다면 전진으로 상태 변경
		bIsMovingForward = !bIsMovingForward;

		// 후진을 시작할 때 고양이의 회전값을 0으로 초기화 (멈춘 모습)
		if (!bIsMovingForward)
		{
			//SkeletalMeshComp->SetRelativeRotation(FRotator::ZeroRotator);
			SkeletalMeshComp->SetRelativeRotation(BaseRotation);
		}
	}



	// 3. 회전 로직 (전진 중일 때만 실행)
	if (bIsMovingForward)
	{
		// 현재 오프셋 계산 (속도-방향-시간)
		CurrentYawOffset += RotationSpeed * RotationDirection * DeltaTime;

		if (FMath::Abs(CurrentYawOffset) >= RotationMaxAngle)
		{
			RotationDirection *= -1.f;
			CurrentYawOffset = FMath::Clamp(CurrentYawOffset, -RotationMaxAngle, RotationMaxAngle);
		}

		FRotator NewRotation = BaseRotation; // 기본 방향에서 시작
		NewRotation.Pitch += CurrentYawOffset; // 기본 방향에 흔들림(Pitch)을 더함
		SkeletalMeshComp->SetRelativeRotation(NewRotation);
	}
}

