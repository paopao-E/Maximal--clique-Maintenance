#include "maximalClique.h"

void get_parameters(int argc, char *argv[], int &alg, double &eta, int &k)
{
    
    for (int i = 2; i < argc; ++i) {
        char *p = argv[i];
        if (strstr(p,"-a="))
            alg = atoi(p+3);
        else if (strstr(p,"-e="))
            eta = atof(p+3);
        else if (strstr(p,"-k="))
            k = atoi(p+3);
    }
}

int main(int argc, char *argv[])
{
    // if (argc < 2) {
    //     printf("Usage: ./uclique [file] [parameters]\n");
    //     printf("Where parameters are -a=1|2 -k=[0,n], -e=[0,1]\n");
    //     return 0;
    // }
    char * argv2[]={"./main", "../example/enron_1.txt","-a=3","-k=1","-e=0.2"};
    int alg = 3, k = 1; //minimal size of cliques
    double eta = 0.2;

    

    get_parameters(argc, argv2, alg, eta, k);
    Algorithm *mc = new maximalClique();
    mc->read_graph(argv2[1]);
    mc->setparemeters(eta, alg, k);
    mc->run();
    printf("Proceudre=%s, Alg=%d\n", argv2[0], alg);
    delete mc;
    return 0;
}