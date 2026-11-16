# GitHub Actions

## 1. Auditoria técnica com Copilot CLI

Workflow: `.github/workflows/copilot-auditoria-inicial.yml`

- Disparo manual: `Actions → Auditoria técnica com Copilot → Run workflow`.
- Usa runner hospedado `ubuntu-latest`.
- Instala Node.js 22 e `@github/copilot`.
- Clona as referências públicas em `audit-sources/` (diretório ignorado pelo Git).
- Executa o prompt `prompts/01-auditoria-inicial.md`.
- O Copilot recebe somente permissões de leitura e escrita de arquivos; shell é negado.
- Valida quatro relatórios e cria branch/PR para revisão.
- Não compila o jogo, não executa scripts dos repositórios clonados e não usa ROMs.

Segredo necessário: `COPILOT_GITHUB_TOKEN`, Fine-grained PAT da conta pessoal com Account permission `Copilot Requests: Write`.

Para permitir criação automática de PR, ativar `Settings → Actions → General → Workflow permissions → Allow GitHub Actions to create and approve pull requests`.

## 2. Prova de conceito: ELF mínimo de PS2

Workflow: `.github/workflows/ps2-elf-smoke-test.yml`

- Disparo manual: `Actions → PS2 — ELF mínimo → Run workflow`.
- Usa runner hospedado `ubuntu-latest` com Docker.
- Usa a imagem oficial `ghcr.io/ps2dev/ps2dev:v2.0.0`, fixada pelo digest Linux AMD64 registrado no workflow.
- Compila `ps2/hello/main.c` com as regras oficiais do PS2SDK.
- Valida o cabeçalho ELF com `mips64r5900el-ps2-elf-readelf` e calcula SHA-256.
- Publica o ELF e o log como artefatos do Actions por 14 dias.

Esta prova não precisa de Copilot, ROM, assets ou segredos. Ela valida compilação/ligação, não o boot no emulador/console.

## Permissões

- O workflow do Copilot usa `contents: write` e `pull-requests: write` para criar branch e PR; o `GITHUB_TOKEN` precisa estar autorizado a abrir PR.
- O workflow de ELF usa somente `contents: read`; não modifica o repositório.
- Nenhum dos dois workflows acessa ROMs.

## Segurança e armazenamento

- ROMs e recursos protegidos não devem ser adicionados ao Git, logs, caches ou artefatos.
- A estratégia de armazenamento permanente é uma GitHub Release privada, documentada em `docs/ARMAZENAMENTO_PRIVADO.md`.
- Nenhuma Release foi criada nem arquivo enviado ainda.
- O download dos ativos deverá ser implementado somente quando uma etapa realmente precisar de ROM.
