//------------------------------------------------------------------------------
// Inclusão da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO 360 + 1 // De -180 até 180, incluindo o 0.
#define CONJUNTOS_BOLA 3 // Esquerda, frente e direita.
#define CONJUNTOS_ALVO 3 // Esquerda, frente e direita.

// Conjuntos relativos à posição da bola.
/*
float Eb[TAMANHO_CONJUNTO];
float Fb[TAMANHO_CONJUNTO];
float Db[TAMANHO_CONJUNTO];
*/
float conjBola[CONJUNTOS_BOLA][TAMANHO_CONJUNTO];

// Conjuntos relativos à posição do alvo.
/*
float Ea[TAMANHO_CONJUNTO];
float Fa[TAMANHO_CONJUNTO];
float Da[TAMANHO_CONJUNTO];
*/
float conjAlvo[CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Conjuntos relativos à posição de saida.
/*
float Er[TAMANHO_CONJUNTO];
float Fr[TAMANHO_CONJUNTO];
float Dr[TAMANHO_CONJUNTO];
*/
float conjSaida[CONJUNTOS_BOLA][CONJUNTOS_ALVO][TAMANHO_CONJUNTO];

// Graus de pertinência dos conjuntos de entrada.
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
        mi = 0;
        return mi;
    }
    
    if ((x >= alfa) && (x <= beta)) 
    {
        mi = (x - alfa)/(beta - alfa);
        return mi;
    }
    
    if ((x >  beta) && (x <  gama)) 
    {
        mi = (gama - x)/(gama - beta);
        return mi;
    }
    
    return mi;
} 

// Inicialização de conjuntos.
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
        conjSaida[0][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.
        conjSaida[1][2][i] = triangulo(i - 180,  -190,   -180,      0); // Esquerda.
        conjSaida[2][0][i] = triangulo(i - 180,    -0,    180,    190); // Direita.
        conjSaida[2][1][i] = triangulo(i - 180,    -0,    180,    190); // Direita.
        conjSaida[0][0][i] = triangulo(i - 180,   -90,      0,     90); // Frente.

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
                // Obtém a força de disparo da regra
                // pela conjunção dos graus de pertinência.
                float alfa = min(uBola[i], uAlvo[j]);
                
                // Corta o conjunto de saída da regra
                // e faz a união com os outros conjuntos cortados
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

    float ballAngle, targetAngle, leftMotor, rightMotor;
    
    float conjFinal[TAMANHO_CONJUNTO];
    
    int k = 15;
    
    while ( true )
    {
        ballAngle = -environment.getBallAngle();
        targetAngle = -environment.getTargetAngle( environment.getRivalGoal() ) - M_PI;
        
        // Obtém graus de pertinência.
        fuzzyficacao(ballAngle, targetAngle);

        // Corta os conjuntos de saída das regras ativadas e faz a união deles
        for(int i = 0; i < TAMANHO_CONJUNTO; i++)
            conjFinal[i] = 0; // A união de 0 com qualquer coisa é qualquer coisa
        
        regras(conjFinal);

        // Obtém saída final
        float resultado = defuzzificacao(conjFinal);
        
        if ((resultado >= -180) && (resultado < -45)) 
        {
            leftMotor = (resultado - (-180))/(90 - (-180));
        }
        
        if ((resultado >= -45) && (resultado < 0)) 
        {
            leftMotor = (resultado - (-135))/(45 - (-135));
        }
        
        if ((resultado >= 0) && (resultado <= 180)) 
        {
            leftMotor = (resultado - (-540))/(180 - (-540));
        }
        
        if ((resultado >= -180) && (resultado < 0)) 
        {
            rightMotor = (540 - resultado)/(540 - (-180));
        }
        
        if ((resultado >= 0) && (resultado < 45)) 
        {
            rightMotor = (135 - resultado)/(135 - (-45));
        }
        
        if ((resultado >= 45) && (resultado <= 180)) 
        {
            rightMotor = (180 - resultado)/(180 - (-90));
        }
        
        leftMotor = (leftMotor*2 -1)*0.1;
        rightMotor  = (rightMotor*2 -1)*0.1;
        
        /*
        leftMotor = (resultado/360 + 0.5)*0.1;
        rightMotor  = (resultado/(-360) + 0.5)*0.1;
        */
        
        if(++k >= 15)
        {
            printf("ballAngle = %d | targetAngle = %d\n", emGraus(ballAngle), emGraus(targetAngle));
            printf("resultado = %f\n", resultado);
            printf("left = %f | right = %f\n\n", leftMotor, rightMotor);
            
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

