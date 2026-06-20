#include<stdio.h>
#include<stdlib.h>

typedef struct polynome{
    int degre;
    float* tab_coef;
}polynome;

void saisie_poly(polynome* P){
    printf("Entrez un degre :\n");
    scanf("%d",&P->degre);
    P->tab_coef = malloc((P->degre+1) * sizeof(float));
    for (int i=0; i<=P->degre;i++){
        printf("Entrez le coefficient %d :\n",i);
        scanf("%f",&P->tab_coef[i]);
    }
}

void affichage_poly(polynome P){
    printf("P(x) = ");
    printf("%.2f ",P.tab_coef[0]);
    for (int i=1; i<=P.degre; i++){
        if (P.tab_coef[i]==0.0){
        }
        else if (i==1 && !(P.tab_coef[i]==1.0)){
            printf("+ %.2fx ",P.tab_coef[1]);
        }
        else if (P.tab_coef[i]<0){
            printf("%.2fx^%d ",P.tab_coef[i],i);
        }
        else if (P.tab_coef[i]==1){
            printf("+ x^%d ",i);
        }
        else{
            printf("+ %.2fx^%d ",P.tab_coef[i],i);
        }

    }
    printf("\n");
}

int max(int a, int b){
    if (a>b){
        return a;
    }
    else{
        return b;
    }
}

void actualiser_degre(polynome *P){
    while (P->degre > 0 && P->tab_coef[P->degre] == 0){
        P->degre--;
    }
}

void free_polynome(polynome *P){
    if (P->tab_coef != NULL){
        free(P->tab_coef);
        P->tab_coef = NULL;
    }
    P->degre = 0;
}

polynome somme_polynome(polynome P, polynome Q){
    polynome R;
    R.degre=max(P.degre,Q.degre);
    R.tab_coef=malloc((R.degre+1)*sizeof(float));
    for (int i=0;i<=R.degre;i++){
        if (i>P.degre){
            R.tab_coef[i]=Q.tab_coef[i];
        }
        else if (i>Q.degre){
            R.tab_coef[i]=P.tab_coef[i];

        }
        else{
            R.tab_coef[i]=P.tab_coef[i]+Q.tab_coef[i];
        }
    }
    return R;
}


polynome difference_polynome(polynome P, polynome Q){
    polynome R;
    R.degre=max(P.degre,Q.degre);
    R.tab_coef=malloc((R.degre+1)*sizeof(float));
    for (int i=0;i<=R.degre;i++){
        if (i>P.degre){
            R.tab_coef[i]=-Q.tab_coef[i];
        }
        else if (i>Q.degre){
            R.tab_coef[i]=P.tab_coef[i];

        }
        else{
            R.tab_coef[i]=P.tab_coef[i]-Q.tab_coef[i];
        }
    }
    actualiser_degre(&R);
    return R;
}

polynome produit_polynome(polynome P, polynome Q){
    polynome R;
    R.degre=P.degre + Q.degre;
    R.tab_coef=malloc((R.degre+1)*sizeof(float));
    for (int i=0; i<=R.degre; i++){
        R.tab_coef[i] = 0;
    }
    for (int i=0; i<=P.degre; i++){
        for (int j=0; j<=Q.degre; j++) {
            R.tab_coef[i+j] += P.tab_coef[i]*Q.tab_coef[j];
        }
    }
    return R;
}




polynome quotient_polynome(polynome P, polynome Q){
    polynome R;

    if (Q.degre == 0){
        if (Q.tab_coef[0] == 0.0){
        }
        else{
        R.degre = P.degre;
        R.tab_coef = malloc((R.degre + 1) * sizeof(float));
        for (int i=0; i<=P.degre; i++){
            R.tab_coef[i] = P.tab_coef[i] / Q.tab_coef[0];
        }
        actualiser_degre(&R);
        return R;
        }
    }

    if (P.degre < Q.degre){
        R.degre = 0;
        R.tab_coef = malloc(sizeof(float));
        R.tab_coef[0] = 0.0;
        return R;
    }

    R.degre = P.degre - Q.degre;
    R.tab_coef = malloc((R.degre + 1) * sizeof(float));
    for (int i=0; i<=R.degre; i++){
        R.tab_coef[i] = 0.0;
    }
    

    //dividende
    polynome reste;
    reste.degre = P.degre;
    reste.tab_coef = malloc((P.degre+1) * sizeof(float));
    for (int i=0; i<=P.degre; i++){
        reste.tab_coef[i] = P.tab_coef[i];
    }

    while (reste.degre >= Q.degre){
        int degre_quotient = reste.degre - Q.degre;
        float coef = reste.tab_coef[reste.degre] / Q.tab_coef[Q.degre];

        //quotient actuel
        polynome terme;
        terme.degre = degre_quotient;
        terme.tab_coef = malloc((degre_quotient+1)* sizeof(float));
        for (int i=0; i<=terme.degre; i++){
            terme.tab_coef[i] = 0.0;
        }
        terme.tab_coef[degre_quotient] = coef;

        
        //ajoute le terme au quotient total
        R.tab_coef[degre_quotient] = coef;

        //multiplier le terme par le diviseur et soustraire au reste
        polynome produit = produit_polynome(terme, Q);
        polynome new_reste = difference_polynome(reste, produit);

        free(terme.tab_coef);
        free(produit.tab_coef);
        free(reste.tab_coef);
        reste = new_reste;
        actualiser_degre(&reste);
    }
    return R;
}



polynome reste_polynome(polynome P, polynome Q){
    polynome quotient = quotient_polynome(P, Q);
    polynome produit = produit_polynome(quotient, Q);
    polynome R = difference_polynome(P, produit);
    actualiser_degre(&R);
    return R;
}


int est_nul(polynome P){
    for (int i=0; i<=P.degre; i++){
        if (P.tab_coef[i] !=0.0){
            return 0;
        }
    }
    return 1;
}

polynome copier_polynome(polynome P){
    polynome C;
    C.degre = P.degre;
    C.tab_coef = malloc((C.degre + 1) * sizeof(float));
    for (int i=0; i<=C.degre; i++){
        C.tab_coef[i] = P.tab_coef[i];
    }
    return C;
}

polynome pgcd_polynome(polynome P, polynome Q){
    if (est_nul(Q)){
        return copier_polynome(P);
    }
    else{
        polynome R = reste_polynome(P,Q);
        
        if (est_nul(R)){
            return copier_polynome(Q);
        }
        else if (R.degre == 0){
            polynome premier;
            premier.degre = 0;
            premier.tab_coef[0] = 1.0;
            return premier;
        }
        return pgcd_polynome(Q, R); 
    }

}




void relation_bezout(polynome R, polynome S, polynome *X, polynome *Y){
    polynome A = copier_polynome(R);
    polynome B = copier_polynome(S);
    X->degre = 0;
    X->tab_coef = malloc(sizeof(float));
    X->tab_coef[0] = 1.0;

    Y->degre = 0;
    Y->tab_coef = malloc(sizeof(float));
    Y->tab_coef[0] = 0.0;
    
    polynome U;
    U.degre = 0;
    U.tab_coef = malloc(sizeof(float));
    U.tab_coef[0] = 0.0;
    
    polynome V;
    V.degre = 0;
    V.tab_coef = malloc(sizeof(float));
    V.tab_coef[0] = 1.0;

    while (!est_nul(B)){
        polynome Q = quotient_polynome(A,B);
        polynome R = reste_polynome(A,B);

        free(A.tab_coef);
        A = copier_polynome(B);
        free(B.tab_coef);
        B = copier_polynome(R);

        polynome newU = difference_polynome(*X,produit_polynome(Q,U));
        polynome newV = difference_polynome(*Y,produit_polynome(Q,V));

        free_polynome(X);
        *X = copier_polynome(U);
        free_polynome(Y);
        *Y = copier_polynome(V);
        U = copier_polynome(newU);
        V = copier_polynome(newV);
    }
    
}

float horner(polynome P, float alpha){
    float res = P.tab_coef[P.degre];
    for (int i=P.degre-1; i>=0; i--){
        res = res * alpha + P.tab_coef[i];
    }
    return res;
}


float dichotomie(polynome P, float a, float b){
    //precision
    float epsilon = 0.00001;
    float fa = horner(P, a);
    float fb = horner(P, b);
    if (fa * fb > 0){
        printf("Pas de racine dans l'intervalle [%.2f,%.2f].\n",a,b);
        return -1;
    }
    else{
        while ((b - a) > epsilon){
            float m = (a+b) / 2.0;
            float fm= horner(P,m);
            //x est trouve
            if (fm == 0.0){
                return m;
            }
            //
            if (fa * fm < 0){
                b = m;
                fb = fm;
            }
            else{
                a = m;
                fa = fm;
            }
        }
    }
    return (a + b) / 2.0;

}


polynome derivee(polynome P){
    polynome R = copier_polynome(P);
    R.degre -= 1;
    for (int i=0;i<=R.degre;i++){
        R.tab_coef[i] = (i+1)*R.tab_coef[i+1];
    }
    return R;
}


void affichage_interface(void){
    printf(" ____       _                                       \n");
    printf("|  _ \\ ___ | |_   _ _ __   ___  _ __ ___   ___  ___ \n");
    printf("| |_) / _ \\| | | | | '_ \\ / _ \\| '_ ` _ \\ / _ \\/ __|\n");
    printf("|  __/ (_) | | |_| | | | | (_) | | | | | |  __/\\__ \n");
    printf("|_|   \\___/|_|\\__, |_| |_|\\___/|_| |_| |_|\\___||___/\n");
    printf("              |___/                                 \n");
}




int main(void){
    affichage_interface();
    polynome P;
    polynome U;

    //saisir les 2 polynomes

    //polynome 1
    printf("-------- Polynome 1 -----------\n");

    saisie_poly(&P);

    printf("-------- Polynome 2 -----------\n");
    saisie_poly(&U);


    // RESULTAT
    printf("\n=========== RESULTATS ===========\n \n");


    //afficher polynome 1
    printf("-------- Polynome 1 -----------\n");
    affichage_poly(P);

    //afficher polynome 2
    printf("-------- Polynome 2 -----------\n");
    affichage_poly(U);

    //faire la somme
    polynome S = somme_polynome(P,U);

    printf("----------- Somme --------------\n");
    affichage_poly(S);

    //faire la difference
    polynome D = difference_polynome(P,U);

    printf("--------- Différence ----------\n");
    affichage_poly(D);
    
    //faire le produit
    polynome E = produit_polynome(P,U);
    printf("----------- Produit ------------\n");
    affichage_poly(E);
    
    //calculer le quotient
    polynome F = quotient_polynome(P,U);
    printf("----------- Quotient ------------\n");
    affichage_poly(F);

    //calculer le reste
    polynome G = reste_polynome(P,U);
    printf("----------- Reste ------------\n");
    affichage_poly(G);

    //calculer le pgcd
    polynome H = pgcd_polynome(P,U);
    printf("----------- PGCD ------------\n");
    affichage_poly(H);

    //relation de bezout
    polynome X;
    polynome Y;
    printf("----------- Relation de Bezout ------------\n");
    relation_bezout(P,U,&X,&Y);
    printf("X : ");
    affichage_poly(X);
    printf("Y : ");
    affichage_poly(Y);

    //evaluer en alpha = 5
    printf("----------- Methode de Horner ------------\n");
    printf("P(5) = %.2f\n",horner(P,5));

    //dichotomie
    printf("----------- Methode de la dichotomie ------------\n");
    printf("P(%.2f) = 0\n",dichotomie(P,-100,100));

    //dérivée
    polynome J = derivee(P);
    printf("--------------- Dérivée ------------------\n");
    affichage_poly(J);
    
    free_polynome(&P);
    free_polynome(&U);
    free_polynome(&S);
    free_polynome(&D);
    free_polynome(&E);
    free_polynome(&F);
    free_polynome(&G);
    free_polynome(&J);

    return 0;
}