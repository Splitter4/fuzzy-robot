//------------------------------------------------------------------------------
// Inclus�o da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMANHO_CONJUNTO 360 + 1 // De -180 at� 180, incluindo o 0.

// Conjuntos relativos � posi��o da bola.
float Eb[TAMANHO_CONJUNTO];
float Fb[TAMANHO_CONJUNTO];
float Db[TAMANHO_CONJUNTO];

// Conjuntos relativos � posi��o do alvo.
float Ea[TAMANHO_CONJUNTO];
float Fa[TAMANHO_CONJUNTO];
float Da[TAMANHO_CONJUNTO];

// Graus de pertin�ncia dos conjuntos de entrada.
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

// Inicializa��o de conjuntos.
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

int main( int argc, char* argv[] ) {

    float ballAngle, targetAngle, leftMotor, rightMotor;

    // Declara��o do objeto que representa o ambiente.
    environm::soccer::clientEnvironm environment;

    if ( argc != 3 )
    {
        printf( "Invalid parameters. Expecting:\n" );
        printf( "SoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER\n" );
        printf( "\nExample:\n" );
        printf( "SoccerPlayer localhost 1024\n\n" );  
              
        return 0;
    }

    // Conecta-se ao SoccerMatch. Sup�e que SoccerMatch est� rodando na m�quina
    // local e que um dos rob�s esteja na porta 1024. Porta e local podem mudar.
    if ( ! environment.connect( argv[1], atoi( argv[2] ) ) )
    {
        printf( "Fail connecting to the SoccerMatch.\n\n" );
        
        return 0;  // Cancela opera��o se n�o conseguiu conectar-se.
    }

    // La�o de execu��o de a��es.
    printf( "Running...\n" );
    
    // Inicializa conjuntos.
    init();

    while ( 1 ) {

        // Deve obter os dados desejados do ambiente. M�todos do clientEnvironm.
        // Exemplos de m�todos que podem ser utilizados.
        ballAngle = environment.getBallAngle();
        targetAngle = environment.getTargetAngle( environment.getRivalGoal() );
        
        // Obt�m graus de pertin�ncia.
        fuzzyficacao(ballAngle,targetAngle);
        
        // Corta conjuntos e os une.
        //inferencia();
        
        // Obt�m valor final.
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

