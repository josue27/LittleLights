# Plan — Save system, videos tutorial, movimiento

Fecha: 2026-10-14. Estado: análisis completo, sin cambios de código aún.

## Contexto

Proyecto Unreal Engine 5.4 `LittleLights`. Tres frentes de trabajo:
1. Save system incompleto.
2. Videos tutorial no aparecen en package builds.
3. Movimiento con control desigual en algunas direcciones.

El análisis se hizo con 3 agentes en paralelo y se verificó contra el código actual. Las conclusiones de una auditoría previa (2026-10-01) siguen vigentes.

## Decisiones tomadas (con el usuario)

- **Triggers de guardado:** entregar pieza + salir de nivel + quit. No guardar por cada recogida.
- **Orbe:** guardar como fracción normalizada `0..1` (`OrbRemainingDelta`).
- **Videos:** el usuario tiene los 4 mp4 originales → re-importar (no re-exportar).
- **Movimiento:** el offset de 45° (`JoystickAnlgeDifference`) es intencional (cámara fija). Mantener.

---

## A. Save system

### Estado actual (verificado)

`FLLGameSaveData` (`Source/LittleLights/Public/SaveSystem/FLLGameSaveData.h`) guarda hoy:
`LevelsCompleted`, `InLevel`, `InitialEntranceCompleted`, `PlayerLocation`, `Totem` (muerto), `DeliveredPieces`, `bHasMidLevelProgress`, `OrbRemainingDelta`, `bTorchLit`.

Guardado vía `ULLGameManager` (`UGameInstanceSubsystem`, slot `"LLSaveGame"`) y `ULLCustomSaveGame` (USaveGame). Save/load por `UGameplayStatics`.

### Gaps (confirmados)

1. Piezas en mano (`Player.TottemPieces`, `PlayerCharacter.h:302`) no persisten. Solo `DeliveredPieces`.
2. Orbe: `SetOrbState` existe (`LLGameManager.h:59`) pero cero callers C++ → `OrbRemainingDelta` queda 1.0.
3. Antorcha: `SetTorchState` existe (`LLGameManager.h:62`) pero cero callers C++; `RestoreLevelProgress` no restaura antorcha.
4. Posición: `SetPlayerLocation` solo al entregar pieza (`LL_Tottem.cpp:142`). No hay save al salir/quit.
5. `bHasMidLevelProgress` solo en `AddDeliveredPiece` (`LLGameManager.cpp:50`). Recoger sin entregar no lo activa → reanudar borra progreso.
6. `RestoreLevelProgress` hardcodea `30.0f` en vez de `MaxLightUpTime` (`LL_GameModeBase.cpp:125`).
7. Campo muerto `GameSave.Totem` sigue presente (`FLLGameSaveData.h:57`).
8. `SetInitialEntranceCompleted` BlueprintCallable, sin caller C++ ni lectura en restore.

Archivos clave: `LLGameManager.h/.cpp`, `LL_GameModeBase.cpp` (`SaveFileLoaded:56`, `RestoreLevelProgress:85`), `LL_Tottem.cpp:141-143`.

Caveat: los setters son `BlueprintCallable`; el wiring puede vivir en Blueprints (uasset binario, no grepeable). Verificar antes de marcar "roto".

### Fases

- **Fase 1 — Writers C++:** `SetOrbState(float)` (GameMode lee `ALL_Orb::RemainingDeltaTime / MaxLightUpTime`), `SetTorchState(bool)` desde `ATorch`, nuevo `CarriedPieces` (`TArray<TottemPieceType>`) + `SetCarriedPieces`, `SetPlayerLocation` al salir nivel y quit.
- **Fase 2 — Restore:** restaurar `CarriedPieces`, orbe con `MaxLightUpTime` (quitar `30.0f`), antorcha. Marcar `bHasMidLevelProgress` al recoger pieza.
- **Fase 3 — Triggers:** salir nivel en `Level_Manager::LevelEnded`; quit en `EndPlay` del GameMode / shutdown GameInstance.
- **Fase 4 — Limpieza:** borrar `GameSave.Totem`.
- **Fase 5 — Verificar BP:** abrir `BP_LL_GameModeBase`, `BP_Orb`, `BP_Tottem` en editor. Confirmar si setters ya se llaman desde BP.

---

## B. Videos tutorial

### Causa raíz

`WBP_TutorialPanel` (`Content/UIX/Tutorial/`) usa `FileMediaSource` con rutas absolutas `file://`:

- `CrossOver_Vid` → `file://C:/Users/meval/Videos/2025-04-01 22-13-22.mp4`
- `CrouchUnderTut_Vid` → `.../2025-03-29 19-09-23.mp4`
- `JumpOverTut_Vid` → `.../2025-03-29 19-25-13.mp4`
- `beastscape` → `file://D:/ParallelsGames/LittleLights/beastscape.mp4`

Ningún `.mp4` existe en el proyecto; no hay `Content/Movies/`; no hay config de cook de media.

### Fix

1. Importar los 4 mp4 a `Content/Movies/`.
2. Repuntar `FileMediaSource.FilePath` a relativo `./Movies/<archivo>.mp4`.
3. Verificar `Movies` no excluido del cook (`Config/DefaultGame.ini`, `[/Script/UnrealEd.ProjectPackagingSettings]`).

**Estado (2026-10-17): HECHO.**
- mp4 copiados a `Content/Movies/`: `CrossOver_Vid.mp4`, `CrouchUnderTut_Vid.mp4`, `JumpOverTut_Vid.mp4`, `beastscape.mp4`.
- 4 `FileMediaSource` repuntados vía ue-mcp `set_asset_property` (persisted). Valor nuevo: `./Movies/<name>.mp4`.
- `bSkipMovies=False` ya presente → `CopyBuildToStagingDirectory` stagea `Content/Movies` como NonUFS (archivos sueltos), lo que FileMediaSource/WMF requiere. Sin config extra.

---

## C. Movimiento

### Hallazgo

`Personaje.cpp` (`MovimientoForward/Right`) es legado muerto. El pawn activo es `APlayerCharacter` / `BP_PlayerCharacter`. Handler real: `APlayerCharacter::Movement()` (`PlayerCharacter.cpp:426-467`).

### Causas (ranked)

1. Base rotada 45° (`JoystickAnlgeDifference`) — intencional, mantener.
2. Orden de modificadores `SwizzleAxis`/`Negate` en `IA_Move` (`LLGeneralInputMap`) — revisar en editor.
3. `* DeltaTimeSeconds` dentro de `AddMovementInput` (`PlayerCharacter.cpp:459-460`) — dependiente de FPS.

### Fix

1. Mantener 45°.
2. Revisar orden `SwizzleAxis`/`Negate` en editor.
3. Quitar `* DeltaTimeSeconds` de `AddMovementInput`.

**Estado (2026-10-18): HECHO en C++, pendiente compilar + verificar en juego.**

- Handler activo confirmado: `Movement()` (único bound a `MoveForward_IA` en `SetupPlayerInputComponent:226`). `ForwardMovement`/`RightMovement` son dead code (sin binding). `MovementInputVector` del Tick (línea 168) es solo para log.
- `MoveForward_IA` → `IA_Move` (Axis2D). Confirmado vía ue-mcp `read_imc` + `read_input_action`.
- **C2 descartado:** modificadores de teclado en `LLGeneralInputMap` son correctos. W=`[SwizzleAxis]`, S=`[SwizzleAxis, Negate]`, A=`[Negate]`, D=`[]`. Gamepad `Gamepad_Left2D` sin modificadores (paso directo del stick). No hay bug de orden.
- **C3 confirmado + fix:** `VelocidadMovimiento = 30.0` (override en `BP_PlayerCharacter`, no el default 1.0 de C++). `AddMovementInput(dir, InputVector * 30 * dt)` ⇒ a 60fps scale≈0.5 (mitad de velocidad), a 30fps scale≈1.0 (velocidad completa). Frame-dependent.
- Fix aplicado: `AddMovementInput(ForwardDirection, InputVector.Y)` / `(RightDirection, InputVector.X)`. Escala 0..1 canónica; velocidad real la da `MaxWalkSpeed` (default 600). `VelocidadMovimiento` queda sin uso en movimiento.

---

## Qué se necesita del usuario

1. Ruta de los 4 mp4 (para B).
2. Acceso al editor o autorización para `unreal-mcp` (para Fase 5 de A y C2).
3. Orden de trabajo. Recomendado: **B → A → C**.

---

## Progreso

- [x] Análisis save system
- [x] Análisis videos tutorial
- [x] Análisis movimiento
- [x] B: importar mp4 + repuntar rutas + cook (2026-10-17)
- [ ] A: Fases 1-5
- [x] C: revisar modifiers (C2 descartado) + quitar `*dt` (2026-10-18, pendiente compilar)
