# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

- **Generate project files:** Right-click `.uproject` → "Generate Visual Studio project files", or run `UnrealVersionSelector.exe /projectfiles "<path>\LittleLights.uproject"`
- **Build (Editor):** `UnrealBuildTool.exe Development Win64 -Project="<path>\LittleLights.uproject" -TargetType=Editor -Progress`
- **Package (Shipping):** `RunUAT.bat BuildCookRun -project="<path>\LittleLights.uproject" -platform=Win64 -clientconfig=Shipping -cook -build -stage -pak -archive -archivedirectory="<output>"`
- Engine path is `E:\Epic\UE_5.4` (or `C:\Program Files\Epic Games\UE_5.4` depending on machine). See `batchBuld.bat` and `batchbinaries.bat` in the project root.

## Architecture

**Engine:** Unreal Engine 5.4, C++20 (`CppStandardVersion.Cpp20`), single runtime module `LittleLights`.

**Core classes (C++):**
- `ALL_GameModeBase` — Central game orchestrator. Manages level flow, beast spawn timer, totem completion, fog, intro movement. The Blueprint child `BP_LL_GameModeBase` is the active game mode (configured in `DefaultEngine.ini`).
- `APlayerCharacter` — Main character with camera/spring-arm, torch, Enhanced Input, ability component, interaction component, and AI perception stimuli. Handles movement, jumping, vaulting, balancing, crouching.
- `ALL_PlayerControllerBase` — Handles pause menu, dialogue UI, interaction prompts (key/arrow widgets), HUD beast distance updates.
- `ULL_InteractorComponent` — Tick-based component that raycasts for actors implementing `ILL_GameplayInterface` and triggers `Interact()`.
- `ALevel_Manager_Base` — Per-level actor tracking game state (`GameInit` → `Playing` → `LevelCompleted` → `LevelEnded`), fog material, intro movement.
- `ULevelConfiguration` — Per-level `UDataAsset` defining initial orb state, light-up, and intro movement behavior.

**Ability system (custom, not GAS):**
- `ULL_AbilityComponent` on PlayerCharacter holds `TArray<ULL_Ability*>`. Abilities are started/stopped by name string.
- Tag-based blocking: `GrantsTags` and `BlockedTags` (`FGameplayTagContainer`) on each ability. Active tags on the component.
- Concrete abilities: `LL_Sprint_Ability`, `LL_RollForward_Ability`, `LL_CrouchCross_Ability`, `LL_JumpVault_Ability`, `LL_Orb_Ability`, `LL_CrossBalancing_Ability`.

**Totem system (level objective):**
- `ALL_Tottem` + `ATottem_Piece` — The current system. Players pick up pieces (A-E types) and deliver them to the totem to complete the level.
- `ATottem` (root-level files `Tottem.h/.cpp`) — Older totem implementation.
- Pieces use `ILL_GameplayInterface::Interact()` for pickup, then stored in `APlayerCharacter::TottemPieces` array.

**AI / Beast:**
- `ALL_AIBeast` — Enemy that chases the player. Uses `UPawnSensingComponent` (sight/hearing), behavior trees, and a teleport mechanic.
- `LL_AIBeastController` — AI controller running behavior trees.
- Behavior tree nodes: `LL_BTService_CheckPlayerRange`, `LL_MoveToPlayerBTTaskNode`, `LLBeastBasicAttack_BTTaskNode`.

**Light / Orb:**
- `ALL_Orb` — The player's light orb with decay timer, refill mechanics, liquid material animation.
- `ATorch` — Torch/fire-pit mechanic.

**Save system:**
- `ULLGameManager` (singleton-like, owned by GameMode) → `ULLCustomSaveGame` (USaveGame subclass) → `FLLGameSaveData` (struct with level progress, player location).
- Slot name: `"LLSaveGame"`.

**Dialogue system:**
- `LL_DialogueComponent`, `LL_DialogueSubsystem`, `LL_DialogueTrigger`.

**Collision channels (custom):**
| Channel | Type | Name |
|---------|------|------|
| ECC_GameTraceChannel1 | Trace | Wall |
| ECC_GameTraceChannel2 | Overlap | OrbArea |
| ECC_GameTraceChannel3 | Overlap | OccludedObject |
| ECC_GameTraceChannel4 | Trace | Fog |
| ECC_GameTraceChannel5 | Overlap | OccludedRock |

**Input:** Enhanced Input system. Key mappings: `MoveForward_IA`, `Interact_IA`, `Sprint_IA`, `Jump_IA`, `AdvanceDialogue_IA`, `DPadUp/Down/Left/Right_IA`, `Pause_IA` (Escape). Both keyboard/mouse and gamepad supported (via CommonUI).

**Console variables:** `ll.ShowDistancePlayerBeast` (debug beast distance), `ll.InfiniteSprint` (disable stamina).

**Map progression:** TutorialA → TutorialB → TutorialC → Lobby → Level1 (Totem A) → Level2 (Totem B) → Level3 (Totem C). Map names use prefix-based convention (e.g., `Tutorial_A_Map`, `Tutorial_B_Map`, `Totem_A_Map`).

**Content organization:** Blueprints in `/Content/Blueprints/`, Maps in `/Content/Maps/`, DataTables in `/Content/DataTables/`. Most gameplay logic lives in Blueprint subclasses of the C++ base classes.

## Code conventions

- Class prefix: `LL_` for newer classes, some legacy classes lack this.
- Enum prefix: `ELL` (e.g., `ELLMapsIndexEntry`, `ELLEInputDirection`).
- Source layout: headers in `Public/`, implementations in `Private/`. Root-level `.h`/`.cpp` pairs are older/legacy.
- `ILL_GameplayInterface` — The core interaction interface. Must be implemented by any actor the player can interact with.
- Blueprint-implementable events marked with `BlueprintImplementableEvent` or `BlueprintNativeEvent` are the primary extension points for Blueprint logic.
