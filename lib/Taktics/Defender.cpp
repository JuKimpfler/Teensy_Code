#include "Defender.h"

DefenderTacticsC DefenderTactics;

void DefenderTacticsC::step()
{
    if (LDR.Aktiv() == true)
    {
        Elem_Taktics.BallCaught();
    }
    else
    {
        Defence();
    }
}

void DefenderTacticsC::Defence()
{
    // Do homing and defend the goal
    // Hier würd ich falls der Attacker vor dem Ball ist immer zwichen Ball und Tor fahren
    if (abs(US.Distance) > 180)
    {
        Elem_Taktics.Ballsearch();
    }
    else
    {
        Robot.Drive(abs(US.Distance) / US.Distance * -45);
    }
}
