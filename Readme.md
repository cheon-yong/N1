## 프로젝트 요약

- Unreal Engine 5의 샘플 프로젝트인 Lyra를 분석하기 위한 프로젝트
- Lyra Project에 적용된 코드들을 적용하고 확장하기
- 원하는 기능을 Lyra Project에서 구현한 방식을 재구현하는 방식으로 개발.
- GameplayAbilities, GameplayEffect, GameplayTags 등 GAS를 사용

## 상세 개발사항

### 플러그인

![Plugin](https://github.com/user-attachments/assets/09f5623f-da8d-485d-8f29-f43b98a55409)

- 프로젝트를 진행할 때 반복적으로 사용하는 코드 또는 컨텐츠들을 플러그인으로 제작하여 재사용성을 높였습니다.

---

### Experience

- Lyra에 적용된 개념인 `Experience`를 적용하였습니다. 게임플레이 중 `GameMode`를 변경하지 않고 `Experience`를 변경하여 `GameMode` 전환 시 발생하는 오버헤드를 줄였습니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/e886c2ea-7f43-4fd0-9de4-a6d44d791ce6/Untitled.png)

- `Experience Definition` 을 기반으로 게임에 등록된 `Experience`는 포탈 형태로 나타나며 포탈로 이동 시 포탈과 일치하는 `Experience`로 변경됩니다.

---

### 기본적인 이동

- `Enhanced Input System` 기반으로 캐릭터의 움직임을 구현하였습니다. `Input Mapping Context` 를 직접 적용하지 않고 `PawnData` 라는 `DataAsset`을 이용하여 적용하였습니다.
- `PawnData` 에는 `Experience` 에서 사용할 기본적인 `Pawn` 의 클래스, 해당 `Pawn`에 적용할 `Ability`와 `InputConfig` 를 지정할 수 있습니다.
- `InputConfig` 는 캐릭터에 적용할 `InputAction`과 `GameplayTag`를 멤버로 가지는 구조체를 배열로 가지고 있고 이를 `EnhancedInputComponent`에 등록합니다.

---

### 사격 시스템

- 무기를 장착한 후 좌클릭 시 총알이 발사됩니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/f330f6f1-f373-457b-98e6-bac14c415619/Untitled.png)

- 우클릭을 누르고 있을 시 카메라 모드가 전환되어 확대사격할 수 있습니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/052405ce-31c8-48c7-830c-cd4bd0cccad3/Untitled.png)

- 적중 시 부위에 따른 데미지가 발생합니다. (약점으로 지정된 부분은 데미지 증가)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/cd70a546-370e-49ec-ba11-f351e15fefb7/Untitled.png)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/22b07795-ba87-4a35-8473-62eafc7e2b29/Untitled.png)

- 사격은 `Ability`로 구현이 되어있으며 총을 습득하였을 경우 캐릭터에게 부여됩니다.
- `Ability`는 `Raycast`를 이용하여 타겟을 확인하여 캐릭터와 일치하면 `GameplayEffect`를 적용합니다.

---

### HUD

- 플레이어 캐릭터의 `HealthComponent`과 연동되어있는 HP바와 현재 무기슬롯이 표시됩니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/ffade11c-b5a3-4840-a790-ad1be6599dd2/Untitled.png)

- 캐릭터의 `HealthComponent`는 `AttributeSet`을 가지고 있는 `ActorComponent`로 `AttributeSet` 값의 변경이 생기면 `Delegate` 를 이용하여 변경된 값을 공유합니다.
- 캐릭터의 무기가 퀵슬롯에 추가되면 우측 하단에 해당 무기의 아이콘이 표시가 되고 숫자 1, 2, 3을 통해 원하는 아이템을 선택할 수 있습니다.
- 무기를 장착하게 되면 HUD 가운데에 무기에 따른 크로스헤어가 표시됩니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/c12f8c6f-eddf-445e-a4e5-956a0448338e/Untitled.png)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/a44956ef-d038-4900-8775-864cabefd1c4/Untitled.png)

---

### 인벤토리

- FPS 중 배틀그라운드의 인벤토리를 모방하여 Lyra 기반 시스템에 새로운 인벤토리를 추가하였습니다.
- 기존 Lyra에는 인벤토리 상황을 확인할 수 있는 방법이 HUD가 유일하여 새로운 `Widget`을 디자인하여 추가하였습니다.
- Lyra는 기본적으로 Pistol 이 기본적으로 주어지고 무기가 있는 포탈에 진입하면 해당 무기를 획득하는 방식입니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/287ba8b9-d48e-4d63-9f77-0b4d3027e884/Untitled.png)

- 새로 추가한 방식은 캐릭터에 `VicinityComponent` 와 `Collision` 을 추가하여 `Collision` 에 `BP_Item`을 상속받은 `Actor`가 Overlap되면 해당 오브젝트를 `VicinityComponent`에서 처리하도록 개발하였습니다.
- 주변에 오브젝트가 추가되어 `VicinityComponent`에 변경사항이 발생하면 기존 Lyra에서 사용하는 방식대로 `UGameplayMessageSubsystem` 를 이용하여 변경사항이 담긴 Message를 Broadcast합니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/5ee19c33-cfa7-4fb8-b411-cd24a4e2c2be/Untitled.png)

- Tab키를 누르면 주변 아이템, 인벤토리, 퀵슬롯이 표시되는 전체화면 Widget이 표시되고 드래그 드롭을 통해서 아이템을 습득, 버리기, 장착할 수 있습니다.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/822bcd81-8eb0-4ea3-8f40-15781eb61fe7/35eaf228-d541-4427-a256-e87a32a12dd2/Untitled.png)

- 드래그 드롭을 통해 인벤토리에 추가된 아이템들은 필드에서 사라지고 다시 버리면 필드에 생성됩니다.

---
