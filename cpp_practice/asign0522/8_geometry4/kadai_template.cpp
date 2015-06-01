/* kadai_template.cpp */
/***************************************
   ɬ�פʲս��Ŭ���ѹ���, ������Ԥ���.
     kadai_template.cpp �ι�¤
   |   include ��ʬ                     |
   |   class �����ʬ                  |
   |   �ץ�ȥ����������ʬ    |
   |   main �ؿ�                          |
   |   sub �ؿ�                             |
   +--------------------------------------+
                             2013ǯ5��4�� �丶
****************************************/

/* include ��ʬ */
//��������

/* class �����ʬ */
class SKEL{
private:
  // �����ѿ�
  std::vector<tutor::PPoint3d> _vertices;   // ĺ��
  std::vector<Polyline> _lines;         // �ޤ���
  cv::Matx44d _matrix;                    // �Ѵ�����
  
  // private���֤����Ȥǥǥե���ȥ��󥹥ȥ饯���θƤӽФ���ػߤǤ���
  SKEL(); // �ǥե���ȥ��󥹥ȥ饯��

public:

  // .skel�ե�������ɤ߹���
  explicit SKEL(const std::string &fn){    // ���󥹥ȥ饯��
    //��������
  }

  // ���å�
  const std::vector<tutor::PPoint3d>& vertices() const{
    return _vertices;
  }
  const std::vector<Polyline>&  polylines() const{
    return _lines;
  }
    
  // ���å�
  void set_matrix(cv::Matx44d matrix){
    _matrix = matrix;
  }
    
  // ��ɸ�Ѵ��μ¹Դؿ�
  // ĺ����ɸ_vertices���Ѵ�����_matrix�򤫤��ƹ�������
  void transformation();
  
};

/* �ץ�ȥ�������� */
// ��ɸ��ʿ�����
tutor::PPoint2d parallel_projection(
				    const tutor::PPoint3d &p);

// ��ɸ��Ʃ�����
tutor::PPoint2d perspective_projection(
				       const tutor::PPoint3d &p);

// ʿ����Ƥ��Ѥ���SKEL������
void draw_skel_parallel(
			cv::Mat &img, const SKEL &s);

// Ʃ����Ƥ��Ѥ���SKEL������
void draw_skel_perspective(
			   cv::Mat &img, const SKEL &s);

// ʿ�԰�ư����
cv::Matx44d Translation(
			double tx, double ty, double tz);

// X�����β�ž����
cv::Matx44d RotationX(double angle);
//Y����ꡤZ������Ʊ�ͤ˲�ž�����������뤳�ȡ�

//�����, 
//��ɸ���Ѵ�����򤫤���ĺ������򹹿�����ؿ�\verb|SKEL::transformation|��������ʤ�����

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* main �ؿ� */
int main()
{
  // ��������
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* sub �ؿ� */

// ��ɸ��ʿ�����
tutor::PPoint2d parallel_projection(
				    const tutor::PPoint3d &p)
{
  //��������
}

// ��ɸ��Ʃ�����
tutor::PPoint2d perspective_projection(
				       const tutor::PPoint3d &p)
{
  // ��������
}

// ʿ����Ƥ��Ѥ���SKEL������
void draw_skel_parallel(
			cv::Mat &img, const SKEL &s)
{
  // ��������
}

// Ʃ����Ƥ��Ѥ���SKEL������
void draw_skel_perspective(
			   cv::Mat &img, const SKEL &s)
{
  // ��������
}

// ʿ�԰�ư����
cv::Matx44d Translation(
			double tx, double ty, double tz)
{
  //��������
}

// X�����β�ž����
cv::Matx44d RotationX(double angle)
{
  //��������
}
//ɬ�פʴؿ��Ͽ����˽�­������.
//Y����ꡤZ������Ʊ�ͤ˲�ž�����������뤳�ȡ�
