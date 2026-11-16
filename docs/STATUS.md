# Estado do projeto

**Data:** 15/11/2026  
**Fase atual:** 0 — preparação do repositório e auditoria técnica.

## Concluído

- [x] Repositório privado criado.
- [x] README inicial adicionado.
- [x] Repositórios centrais e dependências principais identificados.
- [x] Branches padrão/referência consultadas no GitHub.
- [ ] Estrutura de documentação e instruções do agente — neste PR.
- [ ] Estratégia de armazenamento/transferência de ROMs para runners.
- [ ] Autenticação Copilot CLI em GitHub Actions validada.
- [ ] Auditoria detalhada BattleShip-Remix / +EXTRA.
- [ ] Toolchain PS2 reproduzível no GitHub Actions.

## Estado técnico

Ainda não há código de port PS2, backend gráfico PS2, ELF, ROM ou build deste projeto. O BattleShip-Remix é uma base candidata; compatibilidade direta com +EXTRA não foi demonstrada.

## Bloqueios e riscos conhecidos

1. Diferenças de arquitetura entre o runtime do BattleShip-Remix e o hardware PS2.
2. O +EXTRA expande/patcha uma ROM; ainda é preciso mapear como transportar suas modificações para a representação nativa do BattleShip.
3. A cadeia de build de PS2 precisa ser reproduzida em runner hospedado.
4. Os arquivos de entrada privados ainda não têm um mecanismo aprovado de transferência ao runner.
5. Os limites gratuitos do GitHub Actions e do Copilot precisam ser monitorados.

Atualize este arquivo em cada Pull Request relevante. Nunca marque uma etapa como concluída sem evidência.

## Primeira execução do Copilot CLI

- Run: [#1 — 43786813182](https://github.com/tales92/smash-remix-ps2/actions/runs/43786813182).
- O runner iniciou corretamente; encontrou o segredo, instalou Copilot CLI e clonou todas as referências configuradas.
- A etapa do Copilot CLI terminou com `Authentication failed`. A mensagem do GitHub indica token inválido/expirado/revogado ou sem a permissão `Copilot Requests`.
- A mera existência do segredo não comprova a validade do token. É necessário revisar ou substituir o fine-grained PAT e executar novamente.
- Nenhum relatório foi gerado e nenhuma branch/PR de auditoria foi criada nesta execução.
