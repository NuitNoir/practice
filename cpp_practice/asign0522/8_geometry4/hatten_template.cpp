/* kadai_template.cpp */
/***************************************
   ɬ�פʲս��Ŭ���ѹ���, ������Ԥ���.
     kadai_template.cpp �ι�¤
   |   include ��ʬ                     |
   |   main �ؿ�                          |
   +--------------------------------------+
                             2013ǯ5��4�� �丶
****************************************/

/* include ��ʬ */
//��������

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* main �ؿ� */
int main()
{
// ��������fps�������
int width =  500;//Ŭ������
int height = 500;//Ŭ������
int fps	  =  10;//Ŭ������

cv::Size cap_size(width,height);

// ����ư��ν���
cv::VideoWriter writer("tree_out.avi", CV_FOURCC('X','V','I','D'), fps, cap_size, ture);

// ����while�����񤭴�����
while(1) { //�롼�פβ����Ŭ���Ѥ���
   cv::Mat frame = cv::imread("�����ե�����̾", 1);
   if( frame.empty() == 0){
     return -1;
   }
   writer << frame;	// ư��񤭹���
  }
 }
}