# GitHub Actions

Os workflows serão adicionados após a auditoria da autenticação do agente e da toolchain PS2.

## Requisitos para futuros workflows

- Preferir `workflow_dispatch` no início, evitando execuções acidentais a cada push.
- Usar GitHub-hosted runners e registrar versões de ferramentas.
- Permissões mínimas no bloco `permissions`.
- Usar `GITHUB_TOKEN` para operações do próprio repositório quando possível.
- Ler o segredo de autenticação do Copilot apenas no step que precisa dele; nunca imprimir o valor.
- Fixar versões de actions e dependências quando viável.
- Definir timeout para jobs e steps longos.
- Não disponibilizar ROMs, assets protegidos ou segredos como artifacts.
- Publicar logs sanitizados e artefatos de build não protegidos.
- Não fazer push na branch principal; abrir Pull Request.

O segredo `COPILOT_GITHUB_TOKEN` ainda precisa ser validado em uma chamada mínima do Copilot CLI antes de configurar um agente que modifique arquivos.
