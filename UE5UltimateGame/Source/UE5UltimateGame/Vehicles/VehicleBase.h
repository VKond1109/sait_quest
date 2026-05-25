#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VehicleBase.generated.h"

// Типы транспортных средств
UENUM(BlueprintType)
enum class EVehicleType : uint8
{
    Car UMETA(DisplayName = "Автомобиль"),
    Motorcycle UMETA(DisplayName = "Мотоцикл"),
    Tank UMETA(DisplayName = "Танк"),
    Hovercraft UMETA(DisplayName = "Ховеркрафт"),
    Animal UMETA(DisplayName = "Ездовое животное")
};

// Состояния транспортного средства
UENUM(BlueprintType)
enum class EVehicleState : uint8
{
    Stationary UMETA(DisplayName = "Стоит"),
    Moving UMETA(DisplayName = "Движение"),
    Boosting UMETA(DisplayName = "Ускорение"),
    Damaged UMETA(DisplayName = "Повреждено"),
    Destroyed UMETA(DisplayName = "Уничтожено")
};

/**
 * Базовый класс для всех транспортных средств
 */
UCLASS()
class UE5ULTIMATEGAME_API AVehicleBase : public APawn
{
    GENERATED_BODY()

public:
    AVehicleBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Настройки транспорта
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
    EVehicleType VehicleType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
    float MaxSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
    float Acceleration = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
    float TurnSpeed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
    float Health = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
    float MaxHealth = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
    int32 MaxPassengers = 1;

    // Текущее состояние
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
    EVehicleState CurrentState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
    float CurrentSpeed = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
    float Fuel = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
    float MaxFuel = 100.0f;

    // Компоненты
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Components")
    class USkeletalMeshComponent* VehicleMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Components")
    class UBoxComponent* CollisionBox;

    // Управление
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void EnterVehicle(APawn* Driver);

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void ExitVehicle();

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void AddPassenger(APawn* Passenger);

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void RemovePassenger(APawn* Passenger);

    // Движение
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void MoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void TurnRight(float Value);

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void Boost(float Value);

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void Brake(float Value);

    // Бой (для боевых транспортных средств)
    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void FireWeapon();

    UFUNCTION(BlueprintCallable, Category = "Vehicle")
    void TakeDamage(float Damage, AController* InstigatedBy, AActor* DamageCauser);

    // События
    UFUNCTION()
    void OnVehicleEntered(APawn* Driver);

    UFUNCTION()
    void OnVehicleExited(APawn* Driver);

    UFUNCTION()
    void OnVehicleDestroyed();

protected:
    // Внутренние функции
    void UpdateMovement(float DeltaTime);
    void ApplyPhysics();
    void CheckFuelConsumption(float DeltaTime);
    bool CanEnter() const;
    bool CanExit() const;

    // Данные о пассажирах
    UPROPERTY()
    APawn* Driver;

    UPROPERTY()
    TArray<APawn*> Passengers;

    // Входные значения
    float ThrottleInput;
    float SteeringInput;
    float BoostInput;
    float BrakeInput;

    // Физика
    FVector CurrentVelocity;
    FRotator CurrentRotation;
};
