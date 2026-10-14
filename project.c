/**
 * A program exemplifying the use of Doxygen comments in C.
 * @file ex.c
 * @author ist113532 (Afonso Cosya))
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFMAX       65536 /**Tamanho do buffer */
#define MAXVACINAS   1000 /**Numero de vacinas */
#define MAXLENLOTE   21 /**Tamanho do lote */
#define LOTENOME     51 /**Tamanho do nome */

/**Erros em inglês*/
#define NOMEMORY           "No memory"
#define CAPAVACINAS        "too many vaccines"
#define LOTEDUPLICADO      "duplicate batch number"
#define LOTEINVALIDO       "invalid batch"
#define NOMEINVALIDO       "invalid name"
#define DATAINVALIDA       "invalid date"
#define QUANTIDADEINVALIDA "invalid quantity"
#define NOUSER             "no such user"
#define VACINAESGOTADA     "no stock"
#define VACINAPLICADA      "already vaccinated"
#define LOTEINEXISTENTE    "no such batch"
#define VACINAINEXISTENTE  "no such vaccine"
/**Erros em português*/
#define NOMEMORY_PT           "Sem memória"
#define CAPAVACINAS_PT        "demasiadas vacinas"
#define LOTEDUPLICADO_PT      "número de lote duplicado"
#define LOTEINVALIDO_PT       "lote inválido"
#define NOMEINVALIDO_PT       "nome inválido"
#define DATAINVALIDA_PT       "data inválida"
#define QUANTIDADEINVALIDA_PT "quantidade inválida"
#define NOUSER_PT             "utente inexistente"
#define VACINAESGOTADA_PT     "esgotado"
#define VACINAPLICADA_PT      "já vacinado"
#define LOTEINEXISTENTE_PT    "lote inexistente"
#define VACINAINEXISTENTE_PT  "vacina inexistente"

/**
 * @brief Estrutura que representa uma data
 */
typedef struct {
    int dia, mes, ano;
} Data;


/**
 * @brief Estrutura que representa um lote
 */
typedef struct {
    char lote[MAXLENLOTE];
    char nome[LOTENOME];
    Data data;
    unsigned doses_disponiveis;
    unsigned doses_aplicadas;
} lote_de_vacina;


/**
 * @brief Estrutura que representa um utente
 */
typedef struct {
    char *nome;
    char lote[MAXLENLOTE];
    Data datavacinacao;
    int numeroDeVacinacoes;
} Utente;


/**
 * @brief Estrutura que representa o sistema
 */
typedef struct {
    int numero_lotes;
    lote_de_vacina lotes[MAXVACINAS];
    Data data_inicial;
    int idioma;
    Utente *utentes;
    int num_utentes;
    int cap_utentes;
} Sys;


/**
 * @brief faz a escolha do idioma, em inglês ou português
 * @param msg_en mensagem em inglês
 * @param msg_pt mensagem em português
 * @return retorna a mensagem escolhida, idioma.
 */
const char* get_message(const char* msg_en, const char* msg_pt, Sys *sys) {
    return (sys->idioma == 1) ? msg_pt : msg_en;
}


/**
 * @brief Verifica se uma data eh valida
 * @param data data a ser verificada
 * @param data_inicial data inicial
 * @return 1 se a data é valida, 0 caso contrario
 */
int validar_data(Data data, Data data_inicial) {
    int dias_por_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (data.ano % 4 == 0 && (data.ano % 100 != 0 || data.ano % 400 == 0))
        dias_por_mes[2] = 29;
    if (data.mes < 1 || data.mes > 12 || data.dia < 1 || data.dia > dias_por_mes[data.mes])
        return 0;
    if (data.ano < data_inicial.ano ||
       (data.ano == data_inicial.ano && data.mes < data_inicial.mes) ||
       (data.ano == data_inicial.ano && data.mes == data_inicial.mes && data.dia < data_inicial.dia))
        return 0;
    return 1;
}


/**
 * @brief Verifica se um lote eh valido
 * @param lote lote a ser verificado
 * @return 1 se o lote eh valido, 0 caso contrario
 */
int validar_lote(const char *lote) {
    int len = strlen(lote);
    if (len > 20)
        return 0;
    for (int i = 0; i < len; i++) {
        if (!isxdigit(lote[i]) || islower(lote[i]))
            return 0;
    }
    return 1;
}


/**
 * @brief Verifica se um nome eh valido
 * @param nome nome a ser verificado
 * @return 1 se o nome eh valido, 0 caso contrario
 */
static int validar_nome(const char *nome) {
    if (strlen(nome) >= LOTENOME) {
        puts(NOMEINVALIDO);
        return 0;
    }
    for (int i = 0; nome[i] != '\0'; i++) {
        if (nome[i] == ' ' || nome[i] == '\t' || nome[i] == '\n') {
            puts(NOMEINVALIDO);
            return 0;
        }
    }
    return 1;
}


/**
 * @brief Compara duas datas
 * @param d1 primeira data
 * @param d2 segunda data
 * @return -1 se d1 < d2, 1 se d1 > d2 e 0 se d1 == d2
 */
int compare_date(const Data *d1, const Data *d2) {
    if (d1->ano != d2->ano)
        return d1->ano - d2->ano;
    if (d1->mes != d2->mes)
        return d1->mes - d2->mes;
    return d1->dia - d2->dia;
}


/**
 * @brief Compara dois lotes
 * @param l1 primeiro lote
 * @param l2 segundo lote
 * @return -1 se l1 < l2, 1 se l1 > l2 e 0 se l1 == l2
 */
int compare_lote(const lote_de_vacina *l1, const lote_de_vacina *l2) {
    int cmp = compare_date(&l1->data, &l2->data);
    if (cmp == 0)
        cmp = strcmp(l1->lote, l2->lote);
    return cmp;
}



/**
 * @brief Compara dois utentes
 * @param u1 primeiro utente
 * @param u2 segundo utente
 * @return -1 se u1 < u2, 1 se u1 > u2 e 0 se u1 == u2
 */
int compare_utentes(const Utente *u1, const Utente *u2) {
    return compare_date(&u1->datavacinacao, &u2->datavacinacao);
}


/**
 * @brief Ordena um array de lotes de vacina
 * @param lotes array de lotes de vacina
 * @param n tamanho do array
 * @return void
 */
void bubble_sort_lotes(lote_de_vacina *lotes, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (compare_lote(&lotes[j], &lotes[j + 1]) > 0) {
                lote_de_vacina temp = lotes[j];
                lotes[j] = lotes[j + 1];
                lotes[j + 1] = temp;
            }
}



/**
 * @brief Ordena um array de utentes
 * @param utentes array de utentes
 * @param n tamanho do array
 * @return void
 */
void bubble_sort_utentes(Utente *utentes, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (compare_utentes(&utentes[j], &utentes[j + 1]) > 0) {
                Utente temp = utentes[j];
                utentes[j] = utentes[j + 1];
                utentes[j + 1] = temp;
            }
}


/**
 * @brief Cria um lote de vacina, no estilo "create lote <lote> <data> <quantidade>"
 * @param sys sistema
 * @param in string de entrada
 * @return void
 */
void create_lote(Sys *sys, char *in) {
    char *token = strtok(in, " \t\n");
    if (!token || token[0] != 'c')
        return;
    
    char lote[BUFMAX], nome[BUFMAX];
    int dia, mes, ano, qtd;
    
    token = strtok(NULL, " \t\n"); if (!token) return;
    if (strlen(token) >= MAXLENLOTE) { 
        printf("%s\n", get_message(LOTEINVALIDO, LOTEINVALIDO_PT, sys)); 
        return; 
    }
    strcpy(lote, token);
    
    token = strtok(NULL, " \t\n"); if (!token) return;
    if (sscanf(token, "%d-%d-%d", &dia, &mes, &ano) != 3) { 
        printf("%s\n", get_message(DATAINVALIDA, DATAINVALIDA_PT, sys)); 
        return; 
    }
    Data data = {dia, mes, ano};
    if (!validar_data(data, sys->data_inicial)) { 
        printf("%s\n", get_message(DATAINVALIDA, DATAINVALIDA_PT, sys)); 
        return; 
    }
    
    token = strtok(NULL, " \t\n"); if (!token) return;
    qtd = atoi(token);
    if (qtd <= 0) { 
        printf("%s\n", get_message(QUANTIDADEINVALIDA, QUANTIDADEINVALIDA_PT, sys)); 
        return; 
    }
    
    token = strtok(NULL, "\n"); if (!token) return;
    while (isspace(*token)) token++;
    if (strlen(token) >= LOTENOME) { 
        printf("%s\n", get_message(NOMEINVALIDO, NOMEINVALIDO_PT, sys)); 
        return; 
    }
    strcpy(nome, token);


    for (int i = 0; i < sys->numero_lotes; i++) {
        if (strcmp(sys->lotes[i].lote, lote) == 0) {
            printf("%s\n", get_message(LOTEDUPLICADO, LOTEDUPLICADO_PT, sys));
            return; 
        }
    }


    if (strchr(nome, '_') != NULL) {
        printf("invalid character in vaccine name\n");
        return;
    }

    if (!validar_lote(lote)) { 
        printf("%s\n", get_message(LOTEINVALIDO, LOTEINVALIDO_PT, sys)); 
        return; 
    }
    if (!validar_nome(nome)) { 
        printf("%s\n", get_message(NOMEINVALIDO, NOMEINVALIDO_PT, sys)); 
        return; 
    }
    if (sys->numero_lotes == MAXVACINAS) { 
        printf("%s\n", get_message(CAPAVACINAS, CAPAVACINAS_PT, sys)); 
        return; 
    }


    lote_de_vacina novo;
    memset(&novo, 0, sizeof(novo));
    novo.data = data;
    novo.doses_disponiveis = qtd;
    novo.doses_aplicadas = 0;
    strcpy(novo.lote, lote);
    strncpy(novo.nome, nome, LOTENOME - 1);
    novo.nome[LOTENOME - 1] = '\0';
    
    sys->lotes[sys->numero_lotes++] = novo;
    printf("%s\n", novo.lote);
}




/**
 * @brief Altera a data inicial do sistema, no estilo "t <data>"
 * @param sys sistema
 * @param in string de entrada
 * @return void
 */
void nova_data(Sys *sys, char *in) {
    char *token = strtok(in, " \t\n");
    if (!token || token[0] != 't')
        return;
    
    token = strtok(NULL, " \t\n");
    if (!token) {
        printf("%02d-%02d-%d\n", sys->data_inicial.dia, sys->data_inicial.mes, sys->data_inicial.ano);
        return;
    }
    int dia, mes, ano;
    if (sscanf(token, "%d-%d-%d", &dia, &mes, &ano) != 3) {
        printf("%s\n", get_message(DATAINVALIDA, DATAINVALIDA_PT, sys));
        return;
    }
    Data nova = {dia, mes, ano};
    if (!validar_data(nova, sys->data_inicial)) {
        printf("%s\n", get_message(DATAINVALIDA, DATAINVALIDA_PT, sys));
        return;
    }
    sys->data_inicial = nova;
    printf("%02d-%02d-%d\n", sys->data_inicial.dia, sys->data_inicial.mes, sys->data_inicial.ano);
}



/**
 * @brief Lista os lotes de vacina, no estilo "listar lotes"
 * @param sys sistema
 * @param in string de entrada
 * @return void
 */
void listar_lotes(Sys *sys, char *in) {
    char *token = strtok(in, " \t\n");
    char **filtros = NULL;
    int num_filtros = 0;
    while ((token = strtok(NULL, " \t\n")) != NULL) {
        filtros = realloc(filtros, (num_filtros + 1) * sizeof(char *));
        filtros[num_filtros] = strdup(token);
        num_filtros++;
    }
    bubble_sort_lotes(sys->lotes, sys->numero_lotes);
    if (num_filtros == 0) {
        for (int i = 0; i < sys->numero_lotes; i++) {
            printf("%s %s %02d-%02d-%04d %d %d\n",
                   sys->lotes[i].nome, sys->lotes[i].lote,
                   sys->lotes[i].data.dia, sys->lotes[i].data.mes, sys->lotes[i].data.ano,
                   sys->lotes[i].doses_disponiveis, sys->lotes[i].doses_aplicadas);
        }
    } else {
        for (int i = 0; i < num_filtros; i++) {
            int found = 0;
            for (int j = 0; j < sys->numero_lotes; j++) {
                if (strcmp(sys->lotes[j].nome, filtros[i]) == 0) {
                    printf("%s %s %02d-%02d-%04d %d %d\n",
                           sys->lotes[j].nome, sys->lotes[j].lote,
                           sys->lotes[j].data.dia, sys->lotes[j].data.mes, sys->lotes[j].data.ano,
                           sys->lotes[j].doses_disponiveis, sys->lotes[j].doses_aplicadas);
                    found = 1;
                }
            }
            if (!found)
                printf("%s: %s\n", filtros[i], get_message(VACINAINEXISTENTE, VACINAINEXISTENTE_PT, sys));
            free(filtros[i]);
        }
        free(filtros);
    }
}



/**
 * @brief Aplica uma vacina a um utente, no estilo "a <utente> <vacina>"
 * @param sys sistema
 * @param in string de entrada
 * @return void
 */
void vacina_utente(Sys *sys, char *in) {
    char nome_utente[BUFMAX] = "", nome_vacina[BUFMAX] = "";
    char *p = in;
    if (*p != 'a') return;
    p++;
    while (isspace(*p)) p++;
    if (*p == '\"') {
        p++;
        char *q = strchr(p, '\"');
        if (!q) return;
        size_t len = q - p; if (len >= BUFMAX) len = BUFMAX - 1;
        strncpy(nome_utente, p, len); nome_utente[len] = '\0';
        p = q + 1; while (isspace(*p)) p++;
        sscanf(p, "%s", nome_vacina);
    } else {
        sscanf(p, "%s %s", nome_utente, nome_vacina);
    }
    
    Data hoje = sys->data_inicial;
    lote_de_vacina *melhor = NULL;
    for (int i = 0; i < sys->num_utentes; i++) {
        if (strcmp(sys->utentes[i].nome, nome_utente) == 0 &&
            sys->utentes[i].datavacinacao.dia == hoje.dia &&
            sys->utentes[i].datavacinacao.mes == hoje.mes &&
            sys->utentes[i].datavacinacao.ano == hoje.ano) {
            const char *vaccine_name = NULL;
            for (int j = 0; j < sys->numero_lotes; j++) {
                if (strcmp(sys->lotes[j].lote, sys->utentes[i].lote) == 0) {
                    vaccine_name = sys->lotes[j].nome; break;
                }
            }
            if (vaccine_name && strcmp(vaccine_name, nome_vacina) == 0) {
                printf("%s\n", get_message(VACINAPLICADA, VACINAPLICADA_PT, sys));
                return;
            }
        }
    }
    for (int i = 0; i < sys->numero_lotes; i++) {
        lote_de_vacina *lote = &sys->lotes[i];
        if (strcmp(lote->nome, nome_vacina) == 0 &&
            lote->doses_disponiveis > 0 &&
            validar_data(lote->data, hoje)) {
            if (melhor == NULL || compare_date(&lote->data, &melhor->data) < 0)
                melhor = lote;
        }
    }
    if (melhor) {
        melhor->doses_disponiveis--; melhor->doses_aplicadas++;
        if (sys->num_utentes == sys->cap_utentes) {
            int nova_cap = sys->cap_utentes * 2;
            Utente *temp = realloc(sys->utentes, nova_cap * sizeof(Utente));
            if (!temp) { fprintf(stderr, "%s\n", get_message(NOMEMORY, NOMEMORY_PT, sys)); exit(1); }
            sys->utentes = temp; sys->cap_utentes = nova_cap;
            for (int i = sys->num_utentes; i < sys->cap_utentes; i++) { sys->utentes[i].nome = NULL; }
        }
        if (sys->utentes[sys->num_utentes].nome != NULL) free(sys->utentes[sys->num_utentes].nome);
        size_t len = strlen(nome_utente);
        sys->utentes[sys->num_utentes].nome = malloc((len + 1) * sizeof(char));
        if (!sys->utentes[sys->num_utentes].nome) { fprintf(stderr, "%s\n", get_message(NOMEMORY, NOMEMORY_PT, sys)); exit(1); }
        strcpy(sys->utentes[sys->num_utentes].nome, nome_utente);
        snprintf(sys->utentes[sys->num_utentes].lote, MAXLENLOTE, "%s", melhor->lote);
        sys->utentes[sys->num_utentes].datavacinacao = hoje;
        sys->utentes[sys->num_utentes].numeroDeVacinacoes = 1;
        sys->num_utentes++;
        printf("%s\n", melhor->lote);
    } else {
        printf("%s\n", get_message(VACINAESGOTADA, VACINAESGOTADA_PT, sys));
    }
}



/**
 * @brief Lista os utentes, no estilo "u <utente>"
 * @param sys sistema
 * @param in string de entrada
 * @return void
 */
void listar_utentes(Sys *sys, char *in) {
    char filtro[BUFMAX] = "";
    char *p = in;

    if (*p != 'u')
        return;
    p++;
    while (isspace(*p))
        p++;

    if (*p == '\"') {
        p++;
        char *q = strchr(p, '\"');
        if (q) {
            size_t len = q - p;
            if (len >= BUFMAX)
                len = BUFMAX - 1;
            strncpy(filtro, p, len);
            filtro[len] = '\0';
        }
    } else {
        sscanf(p, "%s", filtro);
    }

    Utente filtered[sys->num_utentes];
    int indices[sys->num_utentes];
    int count = 0;

    for (int i = 0; i < sys->num_utentes; i++) {
        if (filtro[0] == '\0' || strcmp(sys->utentes[i].nome, filtro) == 0) {
            filtered[count] = sys->utentes[i];
            indices[count] = i;
            count++;
        }
    }

    if (filtro[0] != '\0' && count == 0) {
        printf("%s: %s\n", filtro, get_message(NOUSER, NOUSER_PT, sys));
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int swap = 0;
            if (filtered[i].datavacinacao.ano < filtered[j].datavacinacao.ano)
                swap = 1;
            else if (filtered[i].datavacinacao.ano == filtered[j].datavacinacao.ano) {
                if (filtered[i].datavacinacao.mes < filtered[j].datavacinacao.mes)
                    swap = 1;
                else if (filtered[i].datavacinacao.mes == filtered[j].datavacinacao.mes) {
                    if (filtered[i].datavacinacao.dia < filtered[j].datavacinacao.dia)
                        swap = 1;
                    else if (filtered[i].datavacinacao.dia == filtered[j].datavacinacao.dia) {
                        if (indices[i] < indices[j])
                            swap = 1;
                    }
                }
            }
            if (swap) {
                Utente temp = filtered[i];
                filtered[i] = filtered[j];
                filtered[j] = temp;
                int tmp = indices[i];
                indices[i] = indices[j];
                indices[j] = tmp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s %s %02d-%02d-%04d\n",
               filtered[i].nome,
               filtered[i].lote,
               filtered[i].datavacinacao.dia,
               filtered[i].datavacinacao.mes,
               filtered[i].datavacinacao.ano);
    }
}


/**
 * @brief Remove um lote, no estilo "r <lote>"
 * @param sys sistema
 * @param in string de entrada
 * @return void
 */
void remove_lote(Sys *sys, char *in) {
    char *token = strtok(in, " \t\n");
    if (!token || token[0] != 'r') return;
    token = strtok(NULL, " \t\n"); if (!token) return;
    char lote[BUFMAX];
    strncpy(lote, token, MAXLENLOTE - 1);
    lote[MAXLENLOTE - 1] = '\0';
    int found = 0;
    for (int i = 0; i < sys->numero_lotes; i++) {
        if (strcmp(sys->lotes[i].lote, lote) == 0) {
            found = 1;
            if (sys->lotes[i].doses_aplicadas == 0) {
                int applied = sys->lotes[i].doses_aplicadas;
                sys->lotes[i] = sys->lotes[sys->numero_lotes - 1];
                sys->numero_lotes--;
                printf("%d\n", applied);
            } else {
                sys->lotes[i].doses_disponiveis = 0;
                printf("%d\n", sys->lotes[i].doses_aplicadas);
            }
            break;
        }
    }
    if (!found)
        printf("%s: %s\n", lote, get_message(LOTEINEXISTENTE, LOTEINEXISTENTE_PT, sys));
}


/**
 * @brief Verifica se um utente existe
 * @param sys sistema
 * @param nome nome do utente
 * @return 1 se o utente existir, 0 caso contrario
 */
int verifica_utente_existe(Sys *sys, const char *nome) {
    for (int i = 0; i < sys->num_utentes; i++) {
        if (strcmp(sys->utentes[i].nome, nome) == 0)
            return 1;
    }
    return 0;
}


/**
 * @brief Verifica se um utente ja foi vacinado
 * @param sys sistema
 * @param nome nome do utente
 * @param dia dia da vacinacao
 * @param mes mes da vacinacao
 * @param ano ano da vacinacao
 * @return 1 se o utente ja foi vacinado, 0 caso contrario
 */
int verifica_data(Sys *sys, const char *nome, int dia, int mes, int ano) {
    for (int i = 0; i < sys->num_utentes; i++) {
        if (strcmp(sys->utentes[i].nome, nome) == 0) {
            if (sys->utentes[i].datavacinacao.dia == dia &&
                sys->utentes[i].datavacinacao.mes == mes &&
                sys->utentes[i].datavacinacao.ano == ano)
                return 1;
        }
    }
    return 0;
}


/**
 * @brief Remove utentes correspondentes
 * @param sys sistema
 * @param nome nome do utente
 * @param dia dia da vacinacao
 * @param mes mes da vacinacao
 * @param ano ano da vacinacao
 * @param lote lote da vacinacao
 * @return numero de utentes removidos
 */
int remove_utentes_correspondentes(Sys *sys, const char *nome, int dia, int mes, int ano, const char *lote) {
    int count = 0;
    for (int i = sys->num_utentes - 1; i >= 0; i--) {
        Utente *u = &sys->utentes[i];
        if (strcmp(u->nome, nome) == 0) {
            if ((dia == -1) ||
                (u->datavacinacao.dia == dia &&
                 u->datavacinacao.mes == mes &&
                 u->datavacinacao.ano == ano)) {
                if (lote[0] == '\0' || strcmp(u->lote, lote) == 0) {
                    free(u->nome);
                    sys->utentes[i] = sys->utentes[sys->num_utentes - 1];
                    sys->num_utentes--;
                    count++;
                }
            }
        }
    }
    return count;
}


/**
 * @brief Remove uma vacina, no estilo "d <utente> <data> <lote>"
 * @param sys sistema
 * @param in string de entrada
 * @return void
 */
void remove_vacina(Sys *sys, char *in) {
    char nome_utente[BUFMAX] = "", lote[MAXLENLOTE] = "";
    int dia = -1, mes = -1, ano = -1;
    
    char *token = strtok(in, " \t\n");
    if (!token || token[0] != 'd')
        return;
    
    token = strtok(NULL, " \t\n"); 
    if (!token)
        return;
    strncpy(nome_utente, token, BUFMAX - 1);
    nome_utente[BUFMAX - 1] = '\0';
    
    token = strtok(NULL, " \t\n");
    if (token && sscanf(token, "%d-%d-%d", &dia, &mes, &ano) == 3) {
        token = strtok(NULL, " \t\n");
        if (token) {
            strncpy(lote, token, MAXLENLOTE - 1);
            lote[MAXLENLOTE - 1] = '\0';
        }
    }
    
    int existe_utente = verifica_utente_existe(sys, nome_utente);
    
    if (dia != -1 && existe_utente && !verifica_data(sys, nome_utente, dia, mes, ano)) {
        printf("%s\n", get_message(DATAINVALIDA, DATAINVALIDA_PT, sys));
        return;
    }
    
    int count = remove_utentes_correspondentes(sys, nome_utente, dia, mes, ano, lote);
    
    if (count == 0) {
        if (!existe_utente)
            printf("%s: %s\n", nome_utente, get_message(NOUSER, NOUSER_PT, sys));
        else if (lote[0] == '\0')
            printf("%s: %s\n", nome_utente, get_message(LOTEINEXISTENTE, LOTEINEXISTENTE_PT, sys));
        else
            printf("%s: %s\n", lote, get_message(LOTEINEXISTENTE, LOTEINEXISTENTE_PT, sys));
    } else {
        printf("%d\n", count);
    }
}


/**
 * @brief Função principal
 * @param argc número de argumentos
 * @param argv argumentos
 * @return 0 se tudo estiver certo
 */
int main(int argc, char *argv[]) {
    char buf[BUFMAX];
    Sys sys;
    memset(&sys, 0, sizeof(Sys));
    sys.data_inicial.dia = 1; sys.data_inicial.mes = 1; sys.data_inicial.ano = 2025;
    sys.numero_lotes = 0;
    sys.idioma = (argc > 1 && strcmp(argv[1], "pt") == 0) ? 1 : 0;
    sys.cap_utentes = 50; sys.num_utentes = 0;
    sys.utentes = malloc(sys.cap_utentes * sizeof(Utente));
    if (!sys.utentes) {
        fprintf(stderr, "%s\n", get_message(NOMEMORY, NOMEMORY_PT, &sys));
        exit(1);
    }
    for (int i = 0; i < sys.cap_utentes; i++) sys.utentes[i].nome = NULL;
    
    while (fgets(buf, BUFMAX, stdin)) {
        char *line = buf;
        while (isspace(*line)) line++;
        if (*line == '\0') continue;
        char command = line[0];
        switch (command) {
            case 'q': 
                for (int i = 0; i < sys.num_utentes; i++) free(sys.utentes[i].nome);
                free(sys.utentes);
                return 0;
            case 'c': create_lote(&sys, line); break;
            case 'l': listar_lotes(&sys, line); break;
            case 'a': vacina_utente(&sys, line); break;
            case 'r': remove_lote(&sys, line); break;
            case 'u': listar_utentes(&sys, line); break;
            case 't': nova_data(&sys, line); break;
            case 'd': remove_vacina(&sys, line); break;
        }
    }
    
    for (int i = 0; i < sys.num_utentes; i++) free(sys.utentes[i].nome);
    free(sys.utentes);
    return 0;
}