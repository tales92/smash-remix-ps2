# Referências técnicas

Última auditoria das cópias em `audit-sources/`: **16/11/2026**. Os branches/SHAs são referências locais observadas nos clones, não uma consulta remota atualizada. Nenhum comando Git foi executado; worktrees limpos e inventário remoto completo de tags não foram confirmados. Consulte `docs/reports/AUDITORIA_INICIAL.md` para evidências, submódulos e limitações.

## 1. Projetos centrais

### BattleShip-Remix — base candidata do port nativo

- Repositório: https://github.com/lucasliet/BattleShip-remix
- Branch observada: `feature/remix-port`
- Commit observado: `93db0f7e7067cc80c36f8314effe2d243e848162`.
- Submódulos efetivamente usados: `lucasliet/ssb-decomp-re-remix` `feature/remix-port` (`3413abab6898eaa76ef7bd426665a495f00f1be7`), `lucasliet/libultraship-remix` `feature/remix-port` (`29f3df755b38506aa366add97b70b3089536be57`) e `lucasliet/Torch-remix` `feature/remix-port` (`80535fb29c98353869e118b49ae7715cb5fa2fe2`). São forks e não os clones upstream listados abaixo.
- Papel: port nativo para PC e Nintendo Switch de **Smash Remix 2.0.1**, construído sobre a descompilação SSB64 e o projeto BattleShip.
- Informação verificada no README: o fork reescreve rotinas MIPS do Remix em C sobre a descompilação; usa libultraship para renderização/áudio/entrada e Torch para extração de recursos.
- Conteúdo declarado: elenco, cenários, modos, menus, bancos de áudio e alterações de IA do Remix 2.0.1.
- Entrada declarada para build de fonte: `baserom.remix.z64` (SHA-1 `13716381ed5e29be2606e0f4724b18fd00789d04`; MD5 `2b2d6b295106c54216b7fc7a2f14346e`). A ROM-base americana original é identificada no README como SHA-1 `e2929e10fccc0aa84e5776227e798abc07cedabf`.
- Limite conhecido: o README lista builds para PC, Switch, Android e PS5, mas não PS2. Portanto, não é um port PS2 pronto.
- Decisão: **primeira base a auditar**, ainda não aprovada como arquitetura final.

### Smash Remix +EXTRA — expansão pretendida

- Repositório: https://github.com/joaorb64/smashremix-plus-extra
- Branch observada: `main`
- Commit observado: `ee3d2f0f0e49aeb1dc598670a7f6f1f75c3e2a82`; `version.txt` informa `0.6.0`.
- O submódulo `smashremix` está em `master`, commit `5e04fe7fcd023cd43c71f25f89bb6e810d254d55`, identificado como Smash Remix 2.0.1.
- Papel: sistema comunitário de conteúdo adicional para Smash Remix; não é uma atualização oficial do projeto Smash Remix.
- O README orienta clone recursivo, pois o código-fonte de Smash Remix fica em um submódulo chamado `smashremix`.
- Pré-requisitos documentados: Python 3.12+, Pipenv; no Linux, Wine também é indicado para executar `patch_extra.bat`.
- Pipeline descrita: `character_appender.py` gera `smashremix/roms/original_extra.z64` e prepara diretórios `src/` e `build/`; depois `patch_extra.bat` conclui a geração da ROM.
- O README pede uma ROM vanilla de SSB64 em `smashremix/roms/ssb.rom`.
- Limite conhecido: esse fluxo expande/patcha a ROM e não demonstra, por si só, que as mudanças estejam convertidas para a arquitetura C/C++ do BattleShip.
- Decisão: **segunda base a auditar**; mapear quais mudanças são dados/assets, assembly, tabelas ou lógica executável.

### SSB64 Decompilation

- Repositório: https://github.com/VetriTheRetri/ssb-decomp-re
- Branch observada: `main`
- Commit observado no clone upstream: `572a747d8171137718fad472c5d7473dc7309ffd`. O BattleShip efetivamente usa um fork separado no SHA listado acima.
- Papel: descompilação de Super Smash Bros. 64; referência de código e estrutura original.
- O README documenta a ROM americana v1.0 com SHA-1 `e2929e10fccc0aa84e5776227e798abc07cedabf` e MD5 `f7c52568a31aadf26e14dc2b6416b2ed`.
- O fluxo documentado utiliza `make init`/extração e `make`, com entrada `baserom.us.z64`.
- Decisão: referência de engenharia reversa e rastreamento de funções; não confundir a geração da ROM original com a geração de um ELF nativo de PS2.

## 2. Fontes upstream e dependências da base candidata

| Projeto | Branch observada | Papel |
|---|---|---|
| [BattleShip](https://github.com/JRickey/BattleShip) | `main` | Projeto upstream do qual BattleShip-Remix é um fork comunitário. |
| [Smash Remix](https://github.com/JSsixtyfour/smashremix) | `master`, commit `5e04fe7fcd023cd43c71f25f89bb6e810d254d55`, tag `2.0.1` observada | Código-fonte upstream do mod; também aparece como submódulo do +EXTRA e é a versão de fonte documentada pelo BattleShip. |
| [libultraship](https://github.com/Kenix3/libultraship) | `main` | Upstream de libultraship; a base BattleShip usa o fork `lucasliet/libultraship-remix`. |
| [Torch](https://github.com/HarbourMasters/Torch) | `main` | Upstream de Torch; a base BattleShip usa o fork `lucasliet/Torch-remix`. |
| [BattleShip upstream](https://github.com/JRickey/BattleShip) | `main`, commit `241a3ae154f249469d8707432ec79f51f997cf54` | Projeto upstream do fork BattleShip-Remix. |
| [libultraship upstream](https://github.com/Kenix3/libultraship) | `main`, commit `759b13401cd6903daa67a00a753066f791622e78` | O submódulo do BattleShip é o fork `lucasliet/libultraship-remix`, commit `29f3df755b38506aa366add97b70b3089536be57`. |
| [Torch upstream](https://github.com/HarbourMasters/Torch) | `main`, commit `22d32182e146716309249a8f668931ad47b5ce0c` | O submódulo do BattleShip é o fork `lucasliet/Torch-remix`, commit `80535fb29c98353869e118b49ae7715cb5fa2fe2`. |

Não adicione todos esses projetos como submódulos automaticamente. Primeiro determine quais são dependências de build, quais são apenas ferramentas de desenvolvimento e quais precisam de forks/adaptações.

## 3. Ferramentas e bibliotecas para PlayStation 2

| Projeto | Branch observada | Papel e observações |
|---|---|---|
| [PS2SDK](https://github.com/ps2dev/ps2sdk) | `master`, commit `b04e8973c8a9839d0653d87a8d30cc67fd3c3f05` | Bibliotecas de desenvolvimento PS2: acesso ao hardware, controles, memory card, USB, HDD e áudio via FreeSD, entre outros. |
| [gsKit](https://github.com/ps2dev/gsKit) | `master`, commit `8ef73d05f022d14b3ded7c70cba670fe333ed61a`, tag `v1.5.1` observada | Interface C de baixo nível para o Graphics Synthesizer do PS2; depende de PS2SDK/PS2Toolchain. |
| [PS2Toolchain](https://github.com/ps2dev/ps2toolchain) | `master`, commit `41610ee0bc15f887c65ea208bcb9234a3b812005` | Scripts para compilar e instalar os compiladores utilizados em homebrew PS2. Avaliar custo/tempo de instalação em runners hospedados. |

## 4. Perguntas que a auditoria precisa responder

1. Qual commit exato de BattleShip-Remix deve ser fixado como ponto de partida?
2. Quais versões/commits de Smash Remix e +EXTRA são compatíveis?
3. O que o BattleShip converteu de MIPS para C e o que continua dependendo da ROM/extração?
4. Como mapear os recursos adicionais do +EXTRA para as estruturas do port nativo?
5. Quais dependências de libultraship são impossíveis ou caras de portar para PS2?
6. É viável criar um backend PS2 dedicado sem reescrever a lógica do jogo?
7. Quais partes do pipeline podem rodar no GitHub Actions e quais exigem arquivos de entrada privados?
8. Qual é o fluxo reproduzível mais simples para gerar primeiro um ELF mínimo de PS2?

## 5. Política de fixação de versões

Não use automaticamente `main` ou `master` como dependência de produção. Na auditoria, registre para cada projeto:

- URL;
- branch/tag;
- commit SHA;
- data de consulta;
- submódulos e seus SHAs;
- licença/avisos;
- comandos de build;
- dependências externas;
- motivo de uso no projeto.

Depois da auditoria, prefira commits ou tags fixos e atualizações deliberadas por PR.
