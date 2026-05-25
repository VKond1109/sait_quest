#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UJoystickInputComponent.generated.h"

// Структура данных джойстика
USTRUCT(BlueprintType)
struct FJoystickData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
    FVector2D InputVector; // Нормализованный вектор (-1 до 1)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
    float Magnitude; // Сила нажатия (0-1)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
    bool bIsPressed;

    FJoystickData() : InputVector(FVector2D::ZeroVector), Magnitude(0.f), bIsPressed(false) {}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE5ULTIMATEGAME_API UJoystickInputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UJoystickInputComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // Настройки джойстика
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float DeadZoneRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float JoystickRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FVector2D JoystickPosition; // Позиция центра джойстика на экране

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bUseFixedPosition; // Фиксированная позиция или динамическая (где коснулись)

    // Выходные данные
    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FJoystickData LeftJoystick;

    UPROPERTY(BlueprintReadOnly, Category = "Output")
    FJoystickData RightJoystick; // Для камеры/прицеливания

    // События
    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnTouchStarted(int32 FingerIndex, FVector2D TouchPosition);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnTouchMoved(int32 FingerIndex, FVector2D TouchPosition);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void OnTouchEnded(int32 FingerIndex, FVector2D TouchPosition);

    // Эмуляция клавиатуры для тестов на ПК
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SimulateKeyboardInput(float AxisX, float AxisY, bool bIsRightStick = false);

private:
    int32 LeftFingerIndex;
    int32 RightFingerIndex;
    bool bLeftTouchActive;
    bool bRightTouchActive;
    FVector2D LeftTouchOrigin;
    FVector2D RightTouchOrigin;

    void CalculateJoystickInput(FVector2D TouchCurrent, FVector2D TouchOrigin, FJoystickData& OutData);
};
