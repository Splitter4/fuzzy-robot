//------------------------------------------------------------------------------
// Inclusão da biblioteca que implementa a interface com o SoccerMatch.
#include "environm.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    while ( 1 ) {

        // Deve obter os dados desejados do ambiente. Métodos do clientEnvironm.
        // Exemplos de métodos que podem ser utilizados.
        ballAngle = environment.getBallAngle();
        targetAngle = environment.getTargetAngle( environment.getOwnGoal() );


        // A partir dos dados obtidos, deve inferir que ações executar. Neste
        // exemplo as forcas destinadas a cada robo sao guardadas em leftMotor e
        // rightMotor. Esta etapa deve ser substituida pelo controlador fuzzy.
        ballAngle = ballAngle - targetAngle;
        if ( ballAngle < -M_PI ) {
            ballAngle += 2 * M_PI;
        }
        if ( ballAngle > M_PI ) {
            ballAngle -= 2 * M_PI;
        }
        if ( ballAngle < ( -M_PI / 2 ) ) {
            ballAngle = -M_PI / 2;
        }
        if ( ballAngle > ( M_PI / 2 ) ) {
            ballAngle = M_PI / 2;
        }
        leftMotor  = cos( ballAngle ) - sin( ballAngle );
        rightMotor = cos( ballAngle ) + sin( ballAngle );


        // Transmite ação do robô ao ambiente. Fica bloqueado até que todos os
        // robôs joguem. Se erro, retorna false (neste exemplo, sai do laco).
        if ( ! environment.act( leftMotor, rightMotor ) ) {
            break; // Termina a execução se falha ao agir.
        }
    }

    return 0;
}
//------------------------------------------------------------------------------

