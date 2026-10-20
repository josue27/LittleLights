# Changelog

Bitácora del proyecto LittleLights. Registra el estado actual del desarrollo y el historial de cambios por fecha.

## Cómo actualizar

- Al terminar una sesión, actualiza la sección **Estado actual**.
- Agrega una entrada nueva en **Historial** con la fecha, qué se hizo y qué sigue.
- Usa una fecha por entrada, en orden descendente.

## Estado actual

Última actualización: 2026-09-30

### Reciente

- Rework del sistema de guardado (`ULLGameManager` → `ULLCustomSaveGame` → `FLLGameSaveData`).
- Ajustes en el inicio de la secuencia de completado de nivel.
- Menús de opciones y ajustes de usuario (GemUserSettings, fondo negro en `WBP_Options`).

### En progreso

- (vacío)

### Pendiente

- Cambios sin commitear: `batchBuld.bat`, `.idea/workspace.xml`, `global.json` eliminado (`global.json.bak` como respaldo).

### Próximos pasos

- Save system: cerrar gaps detectados (2026-10-01). Piezas en mano no persistidas, orbe y antorcha nunca guardados, posición solo al entregar pieza. Plan en 5 fases; arrancar por piezas en mano + orbe + antorcha.

## Historial

### 2026-09

- **2026-09-23** — Rework del sistema de guardado; cambios en el inicio de la secuencia de completado.

### 2026-08

- **2026-08-05** — `Update DefaultGame.ini`.
- **2026-08-04** — Fondo negro en `WBP_Options`; cambios en menús de GemUserSettings y opciones.

### 2026-06

- **2026-06-15** — Cambio de streaming level en la base del golem.
- **2026-06-05** — Fix Tutorial C: diálogos al inicio.
- **2026-06-03** — Null checks y fix en tarea de behavior tree del AI.
- **2026-06-02** — Update BP pond tutorial; fix `BP_Pond` desapareciendo.

### 2026-05

- **2026-05-26** — Conexión de UserSettings con el menú de opciones.
- **2026-05-18** — Update `BP_PlayerCharacter`; nueva cápsula de colisión para el monstruo y trace channels.
- **2026-05-13** — Cambios en el debug de texto del Beast.
- **2026-05-05** — Fix torch/stamina, delegates, spline e init bugs. Lightmaps de la base del golem.
- **2026-05-04** — Config de `CLAUDE.md`.
