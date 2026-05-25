# UE5 Joystick Game - Полноценная игра с виртуальным джойстиком

## Обзор проекта

Это полноценный шаблон игры на Unreal Engine 5 с реализацией виртуального джойстика для управления персонажем на мобильных устройствах и ПК.

## Структура проекта

```
UE5JoystickGame/
├── Config/                          # Конфигурационные файлы
│   ├── DefaultEngine.ini           # Настройки движка
│   ├── DefaultGame.ini             # Настройки игры
│   └── DefaultEditor.ini           # Настройки редактора
├── Source/
│   ├── UE5JoystickGame/            # Исходный код игры
│   │   ├── VirtualJoystickComponent.h/.cpp    # Компонент виртуального джойстика
│   │   ├── JoystickPlayerCharacter.h/.cpp     # Класс игрока
│   │   ├── JoystickGameMode.h/.cpp            # Режим игры
│   │   ├── UE5JoystickGame.h/.cpp             # Модуль игры
│   │   └── UE5JoystickGame.Build.cs           # Настройки сборки
│   ├── UE5JoystickGameEditor/      # Код редактора
│   ├── UE5JoystickGame.Target.cs   # Таргет игры
│   └── UE5JoystickGameEditor.Target.cs  # Таргет редактора
├── Content/
│   ├── Blueprints/                 # Инструкции для Blueprint
│   │   ├── BP_VirtualJoystick_Instructions.txt
│   │   └── BP_MainLevel_Instructions.txt
│   └── Materials/                  # Материалы (пусто, создаются в редакторе)
└── Docs/                           # Документация
    └── README.md                   # Этот файл
```

## Основные компоненты

### 1. UVirtualJoystickComponent

Компонент для обработки ввода с виртуального джойстика:
- Поддержка touch-ввода для мобильных устройств
- Эмуляция джойстика через клавиатуру (WASD) и геймпад
- Настраиваемая мёртвая зона (DeadZone)
- Нормализация вывода от -1 до 1

**Основные функции:**
- `OnTouchStarted(FVector2D)` - начало касания
- `OnTouchMoved(FVector2D)` - перемещение касания
- `OnTouchEnded()` - завершение касания
- `GetAxisValue()` - получение текущего значения оси
- `GetIsPressed()` - проверка нажатия

### 2. AJoystickPlayerCharacter

Класс персонажа игрока с поддержкой джойстика:
- Интегрированный компонент VirtualJoystick
- Камера от третьего лица с SpringArm
- Поддержка ходьбы и бега
- Прыжки и ускорение

**Настройки скорости:**
- WalkSpeed: 600 единиц/сек
- SprintSpeed: 1000 единиц/сек

### 3. AJoystickGameMode

Режим игры, устанавливающий класс персонажа по умолчанию.

## Инструкция по настройке в Unreal Engine 5

### Шаг 1: Создание проекта

1. Откройте Unreal Engine 5.3+
2. Создайте новый проект типа "Games" -> "Third Person"
3. Выберите "Blueprint" или "C++" (рекомендуется C++)
4. Назовите проект "UE5JoystickGame"

### Шаг 2: Импорт кода

Скопируйте файлы из папки `Source` в ваш проект:
- Все `.h` и `.cpp` файлы
- Все `.Build.cs` файлы
- Target файлы

### Шаг 3: Сборка проекта

1. Откройте решение `.sln` в Visual Studio
2. Соберите проект в режиме Development
3. Вернитесь в Unreal Editor и перезагрузите проект

### Шаг 4: Настройка уровня

Следуйте инструкциям в файле `Content/Blueprints/BP_MainLevel_Instructions.txt`:
1. Создайте уровень с геометрией
2. Добавьте Player Start
3. Настройте GameMode на `JoystickGameMode`
4. Добавьте освещение и Post Process Volume

### Шаг 5: Создание виджета джойстика

Следуйте инструкциям в файле `Content/Blueprints/BP_VirtualJoystick_Instructions.txt`:
1. Создайте Widget Blueprint "WBP_VirtualJoystick"
2. Добавьте Canvas Panel и два Image (фон и ручка джойстика)
3. Реализуйте логику обработки touch-событий
4. Свяжите с компонентом VirtualJoystickComponent

### Шаг 6: Настройка ввода

В Project Settings -> Input:

**Action Mappings:**
- Jump: Space / Gamepad Face Button Bottom
- Sprint: Left Shift / Gamepad Left Trigger

**Axis Mappings:**
- MoveForward: W (1.0), S (-1.0)
- MoveRight: D (1.0), A (-1.0)
- LookUp: Mouse Y (-1.0)
- LookRight: Mouse X (1.0)

Для Enhanced Input System создайте Input Mapping Context и назначьте действия.

## Управление

### Мобильные устройства:
- **Перемещение**: Коснитесь экрана и перетаскивайте в любом направлении (виртуальный джойстик)
- **Прыжок**: Кнопка прыжка на экране (требуется добавить в UI)
- **Бег**: Двойное нажатие или отдельная кнопка

### ПК (клавиатура + мышь):
- **W/A/S/D**: Перемещение
- **Мышь**: Поворот камеры
- **Пробел**: Прыжок
- **Shift**: Бег

### Геймпад:
- **Левый стик**: Перемещение
- **Правый стик**: Поворот камеры
- **A/Cross**: Прыжок
- **LT/L2**: Бег

## Расширенные возможности

### Что можно добавить:

1. **Интерфейс пользователя**:
   - Кнопка прыжка
   - Кнопка бега
   - Индикатор здоровья
   - Мини-карта

2. **Геймплей**:
   - Система заданий
   - Враги с AI
   - Коллекционные предметы
   - Транспорт

3. **Визуальные эффекты**:
   - Частицы при беге/прыжке
   - Следы на земле
   - Динамическая погода

4. **Звук**:
   - Шаги персонажа
   - Фоновая музыка
   - Звуковые эффекты

## Оптимизация для мобильных устройств

Включите следующие настройки для мобильной сборки:

1. **Project Settings -> Platforms -> Android/iOS**:
   - Включите Mobile HDR (если нужно)
   - Настройте разрешение текстур
   - Включите пакетную отрисовку

2. **Scalability Settings**:
   - Уменьшите качество теней
   - Отключите лишние Post Process эффекты
   - Используйте LOD для моделей

3. **Input**:
   - Включите Multi-Touch
   - Настройте чувствительность джойстика

## Сборка проекта

### Для Android:
1. Установите Android SDK и NDK
2. Настройте пути в Project Settings
3. Build -> Package Project -> Android

### Для iOS:
1. Установите Xcode на Mac
2. Настройте сертификаты разработчика
3. Build -> Package Project -> iOS

### Для ПК:
1. Build -> Package Project -> Windows/Mac/Linux

## Требования к системе

### Для разработки:
- Unreal Engine 5.3 или выше
- Visual Studio 2022 с поддержкой C++
- Минимум 16 GB RAM
- SSD для быстрой компиляции

### Для запуска (ПК):
- Windows 10/11 64-bit
- DirectX 12 совместимая видеокарта
- 8 GB RAM минимум
- 10 GB свободного места

## Лицензия

Этот проект предоставляется как есть (as-is) для образовательных и коммерческих целей.

## Поддержка

Для вопросов и предложений обращайтесь к документации Unreal Engine:
- [Unreal Engine Documentation](https://docs.unrealengine.com/)
- [Unreal Engine Forums](https://forums.unrealengine.com/)
- [Community Wiki](https://wiki.unrealengine.com/)

## Авторы

Создано как шаблон для разработки игр с виртуальным джойстиком на Unreal Engine 5.
