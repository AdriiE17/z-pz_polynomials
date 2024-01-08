/*
    Arithmetics of polynomials in Z/pZ
    
    The program reads: p, two polynomials (a,b), and a number m
    The maximum p is P_MAX=100
    A polinomial pf is an expression of the form:
        pf(x)= f0 + f1 x + f2 xˆ2 + ... + fn xˆn.
    The polynomial 0 has degree -1.
    The maximum degree is N_MAX=1000.
    The n+1 coeffients are stored in a vector of size D=N_MAX+2.
    The degree n is stored in the element indexed by DEG=N_MAX+1.

    Input and output of a polynomial
    For a polynomial of degree n, the input and output are given in the form:
        n: a0 a1 a2 ... an
    
    Exemple: a(x)= x+ 3 xˆ2 is given by 2: 0 1 3
    and stored as with a[0]=0, a[1]=1, a[2]=3, a[DEG]=2.
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
    int psumz, pdifz, pmulz, pprdz, pquoz, premz;
    int pbquo[D], pbquoplusrem[D], paminuspbquoplusrem[D]; /* for division test */

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
    printf("Sum a+b\n");
    sum_poly(pa, pb, psum);
    printf("%-8s", "a+b:");
    write_coef_poly(psum);

    /* difference */
    printf("Difference a-b\n");
    dif_poly(pa, pb, pdif);
    printf("%-8s", "a-b:");
    write_coef_poly(pdif);

    /* multiplication by a number*/
    printf("Multiplication\n");
    mul_poly(m, pa, pmul); 
    printf("%-8s", "m*a:");
    write_coef_poly(pmul);

    /* product */
    printf("Product a*b\n");
    prd_poly(pa, pb, pprd);
    printf("%-8s", "a*b:");
    write_coef_poly(pprd);

    /* Euclidean division */
    printf("Euclidean division a= q*b + r\n");
    div_poly(pa, pb, pquo, prem);
    printf("%-8s", "a/b:");
    write_coef_poly(pquo);
    printf("%-8s", "a%b:");
    write_coef_poly(prem);


    /* Tests by Evaluation*/
    puts ("\nTests by evaluation");
    puts (" z   a(z) b(z)  -  sum  -  dif  -  mul  -  prd  -  div  ");
    for (z=0; z<p; z++) {
        paz = eval_poly(pa, z);
        pbz = eval_poly(pb, z);
        printf("%2i %4i %4i", z, paz, pbz);
       
        /* Test sum */
        psumz = eval_poly(psum, z);
        if (sum[paz][pbz]==psumz) {
            printf("    -%3i=%-3i" , sum[paz][pbz], psumz);
        } else {
            printf("    -%3i!=%-2i", sum[paz][pbz], psumz);
        }

        /* Test difference */
        pdifz = eval_poly(pdif, z);
        if (dif[paz][pbz]==pdifz) {
            printf("-%3i=%-3i" , dif[paz][pbz], pdifz);
        } else {
            printf("-%3i!=%-2i", dif[paz][pbz], pdifz);
        }

        /* Test multiplication by a number*/
        pmulz = eval_poly(pmul, z);
        if (prd[m][paz]==pmulz) {
            printf("-%3i=%-3i" , prd[m][paz], pmulz);
        } else {
            printf("-%3i!=%-2i", prd[m][paz], pmulz); 
        }
        
        /* Test product */
        pprdz = eval_poly(pprd, z);
        if (prd[paz][pbz]==pprdz) {
            printf("-%3i=%-3i" , prd[paz][pbz], pprdz);
        } else {
            printf("-%3i!=%-2i", prd[paz][pbz], pprdz);
        }

        /* Test Euclidean division */
        pquoz = eval_poly(pquo, z);
        premz = eval_poly(prem, z);
        if (sum [prd[pquoz][pbz]] [premz] == paz) {
            printf("-%3i=%-3i" , paz, sum [prd[pquoz][pbz]] [premz]);
        } else {
            printf("-%3i!=%-2i", paz, sum [prd[pquoz][pbz]] [premz]); 
        }

        printf ("\n");
    }


    /* Test of Euclidean division */
    puts("\nTest of Euclidean division");
    prd_poly(pb, pquo, pbquo);
    sum_poly(pbquo, prem, pbquoplusrem);
    dif_poly(pa, pbquoplusrem, paminuspbquoplusrem);

    printf("%-8s", "a:");
    write_coef_poly(pa);
    printf("%-8s", "q*b+r:");
    write_coef_poly(pbquoplusrem);

    if(paminuspbquoplusrem[DEG]==-1) {
        puts("Test OK");
    } else {
        puts("Test not OK");
    }

    puts("");

    return 0;
}

void zpz_arithmetics (int p) {

    int i, j;
    int a0, a1, r, gcd;

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
        
        /* getting gcd with euclides' algorithm */
        a0 = p;
        a1 = i;
        r = a0%a1;
        while(r!=0) {
            a0 = a1;
            a1 = r;
            r = a0%a1;
        }
        gcd=a1;

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
            dif[i][j]=(i + (p-j) )%p;
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
    if(z < 0) {
        return p-((-z)%p);
    }
    return z%p;
}

int read_coef_poly(int pf[D]) {

    int i;

    printf("Type the degree: ");
    scanf(" %d", &pf[DEG]);

    printf("Type the coefficients: ");
    if(pf[DEG] > N_MAX) {
        printf("The degree is greater than the maximum (%d).\n", N_MAX);
        return 0;
    }

    /* Negative degree is for zero polynomial, and adjust to -1. */
    if(pf[DEG] < 0) {
        pf[DEG]=-1;
    }

    for(i=0; i<pf[DEG]+1; i++) {
        scanf(" %d", &pf[i]);
    }

    return 1;
}

void write_coef_poly(int pf[D]) {
    int i;
    printf("%3d: ", pf[DEG]);
    for(i=0; i<pf[DEG]+1;i++) {
        printf("%d ", pf[i]);
    }
    printf("\n");
}

void true_degree(int pf[D]) {
    /* modify pf[DEG] so that it is the true degree */
    int i;

    for(i=pf[DEG]; i>=0; i--) {
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

    pp[DEG]=pa[DEG]+pb[DEG];

    /* ensure that there are no numbers on pp */
    for(i=0; i<pp[DEG]+1; i++) {
        pp[i]=0;
    }
    
    for(i=0; i<pa[DEG]+1; i++) {
        for(j=0; j<pb[DEG]+1; j++) {
            pp[i+j]=sum [pp[i+j]] [prd [pa[i]] [pb[j]] ];
        } 
    }

    true_degree(pp);

}


int div_poly(int pa[D], int pb[D], int pc[D], int pr[D]) {

    int i, j;
    int pcaux[D], praux[D];

    /* set the remainder as if it was pa */
    for(i=0; i<pa[DEG]+1; i++) {
        pr[i]=pa[i];
    }
    pr[DEG]=pa[DEG];

    /* set quotient to zero polynomial just in case an error occur */
    pc[DEG]=X;

    /* Handling possible division errors */
    if(pb[DEG]>pa[DEG]) {
        printf("Division error: Degree of denominator is bigger than degree of numerator!\n");
        return 0;
    }

    if(pb[DEG]==-1) {
        printf("Division error: Division by 0 polynomial!\n");
        return 0;
    }

    if(inv[pb[pb[DEG]]]==X) {
        printf("Division error: Leading term of denominator is not invertible!\n");
        return 0;
    }

    /* Euclidean division */
    /* find pc degree and set all their coefficients to 0*/
    pc[DEG]=pa[DEG]-pb[DEG];
    for(i=0; i<pc[DEG]+1; i++) {
        pc[i]=0;
    }

    for(i=pc[DEG]; i>=0; i--) {

        pc[i]=div [pr[pr[DEG]]] [pb[pb[DEG]]];
        
        /* get pc[i] as a monomyal and save it in pcaux */
        for(j=0; j<=i; j++) {
            pcaux[j]=pc[j];
        }
        pcaux[DEG]=i;

        prd_poly(pcaux, pb, praux);
        dif_poly(pr, praux, pr);

    }
    
    true_degree(pc);
    true_degree(pr);

    return 1;
}

