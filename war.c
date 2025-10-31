/*
 * Projeto: Mini-War com Missões Estratégicas
 * Descrição: Implementação de um sistema de missões em C,
 * focado em alocação dinâmica, ponteiros e modularização.
 * Feito de forma simples para fins educacionais.
 */

// --- 1. Bibliotecas (Nossas ferramentas) ---
#include <stdio.h>    // Para printf, scanf
#include <stdlib.h>   // Para malloc, free, rand, srand, exit
#include <string.h>   // Para strcpy (copiar strings), strcmp (comparar strings)
#include <time.h>     // Para time() (gerar semente aleatória)

// --- 2. Definições e Estruturas (Nossos moldes) ---

// Struct: Nosso "molde" para criar variáveis do tipo Territorio
struct Territorio {
    char nome[30];
    char cor[10]; // Cor do jogador que domina (ex: "Azul", "Vermelho")
    int tropas;
};

// Vamos usar "typedef" para facilitar. Agora, em vez de "struct Territorio",
// podemos só escrever "Territorio".
typedef struct Territorio Territorio;

// Constantes para organizar o jogo
#define TOTAL_TERRITORIOS 5
#define TOTAL_MISSOES 5
#define MAX_TAM_MISSAO 100 // Tamanho máximo em chars de uma string de missão

// --- 3. Protótipos das Funções (O "menu" de operários) ---
// Declaramos todas as funções aqui para que o 'main' saiba que elas existem.

void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void atacar(Territorio* atacante, Territorio* defensor);
int verificarMissao(char* missao, char* corJogador, Territorio* mapa, int tamanho);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missaoP1, char* missaoP2);
void inicializarMapa(Territorio* mapa);

// --- 4. Função Principal (O "Gerente") ---
int main() {
    // Inicializa o gerador de números aleatórios
    // Isso garante que os 'rand()' sejam diferentes a cada execução
    srand(time(NULL));

    // ---- A. Criação do Vetor de Missões (Requisito 1) ----
    // Este é um vetor de ponteiros para strings. As strings em si
    // ficam guardadas em uma área de memória "somente leitura".
    char* missoes[TOTAL_MISSOES] = {
        "Conquistar 3 territorios (os 3 primeiros da lista)",
        "Eliminar todas as tropas da cor Vermelha",
        "Eliminar todas as tropas da cor Azul",
        "Ter 5 tropas no territorio 'Brasil'",
        "Conquistar os territorios 'Alaska' e 'Argentina'"
    };

    // ---- B. Alocação de Memória (Requisito 3 e Estruturas) ----

    // 1. Alocar memória para o MAPA (vetor de territórios)
    // Usamos 'malloc' para pedir memória suficiente para 5 territórios.
    // (Territorio*) "converte" o ponteiro genérico do malloc para um ponteiro de Territorio.
    Territorio* mapa = (Territorio*) malloc(TOTAL_TERRITORIOS * sizeof(Territorio));
    
    // 2. Alocar memória para as MISSÕES dos jogadores
    // Pedimos espaço para guardar as strings das missões (até 100 caracteres cada)
    char* missaoP1 = (char*) malloc(MAX_TAM_MISSAO * sizeof(char));
    char* missaoP2 = (char*) malloc(MAX_TAM_MISSAO * sizeof(char));

    // Verificação de segurança: Se o sistema não deu a memória (ex: PC sem memória),
    // o 'malloc' retorna NULL. Devemos sempre checar isso.
    if (mapa == NULL || missaoP1 == NULL || missaoP2 == NULL) {
        printf("Erro critico: Falha ao alocar memoria!\n");
        // Libera o que conseguiu alocar antes de sair
        free(mapa);
        free(missaoP1);
        free(missaoP2);
        return 1; // Retorna 1 (código de erro)
    }

    // ---- C. Inicialização do Jogo ----

    // Preenche o mapa com dados iniciais
    inicializarMapa(mapa);

    // ---- D. Sorteio da Missão (Requisito 2) ----
    // Passamos a "seta" (ponteiro) 'missaoP1' para a função.
    // A função 'atribuirMissao' vai usar essa seta para escrever
    // a missão sorteada DIRETAMENTE na memória que alocamos.
    atribuirMissao(missaoP1, missoes, TOTAL_MISSOES);
    atribuirMissao(missaoP2, missoes, TOTAL_MISSOES);

    // ---- E. Exibição Condicional (Requisito 5) ----
    // Mostra a missão de cada jogador UMA VEZ no início.
    printf("--- Bem-vindo ao Mini-War! ---\n\n");
    printf("Missao Jogador 1 (Azul): %s\n", missaoP1);
    printf("Missao Jogador 2 (Vermelho): %s\n", missaoP2);
    printf("\nQue comecem os jogos! Pressione Enter para o primeiro turno...");
    getchar(); // Pausa e espera o usuário apertar Enter

    // ---- F. Loop Principal do Jogo ----
    int jogoAtivo = 1;
    int turno = 0;

    while (jogoAtivo) {
        turno++;
        printf("\n\n--- TURNO %d ---_-\n", turno);
        
        // Sempre exibe o mapa no início do turno
        exibirMapa(mapa, TOTAL_TERRITORIOS);

        // -- Turno do Jogador 1 (Azul) --
        printf("\n-- Vez do Jogador 1 (Azul) --\n");
        int idxAtacante, idxDefensor;
        
        printf("Digite o indice (0-4) do territorio ATACANTE (Azul): ");
        scanf("%d", &idxAtacante);
        printf("Digite o indice (0-4) do territorio DEFENSOR (Inimigo): ");
        scanf("%d", &idxDefensor);

        // Validação simples de indices e ataque
        if (idxAtacante < 0 || idxAtacante >= TOTAL_TERRITORIOS ||
            idxDefensor < 0 || idxDefensor >= TOTAL_TERRITORIOS) {
            printf("Indices invalidos!\n");
        } else {
            // Passamos o ENDEREÇO (&) dos territórios no vetor 'mapa'
            // A função 'atacar' vai modificar os territórios originais.
            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        }

        // ---- G. Verificação da Missão (Requisito 4) ----
        // Verifica se o P1 ganhou APÓS o ataque
        if (verificarMissao(missaoP1, "Azul", mapa, TOTAL_TERRITORIOS)) {
            printf("\n\n*****************************************\n");
            printf("  JOGADOR 1 (AZUL) VENCEU O JOGO!\n");
            printf("  Missao Cumprida: %s\n", missaoP1);
            printf("*****************************************\n");
            jogoAtivo = 0; // Para o loop
            continue;      // Pula o turno do P2
        }

        // Pausa para o próximo jogador
        printf("Pressione Enter para o turno do Jogador 2...");
        // Limpa o buffer do teclado (o '\n' do scanf anterior)
        while (getchar() != '\n'); 
        getchar();

        // -- Turno do Jogador 2 (Vermelho) --
        printf("\n-- Vez do Jogador 2 (Vermelho) --\n");
        
        printf("Digite o indice (0-4) do territorio ATACANTE (Vermelho): ");
        scanf("%d", &idxAtacante);
        printf("Digite o indice (0-4) do territorio DEFENSOR (Inimigo): ");
        scanf("%d", &idxDefensor);

        if (idxAtacante < 0 || idxAtacante >= TOTAL_TERRITORIOS ||
            idxDefensor < 0 || idxDefensor >= TOTAL_TERRITORIOS) {
            printf("Indices invalidos!\n");
        } else {
            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        }

        // Verifica se o P2 ganhou
        if (verificarMissao(missaoP2, "Vermelho", mapa, TOTAL_TERRITORIOS)) {
            printf("\n\n*****************************************\n");
            printf("  JOGADOR 2 (VERMELHO) VENCEU O JOGO!\n");
            printf("  Missao Cumprida: %s\n", missaoP2);
            printf("*****************************************\n");
            jogoAtivo = 0; // Para o loop
        }

        // Limita o jogo para não ficar infinito (caso ninguém vença)
        if (turno > 20) {
            printf("\n\n--- LIMITE DE TURNOS ATINGIDO! O JOGO EMPATOU! ---\n");
            jogoAtivo = 0;
        }

    } // Fim do 'while (jogoAtivo)'

    // ---- H. Liberação de Memória (Requisito Final) ----
    // Chama o "faxineiro" para devolver TODA a memória que pedimos com 'malloc'.
    // Isso é ESSENCIAL para evitar "vazamento de memória".
    liberarMemoria(mapa, missaoP1, missaoP2);

    printf("Fim do jogo. Obrigado por jogar!\n");
    return 0; // Sucesso
}

// --- 5. Implementação das Funções (Os "Operários") ---

/**
 * @brief Sorteia uma missão e a copia para o ponteiro de destino.
 * @param destino Ponteiro para a memória onde a string da missão será copiada.
 * @param missoes Vetor de strings contendo todas as missões possíveis.
 * @param totalMissoes Tamanho do vetor de missões.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    // Sorteia um índice aleatório (de 0 a totalMissoes-1)
    int indiceSorteado = rand() % totalMissoes;
    
    // Copia a string da missão sorteada (missoes[indiceSorteado])
    // para dentro da memória apontada pelo 'destino'.
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * @brief Simula um ataque entre dois territórios.
 * @param atacante Ponteiro para o território que ataca.
 * @param defensor Ponteiro para o território que defende.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // '->' é um atalho para acessar campos de uma struct através de um ponteiro
    // (equivale a (*atacante).cor)

    // Requisito: Validar se o ataque é contra um inimigo
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("ATAQUE INVALIDO: Voce nao pode atacar seu proprio territorio!\n");
        return; // Para a função aqui
    }

    // Requisito: Não pode atacar com 1 tropa (ou menos)
    if (atacante->tropas <= 1) {
        printf("ATAQUE INVALIDO: Voce precisa de mais de 1 tropa para atacar!\n");
        return;
    }

    // Simulação simples de dados
    int dadoAtaque = rand() % 6 + 1; // Número de 1 a 6
    int dadoDefesa = rand() % 6 + 1;

    printf("  > Batalha: Ataque (%s) rolou %d | Defesa (%s) rolou %d\n",
        atacante->nome, dadoAtaque, defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        // Vitória do Atacante
        printf("  > VITORIA! %s conquistou %s!\n", atacante->nome, defensor->nome);
        
        // Requisito: Transfere a cor
        strcpy(defensor->cor, atacante->cor);
        
        // Requisito: Transfere metade das tropas (deixando pelo menos 1 para trás)
        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas == 0) tropasMovidas = 1; // Move pelo menos 1

        defensor->tropas = tropasMovidas;
        atacante->tropas = atacante->tropas - tropasMovidas;

    } else {
        // Derrota do Atacante (ou empate)
        printf("  > DERROTA! %s perdeu a batalha e 1 tropa.\n", atacante->nome);
        
        // Requisito: Atacante perde uma tropa
        atacante->tropas--;
    }
}

/**
 * @brief Verifica se a condição da missão de um jogador foi atingida.
 * @param missao A string da missão do jogador.
 * @param corJogador A cor do jogador (ex: "Azul").
 * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho O número total de territórios.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(char* missao, char* corJogador, Territorio* mapa, int tamanho) {
    // Esta é a lógica "simples" pedida.
    // Usamos 'strcmp' para comparar a string da missão do jogador
    // com as missões que conhecemos. 'strcmp' retorna 0 se forem iguais.

    // Missão 1: "Conquistar 3 territorios (os 3 primeiros da lista)"
    if (strcmp(missao, "Conquistar 3 territorios (os 3 primeiros da lista)") == 0) {
        if (strcmp(mapa[0].cor, corJogador) == 0 &&
            strcmp(mapa[1].cor, corJogador) == 0 &&
            strcmp(mapa[2].cor, corJogador) == 0) {
            return 1; // Missão cumprida!
        }
    }

    // Missão 2: "Eliminar todas as tropas da cor Vermelha"
    if (strcmp(missao, "Eliminar todas as tropas da cor Vermelha") == 0) {
        int tropasVermelhas = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                tropasVermelhas += mapa[i].tropas;
            }
        }
        if (tropasVermelhas == 0) return 1; // Missão cumprida!
    }

    // Missão 3: "Eliminar todas as tropas da cor Azul"
    if (strcmp(missao, "Eliminar todas as tropas da cor Azul") == 0) {
        int tropasAzuis = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) {
                tropasAzuis += mapa[i].tropas;
            }
        }
        if (tropasAzuis == 0) return 1; // Missão cumprida!
    }
    
    // Missão 4: "Ter 5 tropas no territorio 'Brasil'"
    if (strcmp(missao, "Ter 5 tropas no territorio 'Brasil'") == 0) {
        for (int i = 0; i < tamanho; i++) {
            // Procura o território "Brasil"
            if (strcmp(mapa[i].nome, "Brasil") == 0) {
                // Verifica se pertence ao jogador E tem 5+ tropas
                if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 5) {
                    return 1; // Missão cumprida!
                }
                break; // Achou o Brasil, não precisa mais procurar
            }
        }
    }

    // Missão 5: "Conquistar os territorios 'Alaska' e 'Argentina'"
    if (strcmp(missao, "Conquistar os territorios 'Alaska' e 'Argentina'") == 0) {
        // (No nosso mapa, são os índices 0 e 3)
        if (strcmp(mapa[0].cor, corJogador) == 0 &&
            strcmp(mapa[3].cor, corJogador) == 0) {
            return 1; // Missão cumprida!
        }
    }

    // Se chegou até aqui, nenhuma missão foi cumprida
    return 0;
}

/**
 * @brief Exibe o estado atual de todos os territórios no mapa.
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("--- MAPA ATUAL ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("  Indice %d: %-12s (Cor: %-8s, Tropas: %d)\n",
            i,
            mapa[i].nome,
            mapa[i].cor,
            mapa[i].tropas);
    }
}

/**
 * @brief Libera (free) toda a memória alocada dinamicamente.
 */
void liberarMemoria(Territorio* mapa, char* missaoP1, char* missaoP2) {
    printf("\nLiberando memoria...\n");
    
    // Devolve a memória do vetor de territórios
    free(mapa);
    
    // Devolve a memória da string da missão do P1
    free(missaoP1);
    
    // Devolve a memória da string da missão do P2
    free(missaoP2);
    
    printf("Memoria liberada com sucesso.\n");
}

/**
 * @brief Preenche o vetor de mapa com valores iniciais.
 */
void inicializarMapa(Territorio* mapa) {
    // Território 0
    strcpy(mapa[0].nome, "Alaska");
    strcpy(mapa[0].cor, "Azul");
    mapa[0].tropas = 3;

    // Território 1
    strcpy(mapa[1].nome, "Canada");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 2;

    // Território 2
    strcpy(mapa[2].nome, "Brasil");
    strcpy(mapa[2].cor, "Vermelho");
    mapa[2].tropas = 4; // Reduzido de 5 para a missão 4 ser um desafio

    // Território 3
    strcpy(mapa[3].nome, "Argentina");
    strcpy(mapa[3].cor, "Vermelho");
    mapa[3].tropas = 2;

    // Território 4
    strcpy(mapa[4].nome, "Groenlandia");
    strcpy(mapa[4].cor, "Azul");
    mapa[4].tropas = 1;
}
