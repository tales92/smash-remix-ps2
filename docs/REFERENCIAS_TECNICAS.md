# Referências técnicas

Última conferência dos metadados e arquivos README: **15/11/2026**. Branches abaixo são as branches padrão ou a branch indicada no momento da consulta; confirme novamente antes de fixar dependências.

## 1. Projetos centrais

### BattleShip-Remix — base candidata do port nativo

- Repositório: https://github.com/lucasliet/BattleShip-remix
- Branch observada: `feature/remix-port`
- Papel: port nativo para PC e Nintendo Switch de **Smash Remix 2.0.1**, construído sobre a descompilação SSB64 e o projeto BattleShip.
- Informação verificada no README: o fork reescreve rotinas MIPS do Remix em C sobre a descompilação; usa libultraship para renderização/áudio/entrada e Torch para extração de recursos.
- Conteúdo declarado: elenco, cenários, modos, menus, bancos de áudio e alterações de IA do Remix 2.0.1.
- Entrada declarada para build de fonte: `baserom.remix.z64` (SHA-1 `13716381ed5e29be2606e0f4724b18fd00789d04`; MD5 `2b2d6b295106c54216b7fc7a2f14346e`). A ROM-base americana original é identificada no README como SHA-1 `e2929e10fccc0aa84e5776227e798abc07cedabf`.
- Limite conhecido: o README lista builds para PC, Switch, Android e PS5, mas não PS2. Portanto, não é um port PS2 pronto.
- Decisão: **primeira base a auditar**, ainda não aprovada como arquitetura final.

### Smash Remix +EXTRA — expansão pretendida

- Repositório: https://github.com/joaorb64/smashremix-plus-extra
- Branch observada: `main`
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
- Papel: descompilação de Super Smash Bros. 64; referência de código e estrutura original.
- O README documenta a ROM americana v1.0 com SHA-1 `e2929e10fccc0aa84e5776227e798abc07cedabf` e MD5 `f7c52568a31aadf26e14dc2b6416b2ed`.
- O fluxo documentado utiliza `make init`/extração e `make`, com entrada `baserom.us.z64`.
- Decisão: referência de engenharia reversa e rastreamento de funções; não confundir a geração da ROM original com a geração de um ELF nativo de PS2.

## 2. Fontes upstream e dependências da base candidata

| Projeto | Branch observada | Papel |
|---|---|---|
| [BattleShip](https://github.com/JRickey/BattleShip) | `main` | Projeto upstream do qual BattleShip-Remix é um fork comunitário. |
| [Smash Remix](https://github.com/JSsixtyfour/smashremix) | `master` | Código-fonte upstream do mod; também aparece como submódulo do +EXTRA. |
| [libultraship](https://github.com/Kenix3/libultraship) | `main` | Camada usada pelo BattleShip para renderização, áudio e entrada; compatibilidade com PS2 precisa ser investigada. |
| [Torch](https://github.com/HarbourMasters/Torch) | `main` | Ferramenta/biblioteca de extração de assets usada pela base BattleShip. Avaliar se precisa rodar apenas na etapa de preparação de assets ou também no console. |

Não adicione todos esses projetos como submódulos automaticamente. Primeiro determine quais são dependências de build, quais são apenas ferramentas de desenvolvimento e quais precisam de forks/adaptações.

## 3. Ferramentas e bibliotecas para PlayStation 2

| Projeto | Branch observada | Papel e observações |
|---|---|---|
| [PS2SDK](https://github.com/ps2dev/ps2sdk) | `master` | Bibliotecas de desenvolvimento PS2: acesso ao hardware, controles, memory card, USB, HDD e áudio via FreeSD, entre outros. |
| [gsKit](https://github.com/ps2dev/gsKit) | `master` | Interface C de baixo nível para o Graphics Synthesizer do PS2; depende de PS2SDK/PS2Toolchain. O README descreve o projeto como uma interface lean/low-level. |
| [PS2Toolchain](https://github.com/ps2dev/ps2toolchain) | `master` | Scripts para compilar e instalar os compiladores utilizados em homebrew PS2. Avaliar custo/tempo de instalação em runners hospedados. |

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
