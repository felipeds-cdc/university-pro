/*
 * ============================================
 * XOR File Encryptor/Decryptor - Educacional
 * Autor: Felipe Diassis
 * GitHub: github.com/felipeds-cdc
 * 
 * AVISO: Projeto estritamente educacional.
 * Desenvolvido para estudo de criptografia
 * e segurança ofensiva defensiva.
 * Uso malicioso é crime - Lei 12.737/2012.
 * ============================================
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERSION     "1.0"
#define MAX_KEY     256
#define BUFFER_SIZE 4096

/* ============================================
 * CORES PARA TERMINAL
 * ============================================ */
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

/* ============================================
 * ESTRUTURA DE RESULTADO
 * ============================================ */
typedef struct {
    long   bytes_processados;
    double tempo_gasto;
    int    sucesso;
} Resultado;

/* ============================================
 * BANNER
 * ============================================ */
void banner() {
    printf(CYAN BOLD);
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║         XOR FILE ENCRYPTOR v%s          ║\n", VERSION);
    printf("║      github.com/felipeds-cdc             ║\n");
    printf("║   ⚠  Apenas para fins educacionais  ⚠   ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf(RESET "\n");
}

/* ============================================
 * MENU DE AJUDA
 * ============================================ */
void ajuda(const char *prog) {
    printf(BOLD "USO:\n" RESET);
    printf("  %s <modo> <arquivo_entrada> <arquivo_saida> <chave>\n\n", prog);
    
    printf(BOLD "MODOS:\n" RESET);
    printf("  encrypt    Criptografa um arquivo\n");
    printf("  decrypt    Descriptografa um arquivo\n");
    printf("  analyze    Analisa entropia do arquivo\n\n");
    
    printf(BOLD "EXEMPLOS:\n" RESET);
    printf("  %s encrypt  documento.txt  documento.enc  minha_chave_secreta\n", prog);
    printf("  %s decrypt  documento.enc  documento.txt  minha_chave_secreta\n", prog);
    printf("  %s analyze  documento.enc  -              -\n\n", prog);
    
    printf(BOLD "COMO FUNCIONA:\n" RESET);
    printf("  XOR é uma operação bit a bit onde:\n");
    printf("  • 0 XOR 0 = 0\n");
    printf("  • 1 XOR 0 = 1\n");
    printf("  • 0 XOR 1 = 1\n");
    printf("  • 1 XOR 1 = 0\n\n");
    printf("  A mesma operação criptografa E descriptografa.\n");
    printf("  arquivo XOR chave = cifrado\n");
    printf("  cifrado XOR chave = arquivo original\n\n");
}

/* ============================================
 * BARRA DE PROGRESSO
 * ============================================ */
void progresso(long atual, long total) {
    int largura = 40;
    double percent = (total > 0) ? (double)atual / (double)total : 0.0;
    int preenchido = (int)(largura * percent);

    printf("\r  [");
    for (int i = 0; i < largura; i++) {
        if (i < preenchido)
            printf(GREEN "█" RESET);
        else
            printf("░");
    }
    printf("] %5.1f%%  (%ld/%ld bytes)", percent * 100, atual, total);
    fflush(stdout);
}

/* ============================================
 * CALCULA TAMANHO DO ARQUIVO
 * ============================================ */
long tamanho_arquivo(FILE *f) {
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);
    return tamanho;
}

/* ============================================
 * EXIBE INFO DO ARQUIVO
 * ============================================ */
void info_arquivo(const char *nome, long bytes) {
    printf("  Arquivo  : %s\n", nome);
    if (bytes < 1024)
        printf("  Tamanho  : %ld bytes\n", bytes);
    else if (bytes < 1024 * 1024)
        printf("  Tamanho  : %.2f KB\n", (float)bytes / 1024);
    else
        printf("  Tamanho  : %.2f MB\n", (float)bytes / (1024 * 1024));
}

/* ============================================
 * GERA HASH SIMPLES DA CHAVE (para verificação)
 * ============================================ */
unsigned int hash_chave(const char *chave) {
    unsigned int hash = 5381;
    unsigned char c;
    while ((c = (unsigned char)*chave++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

/* ============================================
 * NÚCLEO: OPERAÇÃO XOR
 * ============================================ */
Resultado xor_arquivo(
    const char *entrada,
    const char *saida,
    const char *chave,
    const char *modo
) {
    Resultado res = {0, 0.0, 0};
    
    FILE *arq_entrada = fopen(entrada, "rb");
    if (!arq_entrada) {
        printf(RED "  [ERRO] Não foi possível abrir: %s\n" RESET, entrada);
        return res;
    }

    FILE *arq_saida = fopen(saida, "wb");
    if (!arq_saida) {
        printf(RED "  [ERRO] Não foi possível criar: %s\n" RESET, saida);
        fclose(arq_entrada);
        return res;
    }

    long   tam        = tamanho_arquivo(arq_entrada);
    size_t tam_chave  = strlen(chave);
    unsigned char buffer[BUFFER_SIZE];
    size_t processado = 0;
    clock_t inicio    = clock();

    printf("\n");
    info_arquivo(entrada, tam);
    printf("  Chave    : %s (hash: %u)\n", chave, hash_chave(chave));
    printf("  Modo     : %s\n", modo);
    printf("  Operação : XOR byte a byte\n\n");

    /* ---- Loop principal de XOR ---- */
    size_t lidos;
    while ((lidos = fread(buffer, 1, BUFFER_SIZE, arq_entrada)) > 0) {
        for (size_t i = 0; i < lidos; i++) {
            /* Cada byte é XOR'd com o caractere correspondente da chave
             * A chave é rotacionada ciclicamente usando módulo */
            buffer[i] ^= (unsigned char)chave[processado % tam_chave];
            processado++;
        }
        fwrite(buffer, 1, lidos, arq_saida);
        progresso((long)processado, tam);
    }

    clock_t fim    = clock();
    res.tempo_gasto        = (double)(fim - inicio) / CLOCKS_PER_SEC;
    res.bytes_processados  = (long)processado;
    res.sucesso            = 1;

    fclose(arq_entrada);
    fclose(arq_saida);

    return res;
}

/* ============================================
 * ANÁLISE DE ENTROPIA
 * ============================================ */
void analisar_arquivo(const char *nome) {
    FILE *f = fopen(nome, "rb");
    if (!f) {
        printf(RED "  [ERRO] Não foi possível abrir: %s\n" RESET, nome);
        return;
    }

    long frequencia[256] = {0};
    unsigned char byte;
    long total = 0;

    while (fread(&byte, 1, 1, f) == 1) {
        frequencia[byte]++;
        total++;
    }
    fclose(f);

    /* Calcula entropia de Shannon */
    double entropia = 0.0;
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > 0) {
            double p = (double)frequencia[i] / (double)total;
            entropia -= p * log2(p);
        }
    }

    printf("\n");
    info_arquivo(nome, total);
    printf("\n  " BOLD "ANÁLISE DE ENTROPIA:\n" RESET);
    printf("  Entropia : %.4f bits/byte\n", entropia);
    printf("  Máximo   : 8.0000 bits/byte\n\n");

    /* Interpretação */
    if (entropia > 7.5)
        printf(GREEN "  [RESULTADO] Alta entropia — arquivo provavelmente cifrado ou comprimido\n" RESET);
    else if (entropia > 5.0)
        printf(YELLOW "  [RESULTADO] Entropia média — arquivo misto (texto + binário)\n" RESET);
    else
        printf(CYAN "  [RESULTADO] Baixa entropia — arquivo de texto legível\n" RESET);

    printf("\n  " BOLD "DISTRIBUIÇÃO DE BYTES (Top 10):\n" RESET);
    printf("  %-6s %-10s %s\n", "BYTE", "FREQUÊNCIA", "PROPORÇÃO");
    printf("  %-6s %-10s %s\n", "------", "----------", "--------");

    /* Encontra top 10 bytes mais frequentes */
    for (int top = 0; top < 10; top++) {
        int max_idx = 0;
        for (int i = 1; i < 256; i++)
            if (frequencia[i] > frequencia[max_idx])
                max_idx = i;

        if (frequencia[max_idx] == 0) break;

        printf("  0x%02X   %-10ld %.2f%%\n",
            max_idx,
            frequencia[max_idx],
            (double)frequencia[max_idx] / (double)total * 100.0);

        frequencia[max_idx] = 0;
    }
}

/* ============================================
 * EXIBE RESULTADO FINAL
 * ============================================ */
void exibir_resultado(Resultado res, const char *saida, const char *modo) {
    printf("\n\n  ─────────────────────────────────────\n");
    printf(GREEN BOLD "  ✓ %s concluído com sucesso!\n" RESET, modo);
    printf("  ─────────────────────────────────────\n");
    printf("  Arquivo gerado : %s\n", saida);
    printf("  Bytes tratados : %ld\n", res.bytes_processados);
    printf("  Tempo gasto    : %.4f segundos\n", res.tempo_gasto);

    if (res.tempo_gasto > 0)
        printf("  Velocidade     : %.2f MB/s\n",
            (float)res.bytes_processados / res.tempo_gasto / (1024 * 1024));

    printf("\n  " YELLOW "⚠  Guarde a chave! Sem ela não há descriptografia.\n" RESET);
    printf("  " YELLOW "⚠  Use apenas em arquivos próprios.\n\n" RESET);
}

/* ============================================
 * MAIN
 * ============================================ */
int main(int argc, char *argv[]) {
    banner();

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        ajuda(argv[0]);
        return 0;
    }

    if (argc < 5 && strcmp(argv[1], "analyze") != 0) {
        printf(RED "  [ERRO] Argumentos insuficientes.\n\n" RESET);
        ajuda(argv[0]);
        return 1;
    }

    char *modo    = argv[1];
    char *entrada = argv[2];
    char *saida   = argc > 3 ? argv[3] : NULL;
    char *chave   = argc > 4 ? argv[4] : NULL;

    /* Validação da chave */
    if (chave && strlen(chave) < 4) {
        printf(YELLOW "  [AVISO] Chave muito curta. Use pelo menos 4 caracteres.\n\n" RESET);
    }

    /* ---- Modo: encrypt ou decrypt ---- */
    if (strcmp(modo, "encrypt") == 0 || strcmp(modo, "decrypt") == 0) {
        if (!chave) {
            printf(RED "  [ERRO] Chave não informada.\n" RESET);
            return 1;
        }

        /* XOR é simétrico: encrypt == decrypt */
        Resultado res = xor_arquivo(entrada, saida, chave, modo);

        if (res.sucesso)
            exibir_resultado(res, saida, modo);
        else {
            printf(RED "\n  [ERRO] Operação falhou.\n\n" RESET);
            return 1;
        }

    /* ---- Modo: analyze ---- */
    } else if (strcmp(modo, "analyze") == 0) {
        analisar_arquivo(entrada);

    } else {
        printf(RED "  [ERRO] Modo inválido: '%s'\n\n" RESET, modo);
        ajuda(argv[0]);
        return 1;
    }

    return 0;
}