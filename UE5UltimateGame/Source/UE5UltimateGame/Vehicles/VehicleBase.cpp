#include "VehicleBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

AVehicleBase::AVehicleBase()
{
    PrimaryActorTick.bCanEverTick = true;

    VehicleType = EVehicleType::Car;
    MaxSpeed = 1200.0f;
    Acceleration = 500.0f;
    TurnSpeed = 100.0f;
    Health = 1000.0f;
    MaxHealth = 1000.0f;
    MaxPassengers = 1;
    CurrentState = EVehicleState::Stationary;
    CurrentSpeed = 0.0f;
    Fuel = 100.0f;
    MaxFuel = 100.0f;

    Driver = nullptr;
    ThrottleInput = 0.0f;
    SteeringInput = 0.0f;
    BoostInput = 0.0f;
    BrakeInput = 0.0f;

    // Создание компонентов
    VehicleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMesh"));
    RootComponent = VehicleMesh;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetBoxExtent(FVector(200.0f, 100.0f, 100.0f));
}

void AVehicleBase::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentVelocity = FVector::ZeroVector;
    CurrentRotation = GetActorRotation();
}

void AVehicleBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Driver != nullptr && CurrentState != EVehicleState::Destroyed)
    {
        UpdateMovement(DeltaTime);
        CheckFuelConsumption(DeltaTime);
    }
}

void AVehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (PlayerInputComponent)
    {
        // Привязка входов для транспорта
        PlayerInputComponent->BindAxis("MoveForward", this, &AVehicleBase::MoveForward);
        PlayerInputComponent->BindAxis("TurnRight", this, &AVehicleBase::TurnRight);
        PlayerInputComponent->BindAxis("Boost", this, &AVehicleBase::Boost);
        PlayerInputComponent->BindAxis("Brake", this, &AVehicleBase::Brake);
        
        PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AVehicleBase::FireWeapon);
        PlayerInputComponent->BindAction("ExitVehicle", IE_Pressed, this, &AVehicleBase::ExitVehicle);
    }
}

void AVehicleBase::EnterVehicle(APawn* NewDriver)
{
    if (!NewDriver || !CanEnter())
    {
        return;
    }

    Driver = NewDriver;
    
    // Отключение управления у водителя
    APlayerController* PC = Cast<APlayerController>(Driver->GetController());
    if (PC)
    {
        PC->SetPawn(this);
        PC->Possess(this);
    }

    // Скрытие водителя
    NewDriver->SetActorHiddenInGame(true);
    NewDriver->SetActorEnableCollision(false);

    CurrentState = EVehicleState::Stationary;
    OnVehicleEntered(NewDriver);

    UE_LOG(LogTemp, Log, TEXT("%s entered vehicle"), *NewDriver->GetName());
}

void AVehicleBase::ExitVehicle()
{
    if (!Driver || !CanExit())
    {
        return;
    }

    APawn* OldDriver = Driver;
    
    // Поиск точки выхода
    FVector ExitLocation = GetActorLocation() + GetActorRightVector() * 150.0f;
    ExitLocation.Z += 100.0f;

    // Восстановление водителя
    OldDriver->SetActorLocation(ExitLocation);
    OldDriver->SetActorHiddenInGame(false);
    OldDriver->SetActorEnableCollision(true);

    // Возврат управления
    APlayerController* PC = Cast<APlayerController>(OldDriver->GetController());
    if (PC)
    {
        PC->SetPawn(OldDriver);
        PC->Possess(OldDriver);
    }

    Driver = nullptr;
    ThrottleInput = 0.0f;
    SteeringInput = 0.0f;
    CurrentState = EVehicleState::Stationary;

    OnVehicleExited(OldDriver);

    UE_LOG(LogTemp, Log, TEXT("%s exited vehicle"), *OldDriver->GetName());
}

void AVehicleBase::AddPassenger(APawn* Passenger)
{
    if (!Passenger || Passengers.Num() >= MaxPassengers - 1)
    {
        return;
    }

    Passengers.Add(Passenger);
    Passenger->SetActorHiddenInGame(true);
    Passenger->SetActorEnableCollision(false);

    UE_LOG(LogTemp, Log, TEXT("Passenger added. Total: %d"), Passengers.Num());
}

void AVehicleBase::RemovePassenger(APawn* Passenger)
{
    if (!Passenger)
    {
        return;
    }

    Passengers.Remove(Passenger);
    Passenger->SetActorHiddenInGame(false);
    Passenger->SetActorEnableCollision(true);

    UE_LOG(LogTemp, Log, TEXT("Passenger removed. Total: %d"), Passengers.Num());
}

void AVehicleBase::MoveForward(float Value)
{
    ThrottleInput = Value;
    
    if (Value != 0.0f && CurrentState == EVehicleState::Stationary)
    {
        CurrentState = EVehicleState::Moving;
    }
}

void AVehicleBase::TurnRight(float Value)
{
    SteeringInput = Value;
}

void AVehicleBase::Boost(float Value)
{
    BoostInput = Value;
    
    if (Value > 0.5f && CurrentState != EVehicleState::Destroyed)
    {
        CurrentState = EVehicleState::Boosting;
    }
    else if (CurrentState == EVehicleState::Boosting)
    {
        CurrentState = EVehicleState::Moving;
    }
}

void AVehicleBase::Brake(float Value)
{
    BrakeInput = Value;
    
    if (Value > 0.5f && CurrentSpeed < 100.0f)
    {
        CurrentState = EVehicleState::Stationary;
    }
}

void AVehicleBase::FireWeapon()
{
    if (VehicleType != EVehicleType::Tank)
    {
        return;
    }

    // Логика стрельбы для танков
    UE_LOG(LogTemp, Log, TEXT("Firing weapon!"));
    
    // Здесь должен быть спавн снаряда или лучевой тест
}

void AVehicleBase::TakeDamage(float Damage, AController* InstigatedBy, AActor* DamageCauser)
{
    Health -= Damage;
    
    if (Health <= 0.0f)
    {
        Health = 0.0f;
        CurrentState = EVehicleState::Destroyed;
        OnVehicleDestroyed();
        
        // Принудительный выход всех пассажиров
        if (Driver)
        {
            ExitVehicle();
        }
        
        for (APawn* Passenger : Passengers)
        {
            RemovePassenger(Passenger);
        }

        UE_LOG(LogTemp, Error, TEXT("Vehicle destroyed!"));
    }
    else if (Health < MaxHealth * 0.3f)
    {
        CurrentState = EVehicleState::Damaged;
    }

    UE_LOG(LogTemp, Warning, TEXT("Vehicle took %f damage. Health: %f/%f"), Damage, Health, MaxHealth);
}

void AVehicleBase::OnVehicleEntered(APawn* Driver)
{
    // Делегат для UI и других систем
}

void AVehicleBase::OnVehicleExited(APawn* Driver)
{
    // Делегат для UI и других систем
}

void AVehicleBase::OnVehicleDestroyed()
{
    // Делегат для UI, достижений и других систем
}

void AVehicleBase::UpdateMovement(float DeltaTime)
{
    float TargetSpeed = 0.0f;
    
    // Расчет целевой скорости
    if (ThrottleInput > 0.0f)
    {
        TargetSpeed = MaxSpeed * ThrottleInput;
        
        if (BoostInput > 0.5f && Fuel > 0.0f)
        {
            TargetSpeed *= 1.5f; // Буст на 50%
        }
    }
    else if (ThrottleInput < 0.0f)
    {
        TargetSpeed = MaxSpeed * 0.3f * ThrottleInput; // Задний ход
    }

    // Плавное ускорение/замедление
    if (CurrentSpeed < TargetSpeed)
    {
        CurrentSpeed = FMath::Min(CurrentSpeed + Acceleration * DeltaTime, TargetSpeed);
    }
    else if (CurrentSpeed > TargetSpeed)
    {
        CurrentSpeed = FMath::Max(CurrentSpeed - Acceleration * 2.0f * DeltaTime, TargetSpeed);
    }

    // Торможение
    if (BrakeInput > 0.5f)
    {
        CurrentSpeed = FMath::Max(CurrentSpeed - Acceleration * 3.0f * DeltaTime, 0.0f);
    }

    // Поворот (только при движении)
    if (FMath::Abs(CurrentSpeed) > 10.0f)
    {
        float TurnAmount = SteeringInput * TurnSpeed * DeltaTime * (CurrentSpeed / MaxSpeed);
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw += TurnAmount;
        SetActorRotation(NewRotation);
    }

    // Применение движения
    FVector ForwardVector = GetActorForwardVector();
    FVector Movement = ForwardVector * CurrentSpeed * DeltaTime;
    
    SetActorLocation(GetActorLocation() + Movement);

    CurrentVelocity = Movement / DeltaTime;
}

void AVehicleBase::CheckFuelConsumption(float DeltaTime)
{
    if (BoostInput > 0.5f || FMath::Abs(ThrottleInput) > 0.1f)
    {
        float ConsumptionRate = 5.0f; // Единиц в секунду
        
        if (BoostInput > 0.5f)
        {
            ConsumptionRate *= 3.0f; // Буст потребляет больше
        }

        Fuel -= ConsumptionRate * DeltaTime;
        
        if (Fuel <= 0.0f)
        {
            Fuel = 0.0f;
            ThrottleInput = 0.0f; // Остановка без топлива
            UE_LOG(LogTemp, Warning, TEXT("Vehicle out of fuel!"));
        }
    }
}

bool AVehicleBase::CanEnter() const
{
    return Driver == nullptr && CurrentState != EVehicleState::Destroyed;
}

bool AVehicleBase::CanExit() const
{
    return Driver != nullptr && CurrentState != EVehicleState::Destroyed;
}
