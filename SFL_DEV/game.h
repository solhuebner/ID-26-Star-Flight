#ifndef GAME_H
#define GAME_H

void updateStage()
{

}

boolean checkGameOver()
{
  return false;
}

void stateGamePlaying()
{
  updateStage();
  if (checkGameOver())gameState = STATE_GAME_OVER;
}
void stateGamePause()
{
  if (buttons.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_GAME_PLAYING;
}
void stateGameOver()
{
  if (buttons.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

#endif
