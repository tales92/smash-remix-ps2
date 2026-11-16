# Estado do projeto

**Data:** 16/11/2026  
**Fase atual:** 0 — preparação do repositório e auditoria técnica.

## Concluído

- [x] Repositório privado criado.
- [x] README inicial adicionado.
- [x] Repositórios centrais e dependências principais identificados.
- [x] Branches padrão/referência consultadas no GitHub.
- [ ] Estrutura de documentação e instruções do agente — neste PR.
- [x] Auditoria estática inicial de BattleShip-Remix, +EXTRA, dependências e infraestrutura PS2 documentada em `docs/reports/`.
- [x] Commits/branches locais observados e submódulos efetivamente usados pelo BattleShip identificados.
- [ ] Estratégia de armazenamento/transferência de ROMs para runners.
- [ ] Autenticação Copilot CLI em GitHub Actions validada.
- [ ] Compatibilidade runtime de +EXTRA com o BattleShip nativo demonstrada.
- [ ] Toolchain PS2 reproduzível e ELF mínimo verificados no GitHub Actions.

## Estado técnico

Ainda não há código de port PS2, backend gráfico PS2, ELF, ROM ou build deste projeto. A base Smash Remix 2.0.1 observada no BattleShip e o submódulo do +EXTRA coincidem no commit de fonte, mas as extensões +EXTRA ainda não foram mapeadas para as estruturas e lógica C/C++ do port. PS2 requer backend de GS dedicado e validação por subsistema.

Esta auditoria foi estática: não foram executados scripts dos projetos, builds ou testes com ROM. Os metadados Git foram lidos dos arquivos presentes nos clones; worktrees limpos e inventário remoto completo de tags não foram confirmados. O Copilot CLI não recebeu shell, portanto sua tentativa de validar YAML com Python foi bloqueada. A execução #3 gerou os relatórios e o workflow publicou a branch; a abertura automática do PR foi bloqueada pela configuração do repositório que impede Actions de criar Pull Requests. O PR #4 foi aberto separadamente para revisão.

## Bloqueios e riscos conhecidos

1. Diferenças de arquitetura entre o runtime do BattleShip-Remix e o hardware PS2.
2. +EXTRA expande/patcha ROM e modifica assembly/tabelas; falta mapear suas mudanças executáveis e IDs para o runtime nativo.
3. A cadeia de build PS2 precisa ser fixada e medida em runner hospedado, incluindo o bootstrap frio do toolchain.
4. Os arquivos de entrada privados ainda não têm um mecanismo aprovado de transferência ao runner.
5. O backend gráfico PS2, áudio, controle, filesystem e scheduler não foram implementados ou validados.
6. O custo de Actions depende do plano/saldo; não se pode garantir que o fluxo completo caiba no nível gratuito sem medição.

Atualize este arquivo em cada Pull Request relevante. Nunca marque uma etapa como concluída sem evidência.

## Primeira execução do Copilot CLI

- Run: [#1 — 43786813182](https://github.com/tales92/smash-remix-ps2/actions/runs/43786813182).
- O runner iniciou corretamente; encontrou o segredo, instalou Copilot CLI e clonou todas as referências configuradas.
- A etapa do Copilot CLI terminou com `Authentication failed`. A mensagem do GitHub indica token inválido/expirado/revogado ou sem a permissão `Copilot Requests`.
- A mera existência do segredo não comprova a validade do token. É necessário revisar ou substituir o fine-grained PAT e executar novamente.
- Nenhum relatório foi gerado e nenhuma branch/PR de auditoria foi criada nesta execução.
