# Prompt 01 — Auditoria técnica inicial

Copie este prompt para o agente depois que a autenticação e a execução em nuvem estiverem configuradas.

---

Você é um engenheiro sênior de software especializado em C/C++, engenharia reversa, descompilação, portabilidade de jogos e desenvolvimento homebrew para PlayStation 2.

Leia primeiro `AGENTS.md`, `README.md`, `docs/REFERENCIAS_TECNICAS.md`, `docs/ROMAS_E_DADOS.md` e `docs/ROADMAP.md`.

## Objetivo

Preparar uma auditoria técnica para desenvolver um port nativo de Super Smash Bros. 64 baseado no Smash Remix, com investigação de integração de Smash Remix +EXTRA no PlayStation 2.

O usuário exige desenvolvimento e compilação em nuvem por GitHub Actions. Não dependa de execução local no notebook do usuário.

## Repositórios

- BattleShip-Remix: https://github.com/lucasliet/BattleShip-remix
- Smash Remix +EXTRA: https://github.com/joaorb64/smashremix-plus-extra
- SSB64 decompilation: https://github.com/VetriTheRetri/ssb-decomp-re
- Smash Remix upstream: https://github.com/JSsixtyfour/smashremix
- PS2SDK: https://github.com/ps2dev/ps2sdk
- gsKit: https://github.com/ps2dev/gsKit
- PS2Toolchain: https://github.com/ps2dev/ps2toolchain

Consulte também as dependências citadas no catálogo de referências.

## Escopo desta primeira tarefa

**Somente auditoria e documentação. Não implemente gameplay, renderer ou backend PS2 nesta tarefa.**

### A. Auditar BattleShip-Remix

- Confirme branch, commit, tags e submódulos.
- Analise árvore de pastas, CMake, scripts de build e dependências.
- Separe lógica do jogo, código convertido da assembly MIPS, renderer, áudio, input, filesystem, threading, temporização e camada de plataforma.
- Identifique chamadas de sistema e APIs específicas de PC.
- Registre caminhos de arquivos e símbolos concretos.
- Identifique quais partes poderiam ser reutilizadas sem alteração e quais exigiriam adaptação.

### B. Auditar Smash Remix +EXTRA

- Confirme branch, commit e SHA do submódulo Smash Remix.
- Inspecione `character_appender.py`, scripts, templates, tabelas, assembly e pipeline de patch.
- Trace quais arquivos são alterados e em qual ordem.
- Classifique as alterações como assets/dados, estruturas/tabelas, código assembly ou lógica executável.
- Identifique pré-requisitos, nomes de ROM esperados e artefatos gerados.
- Não execute scripts antes de inspecionar seu conteúdo e efeitos.

### C. Compatibilidade

Compare as versões exatas e commits:
- Smash Remix usado na base BattleShip;
- Smash Remix submódulo usado pelo +EXTRA;
- descompilação SSB64 usada pelo BattleShip;
- alterações aplicadas pelo +EXTRA.

Não conclua que existe compatibilidade apenas porque ambos partem do Smash Remix. Identifique conflitos de endereços, tabelas, símbolos, estruturas, IDs e rotinas.

### D. PlayStation 2

Inspecione PS2SDK, gsKit e PS2Toolchain. Avalie:
- renderer e Graphics Synthesizer;
- Fast3D/modelos/texturas;
- DMA, Emotion Engine, VU e memória;
- áudio e SPU2;
- controle;
- filesystem e armazenamento;
- temporização e threads.

Diga se é realista adaptar libultraship ou se é necessário um backend PS2 dedicado. Justifique com dependências e APIs concretas.

### E. Compilação em nuvem

Investigue um ambiente Ubuntu hospedado no GitHub Actions:
- instalação ou imagem reproduzível da PS2Toolchain;
- tempo de instalação e dependências;
- possibilidade de cache;
- build de um ELF mínimo sem ROM;
- armazenamento de logs e artefatos;
- limites relevantes do runner gratuito.

Não configure segredos nem baixe ROMs nesta fase.

## Entregáveis

Crie em uma branch própria:
- `docs/reports/AUDITORIA_INICIAL.md`
- `docs/reports/COMPATIBILIDADE_EXTRA.md`
- `docs/reports/ARQUITETURA_PS2.md`
- `docs/reports/PLANO_DE_BUILD_NA_NUVEM.md`

Atualize `docs/STATUS.md` e, se necessário, corrija `docs/REFERENCIAS_TECNICAS.md` e `config/repositories.yml`.

Abra Pull Request para revisão. Não faça merge.

## Regras de evidência

- Registre URL, branch, commit SHA e caminhos examinados.
- Separe fatos confirmados, hipóteses e perguntas abertas.
- Não invente métricas de desempenho nem diga que compilou algo que não foi compilado.
- Não adicione ROMs, assets protegidos, credenciais ou arquivos binários ao repositório.
- Se alguma informação não estiver disponível, explique a limitação em vez de adivinhar.

## Perguntas que a conclusão deve responder

1. O que pode ser reaproveitado imediatamente?
2. O que precisa ser reescrito para PS2?
3. Como o +EXTRA se relaciona com o código nativo do BattleShip?
4. Qual é o menor primeiro marco técnico?
5. Quais são os maiores riscos?
6. Todo o fluxo de desenvolvimento e compilação pode ser executado nos GitHub-hosted runners gratuitos, dentro dos limites conhecidos?
