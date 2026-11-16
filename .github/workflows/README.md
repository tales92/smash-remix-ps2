# GitHub Actions

## Auditoria técnica com Copilot CLI

Workflow: `.github/workflows/copilot-auditoria-inicial.yml`

- Disparo manual: `Actions → Auditoria técnica com Copilot → Run workflow`.
- Usa runner hospedado `ubuntu-latest`.
- Instala Node.js 22 e `@github/copilot`.
- Clona as referências públicas em `audit-sources/` (diretório ignorado pelo Git).
- Executa o prompt `prompts/01-auditoria-inicial.md`.
- O Copilot recebe somente permissões de leitura e escrita de arquivos; shell é negado.
- O workflow valida os quatro relatórios e cria uma branch `ai/initial-technical-audit-<run-id>` com Pull Request para `main`.
- Não compila o jogo, não executa scripts dos repositórios clonados e não usa ROMs.

## Segredo necessário

Em `Settings → Secrets and variables → Actions`, deve existir o segredo:

`COPILOT_GITHUB_TOKEN`

Ele deve conter um fine-grained PAT da conta pessoal do usuário com a permissão Account `Copilot Requests: Read`. O token deve permanecer restrito e não pode ser exibido em logs.

O workflow verifica apenas se o segredo existe. Uma chamada bem-sucedida do Copilot CLI é que confirma a autenticação.

## Permissões do workflow

O workflow solicita `contents: write` e `pull-requests: write` para permitir que o runner publique uma branch e abra o Pull Request de auditoria. O token do Actions é usado somente na etapa final, separado do segredo do Copilot.

## Segurança

- Disparo manual, sem agendamento ou execução automática a cada push.
- Não há acesso às ROMs ou aos dados extraídos.
- As fontes externas são clonadas, mas seus scripts não são executados.
- O agente não recebe permissão de shell nem de Git push.
- O token do Actions não fica persistido pelo checkout; a autenticação do push é fornecida temporariamente à chamada Git.
- Os arquivos a serem versionados são explicitamente enumerados no workflow.
- Todos os resultados chegam em Pull Request, nunca diretamente em `main`.

## Limitações

O Copilot Free permite usar o CLI, mas a utilização de chat/agente é limitada. Uma auditoria extensa pode consumir a franquia disponível. Os minutos de GitHub Actions também estão sujeitos aos limites do plano e do tipo de runner.
