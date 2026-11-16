# Auditoria técnica inicial

**Data da auditoria:** 16/11/2026  
**Escopo:** leitura estática dos clones em `audit-sources/`; somente documentação foi alterada.  
**Resultado principal:** BattleShip-Remix contém lógica de jogo do Smash Remix 2.0.1 já portada para C/C++, mas a integração de +EXTRA não é uma conversão desse código. A lógica de jogo e os dados de Remix existentes são candidatos a reaproveitamento; renderer, áudio, input, filesystem, temporização e glue de plataforma do PC não constituem um backend PS2.

## Evidência e limitações

Os commits e branches abaixo foram lidos dos arquivos `.git/HEAD`, `.git/refs/heads/*` e dos metadados dos submódulos nas cópias fornecidas. Foram consultados arquivos com leitores de texto e buscas (`view`/`rg`). **Nenhum comando Git, script dos projetos, compilação ou busca de ROM foi executado.** As cópias são somente leitura para esta tarefa.

Os SHAs identificam as referências locais observadas no clone, não uma consulta remota feita nesta auditoria. Não foi possível consultar `git status`, portanto a limpeza dos worktrees não está confirmada. A enumeração de tags é a que estava disponível nos metadados locais; não se presume que esses clones tenham todas as tags remotas.

| Fonte | Branch/ref observada e commit | Tags observadas | Proveniência/licença identificada |
|---|---|---|---|
| [BattleShip-Remix](https://github.com/lucasliet/BattleShip-remix) | `feature/remix-port` — `93db0f7e7067cc80c36f8314effe2d243e848162` | Nenhuma tag local observada | `LICENSE` declara MIT para código do port/build; exclui a licença da descompilação e dados de jogo. |
| [Smash Remix +EXTRA](https://github.com/joaorb64/smashremix-plus-extra) | `main` — `ee3d2f0f0e49aeb1dc598670a7f6f1f75c3e2a82` | Nenhuma tag local observada | Não foi encontrado arquivo `LICENSE` na raiz examinada; revisar antes de redistribuir/adaptar conteúdo. |
| +EXTRA: submódulo `smashremix` ([Smash Remix](https://github.com/JSsixtyfour/smashremix)) | `master` — `5e04fe7fcd023cd43c71f25f89bb6e810d254d55` | `2.0.1` presente; o README do BattleShip identifica esse commit como a versão 2.0.1 | A cópia não apresentou arquivo de licença na raiz consultada. |
| [SSB64 decompilation](https://github.com/VetriTheRetri/ssb-decomp-re) | `main` — `572a747d8171137718fad472c5d7473dc7309ffd` | Nenhuma tag local observada | Não foi encontrado arquivo `LICENSE` na raiz examinada. |
| BattleShip: submódulo `decomp` | Fork `lucasliet/ssb-decomp-re-remix`, `feature/remix-port` — `3413abab6898eaa76ef7bd426665a495f00f1be7` | Tags do submódulo não enumeradas | É o código efetivamente apontado por `.gitmodules`, não o clone upstream de referência. |
| [Smash Remix upstream](https://github.com/JSsixtyfour/smashremix) | `master` — `5e04fe7fcd023cd43c71f25f89bb6e810d254d55` | `2.0.1` presente | O BattleShip `docs/remix/build_and_roms.md` relaciona o commit à tag 2.0.1 (08/03/2026). |
| [BattleShip upstream](https://github.com/JRickey/BattleShip) | `main` — `241a3ae154f249469d8707432ec79f51f997cf54` | Nenhuma tag local observada | Referência do port, não base PS2. |
| [libultraship upstream](https://github.com/Kenix3/libultraship) | `main` — `759b13401cd6903daa67a00a753066f791622e78` | Nenhuma tag local observada | MIT identificado no aviso de licença do BattleShip. |
| BattleShip: submódulo `libultraship` | Fork `lucasliet/libultraship-remix`, `feature/remix-port` — `29f3df755b38506aa366add97b70b3089536be57` | Tags do submódulo não enumeradas | Fork específico usado pelo projeto; não equivale automaticamente ao clone upstream em `audit-sources/libultraship/`. |
| [Torch upstream](https://github.com/HarbourMasters/Torch) | `main` — `22d32182e146716309249a8f668931ad47b5ce0c` | Nenhuma tag local observada | MIT identificado no aviso de licença do BattleShip. |
| BattleShip: submódulo `torch` | Fork `lucasliet/Torch-remix`, `feature/remix-port` — `80535fb29c98353869e118b49ae7715cb5fa2fe2` | Tags do submódulo não enumeradas | Fork específico usado pelo projeto; o clone upstream é apenas referência. |
| [PS2SDK](https://github.com/ps2dev/ps2sdk) | `master` — `b04e8973c8a9839d0653d87a8d30cc67fd3c3f05` | Nenhuma tag local observada | README declara Academic Free License 2.0. |
| [gsKit](https://github.com/ps2dev/gsKit) | `master` — `8ef73d05f022d14b3ded7c70cba670fe333ed61a` | `v1.5.1` presente | README declara Academic Free License 2.0. |
| [PS2Toolchain](https://github.com/ps2dev/ps2toolchain) | `master` — `41610ee0bc15f887c65ea208bcb9234a3b812005` | Nenhuma tag local observada | Não foi identificado arquivo `LICENSE` na raiz examinada. |

O +EXTRA aponta seu submódulo diretamente para `JSsixtyfour/smashremix`; o BattleShip aponta três submódulos para forks próprios. O commit do clone upstream de `ssb-decomp-re` (`572a...`) não é o commit do submódulo que a base nativa usa (`3413...`). A relação ancestral entre eles não foi verificada.

## BattleShip-Remix: arquitetura observada

O README e `docs/remix/architecture.md` descrevem o produto como port nativo PC/Switch do Smash Remix 2.0.1. As camadas separadas no README são:

1. **Jogo:** `decomp/src/` e, para Remix, `decomp/src/ft/ftremix/`, `decomp/src/gr/grremix/`, `decomp/src/it/itremix/` e arquivos `*remix*`. O C da descompilação contém a lógica do jogo e rotinas Remix reescritas a partir da assembly MIPS.
2. **Glue do port:** `port/`, incluindo `port/remix/` (registro de personagens, imagem RAM e overlays), `port/bridge/`, `port/stubs/`, `port/gameloop.cpp`, áudio e recursos. `#ifdef PORT` e `REGION_REMIX` separam mudanças do runtime nativo dos caminhos originais.
3. **Runtime host:** `libultraship/` (Fast3D, SDL2/input, áudio e archives O2R) e `torch/` (extração/construção dos recursos).

`CMakeLists.txt` exige CMake 3.24, C11 e C++20. Coleta fontes da descompilação em C e código de port C/C++; habilita opções específicas de Android, Switch e PS5, mas não há alvo PS2. `BUILDING.md` usa CMake e submódulos. O build normal também compila Torch e extrai recursos da ROM. CMake configura `GBI_UCODE=F3DEX_GBI_2`; o Fast3D do LUS declara suporte a OpenGL, Direct3D 11/12 e sistemas de janela GLX/DXGI/SDL, não gsKit.

### Separação por subsistema

| Área | Evidência concreta | Implicação para PS2 |
|---|---|---|
| Lógica de jogo | `decomp/src/{ft,gm,gr,if,it,lb,mn,mp,sc,sys,wp}`; `port/remix/`; `docs/remix/architecture.md` | Melhor candidato a reaproveitamento, sujeito ao ABI/ponteiros/endianess e às APIs N64 emulados pela camada `PORT`. |
| Assembly convertida | `decomp/src/ft/ftremix/` e `port/remix/`; `docs/remix/patch_audit.md` rastreia patches Remix para funções C | Converter a lógica não converte renderer ou OS; reuso exige preservar semântica N64 e reconciliar os pontos de patch. |
| Renderer | `port/gameloop.cpp` entrega display lists Fast3D; `libultraship/src/fast/interpreter.cpp`, `fast/backends/gfx_{opengl,direct3d11,direct3d_common,metal}.cpp` | Backend host não é gsKit. A interpretação GBI pode servir de referência; submissão de comandos, rasterização, texturas, sincronização e VRAM exigem backend PS2 dedicado. |
| Áudio | `port/bridge/audio_bridge.cpp`, `port/stubs/audio_stubs.c`, `port/audio/audio_playback.cpp` | O arquivo de playback observado se identifica como “Phase 1: silence pipeline” e alimenta `AudioPlayerPlayFrame`; não assumir que o áudio do jogo já está pronto para portar. |
| Input | `port/stubs/n64_stubs.c` converte dispositivos de `ControlDeck`/LUS em `OSContPad`; PC depende do runtime SDL/LUS | Usar `libpad`/padman de PS2SDK e mapear botões/analógicos ao contrato N64. |
| Filesystem/recursos | `port/port_save.cpp`, `port/resource/`, O2R; `torch/` é ferramenta de extração | SDL caminhos, libzip/O2R e Torch não são garantidamente disponíveis no alvo. Torch pode permanecer ferramenta host; runtime precisa de carregador/armazenamento PS2. |
| Threads/OS | `port/stubs/n64_stubs.c`, `port/coroutine_{posix,win32}.cpp`, `port/gameloop.cpp` | A base emula threads N64 com corrotinas e mecanismos de PC; não reutilizar esses adaptadores no EE. Implementar a semântica necessária com scheduler cooperativo ou primitives PS2SDK. |
| Temporização | `port/gameloop.cpp` usa `std::chrono`; stubs VI documentam SDL timer/retrace | Trocar por VSync GS/timer EE e manter a cadência assumida pelo jogo. |
| Plataforma | `port/port.cpp`, SDL2, caminhos host, CMake platform gates | Inicialização de janela, filesystem, logs, corrotina e callbacks devem ser removidos/trocados por uma camada PS2. |

`decomp/src` usa structs e convenções N64 (OS, GBI, audio lib); o port resolve vários problemas de endianness e relocação em `port/bridge/`. O código C é reutilizável como fonte, mas não está comprovado que compile inalterado com o ABI/compilador EE: a build host usa C++20, enquanto o perfil PS2 e suporte de biblioteca ainda não foram testados.

### Reuso imediato vs. adaptação

**Reuso candidato, sem tomar como já portado:** regras de gameplay puras, descritores e dados C do Remix já convertidos, matemática simples, nomes/IDs e ferramentas de referência; estruturas GBI/reloc como contrato a emular. Assets derivados devem permanecer fora do Git e ser gerados somente em workflow autorizado.

**Exigem adaptação/substituição:** `port/` orientado a SDL/LUS/desktop, os backends gráficos, apresentação de display lists, audio output, controle, path/save, O2R loader, coroutine backend, frame pacing, integrações de desktop e qualquer C++/STL que o toolchain PS2 não prove suportar.

**Não usar como dependência alvo sem prova:** libultraship completo, Torch runtime, OpenGL/SDL, miniaudio ou `std::filesystem`. LUS e Torch podem continuar úteis em ferramentas/build host; o runtime PS2 tende a precisar de backend nativo.

## Primeiros marcos e riscos

- **Menor marco útil:** compilar um ELF mínimo sem ROM e inspecioná-lo no runner. Isso prova a cadeia EE/toolchain, não o boot em emulador ou console.
- **Riscos maiores:** converter Fast3D/RDP para GS; memória local de vídeo de 4 MiB e tráfego DMA; áudio funcional; divergência entre ROM/assembly e runtime C; suporte de compilador/stdlib do EE; compatibilidade de conteúdo +EXTRA; licença/proveniência da decompilação e +EXTRA.
- **Conclusão sobre nuvem:** compilar um ELF mínimo é uma meta plausível em GitHub-hosted runners. Não há evidência de duração real da instalação, build de jogo PS2 ou compatibilidade de runtime; a cobertura do orçamento gratuito depende da visibilidade do repo, plano e saldo de minutos.

## Material examinado

`audit-sources/battleship-remix/{README.md,BUILDING.md,CMakeLists.txt,.gitmodules,LICENSE,docs/remix/architecture.md,docs/remix/patch_audit.md,docs/remix/build_and_roms.md,port/gameloop.cpp,port/stubs/n64_stubs.c,port/audio/audio_playback.cpp,port/port_save.cpp,libultraship/src/fast/README.md,libultraship/src/fast/CMakeLists.txt}`; árvores `decomp/src/`, `port/`, `libultraship/src/fast/`, `torch/`; metadados de branches/submódulos. Para PS2: `ps2sdk/README.md`, `gsKit/README.md`, `ps2toolchain/README.md`, scripts/workflows documentados nos relatórios PS2.
