// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProyectoCCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacter_C.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// AProyectoCCharacter

AProyectoCCharacter::AProyectoCCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 97.0f);
	
	MyState = EPlayerState::IDLE;
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Creaciond de las colisiones de las manos derecha e izquierda
	HandCollisionRight = CreateDefaultSubobject<USphereComponent>(TEXT("HandCollisionRight"));
	HandCollisionRight->SetCollisionProfileName("NoCollision");

	HandCollisionLeft = CreateDefaultSubobject<USphereComponent>(TEXT("HandCollisionLeft"));
	HandCollisionLeft->SetCollisionProfileName("NoCollision");

	//Seteo del tamañlo de las colisiones de las manos
	HandCollisionRight->InitSphereRadius(20);
	HandCollisionLeft->InitSphereRadius(20);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeFistAttackMontageObject(TEXT("AnimMontage'/Game/Mannequin/Animations/Punchs_AM.Punchs_AM'"));
	if (MeleeFistAttackMontageObject.Succeeded())
	{
		PunchsAttackMontage = MeleeFistAttackMontageObject.Object;
	}

	//Set Combo Inicial
	ActualCombo = "start1";
	ActualIndexCombo = 1;

	//float combo timer
	TimerResetCombo = 1.75f;

	/*Set daño de los ataques Esta en cero por defecto pero al cambiarlo se peude eliminar a los enemigos con su aniumacion correspondiente*/
	Damage = 0;

	//Creacion y Atachamiento de AudioComponent
	WooshAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WooshAudioComponent"));
	WooshAudioComponent->SetupAttachment(RootComponent);
}

void AProyectoCCharacter::BeginPlay() {

	Super::BeginPlay();
	//Reglas de atachamiento a los sockets
	const FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
	//Atachando colisiones a los sockets del jugador
	HandCollisionRight->AttachToComponent(GetMesh(), attachmentRules, "hand_rSocket");
	HandCollisionLeft->AttachToComponent(GetMesh(), attachmentRules, "hand_lSocket");

	HandCollisionRight->OnComponentBeginOverlap.AddDynamic(this, &AProyectoCCharacter::OnPunchOverlapBegin);
	HandCollisionRight->OnComponentEndOverlap.AddDynamic(this, &AProyectoCCharacter::OnPunchOverlapEnd);
	HandCollisionLeft->OnComponentBeginOverlap.AddDynamic(this, &AProyectoCCharacter::OnPunchOverlapBegin);
	HandCollisionLeft->OnComponentEndOverlap.AddDynamic(this, &AProyectoCCharacter::OnPunchOverlapEnd);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AProyectoCCharacter::OnPunchOverlapBegin);

	//Asignacion de sonido al audioComponent
	WooshAudioComponent->SetSound(WooshSoundCue);
} 

void AProyectoCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerComboReset(DeltaTime);

	/*Testeo de los estados del personaje
	*Por alguna razon no muestra el estado cuando se usa la funcion MoveForward, esa se debe de imprimir desde 
	*dentro de la misma funcion
	*/
	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Cyan, UEnum::GetValueAsString<EPlayerState>(MyState));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProyectoCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AProyectoCCharacter::PunchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProyectoCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProyectoCCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProyectoCCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProyectoCCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProyectoCCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProyectoCCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProyectoCCharacter::OnResetVR);
}

void AProyectoCCharacter::OnResetVR()
{
	// If ProyectoC is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in ProyectoC.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProyectoCCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AProyectoCCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AProyectoCCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProyectoCCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProyectoCCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		if(MyState != EPlayerState::ATTACKING)
			SetPlayerState(EPlayerState::RUNNING);

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	if (Value == 0.0f && MyState == EPlayerState::RUNNING) {
		SetPlayerState(EPlayerState::IDLE);
	}
}

void AProyectoCCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		if(MyState != EPlayerState::ATTACKING)
			SetPlayerState(EPlayerState::RUNNING);
		
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}

	if (Value == 0.0f && MyState == EPlayerState::RUNNING)
		SetPlayerState(EPlayerState::IDLE);
}

//Funcion que activa las anaimaciones gracias al input del jugador
void AProyectoCCharacter::PunchInput() {

	//Si el personaje no esta atacando o esta herido
	if (MyState != EPlayerState::ATTACKING && MyState != EPlayerState::HURT) {
		//Atacando
		SetPlayerState(EPlayerState::ATTACKING);
		PlayAnimMontage(PunchsAttackMontage, 1.0f, FName(*ActualCombo));
		NextCombo(PunchsAttackMontage, ActualCombo);
		GetCharacterMovement()->MaxWalkSpeed = 400;
	}
}

//Funcion que se activa cuando se inicia la animacion de golpear
void AProyectoCCharacter::PunchStart() {

	//Se cambia las colisiones de la manos Punchs
	HandCollisionRight->SetCollisionProfileName("Punchs");
	HandCollisionRight->SetGenerateOverlapEvents(true);
	HandCollisionLeft->SetCollisionProfileName("Punchs");
	HandCollisionLeft->SetGenerateOverlapEvents(true);
}

//Funcion que se acttiva cuando se termina la animacion de golpear
void AProyectoCCharacter::PunchEnd() {

	//Se cambia las colisiones de las manos a NoCollision
	HandCollisionRight->SetCollisionProfileName("NoCollision");
	HandCollisionRight->SetGenerateOverlapEvents(false);
	HandCollisionLeft->SetCollisionProfileName("NoCollision");
	HandCollisionLeft->SetGenerateOverlapEvents(false);
	GetCharacterMovement()->MaxWalkSpeed = 600;
	MyState = EPlayerState::IDLE;
}

void AProyectoCCharacter::OnPunchOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	//Si colisiona con un enemigo el puño de un enemigo
	if (OtherActor->IsA(AEnemyCharacter_C::StaticClass()))
	{
		//Herido
		SetPlayerState(EPlayerState::HURT);
		GetCharacterMovement()->MaxWalkSpeed = 0;
		//Luego de un segundo vuelve a su estado
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				SetPlayerState(EPlayerState::IDLE); 
				GetCharacterMovement()->MaxWalkSpeed = 600;
				
			}, 1, false);
	}
}

void AProyectoCCharacter::OnPunchOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
}

//Funcion que reinicia el ataque 
void AProyectoCCharacter::TimerComboReset(float DeltaTime) {

	//Si el combo actual es disinto del primero
	if (ActualCombo != "start1") {

		//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Red, FString::SanitizeFloat(TimerResetCombo));
	
		//Timer para reiniciar el combo si es que no se hace otro ataque en el timepo establecido
		TimerResetCombo -= DeltaTime;
		if (TimerResetCombo <= 0)
		{
			ActualCombo = "start1";
			ActualIndexCombo = 1;
		}
	}
}

//Funcion que cambia el ataque de la secuencia de combos
void AProyectoCCharacter::NextCombo(UAnimMontage* Montage, FString str) {

	//Recorriendo cantidad de secciones del montaje
	for (int i = 1; i <= Montage->CompositeSections.Num()/2; i++)
	{
		//Si encuentra la seccion actual
		if (ActualIndexCombo == i) {
			//Se va a la siguiente seccion
			ActualIndexCombo += 1;
			ActualCombo = "start" + FString::FromInt(ActualIndexCombo);
			//Si se llega a la ultima seccion se vuelve al primer combo
			if (ActualIndexCombo == Montage->CompositeSections.Num()) {
				ActualIndexCombo = 1;
				ActualCombo = "start1";
				break;
			}
			TimerResetCombo = 1.75;
			break;
		}
	}
}

