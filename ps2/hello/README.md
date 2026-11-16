# Teste mínimo de ELF para PlayStation 2

Este projeto mínimo valida se o ambiente oficial PS2DEV/PS2SDK consegue compilar e ligar um programa para o Emotion Engine (EE) dentro do GitHub Actions.

## O que faz

- Usa o compilador EE `mips64r5900el-ps2-elf-gcc`.
- Usa as regras oficiais de build do PS2SDK em `$(PS2SDK)/samples/Makefile.pref` e `Makefile.eeglobal`.
- Gera `build/smash-remix-ps2-hello.elf`.
- Não usa ROM, assets, código do BattleShip ou arquivos do +EXTRA.

O código de teste é baseado no exemplo `ee/debug/samples/helloworld` do PS2SDK.

## Compilar na nuvem

Abra **Actions → PS2 — ELF mínimo → Run workflow**.

O workflow usa a imagem oficial `ghcr.io/ps2dev/ps2dev:v2.0.0`, fixada por digest para Linux AMD64. A imagem traz PS2Toolchain, PS2SDK, gsKit e ferramentas auxiliares; o workflow instala somente o utilitário host `make`, se necessário.

## Critério de sucesso

A etapa será considerada compilada quando:
1. o executável ELF for criado e não estiver vazio;
2. `mips64r5900el-ps2-elf-readelf -h` identificar o cabeçalho ELF e a arquitetura MIPS;
3. o SHA-256 do executável for registrado;
4. o ELF e o log forem disponibilizados como artefatos do Actions.

**Importante:** compilar e validar o cabeçalho ELF não prova que ele foi executado em emulador ou console. O teste de boot é uma etapa separada.
