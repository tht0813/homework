#pragma once

template<typename T>
class LinkedList
{
public:
	struct Node
	{
		Node(const T& v) : val{v}{}

		T val;
		Node* Next = nullptr;
	};

	void Add(const T& v)
	{
		Node* Temp = new Node(v);

		if (Tail == nullptr)
		{
			Head = Temp;
			Tail = Temp;
		}
		else
		{
			Tail->Next = Temp;
			Tail = Temp;
		}
	}

	bool Remove(const T& v)
	{
		Node* p = Head;
		Node* prev = nullptr;

		while (p != nullptr)
		{
			if (p->val == v)
			{
				if (prev != nullptr) { prev->Next = p->Next; }
				if (Head == p) { Head = p->Next; }
				if (Tail == p) { Tail = prev; }

				delete p;
				return true;
			}

			prev = p;
			p = p->Next;
		}
		return false;
	}

	bool Find(const T& v)
	{
		Node* p = Head;

		while (p != nullptr)
		{
			if (p->val == v)
			{
				return true;
			}
			p = p->Next;
		}
		return false;
	}

	uint Size()
	{
		Node* p = Head;
		uint Count = 0;

		while (p != nullptr)
		{
			p = p->Next;
			++Count;
		}
		return Count;
	}

	bool Empty()
	{
		return Head == nullptr;
	}

	void Clear()
	{
		Node* p = Head;
		while (p != nullptr)
		{
			Node* temp = p;
			p = p->Next;
			delete temp;
		}

		Head = nullptr;
		Tail = nullptr;
	}

	void Print()
	{
		if (Head == nullptr)
		{
			cout << "비어있음." << endl;
		}
		else
		{
			Node* p = Head;

			while (p != nullptr)
			{
				cout << p->val << endl;
				p = p->Next;
			}
		}
	}




public:
	LinkedList() = default;
	LinkedList(initializer_list<T> l)
	{
		for (auto& v : l)
		{
			Add(v);
		}
	}
	~LinkedList()
	{
		Clear();
	}


private:
	Node* Head = nullptr;
	Node* Tail = nullptr;
};

