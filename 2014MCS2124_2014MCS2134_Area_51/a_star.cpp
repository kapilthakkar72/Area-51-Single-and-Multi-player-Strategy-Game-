#include <vector>
#include <cmath>  // for abs
#include <algorithm>
#include <cstddef> 
using namespace std;

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900

struct parent_coords
{
	int x=-1;
	int y=-1;
} grid_of_parents[WINDOW_WIDTH][WINDOW_HEIGHT];

class Node
{

private:

	int xPosition;
	int yPosition;
	Node *parent;
	double g_m;
	double h_m;
	double f_m;

public:

	Node()
	{
		xPosition=0;
		yPosition=0;
		parent=NULL;
		g_m=f_m=h_m=0;
	}

	Node(int x,int y,double gm, double hm)
	{
		xPosition=x;
		yPosition=y;
		g_m = gm;
		h_m = hm;
		f_m = g_m + h_m;
	}

	Node(const Node &n)
	{
		xPosition=n.xPosition;
		yPosition=n.yPosition;
		parent=n.parent;
		g_m=n.g_m;
		h_m=n.h_m;
		f_m=n.f_m;
	}

	Node* getParent()
	{
		return parent;
	}

	void setParent(Node *n)
	{
		parent=n;
	}

	double get_f_m()
	{
		return f_m;
	}

	double get_g_m()
	{
		return g_m;
	}

	void set_g_m(int gm)
	{
		g_m=gm;
	}

	void update_f_m()
	{
		f_m = g_m + h_m;
	}

	void set_h_m(double hm)
	{
		h_m=hm;
	}

	int get_xPosition()
	{
		return xPosition;
	}

	int get_yPosition()
	{
		return yPosition;
	}

	double get_h_m()
	{
		return h_m;
	}
};

// Determine priority for heap creation
bool operator<(const Node &a, const Node &b)
{
	return a.get_f_m() > b.get_f_m();
}

// To calculate the hueristic
double calculate_h_m(int x_Current,int y_Current,int x_Final, int y_Final)
{
	double x=abs(x_Current-x_Final);
	double y=abs(y_Current-y_Final);
	return sqrt(x*x+y*y);
}

/* to remove element having minimum cost path from vector */
Node removeMinimumElement(vector<Node> &v)
{
	/* this will genetare minimum heap */
	make_heap(v.begin(),v.end());

	/* this will move first element to last position of vector and will re-create heap */
	std::pop_heap(v.begin(), v.end());

	/* access last node */
	Node n = v.back();
	Node temp=n;

	/* now remove the element from last position and return */
	v.pop_back();

	/* return temp */
	return temp; 
}

int presentInOpen(vector<Node> &v , Node temp)
{
	int i=0;
	for (std::vector<Node>::iterator it = v.begin() ; it != v.end(); ++it)
	{
		//Node current_element=v.at(i);
		Node current_element=*it;
		if(current_element.get_xPosition() ==temp.get_xPosition() && current_element.get_yPosition() == temp.get_yPosition())
		{
			/* element found in vector, update values if applicable */
			if(current_element.get_g_m() > temp.get_g_m())
			{
				current_element.set_g_m(temp.get_g_m());
				current_element.update_f_m();
				current_element.setParent(temp.getParent());
				Node *p=current_element.getParent();
				printf("If executed in closed...New Node :%d %d with parent %d %d \n",current_element.get_xPosition(),current_element.get_yPosition(),p->get_xPosition(),p->get_yPosition());
			}
			return 1;
		}
		i++;
	}
	return 0;
}

int presentInClosed(vector<Node> &open, vector<Node> &closed , Node temp)
{
	int i=0;
	for (std::vector<Node>::iterator it = closed.begin() ; it != closed.end(); ++it)
	{
		//Node current_element=closed.at(i);

		Node current_element=*it;
		
		if(current_element.get_xPosition() ==temp.get_xPosition() && current_element.get_yPosition() == temp.get_yPosition())
		{
			/* element found in vector, update values if applicable */
			if(current_element.get_g_m() > temp.get_g_m())
			{
				/* values need to be changed, so remove it from close and add in open */
				closed.erase(it);
				open.push_back(temp);	
				Node *p=temp.getParent();
				printf("If executed in closed...New Node :%d %d with parent %d %d \n",temp.get_xPosition(),temp.get_yPosition(),p->get_xPosition(),p->get_yPosition());
			}
			return 1;
		}
		i++;
	}
	return 0;
}

void putInOpenOrClosed(vector<Node> &open, vector<Node> &closed , Node temp)
{
	printf("Before putInOpenOrClosed : Me: %d %d My Parent: %d %d \n", temp.get_xPosition(),temp.get_yPosition(),temp.getParent()->get_xPosition(),temp.getParent()->get_yPosition());
	if(presentInOpen(open,temp))
	{
		// do nothing
	}
	else if(presentInClosed(open,closed,temp))
	{
		// do nothing
	}
	else
	{
		open.push_back(temp);
	}
	//setting up parents: 

	grid_of_parents[temp.get_xPosition()][temp.get_yPosition()].x=temp.getParent()->get_xPosition();
	grid_of_parents[temp.get_xPosition()][temp.get_yPosition()].y=temp.getParent()->get_yPosition();

	printf("After putInOpenOrClosed : Me: %d %d My Parent: %d %d \n", temp.get_xPosition(),temp.get_yPosition(),temp.getParent()->get_xPosition(),temp.getParent()->get_yPosition());
}

int canMoveUp(int graph[WINDOW_WIDTH][WINDOW_HEIGHT], Node n ,int dx,int dy)
{
	int i=(graph[n.get_xPosition()][n.get_yPosition()-dy-1]!= 1 && graph[n.get_xPosition() + dx][n.get_yPosition()-dy-1]!= 1 && graph[n.get_xPosition()-dx][n.get_yPosition() -dy -1]!= 1);
	return i;
}

int canMoveDown(int graph[WINDOW_WIDTH][WINDOW_HEIGHT], Node n ,int dx,int dy)
{
	return(graph[n.get_xPosition()][n.get_yPosition()+dy+1]!= 1 && graph[n.get_xPosition() + dx][n.get_yPosition()+dy+1]!= 1 && graph[n.get_xPosition()-dx][n.get_yPosition() +dy +1]!= 1);
}

int canMoveRight(int graph[WINDOW_WIDTH][WINDOW_HEIGHT], Node n ,int dx,int dy)
{
	return (graph[n.get_xPosition() + dx +1][n.get_yPosition()]!= 1 && graph[n.get_xPosition() + dx +1][n.get_yPosition()+dy]!= 1 && graph[n.get_xPosition() + dx +1][n.get_yPosition()-dy]!= 1);
}

int canMoveLeft(int graph[WINDOW_WIDTH][WINDOW_HEIGHT], Node n ,int dx,int dy)
{
	return (graph[n.get_xPosition() + dx -1][n.get_yPosition()]!= 1 && graph[n.get_xPosition() + dx -1][n.get_yPosition()+dy]!= 1 && graph[n.get_xPosition() + dx -1][n.get_yPosition()-dy]!= 1);
}

vector<Node *> a_star(int graph[WINDOW_WIDTH][WINDOW_HEIGHT], int xStart, int yStart, int xFinish, int yFinish,int width, int height)
{
	// Generating obstacle :

	for (int i=150;i<=230;i++)
		for(int j=300;j<=350;j++)
			graph[j][i]=1;

	/* This vector will store the path to reach final position */
	std::vector<Node *> path;

	if(xStart==xFinish && yStart==yFinish)
	{
		return path;
	}
	else
	{
		/* change xFinish and yFinish to fit into the screen */
		int dx = width/2;
		int dy = height/2;

		/* First Comparing with screen borders */

		/* left edge */
		if(xFinish < dx)
			xFinish=dx;

		/* right edge */
		if((xFinish + dx) > WINDOW_WIDTH)
			xFinish=WINDOW_WIDTH-dx;

		/* top edge */
		if(yFinish < dy)
			yFinish=dy;

		/* bottom edge */
		if((yFinish+dy) > WINDOW_HEIGHT)
			yFinish=WINDOW_HEIGHT-dy;


		/* Check for obstacle */

		/* right edge overlapping with obstcle */
		/*if(graph[xFinish+dx][yFinish+dy]==1)
		{
			int i=1;
			while(graph[xFinish+dx-i][yFinish+dy-i]==1)
				i++;
			xFinish=xFinish-i;
		}*/

		/* left edge overlapping with obstcle */
		/*if(graph[xFinish-dx][yFinish]==1)
		{
			int i=1;
			while(graph[xFinish+dx+i][yFinish]==1)
				i++;
			xFinish=xFinish-i;
		}*/


		double temp_h_m;
		//Node *n;

		/* creation of open and closed data structure */
		std::vector<Node> open;
		std::vector<Node> closed;

		/* create start node */
		temp_h_m=calculate_h_m(xStart,yStart,xFinish,yFinish);
		Node s(xStart,yStart,0,temp_h_m);

		/* put start node to open */
		open.push_back(s);
		Node n;

		/* if open is empty terminate and fail */
		while(!open.empty())
		{
			n=removeMinimumElement(open);

			/* reacched to final Node */
			if(n.get_xPosition() == xFinish && n.get_yPosition() == yFinish)
			{
				Node *temp=n.getParent();
				break;
			}

			/* put n in closed */
			Node node_n=n;
			closed.push_back(n);

			/* for each successor m of n do calculation (assuming 8 direction) */

			/* moving to right */
			if(canMoveRight(graph, n ,dx,dy))
			{
				/* create node */

				double gm= n.get_g_m()+1;
				double hm= calculate_h_m(n.get_xPosition() +1,n.get_yPosition() , xFinish, yFinish);
				Node temp(n.get_xPosition() +1,n.get_yPosition(),gm,hm);				
				temp.setParent(&n);

				/* is it present in open or close? If present then update its values to minimum and return true*/
				putInOpenOrClosed(open,closed,temp);
			}
		

			/* moving to left */
			if(canMoveLeft(graph, n ,dx,dy))
			{
				/* create node */
				double gm= n.get_g_m()+1;
				double hm= calculate_h_m(n.get_xPosition() -1,n.get_yPosition() , xFinish, yFinish);
				Node temp(n.get_xPosition() -1,n.get_yPosition(),gm,hm);
				temp.setParent(&n);

				/* is it present in open or close? If present then update its values to minimum and return true*/
				putInOpenOrClosed(open,closed,temp);
			}

			/* moving up */
			if(canMoveUp(graph, n ,dx,dy))
			{
				/* create node */
				double gm= n.get_g_m()+1;
				double hm= calculate_h_m(n.get_xPosition(),n.get_yPosition()-1 , xFinish, yFinish);
				Node temp(n.get_xPosition(),n.get_yPosition()-1,gm,hm);
				temp.setParent(&n);

				/* is it present in open or close? If present then update its values to minimum and return true*/
				putInOpenOrClosed(open,closed,temp);
			}
			

			/* moving down */
			if(canMoveDown(graph, n ,dx,dy))
			{
				/* create node */
				double gm= n.get_g_m()+1;
				double hm= calculate_h_m(n.get_xPosition(),n.get_yPosition()+1 , xFinish, yFinish);
				Node temp(n.get_xPosition(),n.get_yPosition()+1,gm,hm);
				temp.setParent(&n);

				/* is it present in open or close? If present then update its values to minimum and return true*/
				putInOpenOrClosed(open,closed,temp);	
			}
			
		}

		// Printing it...
		int x=n.get_xPosition();
		int y=n.get_yPosition();

		for (int i=200;i<250;i++)
		{
			for(int j=200;j<250;j++)
			{
				printf("Me : %d %d My Parent: %d %d\n",i,j, grid_of_parents[i][j].x,grid_of_parents[i][j].y);
			}
		}
			
		/*
		while(grid_of_parents[x][y].x != -1)
		{
			printf("Destination : %d %d\n",xFinish,yFinish );
			printf("Me : %d %d My Parent: %d %d\n",x,y, grid_of_parents[x][y].x,grid_of_parents[x][y].y);
			x=grid_of_parents[x][y].x;
			y=grid_of_parents[x][y].y;
		}
	*/
		while(1);

		/* while loop breaked or ended */
		if(n.get_f_m() == 0)
		{
			/* No Path */
			return path;
		}
		else
		{
			/* Generate Path */
			/* n is the last node */
			Node temp=n;
			/*printf("Temp : %d %d\n",temp->get_xPosition(),temp->get_yPosition());
			temp = temp->getParent();
			printf("New temp : %d %d\n",temp->get_xPosition(),temp->get_yPosition());*/
			
			//while(1);
			vector<Node *>::iterator it;
			//do
			//{
			/*	it=path.begin();
				path.insert(it,temp);
				printf("Temp : %d %d\n",temp->get_xPosition(),temp->get_yPosition());
				temp = temp->getParent();
				printf("New temp : %d %d\n",temp->get_xPosition(),temp->get_yPosition());*/
			//}while(temp!=NULL);
		}

		return path;
	}
} 