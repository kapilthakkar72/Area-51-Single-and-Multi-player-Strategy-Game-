int isInSpawn1(int x,int y)
{
    y=SCREEN_HEIGHT - y;

    if(225*x + 265*y - 225*265 < 0)
        return 1;
    return 0;
}

int isInSpawn2(int x,int y)
{
    y=SCREEN_HEIGHT - y;

    if(230 *x +275 *y -230*1125 -275*900 > 0)
        return 1;
    return 0;
}

int checkCoordsForItemForTeam1(int rand_x,int rand_y)
{
    if(rand_x < 25 || rand_y > 875 || isInSpawn1(rand_x,rand_y))
        return 0;

    if(graph_team1[rand_x][rand_y]!=1 && graph_team1[rand_x-25][rand_y-25]!=1 &&
       graph_team1[rand_x-25][rand_y+25]!=1 && graph_team1[rand_x+25][rand_y+25]!=1 && graph_team1[rand_x+25][rand_y-25]!=1)
    {
        return 1;
    }
    return 0;
}

int checkCoordsForItemForTeam2(int rand_x,int rand_y)
{
    if(rand_x > 1375 || rand_y < 25 || isInSpawn2(rand_x,rand_y))
        return 0;

    if(graph_team2[rand_x][rand_y]!=1 && graph_team2[rand_x-25][rand_y-25]!=1 &&
       graph_team2[rand_x-25][rand_y+25]!=1 && graph_team2[rand_x+25][rand_y+25]!=1 && graph_team2[rand_x+25][rand_y-25]!=1)
    {
            return 1;
    }
    return 0;
}

void generateItemForTeam1(int i)
{
    printf("TEAM 1 :: Changing Item of location -> %d\n",i);
    int rand_x,rand_y;
    int seed=0;

    do
    {
        struct timeval t;
        t.tv_sec = t.tv_usec = 0;
        gettimeofday(&t, NULL);

        srand (t.tv_usec);

        rand_x = rand() % (1195-50) +26;

        int y_from_equation = ((690*1195) - (690*rand_x))/1195;

        rand_y = rand() % y_from_equation ;

        rand_y = (900 - y_from_equation) + rand_y ;

        printf("Rand:: %d %d\n",rand_x,rand_y );
    }while(!checkCoordsForItemForTeam1(rand_x,rand_y));
   
    gameObj.team1.itemCoordinates[i][0]=rand_x;
    gameObj.team1.itemCoordinates[i][1]=rand_y;
   
    gameObj.team1.currentItemsOnMap[i]=gameObj.team1.nextItem;
    //printf("generated Item Number for Team 1 %d \n", gameObj.team1.nextItem);
    gameObj.team1.nextItem = (gameObj.team1.nextItem+1)%5;
    //printf("Next Item which will be generated %d \n", gameObj.team1.nextItem);
}

void generateItemForTeam2(int i)
{
    printf("TEAM 2 :: Changing Item of location -> %d\n",i);
    int rand_x,rand_y;
    int seed=10;

    do
    {
        struct timeval t;
        t.tv_sec = t.tv_usec = 0;
        gettimeofday(&t, NULL);

        srand (t.tv_usec);
        rand_x = rand() % (1400 - 315 - 50) +26 +315;       

        int y_from_equation = ( ((-630) * rand_x )+(630*315) +(1085*900)) /1085;

        rand_y = rand() % (900-y_from_equation) ;
        rand_y = 900 - (y_from_equation + rand_y) ;

        printf("Rand:: %d %d\n",rand_x,rand_y );
    }while(!checkCoordsForItemForTeam2(rand_x,rand_y));
   
    gameObj.team2.itemCoordinates[i][0]=rand_x;
    gameObj.team2.itemCoordinates[i][1]=rand_y;
   
    gameObj.team2.currentItemsOnMap[i]=gameObj.team2.nextItem;
    //printf("generated Item Number for Team 2 %d \n", gameObj.team2.nextItem);
    gameObj.team2.nextItem = (gameObj.team2.nextItem+1)%5;
    //printf("Next Item which will be generated %d \n", gameObj.team2.nextItem);
}
