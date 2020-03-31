//main.c
// Created by Garrison Davis
// 3/31/2020
#include "gba.h"
#include <stdbool.h>

void sync()
{
    //only modify VRAM if in V_BUFFER
    //typically we would put logic, physics, ai
    while(REG_DISPLAY_VCOUNT >= 160);
    while(REG_DISPLAY_VCOUNT < 160);
}

uint16 makeColor(uint8 r, uint8 g, uint8 b)
{
    return (r & 0x1f) | ((g & 0x1f) << 5) | ((b & 0x1f) << 10);
}

uint32 clamp(uint32 value, uint32 min, uint32 max)
{
    return (value < min ? min : (value > max ? max : value));
}

void drawRect(struct Rect rect, uint16 color)
{
    for(int y = 0; y < rect.h; y++)
    {
        for(int x = 0; x < rect.w; x++)
        {
            SCREENBUFFER[(rect.y + y) * SCREEN_WIDTH + rect.x + x] = color;
        }
    }
}

void init7seg()
{
    a.w = 16;
    a.h = 4;
    f.w = 4;
    f.h = 16;

    b = c = e = f;
    d = g = a;

    a.x = d.x = e.x = f.x = g.x = SCREEN_WIDTH/4;
    b.x = c.x = a.x + a.w;

    a.y = b.y = f.y = 0;
    c.y = e.y = g.y = b.y + b.h - a.h;
    d.y = g.y + b.h - a.h;

    h.w = 16;
    h.h = 4;
    m.w = 4;
    m.h = 16;

    i = j = l = m;
    k = n = h;

    h.x = k.x = l.x = m.x = n.x = SCREEN_WIDTH/1.5;
    i.x = j.x = h.x + h.w;

    h.y = i.y = m.y = 0;
    j.y = l.y = n.y = i.y + i.h - h.h;
    k.y = n.y + i.h - h.h;
}

void clear7segOne()
{
    drawRect(a, makeColor(0,0,0));
    drawRect(b, makeColor(0,0,0));
    drawRect(c, makeColor(0,0,0));
    drawRect(d, makeColor(0,0,0));
    drawRect(e, makeColor(0,0,0));
    drawRect(f, makeColor(0,0,0));
    drawRect(g, makeColor(0,0,0));
}

void clear7segTwo(){
  drawRect(h, makeColor(0,0,0));
  drawRect(i, makeColor(0,0,0));
  drawRect(j, makeColor(0,0,0));
  drawRect(k, makeColor(0,0,0));
  drawRect(l, makeColor(0,0,0));
  drawRect(m, makeColor(0,0,0));
  drawRect(n, makeColor(0,0,0));
}

void draw7segOne(uint8 num)
{
    clear7segOne();
    bool w,x,y,z;

    w = (num >= 8 ? ((num-=8) ? 1 : 1) : 0);
    x = (num >= 4 ? ((num-=4) ? 1 : 1) : 0);
    y = (num >= 2 ? ((num-=2) ? 1 : 1) : 0);
    z = (num >= 1 ? ((num-=1) ? 1 : 1) : 0);

    //this is where the crazy if statements go
    if(w || y || (!x && !z) || (x && z))
        drawRect(a, makeColor(0x1f, 0x1f, 0x1f));
    if((y && z) || (!y && !z) || !x)
        drawRect(b, makeColor(0x1f, 0x1f, 0x1f));
    if(x || !y || z)
        drawRect(c, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (y && !z) || (!x && !z) || (!x && y) || (x && !y && z))
        drawRect(d, makeColor(0x1f, 0x1f, 0x1f));
    if((y && !z) || (!x && !z))
        drawRect(e, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (x && !y) || (x && !z) ||(!y && !z))
        drawRect(f, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (!y && x) || (y && !z) || (y && !x))
        drawRect(g, makeColor(0x1f, 0x1f, 0x1f));
}

void draw7segTwo(uint8 num)
{
    clear7segTwo();
    bool w,x,y,z;

    w = (num >= 8 ? ((num-=8) ? 1 : 1) : 0);
    x = (num >= 4 ? ((num-=4) ? 1 : 1) : 0);
    y = (num >= 2 ? ((num-=2) ? 1 : 1) : 0);
    z = (num >= 1 ? ((num-=1) ? 1 : 1) : 0);

    //this is where the crazy if statements go
    if(w || y || (!x && !z) || (x && z))
        drawRect(h, makeColor(0x1f, 0x1f, 0x1f));
    if((y && z) || (!y && !z) || !x)
        drawRect(i, makeColor(0x1f, 0x1f, 0x1f));
    if(x || !y || z)
        drawRect(j, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (y && !z) || (!x && !z) || (!x && y) || (x && !y && z))
        drawRect(k, makeColor(0x1f, 0x1f, 0x1f));
    if((y && !z) || (!x && !z))
        drawRect(l, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (x && !y) || (x && !z) ||(!y && !z))
        drawRect(m, makeColor(0x1f, 0x1f, 0x1f));
    if(w || (!y && x) || (y && !z) || (y && !x))
        drawRect(n, makeColor(0x1f, 0x1f, 0x1f));
}



int main()
{
  while(1){
  init7seg();
  uint8 valOne = 0;
  uint8 valTwo = 0;
  uint8 counter = 0;
  while(valOne < 7 && valTwo < 7){
    REG_DISPLAY = VIDEOMODE | BGMODE;
    drawRect(prevBall, makeColor(0,0,0));
    //setting up temp variables for Ball
    uint32 ballLeft = 120;
    uint32 ballTop = 80;
    uint32 speedX = 1;
    uint32 speedY = 1;

    //setting up values for player
    uint32 topOne = 0;
    uint32 left = 0;

    //setting up values for player
    uint32 topTwo = 0;
    uint32 right = 0;

    //player is 24 pixel height, 8 pixels wide
    playerOne.h = 32;
    playerOne.w = 8;
    playerOne.x = 0;
    playerOne.y = 0;

    playerTwo.h = 32;
    playerTwo.w = 8;
    playerTwo.x = SCREEN_WIDTH - 8;
    playerTwo.y = 0;
    uint16 p_color = makeColor(0, 0x1f, 0);

    ball.h = 8;
    ball.w = 8;
    ball.x = ballLeft;
    ball.y = ballTop;
    prevBall = ball;
    uint16 b_color = makeColor(0, 0, 0x1f);

    while(ball.x != 0 && ball.x != SCREEN_WIDTH - ball.w)
    {
        sync();

        //erase prev ball position
        drawRect(prevBall, makeColor(0,0,0));
        drawRect(prevPlayerOne, makeColor(0,0,0));
        drawRect(prevPlayerTwo, makeColor(0,0,0));

        ballTop += speedY;
        ballLeft += speedX;

        if(!(REG_KEY_INPUT & DOWN))
            topOne++;
        if(!(REG_KEY_INPUT & UP) && topOne != 0)
            topOne--;
        if(!(REG_KEY_INPUT & B))
            topTwo++;
        if(!(REG_KEY_INPUT & A) && topTwo != 0)
            topTwo--;

        topOne = clamp(topOne, 0, SCREEN_HEIGHT - playerOne.h);
        topTwo = clamp(topTwo, 0, SCREEN_HEIGHT - playerTwo.h);

        //do more stuff
        ballTop = clamp(ballTop, 0, SCREEN_HEIGHT - ball.h);
        ballLeft = clamp(ballLeft, 0, SCREEN_WIDTH - ball.w);

        if(ballTop == 0 || ballTop == SCREEN_HEIGHT - ball.h )
            speedY = -speedY;
        if(ballLeft == 0 || ballLeft == SCREEN_WIDTH - ball.w )
            speedX = -speedX;

        ball.x = ballLeft;
        ball.y = ballTop;
        prevBall = ball;

        if(ball.x == playerOne.w)
        {
            if(ball.y < playerOne.y + playerOne.h && ball.y + ball.h > playerOne.y)
            {
                speedX = -speedX;
            }
        }

        if(ball.x == playerTwo.x - 8)
        {
            if(ball.y < playerTwo.y + playerTwo.h && ball.y + ball.h > playerTwo.y)
            {
                speedX = -speedX;
            }
        }

        playerOne.y = topOne;
        prevPlayerOne = playerOne;

        playerTwo.y = topTwo;
        prevPlayerTwo = playerTwo;

        //draw area
        draw7segOne(valOne);
        draw7segTwo(valTwo);
        drawRect(playerOne, p_color);
        drawRect(playerTwo, p_color);
        drawRect(ball, b_color);

    }
    if(ball.x == 0){
      valTwo ++;
    } else if (ball.x == SCREEN_WIDTH - ball.w){
      valOne++;
      }
    }
  }
}
