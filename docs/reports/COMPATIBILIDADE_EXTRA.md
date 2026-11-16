# Compatibilidade: BattleShip-Remix e Smash Remix +EXTRA

**Resultado:** a base de código Smash Remix 2.0.1 coincide no nível de commit entre o upstream usado pelo BattleShip (conforme documentação do projeto) e o submódulo do +EXTRA. Isso **não** demonstra que os conteúdos/patches adicionais do +EXTRA funcionem no port nativo. O +EXTRA constrói uma ROM e modifica fontes de assembly; BattleShip compila C/C++ nativo e extrai recursos para O2R. Não há importador que traduza automaticamente um fluxo no outro.

## Revisão de versão e proveniência

| Elemento | Evidência observada | Conclusão |
|---|---|---|
| Smash Remix declarado pela base BattleShip | `audit-sources/battleship-remix/README.md` e `docs/remix/build_and_roms.md` declaram Smash Remix 2.0.1; a documentação identifica `5e04fe7fcd023cd43c71f25f89bb6e810d254d55`, tag `2.0.1` | Versão da base nativa identificada pela documentação local. |
| Smash Remix usado pelo +EXTRA | `smashremix-plus-extra/.gitmodules` aponta `smashremix` para `JSsixtyfour/smashremix`, branch `master`; checkout local `5e04fe7fcd023cd43c71f25f89bb6e810d254d55`; tag `2.0.1` observada | O submódulo coincide com o commit descrito pelo BattleShip como fonte 2.0.1. |
| SSB64 usado pela base BattleShip | `battleship-remix/.gitmodules` aponta `decomp` a `lucasliet/ssb-decomp-re-remix`, branch `feature/remix-port`, commit local `3413abab6898eaa76ef7bd426665a495f00f1be7` | É um fork com alterações do port; não é simplesmente o clone upstream `VetriTheRetri/ssb-decomp-re`. |
| Clone de referência da decompilação | `ssb-decomp-re`, branch `main`, commit `572a747d8171137718fad472c5d7473dc7309ffd` | Commit upstream observado; ancestralidade em relação ao fork do BattleShip não foi verificada. |
| +EXTRA | `main`, commit `ee3d2f0f0e49aeb1dc598670a7f6f1f75c3e2a82`, `version.txt` informa `0.6.0` | Repositório/ferramentas de comunidade, não uma versão de Smash Remix nem um port nativo. |

Os SHAs são os das referências nos clones fornecidos; nenhum comando Git foi executado. `docs/remix/build_and_roms.md` da base registra também evidência de ROM 2.0.1 e que o código de Remix montado em `0x2C00000` corresponde ao ROM de referência com diferenças em palavras relacionadas a arquivos injetados. Essa ROM e seus bytes não foram procurados nem examinados nesta tarefa.

## Pipeline +EXTRA: ordem e efeitos observados

O pipeline foi traçado pela leitura de `character_appender.py`, dos módulos `smashremix_extra/` e dos scripts `build.bat`, `build.sh`, `patch_extra.bat` e `gen_xdelta.bat`. Não foi executado.

1. O construtor exige `smashremix/src/File.asm` e `smashremix/roms/ssb.rom`. Se `smashremix/roms/original.z64` não existir, tenta produzi-lo aplicando `smashremix/original.xdelta` com `smashremix/xdelta.exe`; depois extrai arquivos do ROM para diretórios de trabalho `scripts/`, acrescenta imagens em arquivos numerados e remove/recria `build/` (`shutil.rmtree`).
2. `main()` chama `copy_remix_src()`: apaga e copia `smashremix/src/` para `src/` e copia `smashremix/main.asm`.
3. `overwrite_files()` sobrepõe `smashremix_overwrite/` no `src/`.
4. `prepare_files()` processa `extra_characters/`, `extra_stages/` e `extra_imports/`, gerando conteúdo sob `build/extra_characters/`, `build/extra_stages/` e demais diretórios. Processadores criam binários, imagens, reqlists, dados de personagens/cenários, sons e instruções assembly.
5. `apply_bio_overflow_patches()` e `write_bio_overflow_asm()` tratam a expansão da tabela de bios.
6. `inject_files_in_rom()` lê `smashremix/roms/original.z64`, substitui os arquivos listados em `MODIFIED_FILES`, acrescenta os arquivos preparados e grava `smashremix/roms/original_extra.z64`. `smashremix_extra/injector/injector.py` atualiza a tabela interna, usa VPK0 em arquivos selecionados, recalcula CRC e padding; expansões de tabela são propagadas por valores `NALEnum`.
7. `edit_src_files()` altera fontes copiadas em `src/`: caminhos/ROM base, nome “REMIX EXTRA”, tamanho/origem ROM, `Boot.asm`, IDs de arquivos, contagem/tabelas de personagens, Character Select, fontes de dados e resultados, Stage/Item/FGM e instruções de áudio/personagem. O tamanho do ROM expandido determina endereços/limites.
8. `patch_extra.bat` chama `smashremix/assembler/bass.exe` com `main.asm`, produz `ssb64asm_extra.z64`, grava `output.log` e solicita `logfile.log` como mapa de símbolos; depois chama `rn64crc.exe`. O build wrapper no Windows também gera RDB/INI; `build.sh` usa Wine no Linux. `gen_xdelta.bat` cria um patch binário a partir do ROM resultante.

### Entradas e saídas esperadas

- Entrada vanilla: `smashremix/roms/ssb.rom` (README pede ROM USA; permite renomear `.z64` para `.rom`).
- Base de assembly: `smashremix/roms/original.z64`, reconstruída de `original.xdelta` se ausente.
- Outros inputs: submódulo Smash Remix; `extra_characters/`, `extra_stages/`, `extra_music/`, `extra_resources/`, `extra_imports/`, `extra_toggles/`, `templates/`, scripts e arquivos de configuração.
- Saídas intermediárias: `scripts/*.bin` alterados, `src/` e `main.asm` gerados/editados, `build/extra_characters/*`, `build/extra_stages/*`, imports binários e tabelas/reqlists.
- ROMs/patches finais: `smashremix/roms/original_extra.z64` (ROM intermediário injetado), `ssb64asm_extra.z64` (montagem final) e `smashremix_extra.xdelta` quando o script correspondente é usado. O wrapper também gera `rdb.txt` e `ini.txt`. Não são arquivos para este repositório.
- Efeitos locais potencialmente destrutivos: remoção/recriação de `build/`, substituição de `src/`, edição de tabelas/fontes e geração/escrita de ROMs. Scripts deliberadamente não executados.

## Classificação das alterações

| Classe | Arquivos/caminhos | Exemplos e natureza |
|---|---|---|
| Assets/dados | `extra_characters/*`, `extra_stages/*`, `extra_music/*`, `extra_resources/*`, `extra_imports/*`, `scripts/*.bin` | Modelos/arquivos de jogo, imagens, músicas/sons, backgrounds, ícones e arquivos comprimidos/injetados. Processadores de cenário (`smashremix_extra/stage/processor.py`) podem editar em cópias de build `stage.bin`, `header.bin` e `background.bin` (colisão, map objects, rebirth, MPGroundData e wallpaper). |
| Estruturas/tabelas/IDs | `smashremix_overwrite/Character.asm`, `File.asm`, `CharacterSelect.asm`, `Stages.asm`, `Item.asm`; `character/processor.py`, `stage/processor.py`, `file_manager.py`, `injector/injector.py` | `ADD_CHARACTERS`, IDs de arquivo/personagem/cenário/item/FGM, tabelas de roster, heaps do CSS, reqlists, file table ROM, offsets de recurso e patches de contagens. |
| Código assembly/execução | `smashremix_overwrite/*.asm`, `extra_characters/*/*.asm`, `extra_stages/*/hazards.asm`, `smashremix/src/*.asm`, `main.asm` | Rotinas MIPS, `OS.patch_start(file_offset, RAM address)`, `origin`, branches, hooks, mudanças a assembly existente e novos includes. Ex.: `smashremix_overwrite/Damage.asm`, `AI.asm`, `KirbyCopy.asm`; `extra_characters/Ken/KenSpecial.asm`. |
| Código/ferramenta de build | `character_appender.py`, `smashremix_extra/{character,stage,audio,injector}/`, `patch_extra.bat`, `build.bat`, `build.sh` | Python gera fontes/ROM expandida; bass monta e `rn64crc` corrige CRC. Não gera C/C++ nativo para o runtime BattleShip. |

Exemplos de endereço diretamente observáveis incluem `Character.asm` (`STRUCT_TABLE(0x92610)`, `ACTION_ARRAY_TABLE_ORIGINAL(0xA6F40)`), `Damage.asm` (`OS.patch_start(0x5F7C4, 0x800E3FC4)`) e `KirbyCopy.asm` (`OS.patch_start(0x62BE4, 0x800E73E4)`). Esses são offsets/RAM addresses do fluxo de ROM/assembly; não são automaticamente símbolos ou endereços do binário nativo.

`MODIFIED_FILES` declara substituições para IDs `0x000B`, `0x000C`, `0x0011`, `0x0014`, `0x0023`, `0x153E`, `0x0A05`, `0x0A06`, `0x10F5` e `0x10F6`. Arquivos novos dependem de IDs/offsets e reqlists calculados pelos processadores.

## O que coincide e o que permanece incompatível/não provado

**Coincide:** a base de assembly identificada pelo BattleShip e o submódulo Smash Remix do +EXTRA usam o mesmo commit `5e04fe7...` (tag declarada 2.0.1). Assim, o +EXTRA parte da mesma versão-base, pelo menos nos metadados locais consultados.

**Não equivale:** +EXTRA aplica conteúdos sobre a imagem ROM e altera assembly/tabelas dessa imagem. BattleShip usa código C/C++ nativo da decompilação, uma imagem RAM extraída/interpretada e recursos no archive `BattleShip-Remix.o2r`, com registries e tabelas gerados. O arquivo `original_extra.z64` não transforma automaticamente os patches assembly em C, nem atualiza `port/remix/remix_roster_data.h`, `gr/grremix/grremixids.h`, factories Torch ou `RelocFileTable`.

**Conflitos a resolver antes de qualquer integração:**

- **Fighter IDs/roster:** o BattleShip declara `kRemixRoster[97]` e documenta IDs Remix `0x00..0x60`. O +EXTRA injeta novos personagens e incrementa contagens/tabelas no assembly (`ADD_CHARACTERS`, `NUM_CHARACTERS`, Character Select, tamanho de heaps). Esses IDs extras e estruturas não estão demonstrados no registry C nativo.
- **Stage/item/file IDs:** +EXTRA gera constantes `File.*`, `Stages.id.*`, itens, imports, IDs de som e req-lists. BattleShip gera/consome IDs e arquivos através das tabelas de Remix e factories de extração; colisão, mapeamento e capacidade precisam ser comparados item a item.
- **Endereços e hooks:** `OS.patch_start`, `origin`, `Character.table_patch_start` referem-se à ROM de Smash Remix. BattleShip traduz os patches da base de Remix em funções/registries nativos. Não há tabela comprovada de correspondência para cada alteração +EXTRA e para as versões/forks da descompilação.
- **Dados binários:** os formatos comprimidos/tabelas internos e encadeamentos de recursos do ROM não equivalem automaticamente aos tipos de recursos e reloc files carregados pelo O2R.
- **Lógica executável:** movimentos, AI, hazards e toggles em assembly precisam de porta semântica para C ou de reimplementação. Reutilizar assets por extração é hipótese viável, não prova de runtime nem de paridade.
- **Licenças:** não foi localizada licença explícita na raiz do +EXTRA, Smash Remix ou ssb-decomp-re examinados. A licença MIT do BattleShip limita-se ao port e declara que não licencia o código da decompilação. Resolver direitos antes de copiar ou redistribuir fontes/dados.

## Próxima verificação necessária

Criar uma matriz por conteúdo (+EXTRA character/stage/item/music/toggle) com: ID de origem, arquivos/binários, tabelas tocadas, patches assembly/endereço, símbolo/função nativa equivalente, factory Torch/O2R, licença/proveniência e teste. Começar com um asset não executável isolado; só depois mapear estruturas/IDs e, por fim, lógica de jogo. Não importar a ROM expandida como solução de port.

## Material examinado

`smashremix-plus-extra/{README.md,.gitmodules,build.bat,build.sh,patch_extra.bat,gen_xdelta.bat,Pipfile,character_appender.py,version.txt,smashremix_extra/injector/injector.py,smashremix_extra/character/processor.py,smashremix_extra/stage/processor.py,smashremix_overwrite/{Character.asm,File.asm,Damage.asm,KirbyCopy.asm},extra_characters/Ken/{main.asm,KenSpecial.asm}}`; metadados do submódulo `smashremix`; e `battleship-remix/docs/remix/{architecture.md,build_and_roms.md,patch_audit.md}`.
