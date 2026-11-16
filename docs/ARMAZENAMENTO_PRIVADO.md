# Armazenamento permanente e privado das ROMs

## Decisão proposta

Manter os arquivos de entrada em **ativos de uma GitHub Release privada deste mesmo repositório**, em vez de adicioná-los ao histórico Git, armazená-los em GitHub Secrets ou publicá-los como artefatos temporários do Actions.

Como o repositório `tales92/smash-remix-ps2` é privado, os ativos da Release seguem o controle de acesso do repositório: pessoas sem acesso de leitura ao repositório não devem conseguir visualizar/baixar esses ativos. Pessoas com acesso de leitura podem visualizar as Releases; somente pessoas com permissão de escrita podem gerenciá-las.

Documentação oficial: https://docs.github.com/en/repositories/releasing-projects-on-github/about-releases

## Limites documentados pelo GitHub

Na data desta revisão, a documentação oficial informa:
- até 1.000 ativos por Release;
- cada arquivo individual deve ter menos de 2 GiB;
- não há limite para o tamanho total dos ativos de uma Release;
- não há limite de largura de banda para ativos de Release.

Esses limites tornam Releases adequadas para manter arquivos grandes enquanto o repositório e a Release existirem. Ainda assim, os limites e políticas devem ser revistos periodicamente.

## Organização proposta

Criar uma Release privada dedicada, sem associá-la a uma versão de software distribuída, por exemplo:

- Tag: `private-rom-library`
- Título: `Biblioteca privada de ROMs — entradas de desenvolvimento`
- Tipo: Release normal ou draft, conforme a facilidade desejada para administrar os uploads.

Ativos sugeridos, após verificar hashes e nomes:
- `baserom.remix.z64` — Smash Remix 2.0.1, se necessário para BattleShip-Remix.
- `baserom.us.z64` — ROM americana original, se a auditoria confirmar necessidade.
- `ssb.rom` — nome esperado pelo pipeline +EXTRA, se necessário.

Não armazene cópias redundantes sem necessidade. Prefira uma única cópia por arquivo e documente SHA-256, tamanho, origem lógica e quais etapas o consomem.

## Como o GitHub Actions consumirá os arquivos

O workflow futuro deverá:
1. receber um token de execução restrito ao próprio repositório, fornecido pelo `GITHUB_TOKEN`;
2. baixar somente os ativos da Release privada designada;
3. validar nomes, tamanho e SHA-256 antes de usar;
4. copiar os dados para o diretório temporário exigido pelo projeto upstream;
5. nunca incluir esses arquivos no Git, nos logs ou nos artefatos de saída;
6. remover cópias temporárias ao terminar o job.

O download autenticado pode ser automatizado com GitHub CLI ou com a API de Releases. O token usado para baixar os ativos é o `GITHUB_TOKEN` do Actions, separado do `COPILOT_GITHUB_TOKEN`, que serve apenas para autenticar o Copilot CLI.

## Upload manual inicial

Depois de decidir quais ROMs são realmente necessárias:
1. Abra a aba **Releases** do repositório privado.
2. Crie a Release com a tag `private-rom-library`.
3. Anexe cada arquivo como Release asset.
4. Publique a Release ou mantenha-a como draft, conforme o comportamento de download que for validado.
5. Registre os SHA-256 e tamanhos em `docs/ROMAS_E_DADOS.md`, sem adicionar os arquivos binários ao Git.

**Não use a pasta `assets/roms/` para fazer commit ou upload normal de arquivos.** Ela permanece como diretório de referência no código. A biblioteca permanente fica nos ativos da Release.

## O que ainda não foi feito

- Nenhuma Release de armazenamento foi criada.
- Nenhum arquivo foi enviado.
- Ainda não foi verificado o tamanho real das ROMs que serão armazenadas.
- O workflow atual de auditoria não baixa ROMs e não deve fazê-lo.
- Um workflow separado deverá ser implementado depois de a auditoria técnica confirmar os arquivos exatos necessários.
