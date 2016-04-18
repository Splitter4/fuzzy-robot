//------------------------------------------------------------------------------
// Inclus�o da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO 360 + 1 // De -180 at� 180, incluindo o 0.
#define CONJUNTOS_BOLA 3 // Esquerda, frente e direita.
#define CONJUNTOS_ALVO 3 // Esquerda, frente e direita.

// Conjuntos relativos � posi��o da bola.
/*
float Eb[TAMANHO_CONJUNTO];
float Fb[TAMANHO_CONJUNTO];
float Db[TAMANHO_CONJUNTO];
*/
float conjBola[CONJUNTOS_BOLA][TAMANHO_CONJUNTO];

// Conjuntos relativos � posi��o do alvo.
/*
float Ea[TAMANHO_CONJUNTO];
float Fa[TAMANHO_CONJUNTO];
float Da[TAMANHO_CONJUNTO];
*/
float conjAlvo[CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Conjuntos relativos � posi��o de saida.
/*
float Er[TAMANHO_CONJUNTO];
float Fr[TAMANHO_CONJUNTO];
float Dr[TAMANHO_CONJUNTO];
*/
float conjSaida[CONJUNTOS_BOLA][CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Graus de pertin�ncia dos conjuntos de entrada.
/*
float bolaDireita;
float bolaFrente;
float bolaEsquerda;
*/
float uBola[CONJUNTOS_BOLA];

/*
float alvoDireita;
float alvoFrente;
float alvoEsquerda;
*/
float uAlvo[CONJUNTOS_ALVO];

/*
float acaoFrente[3];
float acaoEsquerda[3];
float acaoDireita[3];
*/

/*
float acao;
int tipoAcao;
*/

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

// Inicializa��o de conjuntos.
void init()
{
    //int variacao = 360/TAMANHO_CONJUNTO;
    
    for (int i = 0; i <= TAMANHO_CONJUNTO; i++)
    {
        conjBola[0][i] = triangulo(i - 180,  -190,   -180,   0   ); // Esquerda.
        conjBola[1][i] = triangulo(i - 180,  -90 ,    0  ,   90  ); // Frente.
        conjBola[2][i] = triangulo(i - 180,  -0  ,    180,   190 ); // Direita.
        
        conjAlvo[0][i] = triangulo(i - 180,  -190,   -180,   0   ); // Esquerda.
        conjAlvo[1][i] = triangulo(i - 180,  -90 ,    0  ,   90  ); // Frente.
        conjAlvo[2][i] = triangulo(i - 180,  -0  ,    180,   190 ); // Direita.
        
        conjSaida[0][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[0][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][0][i] = triangulo(i - 180,    -0,    180,    190); // Direita.
        conjSaida[1][1][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[1][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[2][0][i] = triangulo(i - 180,    -0,    180,    190); // Direita.
        conjSaida[2][1][i] = triangulo(i - 180,    -0,    180,    190); // Direita.
        conjSaida[2][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.

        //theta = theta + variacao;
    }
}

int emGraus(float radianos)
{
    return int( (radianos*180)/M_PI );
}

void fuzzyficacao(float ballAngle, float targetAngle)
{
    /*
    bolaDireita  = Db[emGraus(ballAngle) + 180];
    bolaFrente   = Fb[emGraus(ballAngle) + 180];
    bolaEsquerda = Eb[emGraus(ballAngle) + 180];
    
    alvoDireita  = Da[emGraus(targetAngle) + 180];
    alvoFrente   = Fa[emGraus(targetAngle) + 180];
    alvoEsquerda = Ea[emGraus(targetAngle) + 180];
    */
    
    for(int i = 0; i < 3; i++)
    {
        uBola[i] = conjBola[i][emGraus(ballAngle) + 180];
        uAlvo[i] = conjAlvo[i][emGraus(targetAngle) + 180];
    }
}

float min(float a, float b)
{
    if(a <= b)
        return a;
    else
        return b;
}

float max(float a, float b)
{
    if(a >= b)
        return a;
    else
        return b;
}

void regras(float *conjFinal)
{
    for(int i = 0; i < CONJUNTOS_BOLA; i++)
    {
        for(int j = 0; j < CONJUNTOS_ALVO; j++)
        {
            if(uBola[i] > 0 && uAlvo[j] > 0) // Regra ativada.
            {
                // Obt�m a for�a de disparo da regra
                // pela conjun��o dos graus de pertin�ncia.
                float alfa = min(uBola[i], uAlvo[j]);
                
                // Corta o conjunto de sa�da da regra
                // e faz a uni�o com os outros conjuntos cortados
                for(int k = 0; k < TAMANHO_CONJUNTO; k++)
                    conjFinal[k] = max(min(alfa, conjSaida[i][j][k]), conjFinal[k]);
            }
        }
    }
}

float defuzzificacao(float *conjFinal)
{
    float dividendo = 0;
    float divisor = 0;
    
    for(int i = 0; i < TAMANHO_CONJUNTO; i++)
    {
        dividendo += conjFinal[i]*(i - 180);
        divisor += conjFinal[i];
    }
    
    return dividendo/divisor;
}

int main( int argc, char* argv[] )
{
    if ( argc != 3 )
    {
        printf( "Invalid parameters. Expecting:\n" );
        printf( "SoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER\n\n" );
        printf( "Example:\n" );
        printf( "SoccerPlayer localhost 1024\n\n" );  
              
        return 0;
    }

    // Declara��o do objeto que representa o ambiente.
    environm::soccer::clientEnvironm environment;

    // Conecta-se ao SoccerMatch.
    if ( environment.connect( argv[1], atoi(argv[2]) ) == false)
    {
        printf( "Fail connecting to the SoccerMatch.\n\n" );
        
        return 0;  // Cancela opera��o se n�o conseguiu conectar-se.
    }

    // La�o de execu��o de a��es.
    printf( "Running...\n\n" );
    
    // Inicializa conjuntos.
    init();

    float ballAngle, targetAngle, leftMotor, rightMotor;
    
    float conjFinal[TAMANHO_CONJUNTO];
    
    int k = 15;
    
    while ( true )
    {
        ballAngle = -environment.getBallAngle();
        targetAngle = -environment.getTargetAngle( environment.getOwnGoal() );
        
        // Obt�m graus de pertin�ncia.
        fuzzyficacao(ballAngle, targetAngle);

        // Corta os conjuntos de sa�da das regras ativadas e faz a uni�o deles
        for(int i = 0; i < TAMANHO_CONJUNTO; i++)
            conjFinal[i] = 0; // A uni�o de 0 com qualquer coisa � qualquer coisa
        
        regras(conjFinal);

        // Obt�m sa�da final
        float resultado = defuzzificacao(conjFinal);

        leftMotor = (resultado/360 + 0.5)*0.1;
        rightMotor  = (resultado/(-360) + 0.5)*0.1;
        
        if(++k >= 15)
        {
            printf("ballAngle = %d | targetAngle = %d\n", emGraus(ballAngle), emGraus(targetAngle));
            printf("resultado = %f\n", resultado);
            printf("left = %f | right = %f\n\n", leftMotor, rightMotor);
            
            k = 0;
        }
        
        // Transmite a��o do rob� ao ambiente. Fica bloqueado at� que todos os
        // rob�s joguem. Se erro, retorna false (neste exemplo, sai do laco).
        if ( environment.act( leftMotor, rightMotor ) == false )
            break; // Termina a execu��o se falha ao agir.
    }

    return 0;
}
//------------------------------------------------------------------------------

