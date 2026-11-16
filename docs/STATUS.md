# Estado do projeto

**Data:** 16/11/2026  
**Fase atual:** 1 — prova de conceito da toolchain PS2 e ELF mínimo.

## Concluído

- [x] Repositório privado criado.
- [x] Estrutura inicial, instruções do agente e catálogo de referências incorporados à `main` (PR #1).
- [x] Workflow manual de auditoria com Copilot CLI incorporado à `main` (PR #2).
- [x] Correção de configuração do modelo automático do Copilot CLI incorporada (PR #3).
- [x] Auditoria estática inicial de BattleShip-Remix, +EXTRA, dependências e infraestrutura PS2 concluída e incorporada (PR #4).
- [x] Copilot CLI autenticado e executando tarefas em GitHub Actions (execução #3).
- [x] Estratégia de armazenamento permanente de ROMs documentada: ativos de uma GitHub Release privada.

## Em andamento

- [x] Compilar e validar o ELF mínimo usando PS2DEV v2.0.0 em GitHub Actions (run #1).
- [x] Publicar ELF e log como artefatos do Actions (artifact `ps2-elf-smoke-test-1`).
- [ ] Executar o ELF em emulador ou hardware real (etapa posterior à compilação).

## Ainda pendente

- [ ] Criar a Release privada `private-rom-library` e enviar os arquivos de entrada confirmados.
- [ ] Implementar download autenticado de ativos de Release em workflows que realmente precisem de ROM.
- [ ] Demonstrar compatibilidade runtime de +EXTRA com o BattleShip nativo.
- [ ] Criar prova de conceito gráfica gsKit, inicialmente com uma primitiva estática.
- [ ] Desenvolver backend PS2 para renderer, áudio, controles, filesystem, scheduler e temporização.

## Estado técnico

A auditoria identificou que BattleShip-Remix contém lógica do Smash Remix 2.0.1 em C/C++, mas a infraestrutura de PC não é compatível diretamente com PS2. O caminho provável exige backend GS dedicado, preservando o máximo possível da lógica de jogo.

O +EXTRA utiliza o mesmo commit-base de Smash Remix 2.0.1 identificado no BattleShip, mas seu pipeline modifica ROM, tabelas e assembly. Isso não equivale a uma integração automática com o runtime nativo.

O teste mínimo de ELF foi compilado e validado no runner hospedado. O ELF tem cabeçalho ELF32, dados little-endian, Machine reportado como MIPS R3000 pelo `readelf`, flags que incluem `5900`, `abi2` e `mips3`, e entry point `0x100e40`. O compilador utilizado foi `mips64r5900el-ps2-elf-gcc`. Isso confirma a compilação/linkagem para o alvo EE, mas não comprova o boot em emulador ou hardware real. O teste é isolado, não utiliza ROM e não implementa gameplay.

## Resultado da primeira compilação PS2

- Workflow: [PS2 — ELF mínimo, run #1](https://github.com/tales92/smash-remix-ps2/actions/runs/43792633427).
- Resultado: sucesso.
- Imagem: `ghcr.io/ps2dev/ps2dev:v2.0.0`, digest AMD64 fixado no workflow.
- ELF: `ps2/hello/build/smash-remix-ps2-hello.elf`.
- SHA-256: `2845936c8050324c479374089bee229aca3e70ef89b7e96e5ede08428d06687f`.
- Artifact: `ps2-elf-smoke-test-1`, ID `14192518057`, retenção até 30/11/2026.
- A compilação e a validação do cabeçalho passaram. Ainda falta executar o ELF em emulador ou PS2 real.

## Histórico da automação Copilot

- Execução #1: falhou na autenticação. A existência do segredo não comprovava sua validade.
- Execução #2: token foi encontrado, mas a CLI parou porque o modelo `auto` não aceitava `--effort=high`.
- Execução #3: Copilot autenticou, clonou as referências e gerou os quatro relatórios. O workflow publicou a branch, mas a criação automática do PR foi bloqueada pela configuração do repositório.
- PR #4 foi aberto separadamente, revisado e incorporado à `main`.

## Limitações e riscos conhecidos

1. A camada gráfica Fast3D/RDP precisará ser traduzida para o Graphics Synthesizer do PS2.
2. +EXTRA expande/patcha ROM e modifica assembly/tabelas; falta mapear essas mudanças para a representação C/C++ nativa.
3. A toolchain precisa ser testada de verdade em runner hospedado; o workflow de ELF mínimo está preparado, mas ainda não há resultado de compilação registrado.
4. Os arquivos de entrada privados ainda não foram armazenados na Release nem disponibilizados a runners.
5. O custo de Actions depende do plano e saldo; não se pode garantir que builds maiores caibam no uso gratuito sem medição.

Atualize este arquivo em cada Pull Request relevante. Nunca marque uma etapa como concluída sem evidência.
