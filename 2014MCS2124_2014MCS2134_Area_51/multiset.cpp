#ifndef _Header_H
#define _Header_H
#include "header.h"
#endif


using namespace std;

struct Node
{
public:

	int x;
	int y;
	double fm;
	double gm;
	double hm;
	int parent_x;
	int parent_y;
};

struct Point
{
	int x;
	int y;
};

class CompareNodes 
{
public:
	bool operator()(const Node *comp1, const Node *comp2) const 
	{
		if((comp1->fm) == (comp2->fm))
       		return comp1->hm < comp2->hm;
       	else
       		return ((comp1->fm) < (comp2->fm));

    }
};

typedef multiset<Node *, CompareNodes> mset;

// To calculate the hueristic
double calculate_h_m(int x_Current,int y_Current,int x_Final, int y_Final)
{
	double x=abs(x_Current-x_Final);
	double y=abs(y_Current-y_Final);
	return sqrt(x*x+y*y)*1.5;
	//return (x+y);
	/*if(x>y)
     	return (14*y+ 10*(x-y))*(1.5);
	else
     	return (14*x+ 10*(y-x))*(1.5);*/
}

void initialiseNode(Node *n,int x,int y, int gm,int parent_x,int parent_y,int xFinish,int yFinish)
{
	n->x=x;
	n->y=y;
	n->hm= calculate_h_m(x,y,xFinish,yFinish);
	n->gm=gm;
	n->fm=n->hm+n->gm;
	n->parent_x=parent_x;
	n->parent_y=parent_y; 
}

Node* pop(mset &m,int xFinish,int yFinish)
{
	Node *first = (Node *)malloc(sizeof(struct Node));;
	if(!m.empty())
	{
		mset::iterator p=m.begin();
		initialiseNode(first,(*p)->x,(*p)->y,(*p)->gm,(*p)->parent_x,(*p)->parent_y,xFinish,yFinish);
		m.erase(p);
		return first;
	}
	return NULL;
}

int findInClosed(Node *n, mset &open, mset &closed)
{
	for(mset::iterator p=closed.begin(); p!=closed.end() ; p++)
	{
		if((*p)->x==n->x && (*p)->y==n->y )
		{
			if((*p)->fm > n->fm)
			{
				closed.erase(p);
				open.insert(n);
			}
			return 1;
		}
		
	}
	return 0;
}

Node* searchInClosed(int x,int y, mset &closed)
{
	for(mset::iterator p=closed.begin(); p!=closed.end() ; p++)
	{
		if((*p)->x==x && (*p)->y==y )
		{
			return (*p);
		}
	}
	return NULL;
}

int findInOpen(Node *n, mset &open)
{
	for(mset::iterator p=open.begin(); p!=open.end() ; p++)
	{
		if((*p)->x==n->x && (*p)->y==n->y )
		{
			if((*p)->fm > n->fm)
			{
				(*p)->fm = n->fm;
				(*p)->gm = n->gm;	
			}
			return 1;
		}
		
	}
	return 0;
}

void doTheNecessary(Node *n, mset &open, mset &closed)
{
	if(!(findInOpen(n,open) || findInClosed(n,open,closed)))
	{
		open.insert(n);
	}
}



int canMoveUp(int graph[SCREEN_WIDTH+1][SCREEN_HEIGHT+1], Node *n ,int dx,int dy)
{
	int x=n->x;
	int y=n->y;

	if(y-1-dy < 0 || (x+dx)>=SCREEN_WIDTH || (x-dx)<0)
	{
		return 0;
	}	
	if(graph[x][y-1-dy]==1)
	{
		return 0;
	}
	if (graph[x+dx][y-1-dy]==1)
	{
		return 0;
	}
	if(graph[x-dx][y-1-dy]==1)
	{
		return 0;
	}
	return 1;
}

int canMoveDown(int graph[SCREEN_WIDTH+1][SCREEN_HEIGHT+1], Node *n ,int dx,int dy)
{
	int x=n->x;
	int y=n->y;

	if(y+1+dy > SCREEN_HEIGHT || (x+dx)>SCREEN_WIDTH || (x-dx)<0 )
	{
		return 0;
	}	
	if(graph[x][y+1+dy]==1)
	{
		return 0;
	}
	else if (graph[x+dx][y+1+dy]==1)
	{
		return 0;
	}
	else if(graph[x-dx][y+1+dy]==1)
	{
		return 0;
	}
	return 1;
}

int canMoveRight(int graph[SCREEN_WIDTH+1][SCREEN_HEIGHT+1], Node *n ,int dx,int dy)
{


	int x=n->x;
	int y=n->y;

	if(x+1+dx > SCREEN_WIDTH || (y+dy)>SCREEN_HEIGHT || (y-dy)<0)
	{
		return 0;
	}	
	if(graph[x+1+dx][y]==1)
	{
		return 0;
	}
	else if (graph[x+1+dx][y-dy]==1)
	{
		return 0;
	}
	else if(graph[x+1+dx][y+dy]==1)
	{
		return 0;
	}
	return 1;
}

int canMoveLeft(int graph[SCREEN_WIDTH+1][SCREEN_HEIGHT+1], Node *n ,int dx,int dy)
{
	int x=n->x;
	int y=n->y;

	if(x-1-dx <0 || (y+dy)>SCREEN_HEIGHT || (y-dy)<0)
	{
		return 0;
	}	
	if(graph[x-1-dx][y]==1)
	{
		return 0;
	}
	else if (graph[x-1-dx][y-dy]==1)
	{
		return 0;
	}
	else if(graph[x-1-dx][y+dy]==1)
	{
		return 0;
	}
	return 1;
}

stack<Point> a_star(int graph[SCREEN_WIDTH+1][SCREEN_HEIGHT+1], int xStart, int yStart,int xFinish,int yFinish,int width, int height)
{
	stack<Point> result;
	int dx=width/2;
	int dy=height/2;

	//checkBoundary(dx,dy);

	/* This vector will store the path to reach final position */
	//vector<Node *> path;

	mset open,closed;

	// create start node and insert in open
	Node *s = (Node *)malloc(sizeof(struct Node));
	initialiseNode(s,xStart,yStart,0,-1,-1,xFinish,yFinish);
	open.insert(s);

	Node *minimum;
	int i=0;

	while(!open.empty())
	{
		// remove minimum element from open
		minimum=pop(open,xFinish,yFinish);
		closed.insert(minimum);
	
		if(minimum->x == xFinish && minimum->y == yFinish)
		{
			break;
		}

		//cout<<"destination x:"<<xFinish<<" y:"<<yFinish<<endl;
		//cout<<"minimum x:"<<minimum->x<<" y:"<<minimum->y<<endl;

		// Right
		if(canMoveRight(graph,minimum,dx,dy))
		{
			Node *right=(Node *)malloc(sizeof(struct Node));
			initialiseNode(right,minimum->x+1,minimum->y,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(right,open,closed);
		}
		// Left
		if(canMoveLeft(graph,minimum,dx,dy))
		{
			Node *left=(Node *)malloc(sizeof(struct Node));
			initialiseNode(left,minimum->x-1,minimum->y,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(left,open,closed);
		}
		// Up
		if(canMoveUp(graph,minimum,dx,dy))
		{
			Node *up=(Node *)malloc(sizeof(struct Node));	
			initialiseNode(up,minimum->x,minimum->y-1,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(up,open,closed);
		}
		// Down
		if(canMoveDown(graph,minimum,dx,dy))
		{
			Node *down=(Node *)malloc(sizeof(struct Node));
			initialiseNode(down,minimum->x,minimum->y+1,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(down,open,closed);
		}
		// right +up
		if(canMoveRight(graph,minimum,dx,dy) && canMoveUp(graph,minimum,dx,dy))
		{
			Node *rightUp=(Node *)malloc(sizeof(struct Node));	
			initialiseNode(rightUp,minimum->x+1,minimum->y-1,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(rightUp,open,closed);
		}
		//left - up
		if(canMoveLeft(graph,minimum,dx,dy) && canMoveUp(graph,minimum,dx,dy))
		{
			Node *leftUp=(Node *)malloc(sizeof(struct Node));
			initialiseNode(leftUp,minimum->x-1,minimum->y-1,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(leftUp,open,closed);
		}

		//left - bottom
		if(canMoveLeft(graph,minimum,dx,dy) && canMoveDown(graph,minimum,dx,dy))
		{
			Node *leftBottom=(Node *)malloc(sizeof(struct Node));
			initialiseNode(leftBottom,minimum->x-1,minimum->y+1,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(leftBottom,open,closed);
		}

		//right - bottom
		if(canMoveRight(graph,minimum,dx,dy) && canMoveDown(graph,minimum,dx,dy))
		{
			Node *rightBottom=(Node *)malloc(sizeof(struct Node));
			initialiseNode(rightBottom,minimum->x+1,minimum->y+1,minimum->gm+1,minimum->x,minimum->y,xFinish,yFinish);
			doTheNecessary(rightBottom,open,closed);
		}
	}

	Node *temp=minimum;

	do
	{
		Point p;
		p.x = temp->x;
		p.y = temp->y;
		result.push(p);
		temp=searchInClosed(temp->parent_x,temp->parent_y,closed);
	}while(temp!=NULL);
	return result;
}