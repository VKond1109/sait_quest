#include "JoystickInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

UJoystickInputComponent::UJoystickInputComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    DeadZoneRadius = 10.f;
    JoystickRadius = 50.f;
    JoystickPosition = FVector2D(200.f, 600.f); // Позиция по умолчанию для левого джойстика
    bUseFixedPosition = true;
    
    LeftFingerIndex = -1;
    RightFingerIndex = -1;
    bLeftTouchActive = false;
    bRightTouchActive = false;
}

void UJoystickInputComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UJoystickInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Здесь можно добавить обработку геймпада или клавиатуры если нужно
    // Для мобильных устройств ввод обрабатывается через события Touch
}

void UJoystickInputComponent::OnTouchStarted(int32 FingerIndex, FVector2D TouchPosition)
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    float ScreenWidth = PC->Viewport->SizeX;
    
    // Разделяем экран: левая часть - левый джойстик, правая - правый
    if (TouchPosition.X < ScreenWidth / 2.f)
    {
        if (!bLeftTouchActive)
        {
            LeftFingerIndex = FingerIndex;
            bLeftTouchActive = true;
            
            if (bUseFixedPosition)
            {
                LeftTouchOrigin = JoystickPosition;
            }
            else
            {
                LeftTouchOrigin = TouchPosition;
            }
            
            LeftJoystick.bIsPressed = true;
        }
    }
    else
    {
        if (!bRightTouchActive)
        {
            RightFingerIndex = FingerIndex;
            bRightTouchActive = true;
            
            if (bUseFixedPosition)
            {
                RightTouchOrigin = FVector2D(ScreenWidth - 200.f, 600.f);
            }
            else
            {
                RightTouchOrigin = TouchPosition;
            }
            
            RightJoystick.bIsPressed = true;
        }
    }
}

void UJoystickInputComponent::OnTouchMoved(int32 FingerIndex, FVector2D TouchPosition)
{
    if (FingerIndex == LeftFingerIndex && bLeftTouchActive)
    {
        CalculateJoystickInput(TouchPosition, LeftTouchOrigin, LeftJoystick);
    }
    else if (FingerIndex == RightFingerIndex && bRightTouchActive)
    {
        CalculateJoystickInput(TouchPosition, RightTouchOrigin, RightJoystick);
    }
}

void UJoystickInputComponent::OnTouchEnded(int32 FingerIndex, FVector2D TouchPosition)
{
    if (FingerIndex == LeftFingerIndex)
    {
        bLeftTouchActive = false;
        LeftFingerIndex = -1;
        LeftJoystick.InputVector = FVector2D::ZeroVector;
        LeftJoystick.Magnitude = 0.f;
        LeftJoystick.bIsPressed = false;
    }
    else if (FingerIndex == RightFingerIndex)
    {
        bRightTouchActive = false;
        RightFingerIndex = -1;
        RightJoystick.InputVector = FVector2D::ZeroVector;
        RightJoystick.Magnitude = 0.f;
        RightJoystick.bIsPressed = false;
    }
}

void UJoystickInputComponent::SimulateKeyboardInput(float AxisX, float AxisY, bool bIsRightStick)
{
    FJoystickData& Data = bIsRightStick ? RightJoystick : LeftJoystick;
    
    Data.InputVector = FVector2D(AxisX, AxisY);
    Data.Magnitude = UKismetMathLibrary::VSize2D(Data.InputVector);
    
    if (Data.Magnitude > 1.f)
    {
        Data.InputVector.Normalize();
        Data.Magnitude = 1.f;
    }
    
    Data.bIsPressed = Data.Magnitude > DeadZoneRadius / JoystickRadius;
}

void UJoystickInputComponent::CalculateJoystickInput(FVector2D TouchCurrent, FVector2D TouchOrigin, FJoystickData& OutData)
{
    FVector2D Delta = TouchCurrent - TouchOrigin;
    float Distance = Delta.Size();
    
    if (Distance < DeadZoneRadius)
    {
        OutData.InputVector = FVector2D::ZeroVector;
        OutData.Magnitude = 0.f;
        OutData.bIsPressed = false;
        return;
    }
    
    // Ограничиваем радиусом джойстика
    if (Distance > JoystickRadius)
    {
        Delta = Delta.GetSafeNormal() * JoystickRadius;
    }
    
    OutData.InputVector = Delta / JoystickRadius; // Нормализуем к радиусу
    OutData.Magnitude = FMath::Min(Distance / JoystickRadius, 1.f);
    OutData.bIsPressed = true;
}
