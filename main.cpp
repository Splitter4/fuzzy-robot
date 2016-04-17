//------------------------------------------------------------------------------
// Inclusão da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO 72

int variacao = 360/TAMANHO_CONJUNTO;


float Eb[TAMANHO_CONJUNTO];
float Fb[TAMANHO_CONJUNTO];
float Db[TAMANHO_CONJUNTO];

float Ea[TAMANHO_CONJUNTO];
float Fa[TAMANHO_CONJUNTO];
float Da[TAMANHO_CONJUNTO];

float bolaDireita;
float bolaFrente;
float bolaEsquerda;
float alvoDireita;
float alvoFrente;
float alvoEsquerda;

float triangulo(float x,float alfa,float beta,float gama)
{
    float mi=0;
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

void init()
{
    int i;

    float theta = -180;
    for (i=0; i<=TAMANHO_CONJUNTO; i++)
    {
        Eb[i] = triangulo(theta,-190,-180,0);
        Fb[i] = triangulo(theta,-90,0,90);
        Db[i] = triangulo(theta,-0,180,190);

        theta = theta + variacao;
    }

    theta = -180;
    for (i=0; i<=TAMANHO_CONJUNTO; i++)
    {
        Ea[i] = triangulo(theta,-190,-180,0);
        Fa[i] = triangulo(theta,-90,0,90);
        Da[i] = triangulo(theta,-0,180,190);

        theta = theta + variacao;
    }

    return;

}

void fuzzyficacao(float ballAngle,float targetAngle)
{
    int i,j=0;
    if (ballAngle<0)
    {
        i = 180 + ballAngle;
        while (i>variacao)
        {
            j++;
            i = i - variacao;
        }
    }else
    {
        i = ballAngle;
        j=36; // Posição 0º
        while (i>=variacao)
        {
            j++;
            i = i - variacao;
        }
    }

    bolaDireita  = Db[i];
    bolaFrente   = Fb[i];
    bolaEsquerda = Eb[i];

    if (targetAngle<0)
    {
        i = 180 + targetAngle;
        while (i>5)
        {
            j++;
            i = i - 5;
        }
    }else
    {
        i = targetAngle;
        j=36; // Posição 0º
        while (i>=variacao)
        {
            j++;
            i = i - variacao;
        }
    }

    alvoDireita  = Da[i];
    alvoFrente   = Fa[i];
    alvoEsquerda = Ea[i];
}

int main( int argc, char* argv[] ) {

    float   ballAngle, targetAngle, leftMotor, rightMotor;

    // Declaração do objeto que representa o ambiente.
    environm::soccer::clientEnvironm environment;

    if ( argc != 3 ) {
        printf( "\nInvalid parameters. Expecting:" );
        printf( "\nSoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER\n" );
        printf( "\nSoccerPlayer localhost 1024\n" );        
        return 0;
    }

    // Conecta-se ao SoccerMatch. Supõe que SoccerMatch está rodando na máquina
    // local e que um dos robôs esteja na porta 1024. Porta e local podem mudar.
    if ( ! environment.connect( argv[1], atoi( argv[2] ) ) ) {
        printf( "\nFail connecting to the SoccerMatch.\n" );
        return 0;  // Cancela operação se não conseguiu conectar-se.
    }

    // Laço de execução de ações.
    printf( "\nRunning..." );

    init();

    while ( 1 ) {

        // Deve obter os dados desejados do ambiente. Métodos do clientEnvironm.
        // Exemplos de métodos que podem ser utilizados.
        ballAngle = environment.getBallAngle();
        targetAngle = environment.getTargetAngle( environment.getOwnGoal() );


        // A partir dos dados obtidos, deve inferir que ações executar.

        fuzzyficacao(ballAngle,targetAngle);



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

