//------------------------------------------------------------------------------
// Inclusão da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO 360 + 1 // De -180 até 180, incluindo o 0.

// Conjuntos relativos à posição da bola.
float Eb[TAMANHO_CONJUNTO];
float Fb[TAMANHO_CONJUNTO];
float Db[TAMANHO_CONJUNTO];

// Conjuntos relativos à posição do alvo.
float Ea[TAMANHO_CONJUNTO];
float Fa[TAMANHO_CONJUNTO];
float Da[TAMANHO_CONJUNTO];

// Graus de pertinência dos conjuntos de entrada.
float bolaDireita;
float bolaFrente;
float bolaEsquerda;
float alvoDireita;
float alvoFrente;
float alvoEsquerda;

float triangulo(float x, float alfa, float beta, float gama)
{
    float mi = 0;
    
    if ((x <= alfa) || (x >= gama)) 
    {
        mi=0;
        return mi;
    }
    
    if ((x >= alfa) && (x <= beta)) 
    {
        mi = (x - alfa)/(beta - alfa);
        return mi;
    }
    
    if ((x >  beta) && (x <  gama)) 
    {
        mi=(gama - x)/(gama - beta);
        return mi;
    }
    
    return mi;
} 

// Inicialização de conjuntos.
void init()
{
    //int variacao = 360/TAMANHO_CONJUNTO;
    
    for (int x = 0; x <= TAMANHO_CONJUNTO; x++)
    {
        Eb[x] = triangulo(x - 180,  -190,   -180,   0   );
        Fb[x] = triangulo(x - 180,  -90 ,    0  ,   90  );
        Db[x] = triangulo(x - 180,  -0  ,    180,   190 );

        Ea[x] = triangulo(x - 180,  -190,   -180,   0   );
        Fa[x] = triangulo(x - 180,  -90 ,    0  ,   90  );
        Da[x] = triangulo(x - 180,  -0  ,    180,   190 );

        //theta = theta + variacao;
    }
}

int emGraus(float radianos)
{
    return int( (radianos*180)/M_PI );
}

void fuzzyficacao(float ballAngle, float targetAngle)
{
    bolaDireita  = Db[emGraus(ballAngle) + 180];
    bolaFrente   = Fb[emGraus(ballAngle) + 180];
    bolaEsquerda = Eb[emGraus(ballAngle) + 180];
    
    alvoDireita  = Da[emGraus(targetAngle) + 180];
    alvoFrente   = Fa[emGraus(targetAngle) + 180];
    alvoEsquerda = Ea[emGraus(targetAngle) + 180];
}


void regras()
{
    for (int i = 0; i < 3; i++) // Inicialização das Variaveis
    {
        acaoFrente[i] = 0;
        acaoEsquerda[i] = 0;
        acaoDireita[i] = 0;
    }
    
    
    if (bolaEsquerda && alvoEsquerda) // Regra 01
    {
        if(bolaEsquerda > alvoEsquerda)
            acaoFrente[0] = alvoEsquerda;
        else
            acaoFrente[0] = bolaEsquerda;
    }
    if (bolaEsquerda && alvoFrente) // Regra 02
    {
        if(bolaEsquerda > alvoFrente)
            acaoEsquerda[0] = alvoFrente;
        else
            acaoEsquerda[0] = bolaEsquerda;
    }
    if (bolaEsquerda && alvoDireita) // Regra 03
    {
        if(bolaEsquerda > alvoDireita)
            acaoEsquerda[1] = alvoDireita;
        else
            acaoEsquerda[1] = bolaEsquerda;
    }
    if (bolaFrente   && alvoEsquerda) // Regra 04
    {
        if(bolaFrente > alvoEsquerda)
            acaoDireita[0] = alvoEsquerda;
        else
            acaoDireita[0] = bolaFrente;
    }
    if (bolaFrente   && alvoFrente) // Regra 05
    {
        if(bolaFrente > alvoFrente)
            acaoFrente[1] = alvoFrente;
        else
            acaoFrente[1] = bolaFrente;
    }
    if (bolaFrente   && alvoDireita) // Regra 06
    {
        if(bolaFrente > alvoDireita)
            acaoEsquerda[2] = alvoDireita;
        else
            acaoEsquerda[2] = bolaFrente;
    }
    if (bolaDireita  && alvoEsquerda) // Regra 07
    {
        if(bolaDireita > alvoEsquerda)
            acaoDireita[1] = alvoEsquerda;
        else
            acaoDireita[1] = bolaDireita;
    }
    if (bolaDireita  && alvoFrente) // Regra 08
    {
        if(bolaDireita > alvoFrente)
            acaoDireita[2] = alvoFrente;
        else
            acaoDireita[2] = bolaDireita;
    }
    if (bolaDireita  && alvoDireita) // Regra 09
    {
        if(bolaDireita > alvoDireita)
            acaoFrente[2] = alvoDireita;
        else
            acaoFrente[2] = bolaDireita;
    }
}

void defuzzificacao()
{
    int i, j;
    acao = 0;
    float maxAcaoEsquerda = 0;
    float maxAcaoFrente = 0;
    float maxAcaoDireita = 0;

    // Conjuntos relativos à posição de saida.
    float Es[TAMANHO_CONJUNTO];
    float Fs[TAMANHO_CONJUNTO];
    float Ds[TAMANHO_CONJUNTO];
    float aux;
    int Econt=0,Fcont=0,Dcont=0;

    for (i = 0; i<3; i++)
    {
        if (acaoEsquerda[i]>maxAcaoEsquerda)
        {
            maxAcaoEsquerda = acaoEsquerda[i]; 
        }
        if (acaoFrente[i]>maxAcaoFrente)
        {
            maxAcaoFrente = acaoFrente[i];
        }
        if (acaoDireita[i]>maxAcaoDireita)
        {
            maxAcaoDireita = acaoDireita[i];
        }
    }

    for (i = 0; i <TAMANHO_CONJUNTO; i++)
    {
        if(Eb[i] > maxAcaoEsquerda)
            Es[i] = maxAcaoEsquerda;
        else
            Es[i] = Eb[i];
        if(Fb[i] > maxAcaoFrente)
            Fs[i] = maxAcaoFrente;
        else
            Fs[i] = Fb[i];
        if(Db[i] > maxAcaoDireita)
            Ds[i] = maxAcaoDireita;
        else
            Ds[i] = Db[i]; 
    }

    j = 0;
    for (i = -180; i <=180; i++)
    {
        if(Es[j] > Fs[j]){
            if(Es[j] > Ds[j]){
                aux = Es[j];
                Econt++;
            }else{
                aux = Ds[j];
                Dcont++;
            }
        }else{
            if(Fs[j] > Ds[j]){
                aux = Fs[j];
                Fcont++;
            }else{
                aux = Ds[j];
                Dcont++;
            }

        }

        acao = acao + (i * aux); 
    }

    acao = acao / (Econt * maxAcaoEsquerda + Fcont * maxAcaoFrente + Dcont * maxAcaoDireita);
}

int main( int argc, char* argv[] ) {

    float ballAngle, targetAngle, leftMotor, rightMotor;

    // Declaração do objeto que representa o ambiente.
    environm::soccer::clientEnvironm environment;

    if ( argc != 3 )
    {
        printf( "Invalid parameters. Expecting:\n" );
        printf( "SoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER\n" );
        printf( "\nExample:\n" );
        printf( "SoccerPlayer localhost 1024\n\n" );  
              
        return 0;
    }

    // Conecta-se ao SoccerMatch. Supõe que SoccerMatch está rodando na máquina
    // local e que um dos robôs esteja na porta 1024. Porta e local podem mudar.
    if ( ! environment.connect( argv[1], atoi( argv[2] ) ) )
    {
        printf( "Fail connecting to the SoccerMatch.\n\n" );
        
        return 0;  // Cancela operação se não conseguiu conectar-se.
    }

    // Laço de execução de ações.
    printf( "Running...\n" );
    
    // Inicializa conjuntos.
    init();

    while ( 1 ) {

        // Deve obter os dados desejados do ambiente. Métodos do clientEnvironm.
        // Exemplos de métodos que podem ser utilizados.
        ballAngle = environment.getBallAngle();
        targetAngle = environment.getTargetAngle( environment.getRivalGoal() );
        
        // Obtém graus de pertinência.
        fuzzyficacao(ballAngle,targetAngle);
        
        // Corta conjuntos e os une.
        regras();
        
        // Obtém valor final.
        defuzzificacao();
        
        leftMotor  = 0.1;
        rightMotor = 0.1;
        
        // Transmite ação do robô ao ambiente. Fica bloqueado até que todos os
        // robôs joguem. Se erro, retorna false (neste exemplo, sai do laco).
        if ( ! environment.act( leftMotor, rightMotor ) ) {
            break; // Termina a execução se falha ao agir.
        }
    }

    return 0;
}
//------------------------------------------------------------------------------

