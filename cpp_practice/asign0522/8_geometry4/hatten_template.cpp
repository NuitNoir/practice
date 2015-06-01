/* kadai_template.cpp */
/***************************************
   必要な箇所は適宜変更し, 課題を遂行せよ.
     kadai_template.cpp の構造
   |   include 部分                     |
   |   main 関数                          |
   +--------------------------------------+
                             2013年5月4日 延原
****************************************/

/* include 部分 */
//何かかく

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* main 関数 */
int main()
{
// サイズ・fps情報取得
int width =  500;//適宜設定
int height = 500;//適宜設定
int fps	  =  10;//適宜設定

cv::Size cap_size(width,height);

// 出力動画の準備
cv::VideoWriter writer("tree_out.avi", CV_FOURCC('X','V','I','D'), fps, cap_size, ture);

// このwhileの中を書き換える
while(1) { //ループの回数も適宜変える
   cv::Mat frame = cv::imread("画像ファイル名", 1);
   if( frame.empty() == 0){
     return -1;
   }
   writer << frame;	// 動画書き込み
  }
 }
}
