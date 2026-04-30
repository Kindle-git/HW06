#include "CatRotatingActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"

ACatRotatingActor::ACatRotatingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성 및 루트 설정
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkeletalMeshComp;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(SkeletalMeshComp);

}

void ACatRotatingActor::BeginPlay()
{
	Super::BeginPlay();

	// 게임이 시작되면 설정된 애니메이션을 재생합니다.
	if (SkeletalMeshComp && TargetAnim)
	{
		// 애니메이션 모드를 '에셋 사용'으로 변경
		SkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		// 애니메이션 재생 (루프 설정: true)
		SkeletalMeshComp->PlayAnimation(TargetAnim, true);
	}
}

void ACatRotatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	// 1. 애니메이션 전체 주기에 맞춘 현재 박자 계산 (예: 2초 주기)
	float CurrentCycleTime = FMath::Fmod(RunningTime, AnimDuration);

	// 2. "변형된 자세" 구간(예: 1초 이후)인지 확인
	if (CurrentCycleTime >= TransformStartTime && CurrentCycleTime <= TransformEndTime)
	{
		// [행동 1] Z축 고속 공회전
		// DeltaTime을 곱해 프레임에 상관없이 일정한 속도로 회전하게 합니다.
		// AddLocalRotation은 기존 회전값에 계속 더해주는 방식이므로 "공회전"에 적합합니다.
		SkeletalMeshComp->AddLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

		// [행동 2] Z축 위아래 부드러운 이동
		// "부드러운" 느낌을 위해 다시 Sin 파동을 사용합니다. 
		// 변형 시작 시점부터의 경과 시간을 활용해 계산합니다.
		float TransformElapsed = CurrentCycleTime - TransformStartTime;
		float ZOffset = FMath::Sin(TransformElapsed * FloatSpeed) * FloatAmplitude;

		// 고양이가 공중에 떠 있는 느낌을 주려면 기본 높이에 ZOffset을 더합니다.
		SkeletalMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, ZOffset + StartPos));

		// [오디오 로직 추가]
		// 오디오가 할당되어 있고, 현재 재생 중이 아니라면 재생 시작
		if (AudioComp && AudioComp->GetSound() && !AudioComp->IsPlaying())
		{
			AudioComp->Play();
		}
	}
	else
	{
		// 3. 가만히 있는 자세일 때는 기본 상태로 복구
		SkeletalMeshComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		SkeletalMeshComp->SetRelativeLocation(FVector::ZeroVector);

		// 변형 구간이 끝나면 오디오를 즉시 정지
		if (AudioComp && AudioComp->IsPlaying())
		{
			AudioComp->Stop();
		}
	}

}

