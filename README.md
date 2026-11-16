# Super Smash Bros. Remix para PlayStation 2

Projeto privado de pesquisa e desenvolvimento de um port nativo de **Super Smash Bros. 64**, baseado no **Smash Remix** e com objetivo de investigar a integração do conteúdo de **Smash Remix +EXTRA** no PlayStation 2.

O trabalho parte de projetos existentes de descompilação, portabilidade e desenvolvimento homebrew. A prioridade é reaproveitar código verificável, documentar incompatibilidades e construir uma base reproduzível de compilação em nuvem.

## Situação atual

> **Fase: preparação do repositório e auditoria técnica.** Ainda não existe um port para PS2 neste repositório.

## Direção técnica inicial

- **Base candidata de portabilidade:** [BattleShip-Remix](https://github.com/lucasliet/BattleShip-remix), que declara portar Smash Remix 2.0.1 para plataformas nativas.
- **Expansão pretendida:** [Smash Remix +EXTRA](https://github.com/joaorb64/smashremix-plus-extra), cujo sistema de construção estende a ROM de Smash Remix.
- **Referência de descompilação:** [ssb-decomp-re](https://github.com/VetriTheRetri/ssb-decomp-re).
- **Alvo:** PlayStation 2, usando [PS2SDK](https://github.com/ps2dev/ps2sdk), [gsKit](https://github.com/ps2dev/gsKit) e [PS2Toolchain](https://github.com/ps2dev/ps2toolchain), sujeitos à validação técnica.

Consulte [docs/REFERENCIAS_TECNICAS.md](docs/REFERENCIAS_TECNICAS.md) para a lista de repositórios, branches e funções conhecidas. A primeira prova de compilação para PS2 fica em [ps2/hello/](ps2/hello/), executada pelo workflow manual [PS2 — ELF mínimo](.github/workflows/ps2-elf-smoke-test.yml).

## Estrutura

- `AGENTS.md`: instruções permanentes para agentes de programação.
- `docs/`: auditoria, referências, dados de entrada e roteiro.
- `assets/roms/`: diretório de referência; ROMs permanentes serão armazenadas como ativos de uma Release privada, conforme [docs/ARMAZENAMENTO_PRIVADO.md](docs/ARMAZENAMENTO_PRIVADO.md).
- `assets/extracted/`: espaço reservado para dados extraídos localmente ou durante a preparação de builds.
- `prompts/`: prompts controlados para tarefas do agente.
- `config/repositories.yml`: catálogo legível por máquina dos repositórios de referência.
- `.github/workflows/`: documentação e, futuramente, automações de compilação em nuvem.

## Importante

- Não coloque tokens, senhas ou credenciais em arquivos do repositório.
- Não envie ROMs, dumps ou recursos de jogos em commits até que o método de armazenamento e transferência para o GitHub Actions esteja definido.
- Não presuma que o +EXTRA é diretamente compatível com o código C/C++ do BattleShip-Remix.
- Não altere a branch principal durante auditorias ou experimentos do agente. Use branches e Pull Requests.
- Nenhuma compilação ou compatibilidade será considerada confirmada sem logs e evidências reproduzíveis.

Leia [AGENTS.md](AGENTS.md) antes de executar tarefas de desenvolvimento.
