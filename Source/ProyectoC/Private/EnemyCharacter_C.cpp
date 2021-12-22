// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter_C.h"
#include "Components/CapsuleComponent.h"
#include "ProyectoCCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

// Sets default values
AEnemyCharacter_C::AEnemyCharacter_C()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Seteo de vida y estado inicial*/
	Health = 100;
	MyState = EEnemyState::IDLE;

	/*Creacion y atachamiento de AudioComponent al enemigo*/
	HurtAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HurtAudioComponent"));
	HurtAudioComponent->SetupAttachment(RootComponent);

	//Creacion y asignacion de collisiones de las manos al enemigo
	HandCollisionRight = CreateDefaultSubobject<USphereComponent>(TEXT("HandCollisionRight"));
	HandCollisionRight->SetCollisionProfileName("NoCollision");

	HandCollisionLeft = CreateDefaultSubobject<USphereComponent>(TEXT("HandCollisionLeft"));
	HandCollisionLeft->SetCollisionProfileName("NoCollision");

	/*Seteo del tamaño de las colisiones de las manos*/
	HandCollisionRight->InitSphereRadius(20);
	HandCollisionLeft->InitSphereRadius(20);
}

// Called when the game starts or when spawned
void AEnemyCharacter_C::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter_C::OnHurtOverlapBegin);

	//Reglas de atachamiento a los sockets
	const FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
	//Atachando colisiones a los sockets del jugador
	HandCollisionRight->AttachToComponent(GetMesh(), attachmentRules, "RightHandSocket");
	HandCollisionLeft->AttachToComponent(GetMesh(), attachmentRules, "LeftHandSocket");

	/*Asignacion de sonido al audio component*/
	HurtAudioComponent->SetSound(HurtSoundCue);
}

// Called every frame
void AEnemyCharacter_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter_C::OnHurtOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	/*Si colisiona con los puños del jugador*/
	if (OtherActor->IsA(AProyectoCCharacter::StaticClass())) {

		AProyectoCCharacter* player = Cast<AProyectoCCharacter>(OtherActor);
		DecreaseHealth(player->GetDamage());

		/*Si su estado es diferente de muerto*/
		if (MyState != EEnemyState::DEATH) {

			/*Ejecuta al azar algun tipo de reaccion*/
			int MontageSectionIndex = rand() % 3 + 1;
			FString MontageSection = "startR" + FString::FromInt(MontageSectionIndex);

			PlayAnimMontage(ReactionMontageAnimation, 1.0f, FName(*MontageSection));
		}
		/*Ejecutar sonido*/
		HurtAudioComponent->Play(0.f);
	}
}

void AEnemyCharacter_C::StartAnimAttack() {

	/*Luego de un segundo comienza el ataque*/
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			MyState = EEnemyState::ATTACKING;
			PlayAnimMontage(ReactionMontageAnimation, 1.0f, FName("Attack1"));
		}, 1, false);
}

void AEnemyCharacter_C::StartAttack() {

	/*Seteo de las colisiones de las manos a EnemyPunch*/
	HandCollisionRight->SetCollisionProfileName("EnemyPunch");
	HandCollisionLeft->SetCollisionProfileName("EnemyPunch");
}

void AEnemyCharacter_C::EndAttack() {

	/*Seteo de las colisiones de las manos a NoCollision*/
	if (MyState != EEnemyState::DEATH) {
		MyState = EEnemyState::IDLE;
		HandCollisionRight->SetCollisionProfileName("NoCollision");
		HandCollisionLeft->SetCollisionProfileName("NoCollision");
	}
}

void AEnemyCharacter_C::DecreaseHealth(int Damage) {

	Health -= Damage;
	/*Si la vida es menor o igual a 0*/
	if (Health <= 0) {

		/*Muerto
		*Cambio de collision
		*Ejecutar animacion de muerte
		*/
		MyState = EEnemyState::DEATH;
		GetCapsuleComponent()->SetCollisionProfileName("IgnoreOnlyPawn");
		GetMesh()->SetCollisionProfileName("IgnoreOnlyPawn");
		StopAnimMontage(ReactionMontageAnimation);
		PlayAnimMontage(ReactionMontageAnimation, 1.0f, FName("start4"));

		/*Luego de dos segundos desaparece*/
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				Destroy();
			}, 2, false);
	}
}


