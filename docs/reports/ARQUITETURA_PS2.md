# Arquitetura técnica do alvo PlayStation 2

**Conclusão:** PS2SDK, gsKit e PS2Toolchain oferecem infraestrutura de homebrew suficiente para iniciar um ELF PS2, mas não tornam o runtime BattleShip compatível. A opção técnica recomendada é manter a lógica do jogo C da base candidata e construir uma camada PS2 dedicada para renderização, áudio, input, storage, OS e frame pacing. Portar libultraship inteiro seria arriscado e de alto custo; reaproveitar ferramentas/formatos isolados ainda pode ser útil.

## Componentes observados

- **PS2Toolchain** (`ps2dev/ps2toolchain`, `master`, SHA local `41610ee0bc15f887c65ea208bcb9234a3b812005`): `toolchain.sh` executa scripts para DVP, IOP e EE. `config/ps2toolchain-config.sh` aponta para os repositórios `ps2toolchain-{dvp,iop,ee}` em `main` por padrão. O README lista gcc/clang, make, patch, texinfo, flex, bison, GSL, GMP, MPFR, MPC e gettext.
- **PS2SDK** (`ps2dev/ps2sdk`, `master`, SHA `b04e8973c8a9839d0653d87a8d30cc67fd3c3f05`): suporte EE/IOP, controlador/multitap, memory card, USB, CD/DVD, HDD, newlib, rede e áudio. `ee/kernel/include/kernel.h` declara `CreateThread`, `StartThread`, `SleepThread`, `WakeupThread`; `timer.h` tem `GetTimerSystemTime`; `ee/rpc/pad/include/libpad.h` declara `padInit`, `padPortOpen`, `padRead`, `padGetState` e `padSetMainMode`.
- A interface `ee/rpc/audsrv/include/audsrv.h` declara output de áudio por RPC (`audsrv_set_format`, `audsrv_play_audio`, `audsrv_wait_audio`); seu cabeçalho identifica licença LGPL v2. A licença de cada biblioteca deve ser verificada separadamente antes de reutilizá-la.
- **gsKit** (`ps2dev/gsKit`, `master`, SHA `8ef73d05f022d14b3ded7c70cba670fe333ed61a`, tag observada `v1.5.1`): interface C de baixo nível para GS, inline assembly, primitives, texturas, double buffering, z-buffer, vsync e DMA via dmaKit. `ee/gs/include/gsKit.h` importa `dmaKit`; o README menciona VU1.

Os commits acima vêm dos clones locais. As tags locais observadas não são prova de que todo o histórico/tag remoto foi obtido.

## Portabilidade por subsistema

| Subsistema | BattleShip / libultraship observado | Infraestrutura PS2 observada | Trabalho necessário |
|---|---|---|---|
| Renderer / GS | `port/gameloop.cpp` submete display lists N64 ao `Fast3dWindow`; `libultraship/src/fast/interpreter.cpp` processa GBI e backends encontrados são OpenGL, Direct3D 11/12 e Metal. O README Fast3D descreve esses backends. | gsKit opera primitivas, texturas, contextos e filas via DMA; acesso direto ao Graphics Synthesizer. | Backend dedicado: traduzir comandos GBI/F3DEX do Smash 64 para geometria/texturas/estados GS, controlar transferência/upload, buffers e VSync. Não existe implementação GS no Fast3D observado. |
| Fast3D, modelos e texturas | `decomp` emite GBI/display lists e referências a `Vtx`, `Gfx`, TLUT e texturas; libultraship executa isso pelo Fast3D host. `port/bridge/` trata relocação e byteswap. | EE, VU1, DMA/DMAC e GS são candidatos para processamento e envio de primitives. VRAM GS local tem 4 MiB; não é equivalente à RDRAM. | Preservar layout/semântica N64 e implementar rasterização equivalente. Decidir quais comandos emular em software EE e quais transformar em batches/VU1. Desenvolver gerenciamento VRAM, formatos/paletas, clamping, clipping, transparência e sincronização. Custo/performance não medidos. |
| Memória | `port/remix/remix_ram_image.*` carrega imagem RAM do Remix; `port/bridge/` mantém registries de ranges/reloc. `docs/remix/architecture.md` descreve imagem de 4 MiB e identificadores/estruturas. | EE dispõe de 32 MiB de RDRAM, além dos 4 MiB locais do GS; buffers de DMA e assets disputam RAM/VRAM conforme caminho. | Orçamento para heap do jogo, overlays/imagem Remix, recursos descompactados, buffers GS/DMA e áudio; substituir pressupostos host/LP64 e ponteiros/reloc tokens. Mais RDRAM não aumenta VRAM nem prova que dados e tarefas cabem. |
| DMA / EE / VU | O processamento da GBI atual termina em API gráfica host. | dmaKit tem `dmaKit_send`, `dmaKit_send_chain`, `dmaKit_wait`; gsKit inclui suporte de contexto/VU1. EE/IOP têm responsabilidades distintas. | Implementar ownership/cache/alignments, command queues e barreiras EE-DMA-GS; usar VU1 só depois de provar layout e comportamento. Não transportar diretamente rotinas RSP N64 para VU1. |
| Áudio / SPU2 | `port/bridge/audio_bridge.cpp` trata estruturas AL; `port/audio/audio_playback.cpp` é explicitamente pipeline inicial de silêncio alimentado a `AudioPlayerPlayFrame` do LUS. | PS2SDK README declara acesso ao áudio por FreeSD. A cópia também contém `ee/rpc/audsrv/include/audsrv.h` (`audsrv_set_format`, `audsrv_play_audio`, `audsrv_wait_audio`) e chamadas SPU2. | Implementar síntese/ADPCM/mixer e output PS2; escolher áudio PS2 SDK apropriado, converter 32 kHz/formatos e garantir buffering sem bloquear o game loop. Tratar áudio como subsistema ainda não concluído na base. |
| Controle | `port/stubs/n64_stubs.c` mapeia devices de `ControlDeck`/SDL para `OSContPad`. | `libpad` e padman oferecem init, status, DualShock, leitura por port/slot e multitap. | Adaptar estados, calibração analógica, botões, disconnect e polling aos quatro jogadores e ao contrato N64. Evitar depender de SDL. |
| Filesystem / assets | ROM é processada por Torch host-side; BattleShip usa `.o2r`, resource factories, libzip/LUS e caminhos `std::filesystem`/SDL. `port/port_save.cpp` usa SDL path/persistência. | PS2SDK cobre CD/DVD, memory card, USB, HDD e acesso estilo POSIX/newlib. `ee/kernel/include/fileio.h` adverte que chamadas diretas `fio*` podem causar problemas no newlib port e recomenda POSIX calls. | Torch pode ficar no runner/host. Implementar archive loader ou formatos pré-processados e backend de save em storage PS2. Verificar APIs de arquivos, `std::filesystem`, zlib/zip e limites de heap no EE antes de selecionar formato. Não assumir que O2R/libzip compile ou caiba. |
| Threads / scheduler | `port/stubs/n64_stubs.c` implementa OS threads via corrotinas e filas; existem `coroutine_posix.cpp`, `coroutine_win32.cpp` e context switch assembly para hosts. Loop retoma threads por frame. | PS2SDK expõe threads EE (`CreateThread`, `StartThread`, `SleepThread` etc.), sincronização e timers. | Mapear só o subconjunto `osCreateThread`, `osStartThread`, `osRecvMesg`, `osSendMesg`, `osStopThread` exigido pelo decomp. Preferir scheduler cooperativo determinístico inicialmente, com sincronização compatível; não reutilizar `ucontext`, Win32 Fiber ou context switch de outra CPU. |
| Temporização | `port/gameloop.cpp` usa `std::chrono`; stubs VI imitam retrace e documentação descreve SDL timer/frames. | `GetTimerSystemTime`, `SetTimerAlarm` e sincronização/vsync gsKit. | Fonte de tempo EE e retrace GS; preservar timing dos subsistemas e chamadas de timers. Nenhuma latência/frame rate foi medida. |
| Plataforma / build | CMake exige 3.24, C11/C++20, SDL/LUS, host GUI/logging; há gates para Android, Switch, PS5, desktop, não PS2. | PS2Toolchain gera compiladores EE/IOP/DVP; PS2SDK e gsKit. | Novo toolchain file/Makefile, linker/startup EE e selecionamento explícito de fontes. Testar suporte do compilador a C++20/STL antes de planejar portar as classes C++ de LUS. |

## Adaptação de libultraship?

Não é realista tratar o LUS atual como backend portável que só precisa de uma implementação OpenGL nova:

- Seu `Fast3D` é um intérprete GBI que termina em APIs gráficas host; os backends do clone são OpenGL/D3D/Metal. PS2 não expõe essas APIs no ambiente homebrew descrito.
- O runtime também depende de SDL2, arquivos/configurações, archives, controle host e APIs de áudio. CMake e dependências do LUS trazem libzip, nlohmann_json, tinyxml2, spdlog, Threads e implementações gráficas/SDL.
- gsKit é uma API de baixo nível para o GS, não emula Fast3D e não oferece compatibilidade automática com as semânticas RSP/RDP.

**Recomendação:** criar um backend PS2 pequeno atrás das interfaces estritamente necessárias ao código de jogo. Reutilizar helpers puros do LUS apenas depois de confirmar dependências/licença/ABI; não adotar o LUS completo como pressuposto. Uma implementação de `gfx` que converte GBI para GS é um projeto próprio e deve ser provada com primitivas isoladas antes do jogo.

## Menor sequência de prova técnica

1. Em runner, compilar um programa C mínimo para EE usando um toolchain fixado e gerar ELF sem ROM.
2. Validar o tipo/arquitetura ELF e guardar log, versão/checksums e artefato.
3. Separadamente, inicializar gsKit e apresentar uma primitiva estática; depois textura, VSync e controle `libpad`.
4. Só então trazer uma lista curta de GBI e uma textura de teste autorizada, sem assets protegidos no GitHub.
5. Testar áudio, storage, filas e memory budget isoladamente antes de associar a lógica do decomp.

Nenhum ELF, backend gráfico ou build foi produzido nesta auditoria.
