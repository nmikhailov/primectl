struct X {
    int a;
    int b;
} SX1;

typedef struct {
    int sdasd;
    char * sdas;
} ST2;

typedef struct SX1* sptr1;
typedef ST2* sptr2;
typedef const sptr2* csptr3;

sptr1 gl_test1(int farg1, char * farg2, sptr2 farg3);
extern int * gl_test2(const int **farg1, int farg2, csptr3 farg3);
int ** gl_test3(int farg1[8], const int *farg2[]);
void gl_test4(const struct X ** farg1);
void gl_test5(int, char*);
extern void (*gl_test6(const int *farg1))( char* );
void gl_test7(const double *farg1[16]);
void gl_test8();
