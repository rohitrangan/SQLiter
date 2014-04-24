#define MAX 5
#include <iostream>

using namespace std;
  
 class node; //To handle forward reference
 struct pair_t
    {
    node *next;
    int key;
    };
  
class node
 {
    public:
    int noofkeys;
    pair_t data[MAX];
    node *father;
    node *first;
    node();
    int leafnode();
    void insertinanode(pair_t x);
    pair_t splitanode(pair_t x);      /* splitting of a leaf node due to
                     overflow,address of the secons half is returned */
     node *nextindex(int x);//index of the subtree containing x
     void display();
  };
  
  void node::display()
     {
    int i;
    cout<<"(";
    for(i=0;i<noofkeys;i++)
        cout<<data[i].key<<" ";
     cout<<")";
     }
  
  node *node::nextindex(int x)
    {
    int i;
    if(x<data[0].key)
       return(first);
    for(i=0;i<noofkeys ;i++)
      if(x <= data[i].key)
        return data[i-1].next;
    return data[i-1].next;
     }
  
 int node::leafnode()
    {
    if(data[0].next==NULL)
        return 1;
    return 0;
     }
  
 void node::insertinanode(pair_t x)
   {
    int i;
    for(i=noofkeys-1;i>=0 && data[i].key>x.key;i--)
        data[i+1]=data[i];
    data[i+1]=x;
    noofkeys++;
   }
  
 pair_t node::splitanode(pair_t x)
     {
    node *T;
    pair_t mypair;
    int i,j,centre;
    centre=(noofkeys-1)/2;
//  cout<<"\ncentre= "<<centre;
    T=new node;
    if(x.key>data[centre].key)  //Divide the node in two parts(original and T)
        {
        for(i=centre+1,j=0;i<=noofkeys;i++,j++)
            T->data[j]=data[i];
         T->noofkeys=noofkeys-centre-1;
         noofkeys=noofkeys-T->noofkeys;
         T->insertinanode(x);
         T->first=T->data[0].next;
         T->father=father;
         mypair.key=T->data[0].key;
         mypair.next=T;
         //Delete the first key from node T
         for(i=1;i<T->noofkeys;i++)
            T->data[i-1]=T->data[i];
         T->noofkeys--;
        }
     else
        {
         for(i=centre,j=0;i<noofkeys;i++,j++)
            T->data[j]=data[i];
         T->noofkeys=noofkeys-centre;
         noofkeys=noofkeys-T->noofkeys;
         insertinanode(x);
         T->father=father;
         mypair.key=T->data[0].key;
         mypair.next=T;
         //Delete the first key from node T
         for(i=1;i<T->noofkeys;i++)
            T->data[i-1]=T->data[i];
         T->noofkeys--;
        }
   return(mypair);
 }
  
 node::node()
    {
          for(int i=0;i<=MAX;i++)
           data[i].next=NULL;
          noofkeys=0;
          father=NULL;
          first=NULL;
     }
  
class Q //To traverse the B-Tree ,level wise
  {
    node *data[60];
    int R,F;
    public:
        Q()
          {
            R=F=0;
          }
        int empty()
          {
            if(R==F)
                return 1;
            else
                return 0;
          }
        node *deque()
          {
            return data[F++];
          }
        void  enque(node *x)
            {
            data[R++]=x;
            }
        void makeempty()
           {
            R=F=0;
           }
   };
class btree
 {
    int mkeys; //maximum number of keys in a node
    node *root;
    public:
        btree(int n)
           {
            mkeys=n;
            root=NULL;
           }
        void insert(int x);
        void displaytree();
        node *search(int x);
        void Delete(int x);
  };
node *  btree::search(int x)
 {
    node *p;
    int i;
    p=root;
    while(p!=NULL)
       {    //p->display();
        //getch();
        for(i=0;i<p->noofkeys;i++)
            if(x==p->data[i].key)
                return(p);
        p=p->nextindex(x);
  
       }
    return NULL;
 }
void btree::displaytree()
   {
    Q q1,q2;
    node *p;
    q1.enque(root);
    while(!q1.empty())
      {
        q2.makeempty();
        cout<<"\n";
        while(!q1.empty())
          {
            p=q1.deque();
            p->display();cout<<"  ";
            if(!p->leafnode())
               {
                q2.enque(p->first);
                for(int i=0;i<p->noofkeys;i++)
                    q2.enque(p->data[i].next);
  
               }
           }
        q1=q2;
       }
      }
void btree::insert(int x)
  {
    int index;
    pair_t mypair;
    node *p,*q;
    mypair.key=x;
    mypair.next=NULL;
    if(root==NULL)
    {
         root = new node;
         root->insertinanode(mypair);
    }
    else
    {
        p=root;
        while(!(p->leafnode()))
          p=p->nextindex(x);
        if(p->noofkeys<mkeys)
             p->insertinanode(mypair);
  
      else
        {
             mypair=p->splitanode(mypair);
             while(1)
               {
             if(p==root)
                {
                   q=new node;
                   q->data[0]=mypair;
                   q->first=root;
                   q->father=NULL;
                   q->noofkeys=1;
                   root=q;
                   q->first->father=q;
                   q->data[0].next->father=q;
                   return;
  
                 }
              else
                 {
                p=p->father;
                if(p->noofkeys < mkeys)
                   {
                    p->insertinanode(mypair);
                    return;
                    }
                 else
                    mypair=p->splitanode(mypair);
                  }
  
             }
          }
  
    }
  }
  
 void btree::Delete(int x)
   {
      node *left,*right;
      pair_t *centre;
      node *p,*q;
      int i,j,centreindex;
      p=search(x);
      for(i=0;p->data[i].key != x; i++)
      ;
      // if p is not a leaf node then locate its succssor  in a leaf node,
      // replace x with its successor/predecessor and delete it.
      if(!p->leafnode())
    {
      q=p->data[i].next;
      while(!q->leafnode())
           q=q->first;
      p->data[i].key=q->data[0].key;
      p=q;                //p points to leaf node
      x=q->data[0].key;
      i=0;
    }
//      if(p->leafnode()) ,p will always be a leaf node
  
        for(i=i+1;i<p->noofkeys;i++)
            p->data[i-1]=p->data[i];
        p->noofkeys--;
  
       while(1)
        {
        if(p->noofkeys >= mkeys/2 )
            return;
        if(p==root )
           if(p->noofkeys>0)
              return;
           else
              {
             root=p->first;
             return;
              }
   //       otherwise
  
            q=p->father;
            if(q->first==p || q->data[0].next==p)
               {
              left=q->first;
              right=q->data[0].next;
              centre=&(q->data[0]);
              centreindex=0;
               }
            else
               {
              for(i=1;i<q->noofkeys;i++)
                if(q->data[i].next==p)
                     break;
              left=q->data[i-1].next;
              right=q->data[i].next;
              centre=&(q->data[i]);
              centreindex=i;
               }
  
     //case 1 : left has one extra key, move a key from left
           if(left->noofkeys > mkeys/2)
             {
                  for(i=right->noofkeys-1;i>=0;i--)
                     right->data[i+1]=right->data[i];
                  right->noofkeys ++;
                  right->data[0].key=centre->key;
                  centre->key=left->data[left->noofkeys - 1].key;
                  left->noofkeys--;
                  return;
              }
     // case 2 : right has one extra key, move a key from right
            else
              if(right->noofkeys >mkeys/2)
                 {
                   left->data[left->noofkeys].key=centre->key;
                   left->noofkeys++;
                   centre->key=right->data[0].key;
                   for(i=1;i<right->noofkeys;i++)
                    right->data[i-1]=right->data[i];
                   right->noofkeys--;
                   return;
                 }
               else
                {  //merge left and right
                left->data[left->noofkeys].key=centre->key;
                left->noofkeys++;
                for(j=0;j<right->noofkeys;j++)
                    left->data[left->noofkeys+j]=right->data[j];
                left->noofkeys+=right->noofkeys;
                //delete the pair from the parent
                for(i=centreindex+1;i<q->noofkeys ;i++)
                    q->data[i-1]=q->data[i];
                q->noofkeys--;
                p=q;//for next iteration
                 }
  
  
          }
  
  
  
   } 
