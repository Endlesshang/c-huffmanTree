#include <iostream>
#include <assert.h>
#include <cstring>
#include <stack>
using namespace std;
/*����ṹ��*/
struct cod
{
	char ccod;
	int rate;
	char convertCod[15];  //��������������Ĺ���������
	int convertCodcount;
};
class textDecod
{
private:
	int length;
	char *text;
public:
	cod *c;
	textDecod(int n, char *a)
	{
		c = new cod[countchar(a)];
		length = countchar(a);
		text = new char[strlen(a) + 1];
		strcpy(text, a);
		setCod();
		setRate();
	}
	//~textDecod(){ delete c; delete text; }
	int getlength(){ return length; }
	int countchar(char *a)
	{
		int cc = 0, dd = 0;
		for (int i = 0; i < strlen(a); i++)
		{
			for (int j = i; j >=0; j--)
			{
				if (a[j] == a[i] && i!=j)
				{
					cc++;
				}
			}
			if (cc == 0)
			{
				dd++;
			}
			cc = 0;
		}
		return dd;
	}
	void setCod()
	{
		int cc = 0, dd = 0;
		for (int i = 0; i < strlen(text); i++)
		{
			for (int j = i; j >=0; j--)
			{
				if (text[i] == text[j] && i!=j)
					cc++;
			}
			if (cc == 0)
			{
				c[dd].ccod = text[i];
				dd++;
			}
			cc = 0;
		}
	}
	cod* getc(){ return c; };
	void setRate()
	{
		int cc=0;
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < strlen(text); j++)
			{
				if (c[i].ccod == text[j])
				{
					cc++;
				}
			}
			c[i].rate = cc;
			cc = 0;
		}
	}
	int* weight()
	{
		int *ww=new int[length];
		for (int i = 0; i < length; i++)
		{
			ww[i] = c[i].rate;
		}
		return ww;
	}
	cod searchCod(char cc)
	{
		for (int i = 0; i < length; i++)
		{
			if (c[i].ccod == cc)
				return c[i];
		}
	}
	void show()
	{
		for (int i = 0; i < length; i++)
		{
			cout << "��" << i + 1 << "���ַ�����ϢΪ��" << c[i].ccod << "  Ƶ��Ϊ��" << c[i].rate << endl;
		}
	}
	void showHuffcod()
	{
		for (int i = 0; i < strlen(text); i++)
		{
			for (int j = 0; j < length; j++)
			{
				if (c[j].ccod == text[i])
				{
					for (int k = 0; k < c[j].convertCodcount; k++)
					{
						cout << c[j].convertCod[k];
					}
				}
				//break;
			}
		}
	}
};
//���ѵ�����
template <typename E, typename Comp>class heap
{
private:
	E *Heap;
	int maxsize;
	int n;
	void siftdown(int pos)
	{
		while (!isLeaf(pos))
		{
			int j = leftchild(pos);
			int rc = rightchild(pos);
			if ((rc < n) && Comp::prior(Heap[rc], Heap[j]))
				j = rc;
			if (Comp::prior(Heap[pos], Heap[j])) return;
			swap(Heap, pos, j);
			pos = j;
		}
	}
public:
	heap(E* h, int num, int max)
	{
		Heap = h; n = num; maxsize = max; buildHeap();
	}
	int size()const
	{
		return n;
	}
	bool isLeaf(int pos) const
	{
		return (pos >= n / 2) && (pos < n);
	}
	int leftchild(int pos) const
	{
		return 2 * pos + 1;
	}
	int rightchild(int pos) const
	{
		return 2 * pos + 2;
	}
	int parent(int pos) const
	{
		return (pos - 1) / 2;
	}
	void buildHeap()
	{
		for (int i = n / 2 - 1; i >= 0; i--) siftdown(i);
	}
	void insert(const E &it)
	{
		assert(n < maxsize, "Heap is full");
		int curr = n++;
		Heap[curr] = it;
		while ((curr != 0) && (Comp::prior(Heap[curr], Heap[parent(curr)])))
		{
			swap(Heap, curr, parent(curr));
			curr = parent(curr);
		}
	}
	void swap(E *p, int a, int b)
	{
		E temp = p[a];
		p[a] = p[b];
		p[b] = temp;
	}
	E removefirst()
	{
		assert(n > 0, "Heap is empty");
		swap(Heap, 0, --n);
		if (n != 0)siftdown(0);
		return Heap[n];
	}
	E remove(int pos)
	{
		assert((pos >= 0) && (pos < n), "Bad position");
		if (pos == (n - 1)) n--;
		else
		{
			swap(Heap, pos, --n);
			while ((pos != 0) && (Comp::prior(Heap[pos], Heap[parent(pos)])))
			{
				swap(Heap, pos, parent(pos));
				pos = parent(pos);
			}
			if (n != 0)siftdown(pos);
		}
		return Heap[n];
	}
};
//Haffman����ʵ��
template <typename E>
class HuffNode
{
	//bool isLeaf;
public:
	char sign;
	virtual ~HuffNode(){}
	virtual int weight() = 0;
	virtual bool isLeaf() = 0;
	void setSign(char x){ sign = x; }
	char getSign(){ return sign; }
	virtual HuffNode<E>* left() = 0;
	virtual HuffNode<E>* right() = 0;
	virtual void changePrinted(){};
	virtual void countplus(){};
	virtual int getvisited(){ return 1; }
	virtual int getvisitedcount(){ return 0; }
	virtual E itt(){ return 0; }
};
template <typename E>
class LeafNode :public HuffNode<E>
{
private:
	E it;
	int wgt;
	int visited;
public:
	LeafNode(const E& val, int freq)
	{
		it = val; wgt = freq; sign = '0'; /*isLeaf = true;*/
		visited = 0;
	}
	int weight(){ return wgt; }
	E val(){ return it; }
	bool isLeaf(){ return true; }
	HuffNode<E>* left(){ return 0; }
	HuffNode<E>* right(){ return 0; }
	void changePrinted(){ visited = 1; }
	int getvisited(){ return visited; }
	E itt(){ return it; }
};
template <typename E>
class IntlNode :public HuffNode<E>
{
private:
	HuffNode<E>* lc;
	HuffNode<E>* rc;
	int wgt;
	int visitedcount;
public:
	IntlNode(HuffNode<E>* l, HuffNode<E>* r)
	{
		wgt = l->weight() + r->weight(); lc = l; rc = r; sign = 0; /*isLeaf = false;*/
		setAllSign();
		visitedcount = 0;
	}
	int weight(){ return wgt; }
	bool isLeaf(){ return false; }
	HuffNode<E>* left() { return lc; }
	void setLeft(HuffNode<E>* b)
	{
		lc = (HuffNode<E>*)b;
	}
	HuffNode<E>* right()  { return rc; }
	void setRight(HuffNode<E>* b)
	{
		rc = (HuffNode<E>*)b;
	}
	void setAllSign()
	{
		lc->setSign('0');
		rc->setSign('1');
	}
	void countplus(){ visitedcount++; }
	int getvisitedcount(){ return visitedcount; }
};
template <typename E>
class HuffTree
{
private:
	HuffNode<E>* Root;
public:
	HuffTree(char val, int freq)
	{
		Root = new LeafNode<E>(val, freq);
	}
	HuffTree(HuffTree<E> *l, HuffTree<E> *r)
	{
		Root = new IntlNode<E>(l->root(), r->root());
	}
	~HuffTree(){}
	HuffNode<E>* root(){ return Root; }
	int weight(){ return Root->weight(); }
};
class minTreeComp
{
public:
	static bool prior(HuffTree<char> *t1, HuffTree<char> *t2)
	{
		if (t1->root()->weight() < t2->root()->weight())
			return true;
		else
			return false;
	}
};
template <typename E>
HuffTree<E>* BuildHuff(HuffTree<E>** TreeArray, int count)                  //���������������ҷ���һ��ָ����
{
	heap <HuffTree<E>*, minTreeComp>* forest =
		new heap<HuffTree<E>*, minTreeComp>(TreeArray, count, count);
	HuffTree<char> *temp1, *temp2, *temp3 = NULL;
	while (forest->size() > 1)
	{
		temp1 = forest->removefirst();
		temp2 = forest->removefirst();
		temp3 = new HuffTree<E>(temp1, temp2);
		forest->insert(temp3);
		delete temp1;
		delete temp2;
	}
	return temp3;
}
template <typename E>
void visit(HuffNode<E> *root)            //����������Ԫ��
{
	cout << root->weight() << " ";
}
template <typename E>
void setccod(HuffNode<E> *root, textDecod it)                     //���ѽ����Ĺ�����������ȡ��������ֵ������ǰ���textCod����
{
	int i = 0;
	char cc[50];
	HuffNode<E> *temp = root;
	for (int j = 0; j < it.getlength(); j++)
	{
		while (temp->isLeaf() == false)
		{
			if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 0 && temp->right()->isLeaf())
			{
				temp->countplus();
				cc[i] = '0';
				temp = temp->left();
				i++;
				//cc[i] = '1';
			}/*temp�Ƿ�֧�������������Ҷ���������δ������*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 0 && !temp->right()->isLeaf() && temp->getvisitedcount() < Leafcount(temp))
			{
				temp->countplus();
				cc[i] = '0';
				temp = temp->left();
				i++;
			}/*temp�Ƿ�֧�����������δ������Ҷ����������Ƿ�֧���*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 1 && !temp->right()->isLeaf() && !temp->right()->left()->isLeaf())
			{
				temp->countplus();
				cc[i] = '1';
				i++;
				temp = temp->right();
			}/*temp�Ƿ�֧������������ѱ�����Ҷ�����������������ŷ�֧���ķ�֧���*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 1 && !temp->right()->isLeaf() && temp->right()->left()->isLeaf())
			{
				cc[i] = '1';
				temp->countplus();
				temp = temp->right();
				i++;
			}/*temp�Ƿ�֧������������ѱ�����Ҷ������������������Ҷ���ķ�֧���*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 1 && temp->right()->isLeaf() && temp->right()->getvisited()==0)
			{
				cc[i] = '1';
				temp->countplus();
				temp = temp->right();
				//temp->changePrinted();
				i++;
				//cc[i] = '1';
			}/*temp�Ƿ�֧������������ѱ�����Ҷ�����������δ������Ҷ���*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && temp->left()->left()->isLeaf() /*&& temp->left()->right()->isLeaf()*/ && temp->left()->getvisitedcount() < Leafcount(temp->left()))
			{
				temp->countplus();
				cc[i] = '0';
				i++;
				temp = temp->left();
			}/*temp�Ƿ�֧������������������Ҷ���ķ�֧��㲢��������δ��������ȫ*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && temp->left()->left()->isLeaf() && temp->left()->getvisitedcount() == Leafcount(temp ->left()))
			{
				temp->countplus();
				cc[i] = '1';
				i++;
				temp = temp->right();
			}/*temp�Ƿ�֧������������������Ҷ���ĵķ�֧��㲢���������ѱ�������ȫ*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && !temp->left()->left()->isLeaf() && !temp->right()->isLeaf() && temp->left()->getvisitedcount() < Leafcount(temp->left()))
			{
				temp->countplus();
				cc[i] = '0';
				i++;
				temp = temp->left();
			}/*temp�Ƿ�֧�����������������ŷ�֧���ķ�֧��㲢��������δ��������ȫ�����������Ƿ�֧���*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && !temp->left()->left()->isLeaf() && !temp->right()->isLeaf() && temp->left()->getvisitedcount() == Leafcount(temp->left()))
			{
				temp->countplus();
				cc[i] = '1';
				i++;
				temp = temp->right();
			}/*temp�Ƿ�֧�����������������ŷ�֧���ķ�֧��㲢���������ѱ�������ȫ�����������Ƿ�֧���*/
			//cout << "��ʱ��ȨֵΪ" << temp->weight() << endl;
		}
		if (temp->isLeaf())
		{
			//cc[i] = temp->getSign();
			temp->changePrinted();
		}
		cout << temp->itt() << "�Ĺ�����������";                  //�Ե�����ĸ��������������
		for (int l = 0; l < i; l++)
		{
			cout << cc[l];
		}
		cout << endl;
		//it.c[j].convertCod = new char[i + 1];
		//it.c[j].convertCodcount = i;
		for (int n = 0; n < it.getlength();n++)
		{
			if (it.c[n].ccod == temp->itt())
			{
				it.c[n].convertCodcount = i;
				for (int k = 0; k < i; k++)
				{
					it.c[n].convertCod[k] = cc[k];
				}
			}
		}
		temp = root;
		for (int ty = 0; ty < i; ty++)
		{
			cc[ty] = NULL;
		}
		i = 0;
	}
}
template <typename E>
void preorder(HuffNode<E> *root)            //ǰ�������������
{
	if (root == NULL) return;
	visit(root);
	if (root->isLeaf() == false)
	{
		preorder(root->left());
		preorder(root->right());
	}
}
template <typename E>
int Leafcount(HuffNode<E> *root)
{
	if (root == NULL)
		return 0;
	else if (root->left() == NULL && root->right() == NULL)
		return 1;
	else
		return Leafcount(root->left()) + Leafcount(root->right());
}
template <typename E>
void cipherToplain(HuffTree<E> *Tree,char *pa)                    //�����ķ��������
{
	char text[15];
	HuffNode<E> *temp = Tree->root();
	int i = 0, j = 0;
	while (i < strlen(pa))
	{
		while (!temp->isLeaf())
		{
			if (pa[i] == '0')
			{
				temp = temp->left();
				i++;
			}
			else if (pa[i] == '1')
			{
				temp = temp->right();
				i++;
			}
		}
		text[j] = temp->itt();
		j++;
		temp = Tree->root();
	}
	for (int k = 0; k < j; k++)
	{
		cout << text[k];
	}
}
template <typename E>
class saveHuffTree
{
	HuffTree<E> *H;
public:
	saveHuffTree(HuffTree<E> *C){H = C;}
	HuffTree<E>* getHuffTree(){ return H; }
};
void main()
{
	cout << "**************************************************************************************"<<endl;
	cout << "********************************* ����������ϵͳ *************************************"<<endl;
	cout << "                                 ���Ƚ�����������" << endl;
	int n=0;
	cout << "����������͵��ַ���Ϣ���ȣ�" << endl;
	cin >> n;
	char *textt=new char[n+1];
	cout << "����������͵��ַ���Ϣ��" << endl;
	cin >> textt;
	while (strlen(textt) > n)
	{
		cout << "�����ַ��������ƣ�����������" << endl;
		cin >> textt;
	}
	textDecod te(n, textt);
	te.show();
	HuffTree<char> **HH = new HuffTree<char> *[te.getlength()];
	for (int i = 0; i < te.getlength(); i++)
	{
	  HH[i] = new HuffTree<char>(te.c[i].ccod, te.c[i].rate);
	}
	saveHuffTree<char> s1(BuildHuff(HH, te.getlength()));
	cout << "�������" << endl;
	cout << "         ������ָ�1.������������� 2.�����Ľ��н���" << endl;
	int b = 10,c;
	while (b)
	{
		cin >> c;
		switch (c)
		{
		case 1:
		{
				  preorder(s1.getHuffTree()->root());
				  cout << endl;
				  //cout<<Leafcount(s1.getHuffTree()->root())<<endl;
				  setccod(s1.getHuffTree()->root(), te);
				  cout << "�����ĵĹ���������ֵΪ��";
				  //cout << strlen(te.c[0].convertCod) << endl;
				  te.showHuffcod();
				  cout << endl;
				  cout << "*********************************************************************************" << endl;
		}break;
		case 2:
		{
				  int m = 0;
				  cout << "��������Ҫת�������ĳ��ȣ�" << endl;
				  cin >> m;
				  char *ap = new char[m + 1];
				  cout << "��������Ҫת��������" << endl;
				  cin >> ap;
				  cipherToplain(s1.getHuffTree(), ap);
				  cout << endl;
				  cout << "*********************************************************************************" << endl;
		}break;
		default:cout << "�����ָ��" << endl;
			break;
		}
		cout << "��Ҫʹ��ʲô���ܣ�������0Ϊ����ϵͳ��" << endl;
		cin >> b;
		cout << "         ������ָ�1.������������� 2.�����Ľ��н���" << endl;
	}
}