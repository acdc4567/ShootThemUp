// Shoot Them Up Game.All Rights Reserved


#include "ArtificialIntelligence/STUAICharacter.h"

ASTUAICharacter(const FObjectInitializer& ObjInit):Super(ObjInit){
    AutoPossessAI=EAutoPossessAI:PlacedInWorldOrSpawned;
    AIControllerClass=ASTUAIController::StaticClass();
}