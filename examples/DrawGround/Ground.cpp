#include <osgViewer/viewer>

#include <osg/PrimitiveSet>
#include <osg/StateSet>
#include <osg/LineWidth>
#include <osg/PolygonMode>
#include <osg/StateAttribute>
#include <osg/PrimitiveRestartIndex>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/array>
#include <osg/Hint>
#include <osg/BlendColor>
#include <osg/BlendEquation>
#include <osg/BlendFunc>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgGA/TrackballManipulator>
#include <osgGA/GUIEventHandler>

#include <iostream>

// ʹ��ͼԪ������������
// dimessionX: X������ϸ�ֶ��ٸ���
// dimessionY: Y������ϸ�ֶ��ٸ���
// width:  X�����򳤶�
// height: Y�����ܳ���
// bCoordinateAllMesh ������ͼƬӳ�䵽mesh��(true)������ӳ�䵽ÿ����Ԫ��(false)
// bFill���Ƿ�ʹ�ö������䣨true),lineģʽ��false)
osg::Node* CreateGround(int dimensionX = 5, int dimensionY= 5, float width = 80, float height = 80,bool bCoordinateAllMesh = true,bool bFill = true)
{
    // ������ƣ�����������л��ƣ�ÿһ��Ϊһ��Quad_Strip,Ȼ������������Ʊ�ʶ����
	float dx = width / (dimensionX - 1); // x����ÿ���ߵĳ���
	float dy = height / (dimensionY - 1); // y����ÿ���ߵĳ���
	float startX = - width / 2;
	float startY = - height / 2;
    float endX =  - startX;
    float endY = - startY;

	osg::ref_ptr<osg::Geode> node = new osg::Geode;
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> coords = new osg::Vec2Array;
  
    // �����������
    // 2*dimessionX * dimessionY ��ʶ���ж��㶼�ظ�ʹ��
    // - dimensionY * 2  ���� ����ʼ�ͽ����������߽�
    // (dimessionX -2) ������������
    int indicesSize = 2 * dimensionX * dimensionY - dimensionY * 2 + (dimensionX - 2);
	osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(osg::PrimitiveSet::QUAD_STRIP, indicesSize);
	unsigned int restartFlag = 0xFFFF;
	// ʹ��ͼԪ������������
	/*	
	  X1 Y1  ----  X2,Y1
			|	|
			|	|
	 X1,Y2	----- X2,Y2
	*/

    // calculate vertices 
    for(int i = 0; i < dimensionX; ++i)
    {
        for(int j = 0; j < dimensionY; ++j)
        {
            float x1 = startX + dx * i;
            float y1 = startY + dy * j;
            osg::Vec3 pos1(x1, y1, 0);
            vertices->push_back(pos1);
            
           
            if(bCoordinateAllMesh)
            {
                osg::Vec2 coord(float(i)/(dimensionX - 1),float(j)/(dimensionY - 1));
                coords->push_back(coord);
            }
            else
            {
                osg::Vec2 coord(i,j);
                coords->push_back(coord);
            }
        }
    }
    vertices->push_back(osg::Vec3(endX,endY,0));// ��洢һ�����㣬������ʹ��ͼԪ�����������¼����Χ�з���Խ�硣
    // calculate indices
    // fist quad strip 
    for (int j=0;j<dimensionY;j++)
    {
        (*indices)[j*2] = j;
        (*indices)[j*2+1] = dimensionY+j;
        
        // ������������
        if(!bCoordinateAllMesh)
        {
             
           
        }
      
        std::cout <<  j*2 <<"," <<  j*2+1 << "-->"<< j <<"," << dimensionY +j <<std::endl;
    }
    std::cout << std::endl;

	unsigned int restartIndex = 0;
	for (int i = 1; i < dimensionX - 1 ; ++i)  
	{
        restartIndex += 2 * dimensionY;
     // �������������õı�ʶ��, �˴�������ô���ã���������Ϊһ������ĵ㣬���磬���һ������,��Ϊosg�ڼ����Χ��ʱ��
     // ��ͨ������ȥ���ʶ���,��Ϊ0xFFFF,����ַ���Խ�磬����ʹ�����һ������
    // (*indices)[restartIndex] = restartFlag;
       (*indices)[restartIndex] = dimensionX * dimensionY;
        restartIndex++; // �洢��һ����Ч������
		for (int j = 0; j < dimensionY; ++j) // ÿ�λ���һ�е�quad_strip
		{
		 
            (*indices)[restartIndex + j*2] = dimensionY*i + j; // ���㵱ǰ��������
            (*indices)[restartIndex + j*2+1] = dimensionY*(i+1) + j;
          
			std::cout << restartIndex + j*2 <<"," << restartIndex + j*2+1 << "-->"<< dimensionY*i + j <<"," << dimensionY*(i+1) + j<<std::endl;
		}
        std::cout << std::endl;
	}
 
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	geometry->addPrimitiveSet(indices);
    //geometry->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP,0,dimensionX * dimensionY));
	geometry->setVertexArray(vertices);
	geometry->setTexCoordArray(0, coords);

	normals->push_back(osg::Vec3d(0, 0, 1));
	geometry->setNormalArray(normals, osg::Array::BIND_PER_PRIMITIVE_SET);

	osg::StateSet* stateset = new osg::StateSet();
	osg::PolygonMode* pm = new osg::PolygonMode(osg::PolygonMode::Face::FRONT_AND_BACK, osg::PolygonMode::Mode::LINE);
	stateset->setAttributeAndModes(pm, osg::StateAttribute::ON);
	stateset->setAttributeAndModes(new osg::LineWidth(1.5f), osg::StateAttribute::ON);

    if(bFill)
    {
	
	    stateset->setMode(GL_POLYGON_SMOOTH, osg::StateAttribute::ON);
        stateset->setAttributeAndModes(new osg::Hint(GL_POLYGON_SMOOTH_HINT, GL_NICEST), osg::StateAttribute::ON);
        // �����ںϲ����ÿ���ݣ�������������ߣ���������Ļ�ں�
        // stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
        // stateset->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA),osg::StateAttribute::ON);
    }
    else
    {
        stateset->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
        stateset->setAttributeAndModes(new osg::Hint(GL_LINE_SMOOTH_HINT, GL_NICEST), osg::StateAttribute::ON);
        stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
        stateset->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA),osg::StateAttribute::ON);
    }

    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    stateset->setAttributeAndModes(new osg::PrimitiveRestartIndex(dimensionX * dimensionY), osg::StateAttribute::ON);
    stateset->setMode(GL_PRIMITIVE_RESTART, osg::StateAttribute::ON);

	//std::string fileName = "Images/primitives.gif";
    std::string fileName = "E:\\project\\osg\\OSG.ms\\OsgEditor\\Data\\textures\\ground3.jpg";
	osg::Image* image = osgDB::readImageFile(fileName);
	osg::Texture2D* tex = new osg::Texture2D;
    	/* mipmap ʹ��˵��
	����GL_NEAREST_MIPMAP_NEAAREST���кܺõ����ܣ�Ҳ�ܹ������˸�����⣬�����Ӿ�Ч���ϻ�Ƚϲ
	����GL_LINEAR_MIPMAP_NEAREST��������Ϸ���٣�ʹ���������ϸߵ����Թ��ˣ��Ϳ��ٵ�ѡ��ķ�ʽ(���ڽ���ʽ����
	ʹ�����ڽ��ķ�ʽ��Ϊmipmapѡ������Ч����Ȼ�����������⡣��ĳһ���Ƕ�ȥ�����������Կ�����������һ��mip��
	����һ��mip���ת�䡣GL_LINEAR_MIPMAP_LINEAR��GL_NEAREST_MIPMAP_LINEAR��������mip��֮��ִ��һЩ��������Բ�ֵ��
	��������ͬ��֮��ı任�ۼ�����Ҳ��ҪһЩ��������ܿ�����GL_LINEAR_MIPMAP_LINEAR������ߵľ��ȡ�
	https://blog.csdn.net/yiting52/article/details/52401133
	*/
	// osg���ô���MipMap���˲�ѡ����ģʽ������û��mipmap�����������Զ�����mipmap
    tex->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_NEAREST);
    tex->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR_MIPMAP_NEAREST);
    tex->setWrap(osg::Texture::WRAP_S,osg::Texture::REPEAT);
    tex->setWrap(osg::Texture::WRAP_T,osg::Texture::REPEAT);
   

	tex->setImage(image);
	stateset->setTextureAttributeAndModes(0, tex,osg::StateAttribute::ON);
	geometry->setStateSet(stateset);
	geometry->setUseVertexBufferObjects(true);
 
	node->addDrawable(geometry);

	return node.release();
}

class keyBoardHandler :public osgGA::GUIEventHandler
{
public:
    keyBoardHandler(osg::Group* groundRoot):m_ground(groundRoot),m_dimessionX(2),m_dimessionY(2),m_bCoordinateAllMesh(false)
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*) 
    {

        if(ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
        {
            if(ea.getKey() == osgGA::GUIEventAdapter::KEY_W)
            {
                osg::Geode* geode = m_ground->getChild(0)->asGeode();
                if(geode)
                {
                    osg::Geometry* geom = geode->getDrawable(0)->asGeometry();
                    if(geom)
                    {
                        osg::StateAttribute* attrib = geom->getOrCreateStateSet()->getAttribute(osg::StateAttribute::POLYGONMODE);
                        osg::PolygonMode* pm = dynamic_cast<osg::PolygonMode*>(attrib);
                        if(pm)
                        {
                            osg::PolygonMode::Mode mode =  pm->getMode(osg::PolygonMode::Face::FRONT_AND_BACK);
                            if(mode == osg::PolygonMode::Mode::LINE)
                            {
                                pm->setMode(osg::PolygonMode::Face::FRONT_AND_BACK, osg::PolygonMode::Mode::FILL);
                            }
                            else
                            {
                                pm->setMode(osg::PolygonMode::Face::FRONT_AND_BACK, osg::PolygonMode::Mode::LINE);
                            }
                        }
                    }
                }
                return true;
            }

            else if(ea.getKey() == osgGA::GUIEventAdapter::KEY_Up)
            {
                m_dimessionY++;
                ChangeLines(m_dimessionX,m_dimessionY,m_bCoordinateAllMesh);
           
                return true;
            }
            else if(ea.getKey() == osgGA::GUIEventAdapter::KEY_Down)
            {
                m_dimessionY--;
                ChangeLines(m_dimessionX,m_dimessionY,m_bCoordinateAllMesh);
          
                return true;

            }
            else if(ea.getKey() == osgGA::GUIEventAdapter::KEY_Left)
            {
                m_dimessionX++;
                ChangeLines(m_dimessionX,m_dimessionY,m_bCoordinateAllMesh);
           
                return true;
            }
            else if(ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)
            {
                m_dimessionX--;
                ChangeLines(m_dimessionX,m_dimessionY,m_bCoordinateAllMesh);
            }
            
            else if(ea.getKey() == osgGA::GUIEventAdapter::KEY_A)
            {
                m_bCoordinateAllMesh = !m_bCoordinateAllMesh;
                ChangeLines(m_dimessionX,m_dimessionY,m_bCoordinateAllMesh);
            }
        }
        return false;
    }

    void ChangeLines(int x, int y,bool bCoordinateAllMesh = false)
    {
        if(x <2) x = 2;
        if(y < 2) y  = 2;
        m_ground->removeChild(0,1);
        m_ground->addChild(CreateGround(x,y,80,80,bCoordinateAllMesh));
    }
private:
    osg::ref_ptr<osg::Group> m_ground;
    int m_dimessionX;
    int m_dimessionY;
    bool m_bCoordinateAllMesh;
};

void main()
{
    osgViewer::Viewer viewer;
    osg::Group* root = new osg::Group;
    osg::Node* pNode = osgDB::readNodeFile("cow.osg");
   
    root->addChild(pNode);
    osg::Group* ground = new osg::Group;
    osg::Node* groundMesh = CreateGround(13,3);
    ground->addChild(groundMesh);
    root->addChild(ground);

    viewer.setSceneData(root);
    viewer.setUpViewInWindow(10,10,800,600);
    //viewer.addEventHandler(new osgGA::TrackballManipulator);
    viewer.addEventHandler(new keyBoardHandler(ground));
    viewer.run();
}

// referenced:
// https://blog.csdn.net/yulinxx/article/details/77896541
// https://blog.csdn.net/qq_36665171/article/details/81459338