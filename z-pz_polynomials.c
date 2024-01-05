/*
    program: CognomNom-Practica2.c (poly-zpz.c)
    Arithmetics of polynomials in Z/pZ
    The program reads: p, two polynomials (a,b), and a number m
    The maximum p is P_MAX= 100
    A polinomial pf is an expression of the form
    pf(x)= f0 + f1 x + f2 xˆ2 + ... + fn xˆn.
    The polynomial 0 has degree -1.
    The maximum degree is N_MAX= 1000.
    The n+1 coeffients are stored in a vector of size D= N_MAX+2.
    The degree n is stored in the element indexed by DEG= N_MAX+1.
    Input and output of a polynomial
    For a polynomial of degree n, the input and output
    are given in the form
    n: a0 a1 a2 ... an
    Exemple: a(x)= x+ 3 xˆ2 is given by 2: 0 1 3
    and stored as with a[0]= 0, a[1]= 1, a[2]= 3, a[DEG]= 2.
*/


#include <stdio.h>

/* ARITHMETICS Z/pZ */
/* maximum modulus */
#define P_MAX 100

/* error number */
#define X (-1)

/* POLYNOMIALS */
/* maximum degree */
#define N_MAX 1000

/* size of a vector containing a polynomial */
#define D (N_MAX+2)

/* position of the degree in the vector */
#define DEG (N_MAX+1)

/* arithmetic tables */
int sum[P_MAX][P_MAX], prd[P_MAX][P_MAX];
int opp[P_MAX], inv[P_MAX];
int dif[P_MAX][P_MAX], div[P_MAX][P_MAX];

void zpz_arithmetics(int p); /* fill arithmetic tables */

int mod(int z, int p); /* return z mod p */

/* input/output polynomials */
int read_coef_poly(int pf[D]); /* Read polynomial. Return 1 if ok, 0 otherwise */
void write_coef_poly(int pf[D]);

/* coefficients mod p */
void mod_poly(int pf[D], int p);

/* computation of the degree of a polynomial (skip zero terms) */
void true_degree(int pf[D]);

/* evaluation of a polynomial (Horner’s rule)*/
int eval_poly (int pa[D], int z);

/* arithmetic operations */
void sum_poly(int pa[D], int pb[D], int ps[D]);
void dif_poly(int pa[D], int pb[D], int pd[D]);
void mul_poly(int a, int pb[D], int pm[D]);
void prd_poly(int pa[D], int pb[D], int pp[D]);
int div_poly(int pa[D], int pb[D], int pc[D], int pr[D]); /* Euclidean division. Return 1 if ok, 0 otherwise */

int main (void) {
    
    int p;
    int pa[D], pb[D];
    int m, z, paz, pbz;
    int psum[D], pdif[D], pmul[D], pprd[D], pquo[D], prem[D];
    int psumz;
    int test_result;


    printf("Mod? (<=%d): ", P_MAX);
    scanf(" %d", &p);

    /* Check p is OK. If not end of program */
    if(p>P_MAX || p<2) {
        printf("Not a valid p.\n");
        return 1;
    }

    printf("Computation of arithmetic tables in Z/%dZ\n", p);
    /* Computation of arithmetic tables in Z/pZ */
    zpz_arithmetics(p);

    /* Reading polynomials a(x) and b(x) */
    printf("Read degree and coefficients of polynomial a(x)\n");
    if(read_coef_poly(pa)==0) {
        printf("Exiting program...\n");
        return 1;
    }

    printf("Read degree and coefficients of polynomial b(x)\n");
    if(read_coef_poly(pb)==0) {
        printf("Exiting program...\n");
        return 1;
    }

    /* Put their coefficients in mod p, and fix their true degree */
    mod_poly(pa, p);
    mod_poly(pb, p);

    /* Reading factor m, and make it mod p */
    printf("Introduce a number m mod %d: ", p);
    scanf(" %d", &m);
    m=mod(m, p);


    /* Results of operations */
    printf("\nResults\n\n");

    /* Write (normalized) input */
    printf("Input\n");
    printf("%-8s", "a:");
    write_coef_poly(pa);
    printf("%-8s", "b:");
    write_coef_poly(pb);
 
    /* sum */
    sum_poly(pa, pb, psum);
    printf("Sum a+b\n");
    printf("%-8s", "a+b:");
    write_coef_poly(psum);

    /* difference */
    dif_poly(pa, pb, pdif);
    printf("Difference a-b\n");
    printf("%-8s", "a-b:");
    write_coef_poly(pdif);

    /* multiplication by a number*/
    mul_poly(m, pa, pmul);
    printf("Multiplication\n");
    printf("%-8s", "m*a:");
    write_coef_poly(pmul);

    /* product */
    prd_poly(pa, pb, pprd);
    printf("Product a*b\n");
    printf("%-8s", "a*b:");
    write_coef_poly(pprd);

    /* Euclidean division */
    div_poly(pa, pb, pquo, prem);
    printf("Euclidean division a= q*b + r\n");
    printf("%-8s", "a/b:");
    write_coef_poly(pquo);


    /* Tests by Evaluation*/

    puts ("\nTests by evaluation");
    puts (" z a(z) b(z) - sum - dif - mul - prd - div ");
    test_result=1;
    for (z=0; z<p; z++) {
        paz = eval_poly(pa, z);
        pbz = eval_poly(pb, z);
        printf("%2i %2i %2i", z, paz, pbz);
       
        /* Test sum */
        psumz = eval_poly(psum, z);

        if (sum[paz][pbz]==psumz) {
            printf(" - %2i= %2i" , sum[paz][pbz], psumz);
        } else {
            printf(" - %2i!=%2i", sum[paz][pbz], psumz);
            test_result=0;
        }

        /* Test difference */
        /***/
        /* Test multiplication by a number*/
        /***/
        /* Test product */
        /***/
        /* Test Euclidean division */
        /***/

        printf ("\n");
    }

    if(test_result) {
        printf("Test OK\n");
    } else {
        printf("Test not OK\n");
    }

    return 0;
}

void zpz_arithmetics (int p) {

    int i, j;
    int gcd;

    /* Sum table */
    for(i=0; i<p; i++) {
        for(j=0; j<p; j++) {
            sum[i][j]=(j+i)%p;
        }
    }

    /* Product table */
    for(i=0; i<p; i++) {
        for(j=0; j<p; j++) {
            prd[i][j]=(j*i)%p;
        }
    }

    /* Opposite table */
    for(i=0; i<p; i++) {
        opp[i]=(p-i)%p;
    }

    /* Inverse table */
    /* X when non-invertible */
    inv[0]=X;
    for(i=1; i<p; i++) {
        
        /* getting gcd with euclides algorithm */
        gcd=i;
        while(p%gcd!=0) {
            gcd=p%gcd;
        }

        /* If gcd is not 1, then inverse does not exist */
        if(gcd!=1)  {
            inv[i]=X;
            continue;
        }

        j=1;
        while((p*j-1)%i!=0){
            j++;
        }
        
        inv[i]= p + (1-p*j)/i;

    }

    /* Difference table */
    for(i=0; i<p; i++) {
        for(j=0; j<p; j++) {
            dif[i][j]=(j+ (p-i) )%p;
        }
    }
   

    /* Division table */
    /* X when not possible */
    for(i=0; i<p; i++) {
        for(j=0; j<p; j++) {
            if(inv[j]==X) {
                div[i][j]=X;
                continue;
            }
            div[i][j]=prd[i][inv[j]];
        }
    }
}

int mod(int z, int p) {
    /* Watch out %! */
    if(z < 0) {
        z=p+z;
    }
    return z%p;
}

int read_coef_poly(int pf[D]) {

    int i;

    printf("Type the degree: ");
    scanf(" %d", &pf[DEG]);

    printf("Type the coefficients: ");
    /* Watch out degree n is lower or equal than N_MAX. */
    if(pf[DEG] > N_MAX) {
        printf("The degree is greater than the maximum (%d).\n", N_MAX);
        return 0;
    }

    /* Negative degree is for zero polynomial, and adjust to -1. */
    if(pf[DEG] < 0) {
        pf[DEG]=-1;
    }

    /* Return 1 if successful reading, 0 otherwise */
    for(i=0; i<pf[DEG]+1; i++) {
        scanf(" %d", &pf[i]);
    }

    return 1;
}

void write_coef_poly(int pf[D]) {
    int i;
    printf("%d: ", pf[DEG]);
    for(i=0; i<pf[DEG]+1;i++) {
        printf("%d ", pf[i]);
    }
    printf("\n");
}

void true_degree(int pf[D]) {
    /* modify pf[DEG] so that it is the true degree */
    int i;

    for(i=pf[DEG]; i>0; i--) {
        if(pf[i] != 0) {
            break;
        }

        pf[DEG]--;

    }
}


void mod_poly(int pf[D], int p) {
    
    int i;

    for(i=0; i<pf[DEG]+1; i++) {
        pf[i]=mod(pf[i], p);
    }

    true_degree(pf);
}

int eval_poly(int pa[D], int z) {
    
    int i, value;

    value=pa[pa[DEG]];
    for(i=pa[DEG]; i>0; i--) {
        value=sum[pa[i-1]][prd[z][value]];
    }
    return value;
}

void sum_poly(int pa[D], int pb[D], int ps[D]) {

    /* Watch out the degrees of pa and pb, and the sum ps */
    int k;
    int lowest_degree, highest_degree;

    /* check which degree is lower and which degree is higher (if they are equal, then it doesn't matter) */
    if(pa[DEG] < pb[DEG]) {
        lowest_degree=pa[DEG];
        highest_degree=pb[DEG];
    } else {
        lowest_degree=pb[DEG];
        highest_degree=pa[DEG];
    }

    /* sum until there are no more coefficients in pa or in pb */
    for(k=0; k<lowest_degree+1; k++) {
        ps[k]=sum[pa[k]][pb[k]];
    }
    
    /* if there are still coefficients in the other polynomial, then just add it to ps */
    /* if the degree of both pa and pb are equal, this is not going to be executed */
    for(k=lowest_degree+1; k<highest_degree+1; k++){
        if(highest_degree==pa[DEG]) {
            ps[k]=pa[k];
        } else {
            ps[k]=pb[k];
        }
    }
    
    /* finally, set the highest degree to ps */
    ps[DEG]=highest_degree;

    true_degree(ps);
}

void dif_poly(int pa[D], int pb[D], int pd[D]) {

    int i;
    int pbopp[D];

    for(i=0; i<pb[DEG]+1;i++) {
        pbopp[i]=opp[pb[i]];
    }

    pbopp[DEG]=pb[DEG];

    sum_poly(pa, pbopp, pd);

}


void mul_poly(int a, int pb[D], int pm[D]) {
    
    int i;

    for(i=0;i<pb[DEG]+1; i++) {
        pm[i] = prd[a][pb[i]];
    }

    pm[DEG]=pb[DEG];

    true_degree(pm);

}


void prd_poly(int pa[D], int pb[D], int pp[D]) {

    int i, j;

    for(i=0; i<pa[DEG]+1; i++) {
        for(j=0; j<pb[DEG]+1; j++) {
            pp[i+j]=sum [pp[i+j]] [prd [pa[i]] [pb[j] ]];
        } 
    }

    pp[DEG]=pa[DEG]+pb[DEG];

    true_degree(pp);

}


int div_poly(int pa[D], int pb[D], int pc[D], int pr[D]) {


    
    true_degree(pc);
    true_degree(pr);

}
