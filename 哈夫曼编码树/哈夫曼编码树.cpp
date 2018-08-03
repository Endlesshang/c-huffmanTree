#include <iostream>
#include <assert.h>
#include <cstring>
#include <stack>
using namespace std;
/*编码结构体*/
struct cod
{
	char ccod;
	int rate;
	char convertCod[15];  //单个编码所代表的哈夫曼编码
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
			cout << "第" << i + 1 << "个字符的信息为：" << c[i].ccod << "  频率为：" << c[i].rate << endl;
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
//最大堆的声明
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
//Haffman树的实现
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
HuffTree<E>* BuildHuff(HuffTree<E>** TreeArray, int count)                  //建立哈夫曼树并且返回一棵指针树
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
void visit(HuffNode<E> *root)            //输出这个结点的元素
{
	cout << root->weight() << " ";
}
template <typename E>
void setccod(HuffNode<E> *root, textDecod it)                     //从已建立的哈夫曼树中提取哈夫曼码值并存入前面的textCod类中
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
			}/*temp是分支结点左右子树是叶结点左子树未被访问*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 0 && !temp->right()->isLeaf() && temp->getvisitedcount() < Leafcount(temp))
			{
				temp->countplus();
				cc[i] = '0';
				temp = temp->left();
				i++;
			}/*temp是分支结点左子树是未被访问叶结点右子树是分支结点*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 1 && !temp->right()->isLeaf() && !temp->right()->left()->isLeaf())
			{
				temp->countplus();
				cc[i] = '1';
				i++;
				temp = temp->right();
			}/*temp是分支结点左子树是已被访问叶结点右子树是左边连着分支结点的分支结点*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 1 && !temp->right()->isLeaf() && temp->right()->left()->isLeaf())
			{
				cc[i] = '1';
				temp->countplus();
				temp = temp->right();
				i++;
			}/*temp是分支结点左子树是已被访问叶结点右子树是左边连着叶结点的分支结点*/
			else if (!temp->isLeaf() && temp->left()->isLeaf() && temp->left()->getvisited() == 1 && temp->right()->isLeaf() && temp->right()->getvisited()==0)
			{
				cc[i] = '1';
				temp->countplus();
				temp = temp->right();
				//temp->changePrinted();
				i++;
				//cc[i] = '1';
			}/*temp是分支结点左子树是已被访问叶结点右子树是未被访问叶结点*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && temp->left()->left()->isLeaf() /*&& temp->left()->right()->isLeaf()*/ && temp->left()->getvisitedcount() < Leafcount(temp->left()))
			{
				temp->countplus();
				cc[i] = '0';
				i++;
				temp = temp->left();
			}/*temp是分支结点左子树是左边连着叶结点的分支结点并且左子树未被访问完全*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && temp->left()->left()->isLeaf() && temp->left()->getvisitedcount() == Leafcount(temp ->left()))
			{
				temp->countplus();
				cc[i] = '1';
				i++;
				temp = temp->right();
			}/*temp是分支结点左子树是左边连着叶结点的的分支结点并且左子树已被访问完全*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && !temp->left()->left()->isLeaf() && !temp->right()->isLeaf() && temp->left()->getvisitedcount() < Leafcount(temp->left()))
			{
				temp->countplus();
				cc[i] = '0';
				i++;
				temp = temp->left();
			}/*temp是分支结点左子树是左边连着分支结点的分支结点并且左子树未被访问完全并且右子树是分支结点*/
			else if (!temp->isLeaf() && !temp->left()->isLeaf() && !temp->left()->left()->isLeaf() && !temp->right()->isLeaf() && temp->left()->getvisitedcount() == Leafcount(temp->left()))
			{
				temp->countplus();
				cc[i] = '1';
				i++;
				temp = temp->right();
			}/*temp是分支结点左子树是左边连着分支结点的分支结点并且左子树已被访问完全并且右子树是分支结点*/
			//cout << "此时在权值为" << temp->weight() << endl;
		}
		if (temp->isLeaf())
		{
			//cc[i] = temp->getSign();
			temp->changePrinted();
		}
		cout << temp->itt() << "的哈夫曼编码是";                  //对单个字母进行输出编码测试
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
void preorder(HuffNode<E> *root)            //前序输出哈夫曼树
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
void cipherToplain(HuffTree<E> *Tree,char *pa)                    //把密文翻译成明文
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
	cout << "********************************* 哈夫曼编码系统 *************************************"<<endl;
	cout << "                                 请先建立哈夫曼树" << endl;
	int n=0;
	cout << "请输入待传送的字符信息长度：" << endl;
	cin >> n;
	char *textt=new char[n+1];
	cout << "请输入待传送的字符信息：" << endl;
	cin >> textt;
	while (strlen(textt) > n)
	{
		cout << "输入字符超过限制，请重新输入" << endl;
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
	cout << "建树完毕" << endl;
	cout << "         请输入指令：1.输出哈夫曼编码 2.对密文进行解码" << endl;
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
				  cout << "该明文的哈夫曼编码值为：";
				  //cout << strlen(te.c[0].convertCod) << endl;
				  te.showHuffcod();
				  cout << endl;
				  cout << "*********************************************************************************" << endl;
		}break;
		case 2:
		{
				  int m = 0;
				  cout << "请输入需要转换的密文长度：" << endl;
				  cin >> m;
				  char *ap = new char[m + 1];
				  cout << "请输入需要转换的密文" << endl;
				  cin >> ap;
				  cipherToplain(s1.getHuffTree(), ap);
				  cout << endl;
				  cout << "*********************************************************************************" << endl;
		}break;
		default:cout << "错误的指令" << endl;
			break;
		}
		cout << "还要使用什么功能？（输入0为结束系统）" << endl;
		cin >> b;
		cout << "         请输入指令：1.输出哈夫曼编码 2.对密文进行解码" << endl;
	}
}