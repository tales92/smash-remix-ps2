# ROMs, assets e dados de entrada

Este documento define os nomes e os pontos de preparação conhecidos. **Nenhuma ROM foi adicionada a este repositório.**

## 1. Diretório reservado

Use `assets/roms/` como referência de organização para entradas fornecidas pelo usuário. O diretório contém apenas arquivos de instrução e um marcador.

**Importante:** a presença da pasta não significa que os arquivos serão automaticamente enviados ao GitHub Actions. O método de transferência privada para os runners ainda precisa ser projetado e testado. Não faça commit de ROMs, ISOs, dumps, patches binários ou assets extraídos.

Não coloque tokens ou credenciais junto dos arquivos.

## 2. Arquivos citados pelos projetos de referência

### BattleShip-Remix

O README do branch consultado documenta:

| Nome esperado | Finalidade | Hashes documentados |
|---|---|---|
| `baserom.remix.z64` | ROM Smash Remix 2.0.1 usada na extração de recursos para o port | SHA-1: `13716381ed5e29be2606e0f4724b18fd00789d04`; MD5: `2b2d6b295106c54216b7fc7a2f14346e` |
| `baserom.us.z64` | ROM americana original usada como origem do patch; o README a descreve como opcional para certos recursos de desenvolvimento/build | SHA-1: `e2929e10fccc0aa84e5776227e798abc07cedabf`; MD5: `f7c52568a31aadf26e14dc2b6416b2ed` |

O README informa que os dados do jogo não são distribuídos no repositório e são extraídos de ROMs fornecidas pelo usuário. Confirmar os caminhos exatos de leitura para o commit escolhido antes de preparar o workflow.

### Smash Remix +EXTRA

O README do branch `main` informa:

- fazer clone recursivo para trazer o submódulo `smashremix`;
- fornecer a ROM vanilla em `smashremix/roms/ssb.rom`;
- a ferramenta `character_appender.py` gera `smashremix/roms/original_extra.z64`;
- `patch_extra.bat` participa da geração final da ROM.

O README consultado não fornece, nesse trecho, hash de ROM para validar a entrada. A auditoria deverá inspecionar o submódulo e os scripts antes de fixar a versão/hash esperado.

### SSB64 Decompilation

O README de `ssb-decomp-re` documenta a ROM americana v1.0 como `baserom.us.z64`, com SHA-1 `e2929e10fccc0aa84e5776227e798abc07cedabf` e MD5 `f7c52568a31aadf26e14dc2b6416b2ed`.

## 3. Organização proposta

```text
assets/
├── roms/
│   ├── README.md
│   └── .gitkeep
└── extracted/
    ├── README.md
    └── .gitkeep
```

- `assets/roms/`: somente área de referência/preparação das ROMs de entrada.
- `assets/extracted/`: área de trabalho para dados extraídos, nunca para commits de assets protegidos.
- builds, objetos, caches e dumps temporários devem permanecer fora do controle de versão.

## 4. Próxima decisão: como entregar entradas ao GitHub Actions

Antes de criar um workflow que consuma ROMs, medir tamanhos e identificar exatamente quais arquivos são necessários. Depois comparar alternativas, incluindo Git LFS e armazenamento privado/artefatos com acesso controlado, considerando limites, custos, retenção e permissões vigentes.

Não use GitHub Secrets para armazenar ROMs grandes: Secrets são destinados a valores de configuração/credenciais, não a arquivos binários volumosos.

O futuro workflow deverá:
1. receber ou localizar entradas apenas em armazenamento privado autorizado;
2. validar nomes, tamanho e hashes antes de executar scripts;
3. nunca imprimir o conteúdo dos arquivos ou segredos nos logs;
4. não empacotar ROMs/seriados de assets em artefatos de saída;
5. publicar somente logs sanitizados, relatórios, builds não protegidas e documentação.
