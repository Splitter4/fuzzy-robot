//------------------------------------------------------------------------------
// Inclus�o da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO 360 + 1 // de -180 at� 180 incluindo o 0

// Conjuntos relativos � posi��o da bola.
float Eb[TAMANHO_CONJUNTO];
float Fb[TAMANHO_CONJUNTO];
float Db[TAMANHO_CONJUNTO];

// Conjuntos relativos � posi��o do alvo.
float Ea[TAMANHO_CONJUNTO];
float Fa[TAMANHO_CONJUNTO];
float Da[TAMANHO_CONJUNTO];

// Graus de pertin�ncia para cada conjunto.
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
    // Inicializa��o de conjuntos.

    //int variacao = 360/TAMANHO_CONJUNTO;
    //float theta = -180;

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
        j = 36; // Posi��o 0�

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
        j=36; // Posi��o 0�
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

    // Declara��o do objeto que representa o ambiente.
    environm::soccer::clientEnvironm environment;

    if ( argc != 3 )
    {
        printf( "\nInvalid parameters. Expecting:" );
        printf( "\nSoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER" );
        printf( "\n\nExample:" );
        printf( "\nSoccerPlayer localhost 1024\n" );

        return 0;
    }

    // Conecta-se ao SoccerMatch. Sup�e que SoccerMatch est� rodando na m�quina
    // local e que um dos rob�s esteja na porta 1024. Porta e local podem mudar.
    if ( environment.connect( argv[1], atoi(argv[2]) ) != true )
    {
        printf( "\nFail connecting to the SoccerMatch.\n" );

        return 0; // Cancela opera��o se n�o conseguiu conectar-se.
    }

    // La�o de execu��o de a��es.
    printf( "\nRunning..." );

    // Inicializa conjuntos.
    init();

    while ( 1 ) {

        ballAngle = environment.getBallAngle();
        targetAngle = environment.getTargetAngle( environment.getRivalGoal() );

        // Obt�m os graus de pertin�ncia.
        fuzzificacao( toDegrees(ballAngle), toDegrees(targetAngle) );

        // Corta os conjuntos e faz a uni�o deles
        //inferencia();

        // Obt�m sa�da final
        //defuzzificacao();

        leftMotor  = 0.1;
        rightMotor = 0.1;

        // Transmite a��o do rob� ao ambiente. Fica bloqueado at� que todos os
        // rob�s joguem. Se erro, retorna false (neste exemplo, sai do laco).
        if ( ! environment.act( leftMotor, rightMotor ) ) {
            break; // Termina a execu��o se falha ao agir.
        }
    }

    return 0;
}
//------------------------------------------------------------------------------

