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

#define CLOSE   1 
#define NEAR    2 
#define FAR     3 

// Conjuntos relativos à posição da bola.
float conjBola[CONJUNTOS_BOLA][TAMANHO_CONJUNTO];

// Conjuntos relativos à posição do alvo.
float conjAlvo[CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Conjuntos relativos à posição de saida quando a bola esta muito perto.
float conjSaidaClose[CONJUNTOS_BOLA][CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Conjuntos relativos à posição de saidaquando a bola esta perto.
float conjSaidaNear[CONJUNTOS_BOLA][CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Conjuntos relativos à posição de saida quando a bola esta longe.
float conjSaidaFar[CONJUNTOS_BOLA][CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Graus de pertinência dos conjuntos de entrada.
float uBola[CONJUNTOS_BOLA];
float uAlvo[CONJUNTOS_ALVO];


float triangulo(float x, float alfa, float beta, float gama)
{
    float mi = 0;
    
    if ((x <= alfa) || (x >= gama)) 
    {
        mi = 0;
    }
    else if ((x > alfa) && (x <= beta)) 
    {
        mi = (x - alfa)/(beta - alfa);
    }
    else if ((x > beta) && (x < gama)) 
    {
        mi = (gama - x)/(gama - beta);
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
        


        // CLOSE

        conjSaidaClose[0][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaClose[0][1][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaClose[0][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaClose[0][3][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[0][4][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        
        conjSaidaClose[1][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[1][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[1][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaClose[1][3][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaClose[1][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaClose[2][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[2][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaClose[2][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaClose[2][3][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[2][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaClose[3][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[3][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[3][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaClose[3][3][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaClose[3][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaClose[4][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaClose[4][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaClose[4][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaClose[4][3][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaClose[4][4][i] = triangulo(i - 180,     0,    180,    190); // Direita.

        // NEAR

        conjSaidaNear[0][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[0][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[0][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[0][3][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[0][4][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        
        conjSaidaNear[1][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[1][1][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaNear[1][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[1][3][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[1][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaNear[2][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[2][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[2][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaNear[2][3][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[2][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaNear[3][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[3][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[3][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[3][3][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaNear[3][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaNear[4][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[4][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaNear[4][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[4][3][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaNear[4][4][i] = triangulo(i - 180,     0,    180,    190); // Direita.

        // FAR

        conjSaidaFar[0][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[0][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[0][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[0][3][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[0][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaFar[1][0][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[1][1][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[1][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[1][3][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaidaFar[1][4][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        
        conjSaidaFar[2][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaFar[2][1][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaFar[2][2][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaFar[2][3][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaidaFar[2][4][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        
        conjSaidaFar[3][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[3][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[3][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[3][3][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[3][4][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        
        conjSaidaFar[4][0][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[4][1][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[4][2][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[4][3][i] = triangulo(i - 180,     0,    180,    190); // Direita.
        conjSaidaFar[4][4][i] = triangulo(i - 180,     0,    180,    190); // Direita.

        //theta = theta + variacao;
    }
}

int emGraus(float radianos)
{
    return int( (radianos*180)/M_PI );
}

void fuzzyficacao(float ballAngle, float targetAngle)
{
    for(int i = 0; i < CONJUNTOS_BOLA; i++)
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

void regras(float *conjFinal, int distBola)
{
    for(int i = 0; i < CONJUNTOS_BOLA; i++)
    {
        for(int j = 0; j < CONJUNTOS_ALVO; j++)
        {
            if(uBola[i] > 0 && uAlvo[j] > 0) // Regra ativada.
            {
                // Obtém a força de disparo da regra
                // pela conjunção dos graus de pertinência.
                float alfa = min(uBola[i], uAlvo[j]);
                
                // Corta o conjunto de saída da regra
                // e faz a união com os outros conjuntos cortados

                switch (distBola) {
                    case CLOSE:
                        for(int k = 0; k < TAMANHO_CONJUNTO; k++)
                            conjFinal[k] = max(min(alfa, conjSaidaClose[i][j][k]), conjFinal[k]);
                        break;
                    case NEAR:
                        for(int k = 0; k < TAMANHO_CONJUNTO; k++)
                            conjFinal[k] = max(min(alfa, conjSaidaNear[i][j][k]), conjFinal[k]);
                        break;
                    case FAR:
                        for(int k = 0; k < TAMANHO_CONJUNTO; k++)
                            conjFinal[k] = max(min(alfa, conjSaidaFar[i][j][k]), conjFinal[k]);
                        break;
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
    int distanciaBola;
    float conjFinal[TAMANHO_CONJUNTO];
    
    int k = 25;
    
    while ( true )
    {
        ballAngle = -environment.getBallAngle();
        targetAngle = -environment.getTargetAngle( environment.getOwnGoal() );
        ballDistance = environment.getDistance();

        if(ballDistance < 75)
            distanciaBola = CLOSE;
        else{
            if(ballDistance < 300)
                distanciaBola = NEAR;
            else
                distanciaBola = FAR;
        }
        
        // Obtém graus de pertinência.
        fuzzyficacao(ballAngle, targetAngle);

        // Corta os conjuntos de saída das regras ativadas e faz a união deles
        for(int i = 0; i < TAMANHO_CONJUNTO; i++)
            conjFinal[i] = 0; // A união de 0 com qualquer coisa é qualquer coisa
        
        regras(conjFinal,distanciaBola);

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

