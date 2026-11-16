# Plano para builds reproduzíveis no GitHub Actions

**Conclusão:** a toolchain PS2 pode ser instalada ou obtida em runner Ubuntu e um ELF mínimo não requer ROM. A cópia examinada do PS2Toolchain compila toolchains DVP, IOP e EE a partir do código-fonte, com branches `main` como padrão e instalação “latest” quando a execução não está associada a uma tag. Não há medida de duração nem execução de build nesta auditoria; o custo/free-tier não pode ser garantido sem conhecer o plano e a cota disponível do repositório.

## Evidência estática do ambiente

- `audit-sources/ps2toolchain/README.md`: requisitos host incluem gcc/clang, make, patch, texinfo, flex, bison, GSL, GMP, MPFR, MPC, gettext e permissões no diretório de instalação.
- `toolchain.sh` enumera `depends/*.sh` e executa scripts ordenados de `scripts/`; por padrão, os componentes DVP, IOP e EE são compilados/instalados em sequência.
- `config/ps2toolchain-config.sh` usa repositórios `ps2toolchain-dvp`, `ps2toolchain-iop` e `ps2toolchain-ee`, branch `main`. Os scripts `scripts/001-dvp.sh`, `002-iop.sh` e `003-ee.sh` fazem `git clone --depth 1` das branches e executam os respectivos `toolchain.sh`. Isso significa que usar configuração padrão não fixa dependências de forma reproduzível.
- O script wrapper `toolchain-sudo.sh` aponta para `/usr/local/ps2dev` e exporta paths; não é a escolha para workflow sem privilégios. A CI do próprio PS2Toolchain usa `config/ci-env.sh`, que define `PS2DEV=$PWD/ps2dev`.
- `.github/workflows/compilation.yml` do PS2Toolchain usa Ubuntu, instala dependências via `apt`, executa `toolchain.sh`, define `timeout-minutes: 340`, compacta `ps2dev` e faz upload do tarball. É evidência de desenho/configuração de CI, **não** duração medida.
- A CI do gsKit usa `ubuntu-latest` em container `ghcr.io/ps2dev/ps2sdk-ports:latest`, instala `build-base`, `git`, `cmake`, `pkgconf`, compila/instala gsKit e publica artefatos dos exemplos. A tag `latest` é mutável; o workflow futuro deve fixar digest para reprodução.

Scripts dos clones foram lidos, não executados. Não se recomenda chamar `toolchain.sh` upstream sem antes fixar as dependências e auditar downloads, efeitos e destino dos arquivos.

## Caminho recomendado

1. **Bootstrap separado da build do jogo:** preferir uma imagem de container oficial com PS2SDK/toolchain, fixada por digest. Alternativamente, compilar o toolchain uma vez em job controlado, gravar `PS2DEV` sob `$RUNNER_TEMP`/workspace e publicar um artefato/cache identificado por commits, sistema operacional e arquitetura.
2. **Fixar dependências:** registrar SHA/tag dos três componentes DVP/IOP/EE e PS2SDK; não aceitar `main`/`latest` implícitos. Preferir versão pré-compilada verificada por checksum ou commits exatos. O workflow oficial observado usa refs padrão e por isso, sozinho, não é uma receita imutável.
3. **Separar cache:** cachear somente toolchain/SDK sem conteúdo de jogo, usando chave dos commits, OS/arquitetura e configuração. Tratar cache como otimização (pode ser removido/evicto); build correta não pode depender do cache existir. Container por digest pode reduzir custo de instalação, mas precisa de verificação de disponibilidade/arquitetura e licença.
4. **ELF mínimo sem ROM:** checkout do próprio projeto, ambiente EE fixado, compilar um único fonte C com startup/linker apropriados ao PS2SDK, validar ELF com ferramentas da toolchain, calcular SHA-256 e armazenar o ELF de teste e log. Essa fase prova somente compilação/ligação; boot em emulador/hardware permanece outra etapa.
5. **Artefatos/logs:** usar `actions/upload-artifact` para ELF mínimo, log de build, versões/SHAs e checksums da toolchain. Não publicar ROM, dump, archive O2R, recursos extraídos, memória privada nem artefatos originados de tais entradas. Sanitizar paths/dados antes de upload; não incluir tokens no log.
6. **Limites do job:** definir `timeout-minutes` menor que o máximo permitido pelo runner e observar duração real. O workflow upstream escolhe 340 minutos, perto do limite usual de job hosted; isso não comprova que a instalação complete em 340 minutos, especialmente em runner frio. Compilação paralela deve ser dimensionada pela memória do runner e não assumir núcleos ilimitados.

## Runner gratuito: o que é e não é possível concluir

- **Tecnicamente possível:** um runner Ubuntu consegue executar ferramentas Linux e workflows PS2Toolchain/gsKit existentes demonstram configurações de build e upload. Um ELF mínimo não depende de ROM, assets ou secrets.
- **Tempo:** os clones não registram tempo real de instalação nem de build. O limite de 340 minutos está configurado no workflow upstream do PS2Toolchain; não é uma medição. O tempo de compilação completa da cadeia permanece aberto.
- **Custo:** uso incluído, minutos e concorrência dependem de repo público/privado, plano, região/configuração da conta e saldo. O README deste projeto descreve o repositório como privado, mas esta auditoria não tem acesso ao plano nem à cota atual; logo, não é possível afirmar que a cadeia completa cabe no orçamento gratuito.
- **Conclusão:** pode-se planejar e validar a etapa mínima em hosted runner; não declarar “todo o fluxo gratuito” até medir toolchain fria/quente e build-alvo em Actions e verificar a cota real. O caminho mais controlável é toolchain containerizada/cacheada e build incremental, com limites e artifacts explícitos.

## Workflow mínimo proposto (não implementado)

- Evento manual `workflow_dispatch`, `permissions` mínimas, `timeout-minutes` explícito.
- Job sem secrets e sem ROM, usando container por digest ou toolchain previamente cacheada.
- Log de ambiente: commits e versões de compiler/SDK; log de build com saída não protegida.
- Compilar somente ELF hello-world, validar ELF/checksum, upload de ELF e log.
- Retenção curta para logs/artefatos de diagnóstico; nenhum arquivo de ROM/asset no cache ou artifact.
- Próxima etapa só após o mínimo passar: gsKit, controle, áudio e storage em workflows separados.

Não foram modificados workflows, instalados pacotes, invocados scripts externos ou compilados ELF/jogo. O fluxo acima é uma proposta para o próximo marco.
