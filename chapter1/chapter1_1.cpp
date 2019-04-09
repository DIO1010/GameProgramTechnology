#include<iostream>

using namespace std;

// Stageの幅、高さ
#define STAGE_WIDTH 8
#define STAGE_HEIGHT 5

// Stageの初期化
const char stage[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

// 列挙
enum StageObject
{
	OBJ_NONE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,
	OBJ_UNKNOWN
};

// 関数プロトタイプ
void initialize(StageObject* state, int width, int height, const char* stageData);
void draw(const StageObject* state, int width, int height);
void update(StageObject* state, char input, int width, int height);
bool checkClear(const StageObject* state, int width, int height);

// 初期化
void initialize(
	StageObject* state,
	int width,
	int height,
	const char* stageData
){
	const char* data = stageData;
	int x = 0;
	int y = 0;
	while(*data != '\0') // NULL文字でない間
	{
		StageObject t;
		switch (*data) {
			case '#': t = OBJ_WALL; break;
			case ' ': t = OBJ_NONE; break;
			case 'o': t = OBJ_BLOCK; break;
			case 'O': t = OBJ_BLOCK_ON_GOAL; break;
			case '.': t = OBJ_GOAL; break;
			case 'p': t = OBJ_MAN; break;
			case 'P': t = OBJ_MAN_ON_GOAL; break;
			case '\n': // 次の行へ
				x = 0; // xを左端へ
				++y; // yは一段下へ
				t = OBJ_UNKNOWN; // データはなし
				break;
			default: t = OBJ_UNKNOWN; break; // 不正データ
		}
		++data;
		if(t != OBJ_UNKNOWN){
			state[y * width + x] = t; // 書き込む
			++x;
		}
	}
}

// 描写
void draw
(
	const StageObject* state,
	int width,
	int height
){
	// StageObjectの列挙型の順
	const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};
	for(int y = 0;y < height;++y)
	{
		for(int x = 0;x < width;++x)
		{
			StageObject so = state[y * width + x];
			cout << font[so];
		}
		cout << endl;
	}
}

// 更新
void update(
	StageObject* state,
	char input,
	int width,
	int height
){
	// 移動量に変換
	int dx = 0;
	int dy = 0;
	switch (input) {
		case 'a': dx = -1; break; // 右
		case 's': dx =  1;  break; // 左
		case 'w': dy = -1; break; // 上
		case 'z': dy =  1;  break; // 下
	}

	// 人座標を検索
	int i = -1; // 特に意味なし。何か初期化。
	for(i = 0;i < width * height;i++)
	{
		if(state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}
	int x = i % width; // xは幅で割った余り
	int y = i / width; // yは幅で割った商

	// 移動後座標
	int tx = x + dx;
	int ty = y + dy;
	// 範囲外に行かないか
	if(tx < 0 || tx >= width || ty < 0 || ty >= height)
	{
		return;
	}

	int p = y * width + x; // 人位置
	int tp = ty * width + tx; // ターゲット位置
	// 移動先が空白、またはゴール
	if(state[tp] == OBJ_GOAL || state[tp] == OBJ_NONE)
	{
		// ゴールならゴール上に
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		// もともとゴール上ならゴールに
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_NONE;
	}
	// 移動先が箱。その方向の次のマスが空白またはゴールなら移動
	else if(state[tp] == OBJ_BLOCK || state[tp] == OBJ_MAN_ON_GOAL)
	{
		// 2マス先が範囲内かチェック
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if(tx2 < 0 || tx2 >= width || ty2 < 0 || ty2 >= height)
		{
			return; // 押せない
		}
		int tp2 = (ty + dy) * width + (tx + dx); // 2マス先
		if(state[tp2] == OBJ_NONE || state[tp2] == OBJ_GOAL)
		{
			// 順序入れ替え
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = (state[p] == OBJ_BLOCK_ON_GOAL) ? OBJ_GOAL : OBJ_NONE;
		}
	}
}

// クリア判定
bool checkClear(
	const StageObject* state,
	int width,
	int height
){
	for(int i = 0;i < width * height;i++)
	{
		if(state[i] == OBJ_BLOCK)
		{
			return (false);
		}
	}

	return (true);
}

int main()
{
	// 状態配列初期化
	StageObject* state = new StageObject[STAGE_WIDTH * STAGE_HEIGHT];

	// ステージ初期化
	initialize(state, STAGE_WIDTH, STAGE_HEIGHT, stage);

	// メインループ
	while(true)
	{
		// 描写
		draw(state, STAGE_WIDTH, STAGE_HEIGHT);

		// クリアチェック
		if(checkClear(state, STAGE_WIDTH, STAGE_HEIGHT))
		{
			break;
		}

		// 入力取得
		cout << "a: left, s: right, w: up, z: down. command?" << endl; // 操作説明
		char input;
		cin >> input;
		// 更新
		update(state, input, STAGE_WIDTH, STAGE_HEIGHT);

	}

	// 祝いのメッセージ
	cout << "Conguratulatin's! you win!" << endl;

	// 後始末
	delete[] state; // 配列初期化したため
	state = 0;

	return (0);
}
