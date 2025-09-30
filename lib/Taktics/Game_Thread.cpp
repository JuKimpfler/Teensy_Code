#include "Game_Thread.h"
#include "Elementar_Taktics.h"
GameC Game;

void GameC::Run(){
    if (LineCalc.Distance != 0){ // Line Avoidance System
        Robot.Drive(LineCalc.DriveAngle,0,HighSpeed);
        Rand_Dir = random(16)*22.5;
    }
    else{
        Robot.Drive(Rand_Dir,0,30);
        /*if (LDR.Aktiv()){
            Taktics.BallSearch();
        }
        else{
            Taktics.GoalAttak();
        }*/
       Taktics.CalculateAttackChances();

       myAttackChances = Taktics.AttackChances;
       //send my attack chances

       //subscribe to opp bluetooth channel

       if(AttackerPreference){
        if(myAttackChances > subOppAttackChances){
            DefenderTactics.step();
        }
        else{
            AttackerTactics.step();
        }
       }
       else{
            if(subOppAttackChances > myAttackChances){
                AttackerTactics.step();
            }
            else{
                DefenderTactics.step();
            }
        }   
    }
}



