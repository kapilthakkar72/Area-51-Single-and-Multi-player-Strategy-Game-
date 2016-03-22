#include "header.h"
#include <pthread.h>

void startSinglePlayer();

void MoveHero(int x, int y, Hero *h);

const int dx = 25;
const int dy = 25;

int current_image = 0;
int current_fire_1 = 5;
int current_fire_2 = 12;
int hero_tex = 0;

int h1 = -1 , t1 = -1;

int timerTick = 0;
int canProceed = 0;
void generateInitialItems()
{
    // for team 1

    for(int i=0;i<2;i++)
    {
        generateItemForTeam1(i);
        printf("Item for team 1 generated %d\n", i);
    }

    // for team 2

    for(int i=0;i<2;i++)
    {
        generateItemForTeam2(i);
        printf("Item for team 2 generated %d\n", i);
    }    
}

void setObstacle(int x, int y, int width, int height)
{
    for (int i = x; i < x + width; ++i)
    {
        for (int j = y; j < y + height; ++j)
        {
            graph_team2[i][j]=graph_team1[i][j] = 1;
        }
    }
}

void changeMouseCoords()
{
    /* First Comparing with screen borders */

    /* left edge */
    if (x_Mouse < dx) x_Mouse = dx;
    
    /* right edge */
    if ((x_Mouse + dx) > SCREEN_WIDTH) x_Mouse = SCREEN_WIDTH - dx;
    
    /* top edge */
    if (y_Mouse < dy) y_Mouse = dy;
    
    /* bottom edge */
    if ((y_Mouse + dy) > SCREEN_HEIGHT) y_Mouse = SCREEN_HEIGHT - dy;
}

int checkMoveblity(int x, int y)
{
    if(mySelf==NULL)
        return 0;
    if(mySelf->teamNumber == 1)
    {
        if (graph_team1[x][y] == 1) return 0;
        if (graph_team1[x - dx][y] == 1 || graph_team1[x + dx][y] == 1 || graph_team1[x][y + dy] == 1
                || graph_team1[x][y - dy] == 1) return 0;
        if (graph_team1[x - dx][y - dy] == 1 || graph_team1[x - dx][y + dy] == 1
                || graph_team1[x + dx][y - dy] == 1 || graph_team1[x + dx][y + dy] == 1)
            return 0;
    }
    else if(mySelf->teamNumber==2)
    {
        if (graph_team2[x][y] == 1) return 0;
        if (graph_team2[x - dx][y] == 1 || graph_team2[x + dx][y] == 1 || graph_team2[x][y + dy] == 1
                || graph_team2[x][y - dy] == 1) return 0;
        if (graph_team2[x - dx][y - dy] == 1 || graph_team2[x - dx][y + dy] == 1
                || graph_team2[x + dx][y - dy] == 1 || graph_team2[x + dx][y + dy] == 1)
            return 0;
    }
    
    return 1;
}

void setAllObstacles()
{
    //truck1
    setObstacle(645 + dx, 720 + dy, 50, 50);
    //truck2
    setObstacle(830 + dx, 800 + dy, 50, 50);
    //Tree at bottom
    setObstacle(395 + dx, 800 + dy, 120 - dx, 80 - dy);
    
    //Robot
    setObstacle(287 , 582 , 43, 28);
    
    //Robot 2
    setObstacle(245, 628, 45, 32); 

    //castle 1
    setObstacle(340+dx, 190+dy, 140 - 2* dx ,190-2*dy);

    //castle 2
    setObstacle(930+dx, 530+dy, 180 - 2* dx ,145-2*dy);

    // lane 1
    setObstacle(10,10,50,50);
    setObstacle(84,60,50,50);
    setObstacle(160,110,50,50);
    setObstacle(230,140,50,50);

    //lane 2
    setObstacle(560,325,50,50);
    setObstacle(610,350,50,50);
    setObstacle(665,375,50,50);
    setObstacle(775,440,50,50);
    setObstacle(800,445,50,50);
    setObstacle(825,460,50,50);
    setObstacle(825,460,50,50);
    setObstacle(685,400,80,80);

    // lane 3
    
    setObstacle(1160,720,50,50);
    setObstacle(1180,745,50,50);
    setObstacle(1205,760,50,50);
    setObstacle(1225,777,50,50);
    setObstacle(1240,790,50,50);
    setObstacle(1268,805,50,50);
    setObstacle(1300,825,50,50);

    // fire around castle 1
    setObstacle(345,109,50,50);
    setObstacle(525,145,50,50);
    setObstacle(569,287,50,50);

    //fire around castle 2
    setObstacle(800,530,50,50);
    setObstacle(875,705,50,50);
    setObstacle(1075,715,50,50);

    //fire 
    setObstacle(210,410,50,50);
    setObstacle(460,485,50,50);
    setObstacle(515,510,50,50);

    //fire 
    setObstacle(1125,430,50,50);
    setObstacle(840,325,50,50);
    setObstacle(895,355,50,50);

    // up
    setObstacle(462+dx,5+dy,50,50);
    setObstacle(670+dx,105+dy,50,50);
     //Robot
    setObstacle(1058 + dx, 275 + dy, 60, 65);
    
    //Robot 2
    setObstacle(1100 + dx, 225 + dy, 30, 35); 

    setObstacle(890 + dx, 5 + dy, 120 - dx, 60 - dy);
}

void loadAllHeroTextures()
{
    int k=0;
    for (int h_no = 1; h_no < 8; h_no++)
    {
        string image_path = heros_path +convertIntToString(h_no) + "/image_";
        for (int i = 1; i <= 8; i++)
        {
            string hero_img = image_path + convertIntToString(i) + ".png";
            printf("%s\n", hero_img.c_str());
            funLoadGLTextures(hero_img.c_str(),gameObj.Gametexture, k);
            k++;
        }
    }

    // Loading Images for Items
    string item=items_path+"attack_speed_item.png";
    int cnt=64;
    funLoadGLTextures(item.c_str(),gameObj.Gametexture, cnt++);
    item=items_path+"damage_item.png";
    funLoadGLTextures(item.c_str(),gameObj.Gametexture, cnt++);
    item=items_path+"health_item.png";
    funLoadGLTextures(item.c_str(),gameObj.Gametexture, cnt++);
    item=items_path+"speed_item.png";
    funLoadGLTextures(item.c_str(),gameObj.Gametexture, cnt++);
    item=items_path+"temple_healer.png";
    funLoadGLTextures(item.c_str(),gameObj.Gametexture, cnt++);
    /*string hide="hide.png";
    funLoadGLTextures(hide.c_str(),gameObj.Gametexture, 70); // 70*/
}

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 5.0, -5.0);
    
    //Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    LoadGLTextures();
    LoadGLTexturesWithPNGImages();
    glEnable(GL_TEXTURE_2D);            // Enable Texture Mapping
            
    // Make some OpenGL properties better for 2D and enable alpha channel.
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    
    //Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);
    
    //x_Coord = dx;
    //y_Coord = SCREEN_HEIGHT - dy;
    //Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("Error initializing OpenGL! %s\n", gluErrorString(error));
        return false;
    }
    
    setAllObstacles();
    loadAllHeroTextures();    

    return true;
}

void drawTexture(int xPosition, int yPosition, int width, int height, int tex[],
        int i)
{
    int dx = width / 2;
    int dy = height / 2;
    
    glBindTexture(GL_TEXTURE_2D, tex[i]);
    
    glBegin(GL_POLYGON);
    
    glTexCoord2d(0.0, 0.0);
    glVertex2d(xPosition - dx, yPosition - dy);
    
    glTexCoord2d(1.0, 0.0);
    glVertex2d(xPosition + dx, yPosition - dy);
    
    glTexCoord2d(1.0, 1.0);
    glVertex2d(xPosition + dx, yPosition + dy);
    
    glTexCoord2d(0.0, 1.0);
    glVertex2d(xPosition - dx, yPosition + dy);
    
    glEnd();
}

void drawItems()
{
    if(mySelf!=NULL)
    {
        if(mySelf->teamNumber == 1)
        {
            int itemNumberToBeDisplayed1 = gameObj.team1.currentItemsOnMap[0];
            int item1_x=gameObj.team1.itemCoordinates[0][0];
            int item1_y=gameObj.team1.itemCoordinates[0][1];

            int itemNumberToBeDisplayed2 = gameObj.team1.currentItemsOnMap[1];
            int item2_x=gameObj.team1.itemCoordinates[1][0];
            int item2_y=gameObj.team1.itemCoordinates[1][1];

            // display texture
            drawTexture(item1_x,item1_y,50, 50, gameObj.Gametexture, 64 + itemNumberToBeDisplayed1);
            drawTexture(item2_x,item2_y,50, 50, gameObj.Gametexture, 64 + itemNumberToBeDisplayed2);

        }
        else if(mySelf->teamNumber == 2)
        {
            int itemNumberToBeDisplayed1 = gameObj.team2.currentItemsOnMap[0];
            int item1_x=gameObj.team2.itemCoordinates[0][0];
            int item1_y=gameObj.team2.itemCoordinates[0][1];

            int itemNumberToBeDisplayed2 = gameObj.team2.currentItemsOnMap[1];
            int item2_x=gameObj.team2.itemCoordinates[1][0];
            int item2_y=gameObj.team2.itemCoordinates[1][1];

            // display texture
            drawTexture(item1_x,item1_y,50, 50, gameObj.Gametexture, 64 + itemNumberToBeDisplayed1);
            drawTexture(item2_x,item2_y,50, 50, gameObj.Gametexture, 64 + itemNumberToBeDisplayed2);
        }
    }
}

void drawHealth(int xPosition, int yPosition, int width, int height,int health)
{
    //printf("X position: %d width:%d health:%d Previous\n",xPosition,width,health );   
    drawTexture( xPosition,  yPosition,  width,  height,texture,27);

    health = (120*health)/100;
    xPosition = xPosition - width/2;
    xPosition = xPosition + health/2;
    width = health; 

    //printf("X position: %d width:%d health:%d\n",xPosition,width,health );   

    if(health>=80)
    {
        drawTexture( xPosition,  yPosition,  width,  height,texture,28);
    }
    else if(health<40)
    {
        drawTexture( xPosition,  yPosition,  width,  height,texture,29);
    }
    else
    {
        drawTexture( xPosition,  yPosition,  width,  height,texture,37);
    }
}

void displayStatistics()
{
    if(mySelf == NULL)
        return;
    if(mySelf->teamNumber==1)
    {
         // Display Name Of Game
        drawTexture(710, 50, 260, 80, texture, 36);
        // team name
        drawTexture(710, 138, 260, 76, texture, 38);

        // health of both team mates

        // team 1

        // ME
        drawTexture(950,75,120,50,texture,30);
        // my health
        drawHealth(1075,75,120,50,mySelf->health);
        //drawHealth(1075,75,120,50,100);


        //my teammate
        drawTexture(1200,75,120,50,texture,31);
        //health
        if(gameObj.team1.player1.ipAddress == myIPAddress)
            drawHealth(1325,75,120,50,gameObj.team1.player2.health);
            //drawHealth(1325,75,120,50,20);
        else
            drawHealth(1325,75,120,50,gameObj.team1.player1.health);
            //drawHealth(1325,75,120,50,20);

        // my castle
        drawTexture(950,165,120,50,texture,32);
        //health
        drawHealth(1075,165,120,50,gameObj.team1.castleHealth /10 );
        //drawHealth(1075,165,120,50,50 );

        // team 2

        // opponent1
        drawTexture(950,255,120,50,texture,33);
        //  health
        drawHealth(1075,255,120,50,gameObj.team2.player1.health);

        //opponent2
        drawTexture(1200,255,120,50,texture,34);
        //health
        drawHealth(1325,255,120,50,gameObj.team2.player2.health);

        // opponent castle
        drawTexture(950,345,120,50,texture,35);
        //health
        drawHealth(1075,345,120,50,gameObj.team2.castleHealth / 10);

        //Display Item if taken
        if(mySelf->attackSpeedTimer == ATTACK_SPEED_FAST)
        {
            drawTexture(1235,425,50,50,gameObj.Gametexture,64);
            drawHealth(1390,425,120,50,(100* (mySelf->attackSpeedTimer)/20));
        }
        if(mySelf->movementSpeedTimer==MOVEMENT_SPEED_FAST)
        {
            drawTexture(1235,475,50,50,gameObj.Gametexture,67);
            drawHealth(1330,475,120,50,(mySelf->movementSpeedTimer)*100/3000);
        }
    }    
    else if(mySelf->teamNumber==2)
    {
         // Display Name Of Game
        drawTexture(710, 800, 260, 80, texture, 36);
        // team name
        drawTexture(710, 720, 260, 76, texture, 39);

        // health of both team mates

        // team 1

        // ME
        drawTexture(50,525,120,50,texture,30);
        // my health
        drawHealth(175,525,120,50,mySelf->health);


        //my teammate
        drawTexture(300,525,120,50,texture,31);
        //health
        if(gameObj.team2.player1.ipAddress == myIPAddress)
            drawHealth(425,525,120,50,gameObj.team2.player2.health);
        else
            drawHealth(425,525,120,50,gameObj.team2.player1.health);

        // my castle
        drawTexture(50,615,120,50,texture,32);
        //health
        drawHealth(175,615,120,50,gameObj.team2.castleHealth/10);

        // team 2

        // opp
        drawTexture(50,705,120,50,texture,33);
        // opp health
        drawHealth(175,705,120,50,gameObj.team1.player1.health);

        //opp teammate
        drawTexture(300,705,120,50,texture,34);
        //opp health
        drawHealth(425,705,120,50,gameObj.team1.player2.health);

        // Opp castle
        drawTexture(50,795,120,50,texture,35);
        //opp health
        drawHealth(175,795,120,50,gameObj.team1.castleHealth/10);

        /*//Display Item if taken
        if(mySelf->attackSpeedTimer == ATTACK_SPEED_FAST)
        {
            drawTexture(1235,425,50,50,gameObj.Gametexture,64);
            drawHealth(1390,425,120,50,(100* (mySelf->attackSpeedTimer)/20);
        }
        if(mySelf->movementSpeedTimer==MOVEMENT_SPEED_FAST)
        {
            drawTexture(1235,475,50,50,gameObj.Gametexture,67);
            drawHealth(1330,475,120,50,(mySelf->movementSpeedTimer)*100/3000);
        }*/
    }
}

void drawCommonThings()
{


    //if(mySelf!=NULL)
        //printf("My Life : %d\n",mySelf->health );
    timerTick = (timerTick + 1) % 60;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();               // Reset The View
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // setting up background
            
    glBegin(GL_POLYGON);
    
    glTexCoord2d(0.0, 0.0);
    glVertex2d(SCREEN_WIDTH, 0);
    
    glTexCoord2d(0.0, 1.0);
    glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 1.0);
    glVertex2d(0, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 0.0);
    glVertex2d(0, 0);
    
    glEnd();

   

    drawTexture(1125+dx, 430+dy, 50, 50, texture, current_fire_2);
    drawTexture(840+dx, 325+dy, 50, 50, texture, current_fire_2);
    drawTexture(895+dx, 355+dy, 50, 50, texture, current_fire_2);


    drawTexture(345+dx, 109+dy, 50, 50, texture, current_fire_2);
    drawTexture(525+dx, 145+dy, 50, 50, texture, current_fire_2);
    drawTexture(569+dx, 287+dy, 50, 50, texture, current_fire_2);
    drawTexture(800+dx, 530+dy, 50, 50, texture, current_fire_2);
    drawTexture(875+dx, 705+dy, 50, 50, texture, current_fire_2);
    drawTexture(1075+dx, 715+dy, 50, 50, texture, current_fire_2);
    drawTexture(210+dx, 410+dy, 50, 50, texture, current_fire_2);
    drawTexture(460+dx, 485+dy, 50, 50, texture, current_fire_2);
    drawTexture(515+dx, 510+dy, 50, 50, texture, current_fire_2);  

    drawItems();

    if (timerTick % 10 == 0)
    {
        current_fire_1++;
        if (current_fire_1 == 11) current_fire_1 = 5;
    }
    
    /*if (timerTick % 10 == 0)
    {
        current_fire_2++;
        if (current_fire_2 == 19) current_fire_2 = 12;
    }*/

    if (timerTick % 10 == 0)
    {
        hero_tex=(hero_tex+1)%8;
    }
    if(gameObj.GameType==1)
    {
        if(mySelf->teamNumber==1)
        {
            drawTexture(gameObj.team1.player1.xPosition,gameObj.team1.player1.yPosition,50, 50, gameObj.Gametexture, (gameObj.team1.player1.heroID-1)*8+hero_tex);
            drawTexture(gameObj.team1.player2.xPosition,gameObj.team1.player2.yPosition,50, 50, gameObj.Gametexture, (gameObj.team1.player2.heroID-1)*8+hero_tex);
            drawTexture(gameObj.team2.player1.xPosition,gameObj.team2.player1.yPosition,50, 50, gameObj.Gametexture, (gameObj.team2.player1.heroID-1)*8+hero_tex);
            drawTexture(gameObj.team2.player2.xPosition,gameObj.team2.player2.yPosition,50, 50, gameObj.Gametexture, (gameObj.team2.player2.heroID-1)*8+hero_tex);
        }
    }
    else if(gameObj.GameType==2)
    {
        if(gameObj.team1.player1.ipAddress != myIPAddress && gameObj.team1.player1.ipAddress!="")
        {
           // printf("Texture display for Player1 of team1\n");
            drawTexture(gameObj.team1.player1.xPosition,gameObj.team1.player1.yPosition,50, 50, gameObj.Gametexture, (gameObj.team1.player1.heroID-1)*8+hero_tex);
        }
        else if(gameObj.team1.player1.ipAddress == myIPAddress)
        {
            //printf("Its Me: team 1 Player 1\n");
            mySelf = &gameObj.team1.player1;
        }

        if(gameObj.team1.player2.ipAddress != myIPAddress && gameObj.team1.player2.ipAddress!="" )
        {
            //printf("Texture display for Player2 of team1\n");
            drawTexture(gameObj.team1.player2.xPosition,gameObj.team1.player2.yPosition,50, 50, gameObj.Gametexture, (gameObj.team1.player2.heroID-1)*8+hero_tex);
        }
        else if(gameObj.team1.player2.ipAddress == myIPAddress)
        {
            //printf("Its Me: team 1 Player 2\n");
            mySelf = &gameObj.team1.player2;
        }

        if(gameObj.team2.player1.ipAddress != myIPAddress  && gameObj.team2.player1.ipAddress!="")
        {
            //printf("Texture display for Player1 of team2\n");
            drawTexture(gameObj.team2.player1.xPosition,gameObj.team2.player1.yPosition,50, 50, gameObj.Gametexture, (gameObj.team2.player1.heroID-1)*8+hero_tex);
        }
        else if(gameObj.team2.player1.ipAddress == myIPAddress)
        {
            //printf("Its Me: team 2 Player 1\n");
            mySelf = &gameObj.team2.player1;
        }

        if(gameObj.team2.player2.ipAddress != myIPAddress  && gameObj.team2.player2.ipAddress!="")
        {
            //printf("Texture display for Player1 of team2\n");
            drawTexture(gameObj.team2.player2.xPosition,gameObj.team2.player2.yPosition,50, 50, gameObj.Gametexture, (gameObj.team2.player2.heroID-1)*8+hero_tex);
        }
        else if(gameObj.team2.player2.ipAddress == myIPAddress)
        {
            //printf("Its Me: team 2 Player 2\n");
            mySelf = &gameObj.team2.player2;
        }

    }

    
    if(mySelf!=NULL)
    {
        if(mySelf->teamNumber ==1 )
        {
            glBindTexture(GL_TEXTURE_2D, texture[26]);
            
            glBegin(GL_POLYGON);
            
            glTexCoord2d(0.0, 0.0);
            glVertex2d(265, 0);
            
            glTexCoord2d(0.0, 1.0);
            glVertex2d(1400, 0);
            
            glTexCoord2d(1.0, 1.0);
            glVertex2d(1400, 670);
            
            /*glTexCoord2d(1.0, 0.0);
            glVertex2d(0, 0);*/
            
            glEnd();
            
        }
        else if(mySelf->teamNumber ==2 )
        {
            glBindTexture(GL_TEXTURE_2D, texture[26]);
            
            glBegin(GL_POLYGON);
            
            glTexCoord2d(0.0, 0.0);
            glVertex2d(0, 210);
            
            glTexCoord2d(0.0, 1.0);
            glVertex2d(0, 900);
            
            glTexCoord2d(1.0, 1.0);
            glVertex2d(1195, 900);
            
            /*glTexCoord2d(1.0, 0.0);
            glVertex2d(0, 0);*/
            
            glEnd();
           
        }
    }

    if(mySelf!=NULL)
    {
        displayStatistics();
    }


    // Display Attack
    while(display_attack)
    {
        drawTexture( attack_x,  attack_y,  100,  100,texture,40);
        display_attack--;
    }
    
    glutSwapBuffers();
}

void displayFirstScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();               // Reset The View
    
    glBindTexture(GL_TEXTURE_2D, texture[23]);   // setting up background
            
    glBegin(GL_POLYGON);
    
    glTexCoord2d(0.0, 0.0);
    glVertex2d(SCREEN_WIDTH, 0);
    
    glTexCoord2d(0.0, 1.0);
    glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 1.0);
    glVertex2d(0, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 0.0);
    glVertex2d(0, 0);
    
    glEnd();

    /*glBindTexture(GL_TEXTURE_2D, texture[30]);   
            
    glBegin(GL_POLYGON);
    
    glTexCoord2d(0.0, 0.0);
    glVertex2d(500, 0);
    
    glTexCoord2d(0.0, 1.0);
    glVertex2d(500, 500);
    
    glTexCoord2d(1.0, 1.0);
    glVertex2d(0, 500);
    
    glTexCoord2d(1.0, 0.0);
    glVertex2d(0, 0);
    
    glEnd();*/

    glutSwapBuffers();
    if ((x_Mouse >= 300 && x_Mouse <= 483)
            && (y_Mouse >= 185 && y_Mouse <= 210))
    {
        State = 1;
        gameObj.initGame(1);
        //printf("Single player Game %d\n",gameObj.team1.player1.xPosition);
        //y_Mouse=y_Coord;
        //x_Mouse=x_Coord;
    }
    else if ((x_Mouse >= 300 && x_Mouse <= 475)
            && (y_Mouse >= 277 && y_Mouse <= 307))
    {
        State = 1;
        gameObj.initGame(2);
        
        printf("Multiple player Game %d\n", gameObj.team1.player1.xPosition);
        
        printf(
                "Do you want to join as server (1) or you want to join existing game? (0)\n");
        scanf("%d", &Server);
        
        if (Server == 0)
        {
            printf("Please Enter IP Address of server to which you want to join...\n");
            cin >> server_ip_address;
            State = 1;
        }
        else if (Server == 1)
        {
            State = 1;
            
        }
        else
        {
            printf("Invalid option provided\n");
        }
        
        //y_Mouse=y_Coord;
        //x_Mouse=x_Coord;
    }
    else if ((x_Mouse >= 300 && x_Mouse <= 358)
            && (y_Mouse >= 370 && y_Mouse <= 385))
    {
        exit(0);
    }
}

void SelectHeroScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();               // Reset The View
    
    glBindTexture(GL_TEXTURE_2D, texture[24]);   // setting up background
            
    glBegin(GL_POLYGON);
    
    glTexCoord2d(0.0, 0.0);
    glVertex2d(SCREEN_WIDTH, 0);
    
    glTexCoord2d(0.0, 1.0);
    glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 1.0);
    glVertex2d(0, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 0.0);
    glVertex2d(0, 0);
    
    glEnd();
    glutSwapBuffers();
}

void FinalSettlement(int i)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    glLoadIdentity();               // Reset The View
       
	if(i==1)
	{
		glBindTexture(GL_TEXTURE_2D, texture[41]);   // setting up background    //Win
	}
	else if(i==2)
	{
		glBindTexture(GL_TEXTURE_2D, texture[42]);   // setting up background   //Lose 
	}
	glBegin(GL_POLYGON);
    
    glTexCoord2d(0.0, 0.0);
    glVertex2d(SCREEN_WIDTH, 0);
    
    glTexCoord2d(0.0, 1.0);
    glVertex2d(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 1.0);
    glVertex2d(0, SCREEN_HEIGHT);
    
    glTexCoord2d(1.0, 0.0);
    glVertex2d(0, 0);
    
    glEnd();
    glutSwapBuffers();
}

void connectToGameServer()
{
    int sockfd , portno , n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];
    portno = server_port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0)
    {
        printf("ERROR opening socket\n");
        exit(0);
    }
    
    string x = server_ip_address;
    
    char *temp = x.c_str();
    
    server = gethostbyname(temp);
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("ERROR connecting\n");
        exit(0);
    }
    printf("Connection establised to :: %s\n", temp);
    
    string msg = "JOIN-REQUEST;" + getMyIPAdress() + ";"
            + convertIntToString(t1) + ";" + convertIntToString(h1)+";";
    n = write(sockfd, msg.c_str(), 30);
    if (n < 0)
    {
        printf("ERROR writing to socket\n");
        exit(0);
    }
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        printf("ERROR reading from socket\n");
        exit(0);
    }
    printf("%s\n", buffer);
    close(sockfd);
    
    listenGameUpdatesThread();
}

void drawRectangle(int x, int y, int width, int height)
{
    //glDisable(GL_TEXTURE_2D);
    glColor3ub(0, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    glColor3ub(255, 255, 255);
}

void heroSelectAndTeam()
{
    //SVEN
    if ((x_Mouse >= 45 && x_Mouse <= 480) && (y_Mouse >= 150 && y_Mouse <= 270))
    {
        drawRectangle(45, 150, 430, 120);
        h1 = 1;
        myPowerID =0;
    }
    //BEAST MASTER
    else if ((x_Mouse >= 45 && x_Mouse <= 480)
            && (y_Mouse >= 285 && y_Mouse <= 405))
    {
        drawRectangle(45, 285, 430, 120);
        h1 = 2;
        myPowerID =1;
    }
    //BREW MASTER
    else if ((x_Mouse >= 45 && x_Mouse <= 480)
            && (y_Mouse >= 420 && y_Mouse <= 540))
    {
        drawRectangle(45, 420, 430, 120);
        h1 = 3;
        myPowerID =2;
    }
    //TITAN
    else if ((x_Mouse >= 45 && x_Mouse <= 480)
            && (y_Mouse >= 585 && y_Mouse <= 705))
    {
        drawRectangle(45, 585, 430, 120);
        h1 = 4;
        myPowerID =3;
    }
    //LYCAN
    else if ((x_Mouse >= 45 && x_Mouse <= 480)
            && (y_Mouse >= 725 && y_Mouse <= 825))
    {

        drawRectangle(45, 725, 430, 120);
        h1 = 5;
        myPowerID =0;
    }
    //WARLOCK
    else if ((x_Mouse >= 635 && x_Mouse <= 1065)
            && (y_Mouse >= 160 && y_Mouse <= 280))
    {
        drawRectangle(635, 160, 430, 120);
        h1 = 6;
        myPowerID =1;
    }
    //INVOKER
    else if ((x_Mouse >= 635 && x_Mouse <= 1065)
            && (y_Mouse >= 295 && y_Mouse <= 415))
    {
        drawRectangle(635, 295, 430, 120);
        h1 = 7;
        myPowerID =2;
    }
    //TEAM VIRUS
    else if ((x_Mouse >= 835 && x_Mouse <= 1153)
            && (y_Mouse >= 720 && y_Mouse <= 770))
    {
        drawRectangle(835, 720, 316, 50);
        if (h1 > 0)
        {
            t1 = 1;
            canProceed = 1;
        }
    }
    //TEAm ANTI VIRUS
    else if ((x_Mouse >= 835 && x_Mouse <= 1225)
            && (y_Mouse >= 778 && y_Mouse <= 828))
    {
        drawRectangle(835, 778, 388, 50);
        if (h1 > 0)
        {
            t1 = 2;
            canProceed = 1;
        }
    }
    else
    {
        t1 = -1;
        h1 = -1;
    }
}

void setGraphObstacleForSinglePlayer()
{
        // Setting up upper part as obstacle
        for(int j=0;j<630;j++)
        {
            int y=900-j;
            for(int i=1400; i>=((1085*900+ 630*315 -1085*y )/630) ;i--)
            {
                graph_team1[i][j]=1;
            }
        }

        // Setting up lower part as obstacle
        for(int j=210;j<=900;j++)
        {
            int y=900-j;
            for(int i=0; i<((690*1195 -1195*y )/690) ;i++)
            {
                graph_team2[i][j]=1;
            }
        }
}

void render()
{
    //printf("My Health : %d\n", mySelf->health);
    if (State == 0)
    {
        displayFirstScreen();
    }
    else if (State == 1)
    {
        SelectHeroScreen();
        heroSelectAndTeam();
        glutSwapBuffers();
        if (canProceed == 1)
        {
            printf("Inside Can proceed\n");
            if(gameObj.GameType==1)
            {
                 string mySetup = "JOIN-REQUEST;" + getMyIPAdress() + ";"
                            + convertIntToString(t1) + ";" + convertIntToString(h1)+";";
                 processClientIPandPort(mySetup);
                 if(t1==1)
                 {
                        int temph1=(h1+1)%8;
                        mySetup = "JOIN-REQUEST;" + getMyIPAdress() + ";"
                                + convertIntToString(1) + ";" + convertIntToString(temph1)+";";
                        processClientIPandPort(mySetup);

                        temph1=(temph1+1)%8;
                        mySetup = "JOIN-REQUEST;" + getMyIPAdress() + ";"
                                + convertIntToString(2) + ";" + convertIntToString(temph1)+";";
                        processClientIPandPort(mySetup);

                        temph1=(temph1+1)%8;
                        mySetup = "JOIN-REQUEST;" + getMyIPAdress() + ";"
                                    + convertIntToString(2) + ";" + convertIntToString(temph1)+";";
                        processClientIPandPort(mySetup);

                        mySelf=&gameObj.team1.player1;
                        mySelf->powerID=myPowerID;
                        setGraphObstacleForSinglePlayer();
                        startSinglePlayer();
                }
                State=2;
            }
            else if(gameObj.GameType==2)
            {
                if (Server == 0)
                {
                    printf("Exectuing.....\n");
                    connectToGameServer();
                    State = 2;
                }
                else if (Server == 1)
                {
                    string mySetup = "JOIN-REQUEST;" + getMyIPAdress() + ";"
                            + convertIntToString(t1) + ";" + convertIntToString(h1)+";";
                    //printf("mySetup sent:: %s\n",mySetup.c_str() );
                    processClientIPandPort(mySetup);
                    listenGameRequest();
                    generateInitialItems();
                    sendJoinMessageToAllThread();
                    State = 2;
                }
            }
        }
    }
    else if (State == 2)
    {
        if(gameObj.GameType==1)
        {
            if(gameObj.team1.castleHealth<=0 || gameObj.team1.player1.health<=0)
            {
                printf("YOU LOSE\n");
                //exit(0);
                
                GameStatus=2;
                State=3;
            }
            else if(gameObj.team2.castleHealth<=0)
            {
                printf("YOU WON\n");
                //exit(0);
                
                GameStatus=1;
                State=3;
            }
            drawCommonThings();
        }
        else if(gameObj.GameType==2)
        {
            if(x_Coord==-1 && y_Coord==-1)
                return;
            
            changeMouseCoords();
            
            if (!checkMoveblity(x_Mouse, y_Mouse))
            {
                //do nothing
            }
            else if (x_Coord != x_Mouse || y_Coord != y_Mouse)
            {
                stack<Point> result;
                if(mySelf->teamNumber==1)
                    result = a_star(graph_team1, x_Coord, y_Coord, x_Mouse, y_Mouse, 50, 50);
                else
                    result = a_star(graph_team2, x_Coord, y_Coord, x_Mouse, y_Mouse, 50, 50);
                
                /* iterate over path and go on drawing image on the given co-ordinate */

                while (!result.empty())
                {
                    drawCommonThings();
                    /*if(x_Coord==-1 && y_Coord==-1)
                        return;*/

                    if(mySelf == NULL)
                        return;
                    // aam aadami
                    
                    x_Coord = (result.top()).x;
                    y_Coord = (result.top()).y;
                    
                    if(mySelf -> movementSpeedTimer > 0)
                    {
                        for(int i=0;i< mySelf -> movementSpeed ;i++)
                            if(!result.empty())
                                result.pop();
                        mySelf -> movementSpeedTimer--;

                        if(mySelf -> movementSpeedTimer == 0)
                        {
                            mySelf->movementSpeed=MOVEMENT_SPEED_MEDIUM;
                        }
                    }
                    else
                    {
                        for(int i=0;i< mySelf -> movementSpeed ;i++)
                            if(!result.empty())
                                result.pop();                
                    }
                    //printf("Before Binding\n");
                    glBindTexture(GL_TEXTURE_2D,
                            gameObj.Gametexture[(mySelf-> heroID-1)*8+hero_tex]); // choose the texture to use.
                    //printf("After Binding\n");
                            
                    if (timerTick % 10 == 0)
                    {
                        current_image = (current_image + 1) % 8;
                    }
                    
                    glBegin(GL_POLYGON);
                    
                    glTexCoord2d(0.0, 0.0);
                    glVertex2d(x_Coord - dx, y_Coord - dy);
                    
                    glTexCoord2d(1.0, 0.0);
                    glVertex2d(x_Coord + dx, y_Coord - dy);
                    
                    glTexCoord2d(1.0, 1.0);
                    glVertex2d(x_Coord + dx, y_Coord + dy);
                    
                    glTexCoord2d(0.0, 1.0);
                    glVertex2d(x_Coord - dx, y_Coord + dy);
                    
                    glEnd();
                    
                    glutSwapBuffers();
                }
            }
            else
            {
                drawCommonThings();

                if(mySelf == NULL)
                        return;
                
                glBindTexture(GL_TEXTURE_2D,gameObj.Gametexture[(mySelf-> heroID-1)*8+hero_tex]); // choose the texture to use.
                        
                glBegin(GL_POLYGON);
                
                glTexCoord2d(0.0, 0.0);
                glVertex2d(x_Coord - dx, y_Coord - dy);
                
                glTexCoord2d(1.0, 0.0);
                glVertex2d(x_Coord + dx, y_Coord - dy);
                
                glTexCoord2d(1.0, 1.0);
                glVertex2d(x_Coord + dx, y_Coord + dy);
                
                glTexCoord2d(0.0, 1.0);
                glVertex2d(x_Coord - dx, y_Coord + dy);
                
                glEnd();
                
                glutSwapBuffers();
            }
            //State=3;
        }
    }
    else if(State==3)
    {
    	if(GameStatus==1)
    	{
    		FinalSettlement(1);
    	}
    	else if(GameStatus==2)
    	{
    		FinalSettlement(2);
    	}
    }
}

void update()
{
    
}

void checkAttackOnCastle(int x,int y,int team,int playerNo)
{
    int x_c;
    int y_c;
    if(team==1)
    {
        //check attack on castle 2

        if(playerNo==1)
        {
            x_c=gameObj.team1.player1.xPosition;
            y_c=gameObj.team1.player1.yPosition;
        }
        else if(playerNo==2)
        {
            x_c=gameObj.team1.player2.xPosition;
            y_c=gameObj.team1.player2.yPosition;
        }

        if((((x_c+230)>x && (x_c-230)<x) && ((y_c+170)>y && (y_c-170)<y)))
        {
           //possible attack on castle 2
            if( 930 < x && 1110 > x && 530 < y && 650 > y) //castle 2 : 930,530 and 1110,650
            {
                gameObj.team2.castleHealth--;
                printf("Health of castle 2 is decreasing\n");
            }
        }
    }
    else if(team==2)
    {
        //check attack on castle 1
        if(playerNo==1)
        {
            x_c=gameObj.team2.player1.xPosition;
            y_c=gameObj.team2.player1.yPosition;
        }
        else if(playerNo==2)
        {
            x_c=gameObj.team2.player2.xPosition;
            y_c=gameObj.team2.player2.yPosition;
        }

        if((((x_c+190)>x && (x_c-190)<x) && ((y_c+240)>y && (y_c-240)<y)))
        {
           //possible attack on castle 2
            if( 340 < x && 480 > x && 190 < y && 380 > y) //castle 1 : 340,190 and 480,380
            {
                gameObj.team1.castleHealth--;
                printf("Health of castle 1 is decreasing\n");
            }
        }
    }
}

void checkValidityofAttackSinglePlayer(int x, int y,int team,int playerNo)
{
    if(team==1 && playerNo==1)
    {
        if(!(((gameObj.team1.player1.xPosition+100)>x && (gameObj.team1.player1.xPosition-100)<x) && ((gameObj.team1.player1.yPosition+100)>y && (gameObj.team1.player1.yPosition-100)<y)))
        {
            //printf("Attack Invalid\n");
            return;
        }
        if((gameObj.team2.player1.xPosition-50) <= x && (gameObj.team2.player1.xPosition+50) >= x
            && (gameObj.team2.player1.yPosition-50) <= y && (gameObj.team2.player1.yPosition+50) >= y )
        {
            //valid attack 
            printf("Attacked on team 2 player 1\n");

            if(gameObj.team1.player1.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team2.player1.health=gameObj.team2.player1.health- gameObj.team1.player1.attackSpeed;
                gameObj.team1.player1.attackSpeedTimer--;

                if(gameObj.team1.player1.attackSpeedTimer == 0)
                {
                    gameObj.team1.player1.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team2.player1.health=gameObj.team2.player1.health- gameObj.team1.player1.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",gameObj.team1.player1.attackSpeed,gameObj.team1.player1.attackSpeedTimer );
        }
        else if((gameObj.team2.player2.xPosition-50) <= x && (gameObj.team2.player2.xPosition+50) >= x
            && (gameObj.team2.player2.yPosition-50) <= y && (gameObj.team2.player2.yPosition+50) >= y)
        {
            //valid attack 
            printf("Attacked on team 2 player 2\n");
           if(gameObj.team1.player1.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team2.player2.health=gameObj.team2.player2.health- gameObj.team1.player1.attackSpeed;
                gameObj.team1.player1.attackSpeedTimer--;

                if(gameObj.team1.player1.attackSpeedTimer == 0)
                {
                    gameObj.team1.player1.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team2.player2.health=gameObj.team2.player2.health- gameObj.team1.player1.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }

    }
    else if(team==1 && playerNo==2)
    {
        if(!(((gameObj.team1.player2.xPosition+100)>x && (gameObj.team1.player2.xPosition-100)<x) && ((gameObj.team1.player2.yPosition+100)>y && (gameObj.team1.player2.yPosition-100)<y)))
        {
            //printf("Attack Invalid\n");
            return;
        }
        if((gameObj.team2.player1.xPosition-50) <= x && (gameObj.team2.player1.xPosition+50) >= x
            && (gameObj.team2.player1.yPosition-50) <= y && (gameObj.team2.player1.yPosition+50) >= y )
        {
            //valid attack 
            printf("Attacked on team 2 player 1\n");

            if(gameObj.team1.player2.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team2.player1.health=gameObj.team2.player1.health- gameObj.team1.player2.attackSpeed;
                gameObj.team1.player2.attackSpeedTimer--;

                if(gameObj.team1.player2.attackSpeedTimer == 0)
                {
                    gameObj.team1.player2.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team2.player1.health=gameObj.team2.player1.health- gameObj.team1.player2.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",gameObj.team1.player1.attackSpeed,gameObj.team1.player1.attackSpeedTimer );
        }
        else if((gameObj.team2.player2.xPosition-50) <= x && (gameObj.team2.player2.xPosition+50) >= x
            && (gameObj.team2.player2.yPosition-50) <= y && (gameObj.team2.player2.yPosition+50) >= y)
        {
            //valid attack 
            printf("Attacked on team 2 player 2\n");
           if(gameObj.team1.player1.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team2.player2.health=gameObj.team2.player2.health- gameObj.team1.player2.attackSpeed;
                gameObj.team1.player2.attackSpeedTimer--;

                if(gameObj.team1.player2.attackSpeedTimer == 0)
                {
                    gameObj.team1.player2.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team2.player2.health=gameObj.team2.player2.health- gameObj.team1.player2.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }
    }
    else if(team==2 && playerNo==1)
    {

        if(!(((gameObj.team2.player1.xPosition+100)>x && (gameObj.team2.player1.xPosition-100)<x) && ((gameObj.team2.player1.yPosition+100)>y && (gameObj.team2.player1.yPosition-100)<y)))
        {
            //printf("Attack Invalid\n");
            return;
        }
        if((gameObj.team1.player1.xPosition-50) <= x && (gameObj.team1.player1.xPosition+50) >= x
            && (gameObj.team1.player1.yPosition-50) <= y && (gameObj.team1.player1.yPosition+50) >= y )
        {
            //valid attack 
            printf("Attacked on team 1 player 1\n");

            if(gameObj.team2.player1.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team1.player1.health=gameObj.team1.player1.health- gameObj.team2.player1.attackSpeed;
                gameObj.team2.player1.attackSpeedTimer--;

                if(gameObj.team2.player1.attackSpeedTimer == 0)
                {
                    gameObj.team2.player1.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team1.player1.health=gameObj.team1.player1.health- gameObj.team2.player1.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",gameObj.team1.player1.attackSpeed,gameObj.team1.player1.attackSpeedTimer );
        }
        else if((gameObj.team1.player2.xPosition-50) <= x && (gameObj.team1.player2.xPosition+50) >= x
            && (gameObj.team1.player2.yPosition-50) <= y && (gameObj.team1.player2.yPosition+50) >= y)
        {
            //valid attack 
            printf("Attacked on team 1 player 2\n");
           if(gameObj.team2.player1.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team1.player2.health=gameObj.team1.player2.health- gameObj.team2.player1.attackSpeed;
                gameObj.team2.player1.attackSpeedTimer--;

                if(gameObj.team2.player1.attackSpeedTimer == 0)
                {
                    gameObj.team2.player1.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team1.player2.health=gameObj.team1.player2.health- gameObj.team2.player1.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }
    }
    else if(team==2 && playerNo==2)
    {
        if(!(((gameObj.team2.player2.xPosition+100)>x && (gameObj.team2.player2.xPosition-100)<x) && ((gameObj.team2.player2.yPosition+100)>y && (gameObj.team2.player2.yPosition-100)<y)))
        {
            //printf("Attack Invalid\n");
            return;
        }
        if((gameObj.team1.player1.xPosition-50) <= x && (gameObj.team1.player1.xPosition+50) >= x
            && (gameObj.team1.player1.yPosition-50) <= y && (gameObj.team1.player1.yPosition+50) >= y )
        {
            //valid attack 
            printf("Attacked on team 1 player 1\n");

            if(gameObj.team2.player2.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team1.player1.health=gameObj.team1.player1.health- gameObj.team2.player2.attackSpeed;
                gameObj.team2.player2.attackSpeedTimer--;

                if(gameObj.team2.player2.attackSpeedTimer == 0)
                {
                    gameObj.team2.player2.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team1.player1.health=gameObj.team1.player1.health- gameObj.team2.player2.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",gameObj.team1.player1.attackSpeed,gameObj.team1.player1.attackSpeedTimer );
        }
        else if((gameObj.team1.player2.xPosition-50) <= x && (gameObj.team1.player2.xPosition+50) >= x
            && (gameObj.team1.player2.yPosition-50) <= y && (gameObj.team1.player2.yPosition+50) >= y)
        {
            //valid attack 
            printf("Attacked on team 1 player 2\n");
           if(gameObj.team2.player1.attackSpeedTimer >0)
            {
                //opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                gameObj.team1.player2.health=gameObj.team1.player2.health- gameObj.team2.player2.attackSpeed;
                gameObj.team2.player2.attackSpeedTimer--;

                if(gameObj.team2.player2.attackSpeedTimer == 0)
                {
                    gameObj.team2.player2.attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                gameObj.team1.player2.health=gameObj.team1.player2.health- gameObj.team2.player2.attackSpeed;
            }
            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }
    }
}

void checkValidityofAttack(int x,int y)
{
    //check where hero is with respect to attack....

    //First check attack on castle...
    if(mySelf->teamNumber == 1)
    {
        // attack on castle 2
        if((((x_Coord+230)>x && (x_Coord-230)<x) && ((y_Coord+170)>y && (y_Coord-170)<y)))
        {
           //possible attack on castle 2
            if( 930 < x && 1110 > x && 530 < y && 650 > y) //castle 2 : 930,530 and 1110,650
            {
                // check attack on castle
                if(mySelf -> attackSpeedTimer >0)
                {
                    attack_count_castle = attack_count_castle + mySelf ->attackSpeed;
                    mySelf->attackSpeedTimer--;

                    if(mySelf->attackSpeedTimer == 0)
                    {
                        mySelf ->attackSpeed=ATTACK_SPEED_MEDIUM;
                    }
                }
                else
                {
                    attack_count_castle = attack_count_castle + mySelf ->attackSpeed;
                }
                attack_x=x;
                attack_y=y;
                display_attack=ATTACK_DISPLAY_TIMER;
                printf("ATTACK ON CASTLE: attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );

            }
        }
    }
    else if(mySelf->teamNumber == 2)
    {
        // attack on castle 2
        if((((x_Coord+190)>x && (x_Coord-190)<x) && ((y_Coord+240)>y && (y_Coord-240)<y)))
        {
           //possible attack on castle 2
            if( 340 < x && 480 > x && 190 < y && 380 > y) //castle 1 : 340,190 and 480,380
            {
                // check attack on castle
                if(mySelf -> attackSpeedTimer >0)
                {
                    attack_count_castle = attack_count_castle + mySelf ->attackSpeed;
                    mySelf->attackSpeedTimer--;

                    if(mySelf->attackSpeedTimer == 0)
                    {
                        mySelf ->attackSpeed=ATTACK_SPEED_MEDIUM;
                    }
                }
                else
                {
                    attack_count_castle = attack_count_castle + mySelf ->attackSpeed;
                }
                attack_x=x;
                attack_y=y;
                display_attack=ATTACK_DISPLAY_TIMER;
                printf("ATTACK ON CASTLE: attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
            }
        }
    }

    if(!(((x_Coord+100)>x && (x_Coord-100)<x) && ((y_Coord+100)>y && (y_Coord-100)<y)))
    {
        printf("Attack Invalid\n");
        return;
    }

    if(mySelf->teamNumber == 1)
    {
        //check for attack on team number 2
        if((gameObj.team2.player1.xPosition-50) <= x && (gameObj.team2.player1.xPosition+50) >= x
            && (gameObj.team2.player1.yPosition-50) <= y && (gameObj.team2.player1.yPosition+50) >= y )
        {
            //valid attack 
            printf("Attacked on team 2 player 1\n");

            if(mySelf -> attackSpeedTimer >0)
            {
                opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                mySelf->attackSpeedTimer--;

                if(mySelf->attackSpeedTimer == 0)
                {
                    mySelf ->attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
            }
            attack_x=x;
            attack_y=y;
            display_attack=ATTACK_DISPLAY_TIMER;
            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }
        else if((gameObj.team2.player2.xPosition-50) <= x && (gameObj.team2.player2.xPosition+50) >= x
            && (gameObj.team2.player2.yPosition-50) <= y && (gameObj.team2.player2.yPosition+50) >= y)
        {
            //valid attack 
            printf("Attacked on team 2 player 2\n");
            if(mySelf -> attackSpeedTimer >0)
            {
                opponent_player_2_attacks = opponent_player_2_attacks + mySelf ->attackSpeed;
                mySelf->attackSpeedTimer--;

                if(mySelf->attackSpeedTimer == 0)
                {
                    mySelf ->attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                opponent_player_2_attacks = opponent_player_2_attacks + mySelf ->attackSpeed;
            }
            attack_x=x;
            attack_y=y;
            display_attack=ATTACK_DISPLAY_TIMER;
            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }
    }
    else if(mySelf->teamNumber == 2)
    {
        //check for attack on team number 2
        if((gameObj.team1.player1.xPosition-50) <= x && (gameObj.team1.player1.xPosition+50) >= x
            && (gameObj.team1.player1.yPosition-50) <= y && (gameObj.team1.player1.yPosition+50) >= y )
        {
            //valid attack 
            printf("Attacked on team 1 player 1\n");
            
            if(mySelf -> attackSpeedTimer >0)
            {
                opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
                mySelf->attackSpeedTimer--;

                if(mySelf->attackSpeedTimer == 0)
                {
                    mySelf ->attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                opponent_player_1_attacks = opponent_player_1_attacks + mySelf ->attackSpeed;
            }
            attack_x=x;
            attack_y=y;
            display_attack=ATTACK_DISPLAY_TIMER;

            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }
        else if((gameObj.team1.player2.xPosition-50) <= x && (gameObj.team1.player2.xPosition+50) >= x
            && (gameObj.team1.player2.yPosition-50) <= y && (gameObj.team1.player2.yPosition+50) >= y)
        {
            //valid attack 
            printf("Attacked on team 2 player 2\n");
            
            if(mySelf -> attackSpeedTimer >0)
            {
                opponent_player_2_attacks = opponent_player_2_attacks + mySelf ->attackSpeed;
                mySelf->attackSpeedTimer--;

                if(mySelf->attackSpeedTimer == 0)
                {
                    mySelf ->attackSpeed=ATTACK_SPEED_MEDIUM;
                }
            }
            else
            {
                opponent_player_2_attacks = opponent_player_2_attacks + mySelf ->attackSpeed;
            }
            attack_x=x;
            attack_y=y;
            display_attack=ATTACK_DISPLAY_TIMER;
            printf("attackSpeed : %d attackSpeedTimer %d\n",mySelf ->attackSpeed,mySelf ->attackSpeedTimer );
        }            
    }
}

int checkForItemUsed(int x,int y)
{
    if(!(((x_Coord+100)>x && (x_Coord-100)<x) && ((y_Coord+100)>y && (y_Coord-100)<y)))
    {
        printf("No Item Present here\n");
        return 0;
    }
    else if(mySelf->teamNumber == 1)
    {
        if(gameObj.team1.itemCoordinates[0][0] - 25 <=x && gameObj.team1.itemCoordinates[0][0] + 25 >=x 
        && gameObj.team1.itemCoordinates[0][1] - 25 <=y && gameObj.team1.itemCoordinates[0][1] + 25 >=y)
        {
            //Item consumed ----- first item of array
            item_used=gameObj.team1.currentItemsOnMap[0];
            item_taken=0;
            printf("Item %d at location 0 used by me\n",gameObj.team1.currentItemsOnMap[0]);
            return 1;
        }
        else if(gameObj.team1.itemCoordinates[1][0] - 25 <=x && gameObj.team1.itemCoordinates[1][0] + 25 >=x 
        && gameObj.team1.itemCoordinates[1][1] - 25 <=y && gameObj.team1.itemCoordinates[1][1] + 25 >=y)
        {
            //Item consumed ----- second item of array
            item_used=gameObj.team1.currentItemsOnMap[1];
            item_taken=1;
            printf("Item %d at location 1 used by me\n",gameObj.team1.currentItemsOnMap[1]);
            return 1;
        }
    }
    else if(mySelf->teamNumber == 2)
    {
        if(gameObj.team2.itemCoordinates[0][0] - 25 <=x && gameObj.team2.itemCoordinates[0][0] + 25 >=x 
        && gameObj.team2.itemCoordinates[0][1] - 25 <=y && gameObj.team2.itemCoordinates[0][1] + 25 >=y)
        {
            //Item consumed ----- first item of array
            item_used=gameObj.team2.currentItemsOnMap[0];
            item_taken=0;
            printf("Item %d at location 0 used by me\n",gameObj.team2.currentItemsOnMap[0]);
            return 1;
        }
        else if(gameObj.team2.itemCoordinates[1][0] - 25 <=x && gameObj.team2.itemCoordinates[1][0] + 25 >=x 
        && gameObj.team2.itemCoordinates[1][1] - 25 <=y && gameObj.team2.itemCoordinates[1][1] + 25 >=y)
        {
            //Item consumed ----- second item of array
            item_used=gameObj.team2.currentItemsOnMap[1];
            item_taken=1;
            printf("Item %d at location 1 used by me\n",gameObj.team2.currentItemsOnMap[1]);
            return 1;
        }
    }
    return 0;
}

void processItemLocallyIfPossible()
{
    // check for attack speed
    if(item_used==0)
    {
        mySelf -> attackSpeed = ATTACK_SPEED_FAST;
        mySelf -> attackSpeedTimer = 20;
    }
    else if(item_used==3)
    {
        mySelf -> movementSpeed = MOVEMENT_SPEED_FAST;
        mySelf -> movementSpeedTimer = 3000;
    }
}

void onMouseClick(int button, int state, int x, int y)
{
    cout << "X::" << x << "Y::" << y << endl;

    if(mySelf!=NULL)
    {
        if(mySelf -> stunMySelf || mySelf->dead)
            return;
    }
    
    if(button == GLUT_LEFT_BUTTON && state==GLUT_UP) 
    {
        x_Mouse = x;
        y_Mouse = y;
        if(gameObj.GameType==1 && State==2)
        {
            changeMouseCoords();
            
            if (checkMoveblity(x_Mouse, y_Mouse))
            {
                MoveHero(x,y,mySelf);
                printf("Moving to %d %d\n",x_Mouse,y_Mouse );
            }            
        }
    }
    else if(button == GLUT_RIGHT_BUTTON  && state==GLUT_UP)
    {
        if(gameObj.GameType==1)
        {
            checkAttackOnCastle(x,y,1,1);
            checkValidityofAttackSinglePlayer(x,y,t1,1);
        }
        else if(gameObj.GameType==2)
        {
            checkValidityofAttack(x,y);
            if(checkForItemUsed(x,y))
            {
                processItemLocallyIfPossible();
            }
        }        
    }
    else if(button == GLUT_MIDDLE_BUTTON && state==GLUT_UP)  
    {
        mySelf->powerCount --;
        if(mySelf->powerCount >=0)
        {
            iUsedMyMagicalPower=myPowerID;
            printf("I used magical power %d .... remaining ... %d\n",myPowerID,mySelf->powerCount);
        }
    }     
    render();
}
