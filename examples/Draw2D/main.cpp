#include <iostream>
#include <osg/Node>
#include <osg/Camera>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/Referenced>

using namespace  osg;


class HudCallback :public osg::NodeCallback
{

public:
    HudCallback(osgViewer::Viewer* viewer):m_viewer(viewer)
    {
    }
       virtual void operator()(Node* node, NodeVisitor* nv)
       {
           /*
           ��ģ����ת����⣬���������漰�������ת��ôת����ģ�͵���ת�����Բ�������˼·��
           1.��ȡ��ǰ���������ͼ����Ȼ�����õ�hudcamera����ͼ�����У�ģ�;��������ķŶ�λ�ü��ɡ�
           2.ͨ�����ºϳ�ģ�͵ı任����ģ������Ҫ��ʾ����Ļ����Ҫ���� v * modelViewMatrx * projection * windowMatrix;
             �򣬿��Զ�hudcamera���úó�ʼλ��(��ʼλ��Ҳ�����ַ�����a������ΪOpenGL��Ĭ�������̬��b������Ϊosg�е������̬��+z���ϣ���
             Ȼ���ڸ��»ص��У���ģ�;���͵�ǰ����ı任������кϳɡ�

             �������ֺϳɷֱ�˵����
             a������ΪOpenGL��Ĭ�������̬:
                hudCamera->setViewMatrixAsLookAt(osg::Vec3(0,0,1),osg::Vec3(0,0,0),osg::Vec3(0,1,0)); // opengl Ĭ�����λ��
             
                ���ڸ��»ص��У���Ҫ�����ϳɣ�
                osg::Camera* camera = m_viewer->getCamera();
                osg::Vec3 translate = pTM->getMatrix().getTrans();
                osg::Vec3 scale = pTM->getMatrix().getScale();
                osg::Matrix mv = camera->getViewMatrix();

                osg::Matrix inv_mv = camera->getInverseViewMatrix();  // ����ͼ����ת��Ϊ�����ı任����
                osg::Quat inv_q = inv_mv.getRotate();  
                osg::Quat q = mv.getRotate();

                mv.setTrans(translate);
                pTM->setMatrix(osg::Matrix::scale(scale) *  mv);

            b: ����Ϊosg�е������̬��
                
                 hudCamera->setViewMatrixAsLookAt(osg::Vec3(0,-1,0),osg::Vec3(0,0,0),osg::Vec3(0,0,1)); // osg Ĭ�����λ��
                 
                 ���ڸ��»ص��У���Ҫ�����ϳɣ�
                 osg::Camera* camera = m_viewer->getCamera();
                 osg::Vec3 translate = pTM->getMatrix().getTrans();
                 osg::Vec3 scale = pTM->getMatrix().getScale();
                 osg::Matrix mv = camera->getViewMatrix();
                 osg::Quat q = mv.getRotate();

                 // ģ�͵�ǰ��������ת����ϵ�����������������ϵ������x����ʱ˳ʱ��ת90��
                 osg::Quat dq(osg::DegreesToRadians(90.0f),osg::Vec3(1.0f,0.0f,0.0f));

                 mv.setTrans(translate);
                 // ����dq�����ڵ�����opengl����ϵ-��osg����ϵ�任������Ϊ���յľ������Ϊ: v * scale * mv * (dq * osgMV) * projection * windowMatrix
                 // dq * osgMV == OpenGL_MV
                 pTM->setMatrix(osg::Matrix::scale(scale) *  mv * osg::Matrix::rotate(dq)); 


                 ����ֻ�����ۣ�b�����ڱ�д����ʱ����������������˸��ĳһʱ������ȷ�ģ�ĳһʱ�̴��ڴ���λ�ã��������䣬������Ϊ���۽������ȷ�ģ�Ӧ�����Լ�д�Ĵ��������⡣
           */
               osg::MatrixTransform* pTM = dynamic_cast<osg::MatrixTransform*>(node);
               if(pTM)
               {
                   osg::Camera* camera = m_viewer->getCamera();
                   osg::Vec3 translate = pTM->getMatrix().getTrans();
                   osg::Vec3 scale = pTM->getMatrix().getScale();
                   osg::Matrix mv = camera->getViewMatrix();
            
                   osg::Matrix inv_mv = camera->getInverseViewMatrix();  // ����ͼ����ת��Ϊ�����ı任����
                   osg::Quat inv_q = inv_mv.getRotate();  
                   osg::Quat q = mv.getRotate();
               
                   // ģ�͵�ǰ��������ת����ϵ�����������������ϵ������x����ʱ˳ʱ��ת90��
                   osg::Quat dq(osg::DegreesToRadians(90.0f),osg::Vec3(1.0f,0.0f,0.0f));
                  // pTM->setMatrix(osg::Matrix::scale(scale)* osg::Matrix::rotate( dq * q ) * osg::Matrix::translate(translate));
                    static osg::Matrix mm = osg::Matrix::rotate(dq);
                   mv.setTrans(translate);
                   pTM->setMatrix(osg::Matrix::scale(scale) *  mv /** mm*/);
               }
       }
private:
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
};

// HUDAxis �ο��ˣ���ͼ����ũ�� �Ĵ���

//--------------------- 
//    ���ߣ�ͼ����ũ 
//    ��Դ��CSDN 
//ԭ�ģ�https://blog.csdn.net/wang15061955806/article/details/49617055 
//��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�
//
class HUDAxis:public Camera
{
public:
    HUDAxis();
    HUDAxis(HUDAxis const& copy, CopyOp copyOp = CopyOp::SHALLOW_COPY);
    META_Node(osg, HUDAxis);
    inline void setMainCamera(Camera* camera){ _mainCamera = camera;}
    virtual void traverse(NodeVisitor& nv);
protected:
    virtual ~HUDAxis();
    observer_ptr<Camera> _mainCamera;
};
/////////////////////////////////////cpp///////////////////////////////////////////////////
HUDAxis::HUDAxis()
{
    //��������ֱ�Ӷ�ȡaxes.osgt;
    // this->addChild(osgDB::readNodeFile("axes.osgt"));
}
HUDAxis::HUDAxis(HUDAxis const& copy, CopyOp copyOp /* = CopyOp::SHALLOW_COPY */):Camera(copy, copyOp),
    _mainCamera(copy._mainCamera)
{
}
void HUDAxis::traverse(NodeVisitor& nv)
{
    double fovy, aspectRatio, vNear, vFar;
    _mainCamera->getProjectionMatrixAsPerspective(fovy, aspectRatio, vNear, vFar);
    //this->setProjectionMatrixAsOrtho(-10.0*aspectRatio, 10.0*aspectRatio, -10.0, 10.0, 2.0, -2.0); //����ͶӰ����ʹ���Ų���Ч��
    this->setProjectionMatrixAsOrtho2D(-10.0*aspectRatio, 10.0*aspectRatio, -10.0, 10.0);
    Vec3 trans(8.5*aspectRatio, -8.5, -8.0);
    if(_mainCamera.valid() && nv.getVisitorType() == NodeVisitor::CULL_VISITOR)
    {
        Matrix matrix = _mainCamera->getViewMatrix();//�ı���ͼ�������ƶ�λ�ù̶�
        matrix.setTrans(trans);
        this->setViewMatrix(matrix);
    }//if
    osg::Camera::traverse(nv);
}
HUDAxis::~HUDAxis()
{
}


osg::Camera* createHudCamera(osgViewer::Viewer* viewer,int width = 1024, int height = 1024)
{
    osg::ref_ptr<osg::Camera> hudCamera = new osg::Camera;
    hudCamera->setProjectionMatrixAsOrtho(0,width,0,height,1,100);
    hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
    hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

    osg::Node* axes = osgDB::readNodeFile("axes.osgt");
    osg::MatrixTransform* pTM = new osg::MatrixTransform;
    pTM->addChild(axes);
    axes->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    pTM->setMatrix(osg::Matrix::scale(osg::Vec3(width/12,width/12,width/12))*osg::Matrix::translate(osg::Vec3(width/20,width/20,1)));
    pTM->setUpdateCallback(new HudCallback(viewer));
    hudCamera->addChild(pTM);
    //hudCamera->setViewMatrixAsLookAt(osg::Vec3(0,-1,0),osg::Vec3(0,0,0),osg::Vec3(0,0,1)); // osg Ĭ�����λ��
    hudCamera->setViewMatrixAsLookAt(osg::Vec3(0,0,1),osg::Vec3(0,0,0),osg::Vec3(0,1,0)); // opengl Ĭ�����λ��

    return hudCamera.release();
}
void main()
{
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(0,0,800,600);

    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<HUDAxis> hudAxes = new HUDAxis;
    osg::ref_ptr<osg::Node> axes = osgDB::readNodeFile("axes.osgt");
    osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
   
    // ʹ��hudAxes����Ƶ�����ϵ
    hudAxes->addChild(axes);
    hudAxes->setMainCamera(viewer.getCamera());
    hudAxes->setRenderOrder(osg::Camera::POST_RENDER);
    hudAxes->setClearMask(GL_DEPTH_BUFFER_BIT);
    hudAxes->setAllowEventFocus(false);
    hudAxes->setReferenceFrame(Transform::ABSOLUTE_RF);
    root->addChild(hudAxes);
 
    // ʹ�ûص���ʽ����������ϵ
    root->addChild(createHudCamera(&viewer));
    root->addChild(cow);

    viewer.setSceneData(root);
    viewer.realize();
    viewer.run();
}