### ゲームにおける二次元配列
**プレビューは「Ctrl+Shift+M」である**

ゲームでは二次元配列は一次元配列で実装したほうがよい。
なぜなら、配列を動的にサイズを変更する際二次元よりに便利であるためである。
なお、配列の参照は以下の式で可能である。

```
// (x,y)
array[y * width + x];
```

これ専用のクラスを作成するのも手もあり、以下に実装方法(int型)を記述する。

```
class IntArray2D{
	public:
		IntArray2D(int size0, int size1) :
		mArray(0), // ポインタに0を入れる
		mSize0(size0),
		mSize1(size1)
		{
			mArray = new int[size0 * size1];
		}

		// デストラクト
		~IntArray2D(){
			delete[] mArray;
			mArray = 0;
		}

		int& operator(int index0,int index1)
		{
			return mArray[index1 * mSize0 + index0];
		}

		const int& operater(int index0, int index1) const
		{
			return mArray[index1 * mSize0 + index0];
		}

	private:
		int* mArray;
		const int mSize0;
		const int mSize1;
};

int main(){
	IntArray2D array(4 , 3); // 4x3の配列
	int a = array(2 , 1); // array[2][1]の要素
	array(3 , 0) = 3; // 代入
	return (0);
}
```

以上である。

### ファイル読み込みについて

以下に実装を記述する。

```
#include<fstream>
#include<iostream>
using namespace std;
int main(){
	ifstream inputFile("Data.txt" , ifstream::binarry);
	inputFile.seekg(0 , ifstream::end); // 末尾へ移動
	// 位置＝ファイルサイズ
	int fileSize = static_cast< int >(inputFile.tellg());
	inputFile.seekg(0 , ifstream::beg); // 最初に戻って
	char* fileImage = new char[fileSize]; // 領域確保
	inputFile.read(fileImage , fileSize); // 読み込み
	cout.Write(fileImage , fileSize); // ちゃんと読めるか表示

	return (0);
}
```

以上である。

### C++補足
#### 1.名前空間

名前空間の説明のために、以下の例をコードに書く。
例えば、一年一組のスズキと二年一組のスズキ
名前空間:n年m組

```
namespace Grade1
{
	namespace Class1
	{
		Student Suzuki;
	}
}

namespace Grade2
{
	namespace Class1
	{
		Student Suzuki;
	}
}

// 使う時
Grade1::Class1::Suzuki;
Grade2::Class1::Suzuki;
```

ここでのスズキは別人であり、クラスも関係ない。
「using namespace宣言」は、「これからnamespace宣言で宣言された名前空間の中にあるものを使う」ことを意味する。
ここで、この宣言中のプログラムにおいて、coutという変数を作ると、「std::coutか変数cout」のどっちかわからなくなる。
usingは打ち消せず、ヘッダファイル内で宣言されると、それを読み込むcppが全部usingされた状態になる。

#### 2.テンプレート

テンプレートはコードを楽に書くためにある機能。

```
template<class SomeClass> classA{
	public:
		SomeClass mMember;
};
```

というコードを書くと

```
A<int> aInt;
int bInt = 5;
aInt.mMember = bInt;

A<float> aFloat;
float bFloat = 5.f;
aFloat.mMember = bFloat;
```

というようにSomeClassはintでもfloatでも使える便利なクラスになる。
cppで書くと他のccpで見えない(includeできない)ので、ヘッダファイルに書き込む。

#### 3.コンストラクト/デストラクト

以下にこのタイミングでは、コンストラクトかデストラクトのどちらが読み込まれたか、示す。

```
void foo()
{
	T t; // クラスTのコンストラクト
	T* tp = new T; // クラスTのコンストラクト
	delete tp; // デストラクト
} // デストラクト
```

コンストラクト/デストラクトの作り方

```
[T.h]
class T
{
	T(); // 引数なしコンストラクト
	T(int n); // int型引数をとるコンストラクト
	~T(); // デストラクト
}
[T.cpp]
T::T() {
	// 何か
}

T::T(int n)
{
	// 何か
}

T::~T()
{
	// 何か
}
```

#### 4.初期設定

コンストラクトで値を設定する場合、

```
Hoge::Hoge()
{
	mNumber = 0;
	mMember = 0;
}
```

とやるより、

```
Hoge::Hoge:
	mNumber(0),
	mMember(0)
{
	// 何か
}
```

とするほうが良い。
また、ｍMemberは変更することがない場合は、

```
clas Hoge{
	Hoge(int n) : mMember(n)
	{

	}const int mMember;
}
```

となる。

#### 5.メンバ関数const

この関数を読んでも、クラスのメンバが変化しないことを保証するため。

```
class classA
{
	void func0() const;
	void func1();
};

void hoge(const A& a)
{
	a.func0(); // OK
	a.func1(); // compile error
}

void func0()::A const
{
	func1(); // constでないため、エラー
}
```

#### 6.二種類のdelet

```
class T{}; // 何らかのクラス

void delete(T* p)
{
	p->~T();
	deallocate(p);
}
```

deleteは以上のような処理を行う。
deallocate関数はメモリ使用していてもう使わないとしるしをつける架空の関数。
また、delete[]は

```
void delete(T* p)
{
	for(int i = 0i < arraSize;i++)
	{
		p[i].~T();
	}
	deallocate(p);
}
```

である。
配列のdeleteは必ず、delete[]で行う。

#### 7.文字列定数

文字列定数は以下のように記述する。

```
const char strArray[] = "strArray"; // 1
const char* str = "str"; // 2
```

C言語の人が親切心(？)で考えられたもの。
char配列の定義を楽に書くためのもの。
以下に楽でない書き方を示す。

```
const char strArray[9] = {'s', 't', 'r', 'A', 'r', 'r', 'a', 'y', '\0'};
// ＾は上記1と同じ
const char strINITIALISE[4] = {'s', 't', 'r', '\0'};
const char* str = &strINITIALISE[0];
// ＾は上記2と同じ
```
