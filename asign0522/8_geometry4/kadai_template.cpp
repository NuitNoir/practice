/* kadai_template.cpp */
/***************************************
   必要な箇所は適宜変更し, 課題を遂行せよ.
     kadai_template.cpp の構造
   |   include 部分                     |
   |   class 宣言部分                  |
   |   プロトタイプ宣言部分    |
   |   main 関数                          |
   |   sub 関数                             |
   +--------------------------------------+
                             2013年5月4日 延原
****************************************/

/* include 部分 */
//何かかく

/* class 宣言部分 */
class SKEL{
private:
  // メンバ変数
  std::vector<tutor::PPoint3d> _vertices;   // 頂点
  std::vector<Polyline> _lines;         // 折れ線
  cv::Matx44d _matrix;                    // 変換行列
  
  // privateに置くことでデフォルトコンストラクタの呼び出しを禁止できる
  SKEL(); // デフォルトコンストラクタ

public:

  // .skelファイルの読み込み
  explicit SKEL(const std::string &fn){    // コンストラクタ
    //何かかく
  }

  // ゲッタ
  const std::vector<tutor::PPoint3d>& vertices() const{
    return _vertices;
  }
  const std::vector<Polyline>&  polylines() const{
    return _lines;
  }
    
  // セッタ
  void set_matrix(cv::Matx44d matrix){
    _matrix = matrix;
  }
    
  // 座標変換の実行関数
  // 頂点座標_verticesを変換行列_matrixをかけて更新する
  void transformation();
  
};

/* プロトタイプ宣言 */
// 座標の平行投影
tutor::PPoint2d parallel_projection(
				    const tutor::PPoint3d &p);

// 座標の透視投影
tutor::PPoint2d perspective_projection(
				       const tutor::PPoint3d &p);

// 平行投影を用いてSKELを描画
void draw_skel_parallel(
			cv::Mat &img, const SKEL &s);

// 透視投影を用いてSKELを描画
void draw_skel_perspective(
			   cv::Mat &img, const SKEL &s);

// 平行移動行列
cv::Matx44d Translation(
			double tx, double ty, double tz);

// X軸回りの回転行列
cv::Matx44d RotationX(double angle);
//Y軸回り，Z軸回りも同様に回転行列を作成すること．

//さらに, 
//座標に変換行列をかけて頂点情報を更新する関数\verb|SKEL::transformation|を作成しなさい．

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* main 関数 */
int main()
{
  // 何かかく
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* sub 関数 */

// 座標の平行投影
tutor::PPoint2d parallel_projection(
				    const tutor::PPoint3d &p)
{
  //何かかく
}

// 座標の透視投影
tutor::PPoint2d perspective_projection(
				       const tutor::PPoint3d &p)
{
  // 何かかく
}

// 平行投影を用いてSKELを描画
void draw_skel_parallel(
			cv::Mat &img, const SKEL &s)
{
  // 何かかく
}

// 透視投影を用いてSKELを描画
void draw_skel_perspective(
			   cv::Mat &img, const SKEL &s)
{
  // 何かかく
}

// 平行移動行列
cv::Matx44d Translation(
			double tx, double ty, double tz)
{
  //何かかく
}

// X軸回りの回転行列
cv::Matx44d RotationX(double angle)
{
  //何かかく
}
//必要な関数は新たに書き足すこと.
//Y軸回り，Z軸回りも同様に回転行列を作成すること．
