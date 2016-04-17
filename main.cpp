//------------------------------------------------------------------------------
// Inclusão da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO 360 + 1 // de -180 até 180 incluindo o 0

// Conjuntos relativos à posição da bola.
float Eb[TAMANHO_CONJUNTO];
float Fb[TAMANHO_CONJUNTO];
float Db[TAMANHO_CONJUNTO];

// Conjuntos relativos à posição do alvo.
float Ea[TAMANHO_CONJUNTO];
float Fa[TAMANHO_CONJUNTO];
float Da[TAMANHO_CONJUNTO];

// Graus de pertinência para cada conjunto.
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

void init()
{
    // Inicialização de conjuntos.

    //int variacao = 360/TAMANHO_CONJUNTO;
    //float theta = -180;

    for (int x = -180; x <= TAMANHO_CONJUNTO; x++)
    {
        Eb[x] = triangulo(x,-190,-180,0);
        Fb[x] = triangulo(x,-90,0,90);
        Db[x] = triangulo(x,-0,180,190);

        Ea[x] = triangulo(x,-190,-180,0);
        Fa[x] = triangulo(x,-90,0,90);
        Da[x] = triangulo(x,-0,180,190);

        //theta = theta + variacao;
    }
}

float toDegrees(float radians)
{
    return (radians*180)/M_PI;
}

void fuzzificacao(float ballAngle, float targetAngle)
{
    /*
    int i, j = 0;

    if (ballAngle < 0)
    {
        i = 180 + ballAngle;

        while (i>variacao)
        {
            j++;
            i = i - variacao;
        }
    }
    else
    {
        i = ballAngle;
        j = 36; // Posição 0º

        while (i>=variacao)
        {
            j++;
            i = i - variacao;
        }
    }
    */

    bolaDireita  = Db[int(ballAngle)];
    bolaFrente   = Fb[int(ballAngle)];
    bolaEsquerda = Eb[int(ballAngle)];

    /*
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
    */

    alvoDireita  = Da[int(targetAngle)];
    alvoFrente   = Fa[int(targetAngle)];
    alvoEsquerda = Ea[int(targetAngle)];
}

int main( int argc, char* argv[] ) {

    float ballAngle, targetAngle, leftMotor, rightMotor;

    // Declaração do objeto que representa o ambiente.
    environm::soccer::clientEnvironm environment;

    if ( argc != 3 )
    {
        printf( "\nInvalid parameters. Expecting:" );
        printf( "\nSoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER" );
        printf( "\n\nExample:" );
        printf( "\nSoccerPlayer localhost 1024\n" );

        return 0;
    }

    // Conecta-se ao SoccerMatch. Supõe que SoccerMatch está rodando na máquina
    // local e que um dos robôs esteja na porta 1024. Porta e local podem mudar.
    if ( environment.connect( argv[1], atoi(argv[2]) ) != true )
    {
        printf( "\nFail connecting to the SoccerMatch.\n" );

        return 0; // Cancela operação se não conseguiu conectar-se.
    }

    // Laço de execução de ações.
    printf( "\nRunning..." );

    // Inicializa conjuntos.
    init();

    while ( 1 ) {

        ballAngle = environment.getBallAngle();
        targetAngle = environment.getTargetAngle( environment.getRivalGoal() );

        // Obtém os graus de pertinência.
        fuzzificacao( toDegrees(ballAngle), toDegrees(targetAngle) );

        // Corta os conjuntos e faz a união deles
        //inferencia();

        // Obtém saída final
        //defuzzificacao();

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

