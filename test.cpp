#include"test.hh"
using namespace std;


struct point
{
   double x;
   double y;
};

struct arc;

struct event {
   double x;
   point p;
   arc *a;
   bool valid;

   event(double xx, point pp, arc *aa)
   {
      x=xx;
      p=pp; 
      a=aa; 
      valid=true;
   }
};

struct seg;
Vector<seg*> output;  // Array of output segments.

struct seg 
{
   point start, end;
   bool done;

   seg(point p)
      { 
         start=p;
         end.x=0;
         end.y=0;
         done=false;
      
      output.push(this); 
      }

   // Set the end point and mark as "done."
   void finish(point p) { if (done) return; end = p; done = true; }
};

bool gtP(point a, point b) 
{return a.x==b.x ? a.y>b.y : a.x>b.x;}

//priority queue for point.
struct Node
{
    point data;
 
    Node* next;
 
};
 
// Function to create a new node
Node* newNode(point d)
{
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = d;
    temp->next = NULL;
 
    return temp;
}
 
// Return the value at head
point peek(Node** head)
{
    return (*head)->data;
}
 
// Removes the element with the
// highest priority form the list
void pop(Node** head)
{
    Node* temp = *head;
    (*head) = temp->next;
    free(temp);
}

Node* parent(Node** head,Node* a)
{
   Node* temp = (*head);
   while(temp->next != a)
   {
      temp=temp->next;
   }
   return temp;
}
 
// Function to push according to priority
void push(Node** head, point p)
{
    Node* start = (*head);
 
    // Create new Node
    Node* temp = newNode(p);
 
    // Special Case: The head of list has
    // lesser priority than new node. So
    // insert newnode before head node
    // and change head node.
    while(start != nullptr)
    {
    if (start->data.x > p.x)
    {
        start=start->next;
    }
    else if(start->data.x == p.x)
    {
       if(start->data.y > p.y)
       {
         // temp->next = *head;
         // (*head) = temp;
         start = start->next;
       }
       else if(start->data.y < p.y)
       {
          Node* temp1=parent(head,start);
          temp1->next = temp;
          temp->next = start;
       }
    }
    else if(start->data.x < p.x)
    {
       temp->next = start;
       (*head)->next=temp;
    }
    }
}

Node* end(Node** head)
{
    Node* temp = *head;

    while(temp->next != NULL)
    {
        temp= temp->next;
    }
    return temp;
}

Node* begin(Node** head)
{
    return *head;
}
 
// Function to check is list is empty
bool isEmpty(Node** head)
{
    return (*head) == NULL;
}
//end of priority queue for point



Node* points;


//start of priority queue for event*
struct NodeE
{
    event* data;
    int priority;
 
    NodeE* next;
 
};
 
// Function to create a new node
NodeE* newNodeE(event* d, int p)
{
    NodeE* temp = (NodeE*)malloc(sizeof(NodeE));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;
 
    return temp;
}
 
// Return the value at head
event* peekE(NodeE** head)
{
    return (*head)->data;
}
 
// Removes the element with the
// highest priority form the list
void popE(NodeE** head)
{
    NodeE* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}
 
// Function to push according to priority
void pushE(NodeE** head, event* d, int p)
{
    NodeE* start = (*head);

    NodeE* temp = newNodeE(d, p);
 
    if ((*head)->priority > p)
    {

        temp->next = *head;
        (*head) = temp;
    }
    else
    {
        while (start->next != NULL &&
            start->next->priority < p)
        {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }
}

NodeE* endE(NodeE** head)
{
    NodeE* temp = *head;

    while(temp->next != NULL)
    {
        temp= temp->next;
    }
    return temp;
}

NodeE* beginE(NodeE** head)
{
    return *head;
}
 
// Function to check is list is empty
bool isEmptyE(NodeE** head)
{
    return (*head) == NULL;
}
//end of priority queue for events*.

NodeE* events;




bool gtE(event *a, event *b) 
{
    if(a->x > b->x)
    return true;
    
    return false;
}

struct arc
{
    point f;//focus of the arc
    event* e;
    arc* prev;
    arc* next;//arcs which are to the left and right of the arc.
    seg *s1,*s2;

    arc(point pp, arc *a=0, arc *b=0)
    {
      f=pp;
      prev=a;
      next=b;
      s1=0;
      s2=0;
    }

};

arc *root; // First item in the parabolic front linked list.



//point of intersection of parabolas with foci a,b and directrix as x=l.
point intersection(point a,point b,double l)
{
    point res, p = a;

   if (a.x == b.x)
      res.y = (a.y + b.y) / 2;
   else if (b.x == l)
      res.y = b.y;
   else if (a.x == l) {
      res.y = a.y;
      p = b;
   } else 
    {
      // Use the quadratic formula.
      double z0 = 2*(a.x - l);
      double z1 = 2*(b.x - l);

      double t = 1/z0 - 1/z1;
      double u = -2*(a.y/z0 - b.y/z1);
      double v = ((a.y)*(a.y) + (a.x)*(a.x) - l*l)/z0 - ((b.y)*(b.y) + (b.x)*(b.x) - l*l)/z1;

      res.y = ( -u - sqrt(u*u - 4*t*v) ) / (2*t);
    }
   // Plug back into one of the parabola equations.
   res.x = (p.x*p.x + (p.y-res.y)*(p.y-res.y) - l*l)/(2*p.x-2*l);
   return res;
}

//function to check wether the new point p intersects the arc i if yes then at the point res.
bool isintersecting(point p, arc *i, point *res)
{
 
   double a,b;
   if (i->prev!=NULL) // Get the intersection of i->prev, i.
      a = intersection(i->prev->f, i->f, p.x).y;
   if (i->next!=NULL) // Get the intersection of i->next, i.
      b = intersection(i->f, i->next->f, p.x).y;

   if ((!i->prev || a <= p.y) && (!i->next || p.y <= b)) {
      res->y = p.y;

      // Plug it back into the parabola equation.
      res->x = (i->f.x*i->f.x + (i->f.y-res->y)*(i->f.y-res->y) - p.x*p.x)/ (2*i->f.x - 2*p.x);

      return true;
   }
   return false;
}


//function for x which is the x-coordinate of right most point of the circle 
//formed by the points a,b,c.
//and o is the centre of the circle.
bool circle(point a, point b, point c, double *x, point *o)
{
   // Check that bc is a "right turn" from ab.
   if ((b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y) > 0)
      return false;

   double A = b.x - a.x,  B = b.y - a.y,
          C = c.x - a.x,  D = c.y - a.y,
          E = A*(a.x+b.x) + B*(a.y+b.y),
          F = C*(a.x+c.x) + D*(a.y+c.y),
          G = 2*(A*(c.y-b.y) - B*(c.x-b.x));

   if (G == 0) return false;  // Points are co-linear.

   // Point o is the center of the circle.
   o->x = (D*E-B*F)/G;
   o->y = (A*F-C*E)/G;

   // o.x plus radius equals max x coordinate.
   *x = o->x + sqrt( pow(a.x - o->x, 2) + pow(a.y - o->y, 2) );
   return true;
}


//function which checks the circle event with foci of arcs i,i->next and i->prev.
void check_circle_event(arc *i, double x0)
{
   // Invalidate any old event.
   if (i->e && i->e->x != x0)
      i->e->valid = false;
   i->e = NULL;

   if (!i->prev || !i->next)
      return;

   double x;
   point o;

   //checking of circle event.
   if (circle(i->prev->f, i->f, i->next->f, &x,&o) && x > x0) 
   {
      // Create new event.
      i->e = new event(x, o, i);
      pushE(&events,i->e,5);
   }
}


// Bounding box coordinates.
double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0;

//function for inserting a new arc on the beach line who's focus is on the sweep line.
void frontInsert(point p)
{
   if (!root) {
      root = new arc(p);
      return;
   }

   // Find the current arc(s) at height p.y (if there are any).
   for (arc *i = root; i; i = i->next) {
      point z, zz;
      if (isintersecting(p,i,&z)) {
         // New parabola intersects arc i.  If necessary, duplicate i.
         if (i->next && !isintersecting(p,i->next, &zz)) {
            i->next->prev = new arc(i->f,i,i->next);
            i->next = i->next->prev;
         }
         else i->next = new arc(i->f,i);
         i->next->s1 = i->s1;

         // Add p between i and i->next.
         i->next->prev = new arc(p,i,i->next);
         i->next = i->next->prev;

         i = i->next; // Now i points to the new arc.

         // Add new half-edges connected to i's endpoints.
         i->prev->s2 = i->s1 = new seg(z);
         i->next->s1 = i->s2 = new seg(z);

         // Check for new circle events around the new arc:
         check_circle_event(i, p.x);
         check_circle_event(i->prev, p.x);
         check_circle_event(i->next, p.x);

         return;
      }
   }

   // Special case: If p never intersects an arc, append it to the list.
   arc *i;
   for (i = root; i->next; i=i->next) ; // Find the last node.

   i->next = new arc(p,i);
   // Insert segment between p and i
   point start;
   start.x = Xmin;
   start.y = (i->next->f.y + i->f.y) / 2;
   i->s1 = i->next->s1 = new seg(start);
}

void nextPoint()
{
   // Get the next point from the queue.
   point p = begin(&points)->data;
   pop(&points);

   // Add a new arc to the parabolic front.
   frontInsert(p);
}

void nextEvent()
{
   // Get the next event from the queue.
   event *e = beginE(&events)->data;
   popE(&events);

   if (e->valid) {
      // Start a new edge.
      seg *s = new seg(e->p);

      // Remove the associated arc from the front.
      arc *a = e->a;
      if (a->prev) {
         a->prev->next = a->next;
         a->prev->s2 = s;
      }
      if (a->next) {
         a->next->prev = a->prev;
         a->next->s1 = s;
      }

      // Finish the edges before and after a.
      if (a->s1) a->s1->finish(e->p);
      if (a->s2) a->s2->finish(e->p);

      // Recheck circle events on either side of p:
      if (a->prev) check_circle_event(a->prev, e->x);
      if (a->next) check_circle_event(a->next, e->x);
   }
   delete e;
}

void printOutput(point tp)
{
   // Bounding box coordinates.
   cout << Xmin << " "<< Xmax << " " << Ymin << " " << Ymax << endl;

   // Each output segment in four-column format.
   point res;
   arc* temp = root;
   while(!isintersecting(tp,temp,&res))
   {
      temp = temp->next;
   }
   isintersecting(tp,temp,&res);
   cout <<"(" << res.x << "," << res.y << ")" ;
}

int main()
{
     int n;
     point p;
     cout<<"enter number of sites to be present for the voronoi diagram: ";
     cin >> n;

     point testPoint;
     cout<< "Enter the x-coordinate of your test point: ";
     cin >> testPoint.x ;

      cout << "Enter the y-coordinate of your test point: ";
      cin >> testPoint.y;
    
     for(int j=0;j<n;j++) 
     {

      printf("Enter the x-coordinate of your point %d: ",j);
      cin >> p.x;
      printf("Enter the y-coordinate of your point %d: ",j);
      cin >> p.y;
      
      push(&points,p);

      // Keep track of bounding box size.
      if (p.x < Xmin) Xmin = p.x;
      if (p.y < Ymin) Ymin = p.y;
      if (p.x > Xmax) Xmax = p.x;
      if (p.y > Ymax) Ymax = p.y;
   }

   if (testPoint.x < Xmin) Xmin = testPoint.x;
   if (testPoint.y < Ymin) Ymin = testPoint.y;
   if (testPoint.x > Xmax) Xmax = testPoint.x;
   if (testPoint.y > Ymax) Ymax = testPoint.y;

   // Add margins to the bounding box.
   double dx = (Xmax-Xmin+1)/5.0, dy = (Ymax-Ymin+1)/5.0;
   Xmin -= dx;  Xmax += dx;  Ymin -= dy;  Ymax += dy;

   // Process the queues; select the top element with smaller x coordinate.
   while (!isEmpty(&points))
      if (!isEmptyE(&events) && beginE(&events)->data->x <= begin(&points)->data.x)
         nextEvent();
      else
         nextPoint();

   // After all points are processed, do the remaining circle events.
   while (!isEmptyE(&events))
      nextEvent();

  
   printOutput(testPoint); // Output the voronoi diagram.

    return 0;
}