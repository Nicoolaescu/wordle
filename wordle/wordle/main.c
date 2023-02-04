#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#define LUNG_MAX 7
#define LENGTH 100
#define HEIGHT 16



void citire_cuv(char* cuv){ //mai convenabil deoarece deja stim ca lungimea maxima este de 7 litere
    scanf("%s", cuv);
    printf("\n\n");

    if (strlen(cuv) > 7) { //restrictionarea lungimii cuvantului
        printf("Cuvantul este prea mare, el trebuie sa aiba intre 4 si 7 litere\nIntrodu altul\n\n");
        citire_cuv(cuv);
    }
    else if(strlen(cuv) < 4) {
        printf("Cuvantul este prea mic, el trebuie sa aiba intre 4 si 7 litere\nIntrodu altul\n\n");
        citire_cuv(cuv);
    }
}

void citire_incerc(char* cuv, unsigned lung){ //citirea si restrictionarea cuvantului incercat
    scanf("%s", cuv);
    printf("\n\n");

    if (strlen(cuv) != lung) { //restrictionarea lungimii cuvantului
        printf("Cuvantul cautat are %u litere\nIntrodu altul\n\n", lung);
        citire_incerc(cuv,lung);
    }
}

char* citire_cuv_dinamic(){ //citire a unui cuvant de nr necunoscut de litere folosind alocare dinamica

	char *cuv = malloc(sizeof(char));
	char c;
	int lung = 0;
	cuv[lung] = '\0';

	do{
		scanf("%c", &c);
		if(c == '\n') break;
		cuv[lung] = c;
		lung ++;
		cuv = realloc(cuv, sizeof(char) * lung);
		cuv[lung] = '\0';

	}while(c != '\n');

	return cuv;
}

void retine_cuv(char *cuv){ //scrie un cuvant introdus in biblioteca de cuvinte deja retinute
    FILE * biblioteca;
    biblioteca = fopen("biblioteca.txt", "a");

    if (biblioteca == NULL) {
    printf("Fisierul nu poate fi creat.");
    }
    else {
    fprintf(biblioteca, "%s\n", cuv); //scrie cuvantul nou;
    fclose(biblioteca);
    }
}

int exista_cuvantul(char *cuv){ //verifica daca un cuvant anume este deja in biblioteca
    FILE * biblioteca;
    biblioteca = fopen("biblioteca.txt", "r");

    fseek(biblioteca, 0, SEEK_END); //punem cursorul la ultimul element
    int poz = ftell(biblioteca);

    fseek(biblioteca, 0 , SEEK_SET); //punem cursorul la elementul cu indexul 0 din fisier

    char *sir = malloc(poz * sizeof(char));

    int i=0;
    while(fscanf(biblioteca,"%c", &sir[i]) != EOF) i++;
    sir[i] = '\0';

    if(strstr(sir, cuv) == NULL) return 0;
    else return 1;
}

unsigned nr_cuvinte(){ //inumara cate cuvinte sunt in biblioteca folosind ca si contor caracterul '\n'
    unsigned cont=0;
    FILE * biblioteca;
    biblioteca = fopen("biblioteca.txt", "r");
    fseek(biblioteca, 0, SEEK_END);
    int poz = ftell(biblioteca);
    //printf("%d", poz);
    fseek(biblioteca, 0 , SEEK_SET);

    for(int i=0;i<=poz;i++){
        char c;
        fscanf(biblioteca, "%c", &c);
        if(c == '\n') cont++;
        fseek(biblioteca, i, SEEK_SET);
    }

    return cont/2;
}

unsigned random_nr(){ //generator de nr luat la intamplare
    time_t t;

    srand((unsigned) time(&t));

    return ((unsigned)rand());
}

unsigned random_in_range(unsigned range, unsigned min){ //generator de nr luat la intmplare int-un interval [min, min+range]
    return (random_nr() % range + min);
}

char *cautare_cuv_index(unsigned index){ //cauta cuvantul din fisierul biblioteca.txt cu indexul specificat
    char *word = malloc(LUNG_MAX * sizeof(char));
    FILE * biblioteca = fopen("biblioteca.txt", "r");

    if(index == 0) fscanf(biblioteca,"%s", word);
    else{
        unsigned cont=0;
        fseek(biblioteca, 0, SEEK_END);
        int poz = ftell(biblioteca);

        fseek(biblioteca, 0 , SEEK_SET);

        for(int i=0;i<=poz;i++){
            char c;
            fscanf(biblioteca, "%c", &c);
            if(c == '\n') cont++;
            fseek(biblioteca, i, SEEK_SET);
            if(cont / 2 == index) {
                fscanf(biblioteca, "%s", word);
                return word;
            }
        }
    }

    return word;
}

char *select_cuv_random(){ //luarea unui cuvant aleatoriu din fisierul text

    return cautare_cuv_index( random_in_range( nr_cuvinte() , 0 ) );
}

void alg_joc(char *word){
    unsigned incerc,lung=strlen(word);
    if(lung == 4) incerc = 4;
    else if(lung == 5) incerc = 6;
    else if(lung == 6) incerc = 8;
    else if(lung == 7) incerc = 10;

    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE); //SetConsoleTextAttribute(console_color, P);

    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    GetConsoleScreenBufferInfo(console_color, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;

    printf("Un cuvant de %d litere a fost selectat, ai %d incercari sa il ghicesti\n\n", lung, incerc);

    for(int i=0; i<incerc; i++){
        char cuv[LUNG_MAX];
        citire_incerc(cuv,lung);

        if(strcmp(word,cuv) == 0) {
            SetConsoleTextAttribute(console_color, wOldColorAttrs);

            printf("Ai ghicit cuvantul!\n\n");
            break;
        }
        else if(i == incerc-1) {
            SetConsoleTextAttribute(console_color, wOldColorAttrs);

            printf("Nu ai ghicit cuvantul, cuvantul cautat era ");
            SetConsoleTextAttribute(console_color, 41);
            printf("%s", word);
            SetConsoleTextAttribute(console_color, wOldColorAttrs);
            printf("!!!\n\n");
            break;
        }
        else {
            for(int j=0;j<lung;j++){
                if(word[j] == cuv[j]) {
                    SetConsoleTextAttribute(console_color, 42);
                    printf("%c", cuv[j]);
                    SetConsoleTextAttribute(console_color, wOldColorAttrs);
                }
                else if(strchr(word, cuv[j]) != NULL) {
                    SetConsoleTextAttribute(console_color, 46);
                    printf("%c", cuv[j]);
                    SetConsoleTextAttribute(console_color, wOldColorAttrs);
                }
                else if(strchr(word, cuv[j] == NULL)) {
                    SetConsoleTextAttribute(console_color, 44);
                    printf("%c", cuv[j]);
                    SetConsoleTextAttribute(console_color, wOldColorAttrs);
                }
            }
            printf("\n");
        }
    }

}

void moduri_joc(){ //interfata si modurile de joc

    printf("Alege modul de joc:\n1.Cuvant aleatoriu\n2.Introduce tu un cuvant care sa fie retinut in biblioteca\n3.Lasa pe altcineva sa introduca un cuvant\n4.Exit game\n\n");

    unsigned mod;
    scanf("%u", &mod);
    printf("\n\n");

    if (mod == 1){
        printf("Un cuvant aleatoriu din biblioteca va fi selectat, inceraca sa-l ghicesti!\n\n");
        alg_joc(select_cuv_random());

        printf("Apasa 1 sa te intorci la meniu sau 2 ca sa iesi din joc\n\n");
        unsigned mod2;
        scanf("%u", &mod2);
        printf("\n\n");

        if(mod2 == 2) return 0;
        else if(mod2 == 1) moduri_joc();
    }
    else if (mod == 2){
        while(1){
            printf("Introduceti un cuvant\n");

            char cuv[LUNG_MAX];
            citire_cuv(cuv);

            if(exista_cuvantul(cuv) == 0) {
                retine_cuv(cuv);
                printf("Cuvantul a fost introdus\n\n");
                break;
            }
            else {
                printf("Exista cuvantul\n");
                printf("Apasa 1 sa reincerci sau 2 ca sa iesi\n\n");

                unsigned mod2;
                scanf("%u", &mod2);
                printf("\n\n");
                if(mod2 == 2) break;
                else if(mod2 == 1) continue;
            }
        }

        moduri_joc();
    }
    else if (mod == 3) {
        while(1){
            printf("Introduceti un cuvant\n");

            char cuv[LUNG_MAX];
            citire_cuv(cuv);

            if(exista_cuvantul(cuv) == 0) {
                retine_cuv(cuv);

                printf("Un cuvant a fost introdus, inceraca sa-l ghicesti!\n\n");
                alg_joc(cuv);

                printf("Apasa 1 sa te intorci la meniu sau 2 ca sa iesi din joc\n\n");
                unsigned mod2;
                scanf("%u", &mod2);
                printf("\n\n");

                if(mod2 == 2) return 0;
                else if(mod2 == 1) moduri_joc();
            }
            else {
                printf("Un cuvant a fost introdus, inceraca sa-l ghicesti!\n\n");
                alg_joc(cuv);

                printf("Apasa 1 sa te intorci la meniu sau 2 ca sa iesi din joc\n\n");
                unsigned mod2;
                scanf("%u", &mod2);
                printf("\n\n");

                if(mod2 == 2) return 0;
                else if(mod2 == 1) moduri_joc();
            }
        }
        moduri_joc();
    }
    else if (mod == 4) exit;
}

int main() {

    moduri_joc();

	return 0;
}
