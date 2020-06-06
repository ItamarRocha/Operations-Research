#include "../include/Data.hpp"
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

void solve(Data& data);

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage:\n./bin instance\n");
        return 0;
    }

    Data data(argv[1]);
    solve(data);

    return 0;
}

void solve(Data& data)
{
    IloEnv env;
    IloModel modelo(env);

    ///////////////////////////////////////////////////////
    //variavel Xik: Xik = 1, se o item i esta no bin k
    IloArray< IloBoolVarArray > x(env, data.getNItems());
    for(int i = 0; i < data.getNItems(); i++)
    {
        IloBoolVarArray vetor(env, data.getNItems());
        x[i] = vetor;
    }
   
    //adiciona a variavel x ao modelo
    for(int i = 0; i <  data.getNItems(); i++)
    {
        for(int k = 0; k < data.getNItems(); k++)
        {
            char name[100];
            sprintf(name, "X(%d,%d)", i, k);
            x[i][k].setName(name);
            modelo.add(x[i][k]);
        }
    }

    //Variavel Yk: Yk = 1 se o k-esimo bin esta sendo usado
    IloBoolVarArray y(env, data.getNItems());

    //Adicionando a variável y ao modelo

    //fim das variaveis
    ///////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    //Criando a Função Objetivo (FO) 
    IloExpr sumY(env);
    for(int k = 0; k < data.getNItems(); k++)
    {
        sumY += y[k];
    }
    // Adicionando a FO
    modelo.add(IloMinimize(env, sumY));
    //////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    //Restricoes

    //Restricoes (1): cada item deve estar em um bin
    for(int i = 0; i < data.getNItems(); i++) 
    {
        IloExpr sumX(env);
        for(int k = 0; k < data.getNItems(); k++)
        {
            sumX += x[i][k];
        }

        IloRange r = (sumX == 1);
        char name[100];
        sprintf(name, "COV(%d)", i);
        r.setName(name);
        modelo.add(r);
    }

    //Restricoes (2): capacidade dos bins
    for(int k = 0; k < data.getNItems(); k++)
    {
        IloExpr sumX(env);
        for(int i = 0; i < data.getNItems(); i++) 
        {
            sumX += data.getItemWeight(i)*x[i][k];
        }

        IloRange r = (sumX - data.getBinCapacity()*y[k] <= 0);
        char name[100];
        sprintf(name, "CAP(%d)", k);
        r.setName(name);
        modelo.add(r);
    }

    //fim das restricoes
    ////////////////////////////////////////////////////////

    //resolve o modelo
    IloCplex bpp(modelo); // pula o passo do extract fazendo isso
    bpp.setParam(IloCplex::TiLim, 2*60*60);
    bpp.setParam(IloCplex::Threads, 1);
//    bpp.setParam(IloCplex::Param::MIP::Interval, 1);
//    bpp.setParam(IloCplex::Param::MIP::Display, 5);
    bpp.exportModel("modelo.lp");

    try
    {
        bpp.solve();
    }
    catch(IloException& e)
    {
        std::cout << e;
    }

    std::cout << "status:" << bpp.getStatus() << std::endl;
    std::cout << "numero de bins usados:" << bpp.getObjValue() << std::endl;
    for(int i = 0; i < data.getNItems(); i++) 
    {
        for(int k = 0; k < data.getNItems(); k++)
        {
            if(bpp.getValue(x[i][k]) > 0.9)
            {
                std::cout << "item " << i << " no bin " << k << std::endl;
            }
        }
    }
    env.end(); //FREE MEMORY AS ENV IS A POINTER

    exit(0);
}
