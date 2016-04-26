// Artur de Almeida Scheibler
// Julio Pinto
//------------------------------------------------------------------------------
// Inclusão da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO    360 // De -180 até 180.
#define CONJUNTOS_BOLA      5 // Extrema Esquerda, Esquerda, Frente, Direita e Extrema Direita.
#define CONJUNTOS_ALVO      5 // Extrema Esquerda, Esquerda, Frente, Direita e Extrema Direita.
#define DISTANCIAS_BOLA     3 // Perto, Médio e Longe.

#define CLOSE   1 
#define NEAR    2 
#define FAR     3 

// Conjuntos relativos à posição da bola.
float conjBola[CONJUNTOS_BOLA][TAMANHO_CONJUNTO];

// Conjuntos relativos à posição do alvo.
float conjAlvo[CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Conjuntos relativos à distância da bola.
float conjDist[DISTANCIAS_BOLA][2000];

// Conjuntos relativos à posição de saida.
float conjSaida[CONJUNTOS_BOLA][CONJUNTOS_ALVO][DISTANCIAS_BOLA][TAMANHO_CONJUNTO];

// Graus de pertinência dos conjuntos de entrada.
float uBola[CONJUNTOS_BOLA];
float uAlvo[CONJUNTOS_ALVO];
float uDist[DISTANCIAS_BOLA];


float triangulo(float x, float alfa, float beta, float gama)
{
    float mi = 0;
    
    if ((x < alfa) || (x > gama)) 
    {
        mi = 0;
    }
    else if ((x >= alfa) && (x <= beta)) 
    {
        mi = (x - alfa)/(beta - alfa);
    }
    else if ((x > beta) && (x <= gama)) 
    {
        mi = (gama - x)/(gama - beta);
    }
    
    return mi;
}

float trapezio(float x, float alfa, float beta, float gama, float delta)
{
    float mi = 0;
    
    if ((x < alfa) || (x > delta)) 
    {
        mi = 0;
    }
    else if ((x >= alfa) && (x <= beta)) 
    {
        mi = (x - alfa)/(beta - alfa);
    }
    else if ((x > beta) && (x <= gama)) 
    {
        mi = 1;
    }
    else if ((x > gama) && (x <= delta)) 
    {
        mi = (delta - x)/(delta - gama);
    }
    
    return mi;
}

// Inicialização de conjuntos.
void init()
{
    //int variacao = 360/TAMANHO_CONJUNTO;
    
    for (int i = 0; i <= TAMANHO_CONJUNTO; i++)
    {
        conjBola[0][i] = triangulo(i - 180,  -190,  -180,  -90); // Extrema Esquerda.
        conjBola[1][i] = triangulo(i - 180,  -180,   -90,    0); // Esquerda.
        conjBola[2][i] = triangulo(i - 180,   -90,     0,   90); // Frente.
        conjBola[3][i] = triangulo(i - 180,     0,    90,  180); // Direita.
        conjBola[4][i] = triangulo(i - 180,    90,   180,  190); // Extrema Direita.
        
        conjAlvo[0][i] = triangulo(i - 180,  -190,  -180,  -90); // Extrema Esquerda.
        conjAlvo[1][i] = triangulo(i - 180,  -180,   -90,    0); // Esquerda.
        conjAlvo[2][i] = triangulo(i - 180,   -90,     0,   90); // Frente.
        conjAlvo[3][i] = triangulo(i - 180,     0,    90,  180); // Direita.
        conjAlvo[4][i] = triangulo(i - 180,    90,   180,  190); // Extrema Direita.
        
        
        // Perto
        
        conjSaida[0][0][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][1][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[0][2][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[0][3][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[0][4][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        
        conjSaida[1][0][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[1][1][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[1][2][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][3][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][4][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[2][0][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[2][1][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[2][2][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[2][3][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[2][4][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[3][0][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][1][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][2][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][3][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[3][4][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[4][0][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[4][1][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[4][2][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[4][3][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[4][4][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.

        // Médio

        conjSaida[0][0][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][1][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][2][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][3][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[0][4][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        
        conjSaida[1][0][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[1][1][1][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[1][2][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][3][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][4][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[2][0][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[2][1][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[2][2][1][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[2][3][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[2][4][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[3][0][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][1][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][2][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][3][1][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[3][4][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[4][0][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[4][1][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[4][2][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[4][3][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[4][4][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.

        // Longe

        conjSaida[0][0][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][1][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][2][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][3][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[0][4][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[1][0][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][1][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][2][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][3][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[1][4][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaida[2][0][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[2][1][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[2][2][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[2][3][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[2][4][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        
        conjSaida[3][0][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][1][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][2][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][3][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[3][4][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        
        conjSaida[4][0][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[4][1][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[4][2][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[4][3][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaida[4][4][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.

        //theta = theta + variacao;
    }
    
    for(int i = 0; i < 2000; i++)
    {
        conjDist[0][i] = trapezio(i, -10, 0, 75, 100); // Perto.
        conjDist[1][i] = trapezio(i, 75, 100, 300, 325); // Médio.
        conjDist[2][i] = trapezio(i, 300, 325, 2000, 2010); // Longe.
    }
}

int emGraus(float radianos)
{
    return int( (radianos*180)/M_PI );
}

void fuzzyficacao(float ballAngle, float targetAngle, float ballDistance)
{
    for(int i = 0; i < CONJUNTOS_BOLA; i++)
    {
        uBola[i] = conjBola[i][emGraus(ballAngle) + 180];
        uAlvo[i] = conjAlvo[i][emGraus(targetAngle) + 180];
    }
    
    for(int i = 0; i < DISTANCIAS_BOLA; i++)
        uDist[i] = conjDist[i][int(ballDistance)];
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
            for(int k = 0; k < DISTANCIAS_BOLA; k++)
            {
                if(uBola[i] > 0 && uAlvo[j] > 0 && uDist[k] > 0) // Regra ativada.
                {
                    // Obtém a força de disparo da regra
                    // pela conjunção dos graus de pertinência.
                    float alfa = min(uBola[i], min(uAlvo[j], uDist[k]));
                    
                    // Corta o conjunto de saída da regra
                    // e faz a união com os outros conjuntos cortados

                    for(int l = 0; l < TAMANHO_CONJUNTO; l++)
                        conjFinal[l] = max(min(alfa, conjSaida[i][j][k][l]), conjFinal[l]);
                }
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

    // Declaração do objeto que representa o ambiente.
    environm::soccer::clientEnvironm environment;

    // Conecta-se ao SoccerMatch.
    if ( environment.connect( argv[1], atoi(argv[2]) ) == false)
    {
        printf( "Fail connecting to the SoccerMatch.\n\n" );
        
        return 0;  // Cancela operação se não conseguiu conectar-se.
    }

    // Laço de execução de ações.
    printf( "Running...\n\n" );
    
    // Inicializa conjuntos.
    init();

    float ballAngle, targetAngle, leftMotor, rightMotor,  ballDistance;
    float conjFinal[TAMANHO_CONJUNTO];
    
    int k = 25;
    
    while ( true )
    {
        ballAngle = -environment.getBallAngle();
        targetAngle = -environment.getTargetAngle( environment.getOwnGoal() );
        ballDistance = environment.getDistance();
        
        // Obtém graus de pertinência.
        fuzzyficacao(ballAngle, targetAngle, ballDistance);

        // Corta os conjuntos de saída das regras ativadas e faz a união deles
        for(int i = 0; i < TAMANHO_CONJUNTO; i++)
            conjFinal[i] = 0; // A união de 0 com qualquer coisa é qualquer coisa
        
        regras(conjFinal);

        // Obtém saída final
        float resultado = defuzzificacao(conjFinal);

        leftMotor = (resultado/360 + 0.5)*0.6;
        rightMotor  = (resultado/(-360) + 0.5)*0.6;
        
        if(++k >= 25)
        {
            printf("ballAngle = %d | targetAngle = %d\n", emGraus(ballAngle), emGraus(targetAngle));
            printf("resultado = %f\n", resultado);
            printf("left = %f | right = %f\n", leftMotor, rightMotor);
            printf("ballDistance = %f \n\n", ballDistance);
            
            k = 0;
        }
        
        // Transmite ação do robô ao ambiente. Fica bloqueado até que todos os
        // robôs joguem. Se erro, retorna false (neste exemplo, sai do laco).
        if ( environment.act( leftMotor, rightMotor ) == false )
            break; // Termina a execução se falha ao agir.
    }

    return 0;
}
//------------------------------------------------------------------------------

