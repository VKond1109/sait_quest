# Полноценная игра на Unreal Engine 5 с джойстиком

## 🎮 Обзор проекта

Это полноценный проект игры на **Unreal Engine 5.3+** с виртуальным джойстиком и расширенными игровыми механиками:

### ✅ Реализованные системы

1. **Виртуальный джойстик** (UVirtualJoystickComponent)
   - Поддержка touch-управления для мобильных устройств
   - Эмуляция клавиатуры (WASD) и геймпада
   - Настраиваемый размер, позиция, прозрачность

2. **Система взаимодействия** (Interaction System)
   - Интерфейс `IInteractableInterface` для всех взаимодействуемых объектов
   - Базовый класс `AInteractableActor`
   - Двери (`ADoorActor`) с авто-открытием и анимацией
   - Собираемые предметы (`ACollectibleActor`) с парением и эффектами

3. **Боевая система** (Combat System)
   - Компонент здоровья (`UHealthComponent`)
     - Урон, исцеление, регенерация
     - Неуязвимость после получения урона
     - События изменения здоровья и смерти
   - Базовый класс врага (`ABaseEnemyCharacter`)
     - ИИ с Behavior Tree
     - Состояния: бездействие, патрулирование, преследование, атака
     - Награды и выпадающие предметы

4. **Продвинутые механики движения**
   - Двойной прыжок
   - Рывок (Dash) с кулдауном
   - Бег (Sprint)
   - Переключение камеры 1-е/3-е лицо

5. **Камера от 1-го и 3-го лица**
   - Плавное переключение режимов
   - Camera Lag для кинематографичности
   - Независимое управление камерой

6. **Игровой цикл**
   - Система очков
   - Сбор предметов и монет
   - Бой с врагами
   - События UI через Blueprint delegates

---

## 📁 Структура проекта

```
UE5JoystickGame/
├── Source/
│   └── UE5JoystickGame/
│       ├── Public/
│       │   ├── Interactions/
│       │   │   ├── InteractableInterface.h
│       │   │   ├── InteractableActor.h
│       │   │   ├── DoorActor.h
│       │   │   └── CollectibleActor.h
│       │   ├── Combat/
│       │   │   └── HealthComponent.h
│       │   ├── AI/
│       │   │   └── BaseEnemyCharacter.h
│       │   └── GameFramework/
│       │       └── EnhancedJoystickPlayerCharacter.h
│       ├── Private/
│       │   ├── Interactions/
│       │   ├── Combat/
│       │   ├── AI/
│       │   └── GameFramework/
│       └── UE5JoystickGame.Build.cs
├── Config/
│   ├── DefaultEngine.ini
│   └── DefaultGame.ini
└── README.md
```

---

## 🚀 Установка и запуск

### Требования
- **Unreal Engine 5.3 или новее**
- **Visual Studio 2019/2022** (для Windows)
- **Xcode** (для macOS/iOS)
- **Android Studio + NDK** (для Android)

### Шаги по установке

1. **Скопируйте проект в нужную директорию:**
   ```bash
   cp -r /workspace/UE5JoystickGame ~/UnrealProjects/
   ```

2. **Откройте проект в Unreal Engine:**
   - Запустите Unreal Engine
   - Нажмите "Open Project"
   - Выберите файл `UE5JoystickGame.uproject`

3. **Пересоберите модули:**
   - При первом открытии нажмите "Yes" для перекомпиляции
   - Или запустите генерацию проекта:
     ```bash
     cd UE5JoystickGame
     dotnet build
     ```

4. **Настройте Input Actions:**
   - Откройте `Project Settings → Input`
   - Или создайте Input Mapping Context в Blueprint

---

## 🎯 Создание игрового уровня

### 1. Добавление игрока

1. Создайте Blueprint класса `EnhancedJoystickPlayerCharacter`
2. Настройте параметры:
   - Walk Speed: 300
   - Sprint Speed: 600
   - Max Double Jumps: 1
   - Dash Force: 2000
3. Поместите на уровень

### 2. Создание двери

1. Создайте Blueprint класса `DoorActor`
2. Настройте:
   - Open Angle: 90°
   - Open Speed: 2.0
   - Auto Open: true
   - Auto Close: true
3. Добавьте статический меш двери

### 3. Создание собираемого предмета

1. Создайте Blueprint класса `CollectibleActor`
2. Выберите тип:
   - Coin (монета)
   - Health (аптечка)
   - Ammo (патроны)
   - PowerUp (бонус)
3. Настройте внешний вид (mesh, материалы)

### 4. Создание врага

1. Создайте Blueprint класса `BaseEnemyCharacter`
2. Настройте:
   - Max Health: 100
   - Attack Damage: 20
   - Detection Range: 1500
   - Behavior Tree (создать отдельно)
3. Добавьте скелетный меш и анимации

---

## 🎮 Управление

### Мобильные устройства
- **Левый стик**: Перемещение
- **Правый стик**: Камера
- **Кнопка прыжка**: Прыжок / двойной прыжок
- **Кнопка взаимодействия**: Взаимодействие с объектами
- **Кнопка атаки**: Атака
- **Кнопка рывка**: Dash

### ПК (клавиатура + мышь)
- **WASD**: Перемещение
- **Мышь**: Камера
- **Пробел**: Прыжок
- **E**: Взаимодействие
- **ЛКМ**: Атака
- **Shift**: Бег
- **F**: Рывок
- **V**: Смена камеры (1-е/3-е лицо)

### Геймпад
- **Левый стик**: Перемещение
- **Правый стик**: Камера
- **A/Cross**: Прыжок
- **X/Square**: Взаимодействие
- **RT/R2**: Атака
- **LT/L2**: Рывок
- **LB/L1**: Смена камеры

---

## 🔧 Настройка ввода (Input Actions)

Создайте следующие Input Actions в проекте:

| Действие | Тип | Описание |
|----------|-----|----------|
| IA_Move | Vector2D | Перемещение (джойстик/WASD) |
| IA_Look | Vector2D | Вращение камеры |
| IA_Jump | Button | Прыжок |
| IA_Sprint | Button | Бег |
| IA_Interact | Button | Взаимодействие |
| IA_Attack | Button | Атака |
| IA_Ability | Button | Способность (рывок) |
| IA_ToggleCamera | Button | Смена камеры |

Создайте Input Mapping Context и привяжите действия к управлениям.

---

## 📝 Примеры использования в Blueprint

### Получение компонента здоровья
```blueprint
Get Player Character → Cast to EnhancedJoystickPlayerCharacter → GetHealthComponent
```

### Добавление очков
```blueprint
Cast to EnhancedJoystickPlayerCharacter → AddScore(100)
```

### Проверка жив ли игрок
```blueprint
Cast to EnhancedJoystickPlayerCharacter → IsAlive() → Branch
```

---

## 🛠️ Расширение функционала

### Добавление нового типа взаимодействия
1. Создайте класс наследник `AInteractableActor`
2. Переопределите метод `Interact()`
3. Добавьте свою логику

### Создание нового врага
1. Создайте Blueprint на основе `BaseEnemyCharacter`
2. Настройте Behavior Tree
3. Добавьте уникальные способности

### Новые способности игрока
1. Добавьте новую Input Action
2. Привяжите в `SetupPlayerInputComponent()`
3. Реализуйте логику в отдельном методе

---

## 📱 Сборка для мобильных платформ

### Android
1. Установите Android Studio + NDK
2. В `Project Settings → Platforms → Android`:
   - Укажите путь к SDK и NDK
   - Настройте пакетное имя
3. Соберите через `File → Package Project → Android`

### iOS
1. Установите Xcode
2. В `Project Settings → Platforms → iOS`:
   - Укажите Bundle Identifier
   - Настройте подписку
3. Соберите через `File → Package Project → iOS`

---

## ⚙️ Конфигурация

### DefaultEngine.ini
```ini
[/Script/Engine.InputSettings]
bEnableLegacyInputScales=False
DefaultTouchInterface=/Game/UI/BP_VirtualJoystick.BP_VirtualJoystick

[/Script/Engine.GameEngine]
MaxFPS=60
```

### DefaultGame.ini
```ini
[/Script/EngineSettings.GeneralProjectSettings]
ProjectID=YOUR_PROJECT_ID
CompanyName=YourCompany
ProjectName=UE5 Joystick Game

[/Script/Engine.InputSettings]
DefaultPlayerInputClass=/Script/EnhancedInput.EnhancedPlayerInput
```

---

## 🐛 Известные проблемы и решения

### Проблема: Джойстик не отображается
**Решение:** Убедитесь, что в `DefaultEngine.ini` указан правильный путь к виджету джойстика.

### Проблема: Враги не атакуют
**Решение:** Проверьте Behavior Tree и Blackboard настройки.

### Проблема: Не работает взаимодействие
**Решение:** Убедитесь, что объект реализует интерфейс `IInteractableInterface`.

---

## 📄 Лицензия

Этот проект предоставляется "как есть" для образовательных целей. Вы можете свободно использовать его в своих проектах.

---

## 👨‍💻 Авторы

Создано с помощью AI Assistant для Unreal Engine 5 разработки.

## 🤝 Вклад

Предложения по улучшению приветствуются!

---

## 📚 Дополнительные ресурсы

- [Документация Unreal Engine](https://docs.unrealengine.com/)
- [Enhanced Input System](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-system-in-unreal-engine)
- [Behavior Trees](https://dev.epicgames.com/documentation/en-us/unreal-engine/behavior-trees-in-unreal-engine)
- [Mobile Gaming Best Practices](https://dev.epicgames.com/documentation/en-us/unreal-engine/mobile-game-development)
