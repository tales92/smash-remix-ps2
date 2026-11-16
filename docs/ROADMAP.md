# Roteiro de desenvolvimento

Este roteiro é preliminar. A auditoria técnica pode alterar a ordem, o escopo e a viabilidade de cada fase.

## Fase 0 — Preparação e auditoria

**Objetivo:** conhecer os projetos, fixar versões e entender a diferença entre o port nativo BattleShip-Remix e o pipeline ROM do +EXTRA.

Entregas:
- inventário de repositórios, branches, commits e submódulos;
- mapa do fluxo de build do BattleShip-Remix;
- mapa do pipeline +EXTRA;
- matriz de compatibilidade entre versões;
- inventário de dependências e licenças;
- identificação das ROMs/entradas necessárias.

Critério de saída: relatório baseado em caminhos, commits e comandos reais; nenhuma suposição de compatibilidade sem evidência.

## Fase 1 — Toolchain reproduzível na nuvem

**Objetivo:** preparar um runner GitHub Actions que instale ou obtenha uma PS2Toolchain/PS2SDK reproduzível.

Entregas:
- workflow manual (`workflow_dispatch`);
- cache de dependências quando apropriado;
- logs de versão e checksums das ferramentas;
- tempo máximo de execução;
- artefatos apenas de diagnóstico.

Critério de saída: runner instala/usa a toolchain e compila um exemplo mínimo, sem ROM do jogo.

## Fase 2 — ELF mínimo de PS2

**Objetivo:** gerar um executável ELF simples para PlayStation 2 em runner hospedado.

Entregas:
- projeto mínimo C/C++;
- Makefile ou CMake mínimo compatível com a toolchain;
- ELF como artefato privado de build;
- log completo de compilação.

Critério de saída: ELF gerado e validado estruturalmente; teste em emulador/console será registrado separadamente.

## Fase 3 — Prova de conceito gráfica e entrada

**Objetivo:** desenhar primitivas e texturas simples com gsKit e ler controle DualShock.

Critério de saída: demonstração mínima e reproduzível, com custos de memória documentados.

## Fase 4 — Áudio e filesystem

**Objetivo:** testar áudio, temporização e carregamento de arquivos no PS2, incluindo limites de memória.

Critério de saída: testes isolados para cada subsistema.

## Fase 5 — Base BattleShip-Remix

**Objetivo:** separar lógica reutilizável da camada específica de PC e criar backend PS2 sem destruir a arquitetura upstream.

Critério de saída: protótipo incremental; cada subsistema substituído deve ter testes próprios.

## Fase 6 — Integração +EXTRA

**Objetivo:** mapear e integrar, de maneira incremental, os conteúdos e alterações +EXTRA.

Ordem sugerida:
1. recursos/assets;
2. tabelas e identificadores;
3. personagens e movesets;
4. cenários e itens;
5. áudio;
6. rotinas de lógica/assembly equivalentes;
7. menus e funções adicionais.

Critério de saída: cada elemento integrado tem origem, versão e teste identificados.

## Fase 7 — Otimização e validação

**Objetivo:** reduzir uso de RAM/VRAM, tempo de carregamento e custo de renderização; validar em emulador e hardware real.

Critério de saída: medições reproduzíveis, lista de limitações e build identificada por commit.

## Regras para avançar

- Não pular a Fase 0.
- Não tentar compilar o jogo completo antes de provar a toolchain com um ELF mínimo.
- Não confundir uma ROM expandida funcional em emulador N64 com um port nativo para PS2.
- Não declarar suporte ao +EXTRA até validar a lógica, dados e recursos específicos.
