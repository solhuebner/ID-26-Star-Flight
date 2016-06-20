#ifndef LEVELS_H
#define LEVELS_H

#include <Arduino.h>
#include "globals.h"
#include "enemies.h"
#include "player.h"

#define TOTAL_AMOUNT_OF_LEVELS  2
#define TOTAL_AMOUNT_OF_WAVES   5

byte currentWave;
byte previousWave;

boolean checkStartWave()
{
  if (currentWave != previousWave)
  {
    previousWave = currentWave;
    return true;
  }
  return false;
}


boolean checkEndWave()
{
  byte test = 0;
  for (byte i = 0; i < MAX_ONSCREEN_ENEMIES; i++)
  {
    test = test + enemy[i].isActive;
  }
  if (test < 1) currentWave++;
}

boolean checkEndLevel()
{
  if (currentWave > TOTAL_AMOUNT_OF_WAVES - 1)
  {
    return true;
  }
  return false;
}

void enemySetInLine(byte firstEnemy, byte lastEnemy, byte enemyType, byte x, int y, int spacingX, int spacingY)
{
  for (byte i = firstEnemy; i < lastEnemy; i++)
  {
    enemy[i].isActive = true;
    enemy[i].type = enemyType;
    enemy[i].x = x + (spacingX * (i - firstEnemy));
    enemy[i].y = y + (spacingY * (i - firstEnemy));
    enemy[i].HP = 2;
  }
}

void enemyFlyRightLeft(byte firstEnemy, byte lastEnemy, byte speedEnemy)
{
  for (byte i = firstEnemy; i < lastEnemy; i++)
  {
    enemy[i].x = enemy[i].x - speedEnemy;
  }
}

void enemyFlyToMiddle(byte firstEnemy, byte lastEnemy, byte speedEnemy)
{
  for (byte i = firstEnemy; i < lastEnemy; i++)
  {
    enemy[i].x = enemy[i].x - speedEnemy;
    if (enemy[i].x < 64)
    {
      if (enemy[i].y < 31) enemy[i].y++;
      if (enemy[i].y > 32) enemy[i].y--;
    }
  }
}

void enemyFlySine(byte firstEnemy, byte lastEnemy, byte speedEnemy)
{
  for (byte i = firstEnemy; i < lastEnemy; i++)
  {
    enemy[i].x = enemy[i].x - speedEnemy;
    if ((enemy[i].x < 120 ) && (enemy[i].x > 104) && (enemy[i].y > 16)) enemy[i].y--;
    if ((enemy[i].x < 105) && (enemy[i].x > 73) && (enemy[i].y < 48)) enemy[i].y++;
    if ((enemy[i].x < 74 ) && (enemy[i].x > 42) && (enemy[i].y > 16)) enemy[i].y--;
    if ((enemy[i].x < 43) && (enemy[i].x > 10) && (enemy[i].y < 48)) enemy[i].y++;
    if ((enemy[i].x < 11) && (enemy[i].y > 16)) enemy[i].y--;
  }
}

void enemyFlyDownUpToMiddle(byte firstEnemy, byte lastEnemy, byte speedEnemy)
{
  for (byte i = firstEnemy; i < lastEnemy; i++)
  {
    if (enemy[i].y > 23 ) enemy[i].y--;
    else enemy[i].x -= speedEnemy;
  }
}

void enemyFollowUpSpaceShip(byte firstEnemy, byte lastEnemy, byte speedEnemy)
{
  for (byte i = firstEnemy; i < lastEnemy; i++)
  {
    if ((enemy[i].y > spaceShip.y) && (enemy[i].x > spaceShip.x + 24)) enemy[i].y--;
    else enemy[i].x -= speedEnemy;
  }
}

void enemyFollowDownSpaceShip(byte firstEnemy, byte lastEnemy, byte speedEnemy)
{
  for (byte i = firstEnemy; i < lastEnemy; i++)
  {
    if ((enemy[i].y < spaceShip.y) && (enemy[i].x > spaceShip.x + 24)) enemy[i].y++;
    else enemy[i].x -= speedEnemy;
  }
}

void wave001()
{
  if (checkStartWave())enemySetInLine(0, 3, ENEMY_AQY_039, 128, 12, 20, 0);
  enemyFlyRightLeft(0, 3, 1);
  checkEndWave();
}

void wave002()
{
  if (checkStartWave())enemySetInLine(0, 3, ENEMY_HAL_506, 128, 32, 20, 0);
  enemyFlyRightLeft(0, 3, 1);
  checkEndWave();
}

void wave003()
{
  if (checkStartWave())enemySetInLine(0, 3, ENEMY_LQP_308, 64, 64, 20, 8);
  enemyFlyDownUpToMiddle(0, 3, 3);
  checkEndWave();
}

void wave004()
{
  if (checkStartWave())enemySetInLine(0, 3, ENEMY_ZPV_127, 96, 64, 20, 16);
  enemyFollowUpSpaceShip(0, 3, 3);
  checkEndWave();
}

void wave005()
{
  if (checkStartWave())enemySetInLine(0, 3, ENEMY_ZPV_127, 96, -16, 20, -16);
  enemyFollowDownSpaceShip(0, 3, 3);
  checkEndWave();
}

typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM Levels[TOTAL_AMOUNT_OF_LEVELS][TOTAL_AMOUNT_OF_WAVES] =
{
  { //LEVEL 01
    wave005,
    wave004,
    wave003,
    wave002,
    wave001,
  },
  { //LEVEL 02
    wave001,
    wave002,
    wave003,
    wave004,
    wave005,
  }
};


void drawLifesHUD()
{
  for (byte i = 0; i < spaceShip.life; i++) sprites.drawPlusMask(i * 6, 0, hudDisplay_plus_mask, 10);
}

void drawBombsHUD()
{
  for (byte i = 0; i < spaceShip.bombs; i++) sprites.drawPlusMask(40+(i*8), 0, hudDisplay_plus_mask, 11);
}


void drawScore(byte scoreX, byte scoreY, byte fontType)
{
  char buf[10];
  //scorePlayer = arduboy.cpuLoad();
  ltoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 7 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawPlusMask(scoreX + (5 * i), scoreY, hudDisplay_plus_mask, 0);
        break;
      case SCORE_BIG_FONT:
        //sprites.drawSelfMasked(scoreX + (7 * i), scoreY, numbersBig, 0);
        break;
    }
  }

  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    byte j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (byte z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    switch (fontType)
    {
      case SCORE_SMALL_FONT:
        sprites.drawPlusMask(scoreX + (pad * 5) + (5 * i), scoreY, hudDisplay_plus_mask, digit);
        break;
      case SCORE_BIG_FONT:
        //sprites.drawSelfMasked(scoreX + (pad * 7) + (7 * i), scoreY, numbersBig, digit);
        break;
    }
  }
}


#endif