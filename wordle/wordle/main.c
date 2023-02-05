#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#define LENGTH 100
#define HEIGHT 16


/* \brief Declararea variabilelor care contin lungimea maxima si minima a unui cuvant. -> LUNG_MAX LUNG_MIN
 *
 */
unsigned LUNG_MAX, LUNG_MIN;

/* \brief Subprogramul de setare a lungimii. -> ( set_lung() )
 *
 * Subprogramul va deschide fisierul setari.txt si va extrage valorile corespunzatoare
 * pentru lungimea maxima si respectiv minima a unui cuvant.
 */
void set_lung(){
    FILE * setari = fopen("setari.txt", "r");

    char parametru[10];
    fscanf(setari, "%s %u", parametru, &LUNG_MAX);
    fscanf(setari, "%s %u", parametru, &LUNG_MIN);

}

/* \brief Subprogramul de citire a cuvintelor. -> ( citire_cuv(char* cuv) )
 *
 * Acest subprogram citeste de la tastatura cuvinte introduse si le evalueaza astfel incat
 * acestea sa aiba marimea corespunzatoare.
 *
 * In cazul in care cuvantul introdus este prea lung sau prea scurt, subprogramul
 * va afisa mai intai un text de eroare si apoi se va reapela.
 */
void citire_cuv(char* cuv){ //mai convenabil deoarece deja stim ca lungimea maxima este de 7 litere
    scanf("%s", cuv);
    printf("\n\n");

    if (strlen(cuv) > LUNG_MAX) { //restrictionarea lungimii cuvantului
        printf("Cuvantul este prea mare, el trebuie sa aiba intre 4 si 7 litere\nIntrodu altul\n\n");
        citire_cuv(cuv);
    }
    else if(strlen(cuv) < LUNG_MIN) {
        printf("Cuvantul este prea mic, el trebuie sa aiba intre 4 si 7 litere\nIntrodu altul\n\n");
        citire_cuv(cuv);
    }
}

/* \brief Citirea unui cuvant cand scopul este de a ghici altul. -> ( citire_incerc(char* cuv, unsigned lung) )
 *
 * Subprogramul va compara lungimea cuvantului care trebuie ghicit cu cel introdus si se va reapela
 * daca lungimile nu coincid
 */
void citire_incerc(char* cuv, unsigned lung){ //citirea si restrictionarea cuvantului incercat
    scanf("%s", cuv);
    printf("\n\n");

    if (strlen(cuv) != lung) { //restrictionarea lungimii cuvantului
        printf("Cuvantul cautat are %u litere\nIntrodu altul\n\n", lung);
        citire_incerc(cuv,lung);
    }
}

/* \brief Citirea unui cuvant in mod dinamic. -> ( citire_cuv_dinamic() )
 *
 * Un subprogram prototip care citeste un sir de caractere de nr necunoscut de litere si il stocheaza
 * in memorie. Am renunat insa la ideea aceasta deoarece stiind deja restrictiile de lungime am putut
 * scrie un subprogram mai simplu de citire.
 */
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

/* \brief Retinerea unui cuvat in biblioteca. -> ( retine_cuv(char* cuv) )
 *
 *
 */
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

/* \brief Verificarea existentei cuvantului in biblioteca. -> ( exista_cuvantul(char* cuv) )
 *
 * Aloca in HEAP memorie pt un sir de caractere de lungimea nr de caractere din biblioteca.
 * Apoi cauta daca un cuvant exista deja in sir, adica in biblioteca folosind strstr.
 */
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

/* \brief Subprogramu care inumara nr de cuvinte din biblioteca. -> ( nr_cuvinte() )
 *
 */
unsigned nr_cuvinte(){ //inumara cate cuvinte sunt in biblioteca folosind ca si contor caracterul '\n'
    unsigned cont=0;
    FILE * biblioteca;
    biblioteca = fopen("biblioteca.txt", "r");
    fseek(biblioteca, 0, SEEK_END);
    unsigned poz = ftell(biblioteca);
    fseek(biblioteca, 0 , SEEK_SET);

    for(unsigned i=0;i<=poz;i++){
        char c;
        fscanf(biblioteca, "%c", &c);
        if(c == '\n') cont++;
        fseek(biblioteca, i, SEEK_SET);
    }

    return cont/2;
}

/* \brief Generarea unui cuvant random. -> ( random_nr() )
 *
 */
unsigned random_nr(){ //generator de nr luat la intamplare
    time_t t;

    srand((unsigned) time(&t));

    return ((unsigned)rand());
}

/* \brief Generarea unui cuvant random intr-un interval [min , range+min]. -> ( random_in_range() )
 *
 */
unsigned random_in_range(unsigned range, unsigned min){ //generator de nr luat la intmplare int-un interval [min, min+range]
    return (random_nr() % range + min);
}

/* \brief Cautarea unui cuvant anume in biblioteca. -> ( cautare_cuv_index(unsigned index) )
 *
 */
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

/* \brief Fuziunea a 3 alte subprograme care are ca rezultat extragerea unui cuvant random din biblioteca. -> ( select_cuv_random() )
 *
 */
char *select_cuv_random(){ //luarea unui cuvant aleatoriu din fisierul text

    return cautare_cuv_index( random_in_range( nr_cuvinte() , 0 ) );
}

/* \brief Algoritmul principal al jocului de ghicit. -> ( alg_joc(char* word) )
 *
 * Primeste un cuvant din biblioteca si acorda incercari in functie de lungimea cuvantului de ghicit.
 * 4 incercari pentru 4 litere
 * 6 incercari pentru 5 litere
 * 8 incercari pentru 6 litere
 * 10 incercari pentru 7 litere
 *
 * La fiecare incercare daca nu este cuvantul corect cel introdus acesta va fi afisat colorat astfel:
 * ->cu rosu vor fi literele care nu fac parte din cuvantul cautat
 * ->cu galben vor fi literele care fac parte din cuvantul cautat dar nu sunt pe pozitia corecta
 * ->cu verde vor fi literele care fac parte din cuvantul cautat si sunt pe pozitia corecta
 */
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

/* \brief Meniul si modurile jocului. -> ( moduri_joc() )
 *
 * Pentru navigare se va introduce de la tastatura cifra corespunzatoare modului dorit.
 * Exista 3 moduri de joc:
 * 1.Cuvant aleatoriu
 * 2.Introduce cuvinte in biblioteca
 * 3.Lasa pe altcineva sa introduca un cuvant pe care sa il ghicesti
 */
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

    set_lung();

    moduri_joc();

	return 0;
}
