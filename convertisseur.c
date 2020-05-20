
/*Inclusion des bibliotheques*/
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

/*Definition des structures noeuds*/
typedef struct node_s{
	char symbol;			//un caractère pour stoquer la lettre
	int freq;				// un entier pour stoquer la fréquence
	struct node_s *fg;		// pointeur vers le fils gauche 
	struct node_s *fd;		// pointeur vers le fils droit  
}node;	

/*Definition des structures tas */
typedef struct tas_s{
	int m;				//un entier pour la taille max du tableau 
	int n;				//un entier pour le nombre d'élements actuels 
	node **tab;			// tableau d'adresse de noeuds
}tas;

/*Element Bonus*/
typedef struct Element Element;
struct Element 
{
	char symb;
	int code;
	Element *suivant;
};

typedef struct Dliste Dliste;
struct Dliste
{
	int taille; 
	Element *premier;
};



/*Entête des fonctions utilisées dans le programme*/

node *cree_node(char c, int a);
tas *inic_tas(int m);
int est_vide_tas(tas *t);
void swap(node** x,node** y);
void inserer_tas(tas *t, node *n);
void imprimer_tas(tas *t);
tas *saisie_alphabet();
node *supprimer_tas(tas *t);
node *creer_arbres(tas *t);
int est_feuille(node* n);
void imprimer_arbre(node *n);
void imprimer_code(node *n,char *chaine, int i);
void supprimer_arbre(node *n);
void libere_memoire_tas(tas *t);
void inser_element(Dliste *l, int code, char symb);
void affiche_liste(Dliste *l);
void saisie_utilisateur(Dliste *l);
int cherche_code(Dliste *l, char c);
void print_result(char *s,Dliste *l);
Dliste* creer_liste();
void print_depuis_fichier(Dliste *l);
void free_liste(Dliste *l);


/* fonction main */
int main(){
	int n;
	int a;
	tas *t = saisie_alphabet();		// initialisation du tas principal grâce a la fonction saisie_alphabet()
	imprimer_tas(t);				// affiche le tas saisi précedemment 
	printf("creer arbre\n");			// un affichage  
	node* b =creer_arbres(t);		// création de l'arbre grâce à la fonction creer_arbres()
	imprimer_arbre(b);				// impréssion de l'arbre précedemment crée
	char chaine[100] = "";			// initialisation d'une chaine de caractère pour l'utiliser dans la fonction imprimer_code()
	imprimer_code(b,chaine,0);		// appelle de la fonction imprimer_code() 
	printf("\n");
	supprimer_tas(t);				//libère l'éspace mémoire alloué	
	if(t != NULL)
		printf("reussi 1 \n");
	supprimer_arbre(b);
	if(b != NULL)
		printf("reussi 2 \n");
	
	printf("Voulez vous lancer les fonctions bonus\n Oui : 1\n non : 2 \n");
	scanf("%d",&a);
	if(a == 1){
		printf("Lancement des fonction Bonus\n");
		Dliste *l = creer_liste();
		saisie_utilisateur(l);
		printf("********************************************\n");
		printf("* 1/ pour saisir la chaine de caractère    *\n");
		printf("* 2/ pour importer un fichier              *\n");
		printf("********************************************\n");
		scanf("%d",&n);
		if(n == 1){
			char chaine[1000];
			printf("veuillez saisir votre chaine de caractère : \n");
			scanf("%s",chaine);
			print_result(chaine,l);
		}
		if(n == 2){
			print_depuis_fichier(l);
		}
		free_liste(l);
	}
	
	return 0;

}


/*corps de la fonction cree_node()*/
node *cree_node(char c, int a){					//prend en paramètre un caractère (la lettre de l'alphabet) et un entier(la fréquence)
	node* n= (node*)malloc(sizeof(node));			//on alloue de la mémoire pour le noeud
	n->symbol = c;						//affectation du symbole mis en parametre
	n->freq = a;						//affectation de la frequence mis en parametre
	n->fg= NULL;						// initialisation du fils gauche
	n->fd=NULL;						// initialisation du fils droit 
	return n;						// retourne le noeud précedemment crée
}


/*corps de la fonction cree_node()*/
tas *inic_tas(int m){						// prend en paramètre la taille du tas 
	tas *newt = (tas*)malloc(sizeof(tas));			// on alloue de la mémoire pour le tas
	newt->tab = (node**)malloc(m*sizeof(node*));		// on alloue la mémoire pour le tableau d'adresse
	newt->m = m;						// initialisation de la taille du tas avec la variable rentrée en paramètre
	newt->n = 0;						// initialisation du nombre d'élements à 0
	return newt;						// retourne le tas crée
}


/*corps de la fonction est_vide_tas()*/
/*retourne 1 si le tas est vide 
	0 si non*/
int est_vide_tas(tas *t){    						// prend en paramètre le un tas
	if(t->n == 0)							// la condition pour vérifier si le tas est vide grâce au nombre d'élement 
		return 1;						// retourne 1 s'il rentre de la condition 
	return 0;							// retourne 0 si non
}

/*corps de la fonction swap()
	pour interchanger 2 noeuds entre eux*/

void swap(node** x,node** y){						// prend en paramètre les 2 adresses des 2 noeuds
	node* tmp = *x;							// variable temporaire ou on affecte x 
	*x = *y;							// affectation de y dans x
	*y = tmp;							// affectation de tmp dans y 
}

/*corps de la fonction inserer_tas()
	qui prend en parametre t et un noeud*/

void inserer_tas(tas *t, node *n){
	
	if(t->m == t->n){								// Si la taille n'est pas suffisante on la double
		t->m *=2;								// On double la taille
		t->tab = realloc(t->tab,t->m*sizeof(node*));				// utilisation de la fonction realloc pour realouer la memoire
	} 
	t->tab[t->n] = n;								// Affectation de la taille n 
	int i = t->n;									// I prend la valeur de la taille (element actuel)
	int j = (i-1)/2;								// J prend la valeur du pere
	while(t->tab[i]->freq < t->tab[j]->freq && j>=0){
		swap(&t->tab[j], &t->tab[i]);						// echange l'element et son pere si l'element actuel est plus petit 
		i = j;									// L'element devient le pere
		j = (i-1)/2;								// le pere devient lui meme son propre pere
	}
	t->n++;										// on incremente la taille
		
	
		
}

/*Corps de la fonction imprimer tas*/
void imprimer_tas(tas *t){					
	if(t == NULL){									// Si le tas est null on quitte le programme
		printf("NULL\n");
		exit(EXIT_FAILURE) ;							//  Exit failure quitte le programme
	}
	for(int i = 0 ; i < t->n;i++){							// Pour chaque element du tas on affiche son symbole et sa frequence
		printf("char = %c | frequence = %d\n",t->tab[i]->symbol, t->tab[i]->freq);
	}
}


/*Corps de la fonction saisie_alphabet*/
tas *saisie_alphabet(){
	int n;											// Declaration de n le nombre de saisies
	int freq;										// Declaration de la frequence saisies
	char c;											// Declaration du caractere saisie
	node *ne = NULL;									// Declaration d'un pointeur null

	printf("saisir le nombre de lettres de votre alphabet : \n");
	scanf("%d",&n);									// saisie du nombre d'alphabet
	tas *t= inic_tas(n);								// initialise le tas
	imprimer_tas(t);								// imprime le tas initialiser precedemment 

	for(int i =0;i<n;i++){								// On remplie n fois la frequence et le symbole voulu puis
		//imprimer_tas(t);
		printf("saisissez la lettre numero %d  :",i+1);
		scanf("%s",&c);
		printf("saisissez sa fréquence : ");
		scanf("%d",&freq);
		ne = cree_node(c,freq);							// On crée le node avec le caractere et sa frequence
		inserer_tas(t ,ne);							// On insere dans le tas
		ne = NULL;								// On vide le contenu du noeud pour eviter une erreur lors de l'execution par simple précaution
	}
	return t;									// on renvoie le tas créer par l'alphabet saisie
}


/* Corps de la fonction supprimer_tas */
node *supprimer_tas(tas *t){

	node* temp=t->tab[0];							// On stoque la racine qui sera supprimée
	t->tab[0]=t->tab[t->n-1];						// le dernier élément du tas prend la place de la racine 
	t->n--;							// On decremente le nombre d'elemenet actuel du tas
	int i = t->n;								// i prend l'element actuel
	int j = (i-1)/2;							// j prend le pere de l'element actuel
	if(t->tab[0]==NULL)							// Si le tas est null alors on renvoie null
	{
		return NULL;
	}
	while(t->tab[i]->freq < t->tab[j]->freq && j>=0){			// Si l'element est plus petit que son pere on swipe le pere et son fils
		swap(&t->tab[j], &t->tab[i]);
		i = j;								
		j = (i-1)/2;
	}
	if(t->n<=1)								/* Si le tas contient un seul élément alors on le renvoi*/
	{
		return temp;
	}
	if(t->tab[0]->freq > t->tab[1]->freq || t->tab[0]->freq > t->tab[2]->freq)	// Lors du tri du tas on s'est rendu compte que la fonction ne trier pas le premier sous arbre (racine)
	{
		if(t->tab[1]->freq < t->tab[2]->freq)
		{												// Donc on verifie l'élément avec la plus petite frequence 
		
			swap(&t->tab[0],&t->tab[1]);					// On swipe l'élément le plus petit avec la racine
		}
		else{
			swap(&t->tab[0],&t->tab[2]);
		}
	}
	return temp;
										/* On renvoi l'élément supprimer*/
}

/*Corps de la fonction creer arbres*/
node *creer_arbres(tas *t){
	int n= t->n;								// On applique l'agorithme du sujet , on stocke la taille du tas
	tas *F=inic_tas(t->m);							// On initialise le tas copie
	int  i;									// On déclare la variable de la boucle
	for(i =0;i<n;i++)							// Création du tas de l'arbre
	{
		inserer_tas(F, t->tab[i]);
	}
										
	for(i=0;i<n-1;i++)							// 
	{
		node *z= cree_node('\0',0);					// Creation d'un noeud vide
		node *x=supprimer_tas(F);					// On récupere les deux minimum du tas x et y 
		node *y=supprimer_tas(F);
		z->fg=x;							// x devient son fils gauche
		z->fd=y;							// y devient son fils droit
		z->freq = x->freq+y->freq;					// La frequence de z devient la somme des frequence de x et y 
		inserer_tas(F,z);						// Puis on insere dans le tas le nouveau noeud
		//imprimer_arbre(z);
	}
	return supprimer_tas(F);						// On renvoie la racine de l'arbre

}

/*Corps de la fonction est feuille*/
int est_feuille(node* n)						
{
	if(n->fd== NULL && n->fg == NULL)					// Si ses deux fils sont null alors c'est une feuille
	{	
		return 1;							// on retourne 1
	}
	return 0;								// sinon on retourne 0
}

/*Corps de la fonction imprimer_code*/
void imprimer_code(node *n,char *chaine, int i)
{
		if(n->fg != NULL){						// Si le fils gauche est non null on rajoute 0 a la chaine de caractere correspondant au "parcour" de l'arbre pour une feuille
			chaine[i]='0';
			imprimer_code(n->fg,chaine,i+1);			// Ensuite on continue le chemin et on incrémente i+1 pour acceder a la case suivante de la chaine de caractere
		}
		
		if( (est_feuille(n)) == 1){					// On verifie si le noeud est une feuille dans ce cas là on affiche son code correspondant
			printf("%c = %s\t",n->symbol, chaine);
			chaine[i] = '\0';					
		}
		
		if(n->fd != NULL){						// Si le fils droit est non null on rajoute 0 a la chaine de caractere correspondant au "parcour" de l'arbre pour une feuille
			chaine[i] = '1';
			imprimer_code(n->fd,chaine,i+1);			// Ensuite on continue le chemin et on incrémente i+1 pour acceder a la case suivante de la chaine de caractere
		}
	

	
}

/*Corps d'imprimer_arbres*/		
void imprimer_arbre(node *n)
{
	if(n !=NULL)								// On veut un parcour infixe pour cela
	{
		imprimer_arbre(n->fg);						// On commence par imprimer la partie gauche de l'arbre 
		printf("(%c)-(%d) \n",n->symbol,n->freq);			// Affichage de la frequence et du symbol du noeud 
		imprimer_arbre(n->fd);						// On imprime les fils droits
	}
	
}

/*Corps de la fonction supprimer_arbre*/
void supprimer_arbre(node *n)
{
	if(n!=NULL)								// Si le noeud n'est pas null alors on supprime le noeud
	{
		supprimer_arbre(n->fg);						// appel récursif					
		supprimer_arbre(n->fd); 					// appel récursif
		free(n);							// Libere la mémoire du noeud
	}
}

/*Corps de la focntion supprimer_tas*/
void liberer_memoire_tas(tas *t)
{
	int i;									// Déclaration de la variable de la boucle 
	for(i=0;i<t->n-1;i++)							// On libere l'espace de chaque case du tableu du tas
	{
		free(t->tab[i]);
		
	}
	free(t);								// On libere l'espace du tas
}

/*Fonction bonus*/

Dliste *creer_liste()
{
	Dliste *liste = malloc(sizeof(Dliste));
	
	if(liste == NULL)
		exit(EXIT_FAILURE);
	liste->taille = 0;
	liste->premier = NULL;
	return liste;
}

void inser_element(Dliste *l, int code, char symb)
{
	Element *element = malloc(sizeof *element);
	if(l == NULL)
		exit(EXIT_FAILURE);
	else
	{
		element->code = code;
		element->symb = symb;
		element->suivant = l->premier;
		l->premier = element;
		l->taille ++;
	}
}



void affiche_liste(Dliste *l)	
{
	Element *element = malloc(sizeof(Element));
	if(l == NULL){
		exit(EXIT_FAILURE);
	}
	else{
		
		element = l->premier;
		while(element != NULL){
			printf("char : %c | code : %d \n",element->symb,element->code);
			element = element->suivant;
		}
	}
	free(element);
}
	

void saisie_utilisateur(Dliste *l)
{
	int n;
	int code;
	char symb;
	printf("de combien de lettre se compose votre alphabet : ");
	scanf("%d",&n);
	for(int i =1; i<=n; i++){
		printf("saisissez la lettre numero %d  :",i	);
		scanf("%s",&symb);
		printf("saisissez son code : ");
		scanf("%d",&code);
		inser_element(l,code,symb);	

	}
}
int cherche_code(Dliste *l, char c)
{
	int code;
	int cmp=0;
	Element *e = malloc(sizeof(Element));
	e = l->premier;
	while(e != NULL){
		if (e->symb == c){
			code = e->code;
			cmp++;
			break;
		}
		else
			e = e->suivant;
	}
	if(cmp==0){
		code = 9;
	}
	e=NULL;
	free(e);
	return code;
}

void print_result(char *s,Dliste *l)
{
	int code;
	int taille = strlen(s);
	FILE *fichier = NULL;
	fichier = fopen("resultat.txt","a+");
	if(fichier == NULL)
		printf("Impossible d'ouvrir le fichier \n");
	else{
		for(int i =0; i< taille; i++)
		{
			code = cherche_code(l,s[i]);
			printf("%d",code);
			fprintf(fichier,"%d",code);
		
		}
		printf("\n");
		printf("le fichier qui contient le code est resultat.txt \n");
		fclose(fichier);
	}
}

void print_depuis_fichier(Dliste *l){

	int code;
	char rep[20];
	FILE *fichier = NULL;
	FILE *fic = NULL;
	printf("donner le nom du fichier a convertir : \n");
	scanf("%s",rep);
        int caractereActuel = 0;
        fichier = fopen(rep, "r");
	fic = fopen("text.txt", "a+");
        if (fichier != NULL)
        {
        	do
       		{
        	    caractereActuel = fgetc(fichier); // On lit le caractère
        	    code = cherche_code(l,caractereActuel);
		    printf("%d",code);
		    fprintf(fic,"%d",code);

        	} while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)

	}
	printf("\n");
        printf("le fichier qui contient le code est text.txt \n");
	fclose(fichier);
	fclose(fic);
}

void free_liste(Dliste *l){

	Element *element = malloc(sizeof(Element));
	Element *tmp = malloc(sizeof(Element));
	if(l == NULL){
		exit(EXIT_FAILURE);
	}
	else{
		
		element = l->premier;
		while(element != NULL){
			tmp = element;
			element = tmp->suivant;
			free(tmp);
		}
	}
	free(element);
}


		



	



















