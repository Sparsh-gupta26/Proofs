#include<iostream>
#include<math.h>
#include <bits/stdc++.h>

using namespace std;

template<typename T>class Vector
{
    public:
    T* arr;//stores the address of the vector.

    int max;//Maxium number of elements which can be stored in the vector.

    int num;//number of elements in the vector.
 
    Vector()//constructor with max=1.
    {
        arr = new T[1];//dynamic memory allocation.
        max = 1;
        num = 0;
    }

    void push(T data)//Inserting at the end.
    {
        if (num == max) 
        {
            T* temp = new T[2 * max];
            for (int i = 0; i < max; i++) 
            {
                temp[i] = arr[i];
            }
            delete[] arr;
            max = 2*max;
            arr = temp;
        }

        arr[num] = data;
        num++;
    }
 
    void pop() //deletion of element at the end.
    { 
        num--; 
    }

    int size() //size of the Vector.
    {
        return num; 
    }

    int getmax()//maxium number of elements that the Vector can store now. 
    { 
        return max; 
    }
 
    void print()// function to print array elements.
    {
        for (int i = 0; i < num; i++) 
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    T start()
    {
        return arr[0];
    }
    T end()
    {
        return arr[num-1];
    }

};

template<typename T>class node
{
    public:
    T data;
    node* next;

    node()
    {
        data;
        next = nullptr;
    }

    node(T i)
    {
        this->data=i;
        this->next=nullptr;
    }
};

template< typename T>class List
{

    node<T>* head;
    
    public:
    
    List() { head=NULL ;}
    
    void insertBack(T i)// inserting node at the end.
    {
        node<T>* newNode = new node<T>(i);
   
        if(head == NULL)
        {
            head = newNode;
            return;
        }
        node<T>* temp=head;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
        
    }

    void insertFront(T i)//inserting at the starting.
    {
        node<T>* newNode = new node<T>(i);

        node<T>* temp= head;
        head = newNode;
        newNode->next =temp;
    }

    void deleteFront()//deletes the element at the starting.
    {
        head = head->next;
    }

    void deleteBack()//deletes the element at the end.
    {
        node<T>* temp=head;
        while(temp->next != nullptr)
        {
            temp = temp->next;
        }

        temp->next = nullptr;

    }

    void print() //prints all the elements of the List.
    {
        node<T>* temp=head;
        while(temp!= nullptr)
        {
            cout << temp->data << " ";

            temp = temp->next;
        }
    }


    int size()//returns the size of the List.
     {
        node<T>* temp=head;
        int i=0;

        while(temp!= nullptr)
        {
            temp = temp->next;
            i++;
        }
        return i;
    }
};  

 
   


