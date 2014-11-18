#ifndef __STACK_H__
#define __STACK_H__

#define MAX_STACK_SIZE 1000

template <class T>
class Stack
{
	T elements[MAX_STACK_SIZE];
	int topo;
public:
	Stack();
	~Stack();

	void push(T element);
	void pop();
	T top();
	bool empty();
};

template <class T>
Stack<T>::Stack() : topo(-1)
{

}

template <class T>
Stack<T>::~Stack()
{

}

template <class T>
void Stack<T>::push(T element)
{
	if (topo < MAX_STACK_SIZE - 1)
		elements[++topo] = element;
}

template <class T>
void Stack<T>::pop()
{
	if (topo > -1) topo--;
}

template <class T>
T Stack<T>::top()
{
	return elements[topo];
}

template <class T>
bool Stack<T>::empty()
{
	return (topo == -1);
}
#endif