#ifndef __DEFERRED_SHADING_H__
#define __DEFERRED_SHADING_H__

// reference from  https://bitbucket.org/kornerr/osg-deferred-shading/downloads/ by  Michael Kapelko

#include <osg/Camera>
#include <osg/Group>
#include <osg/Texture2D>
#include <osg/TextureRectangle>
#include <osg/LightSource>
#include <osgShadow/ShadowedScene>

 #define  USE_EFFECT_COMPOSITOR
   
    struct Pipeline
    {

        int _textureSize; // �����С
        osg::ref_ptr<osg::Group>  _graph;
        osg::Texture* _pass1Shadows;        // ��Ӱͨ��
        osg::Texture* _pass2Colors;         // ��ɫͨ��������diffuse
        osg::Texture* _pass2Normals;        // ����
        osg::Texture* _pass2Positions;      // λ��
        osg::Texture* _pass3Final;          // �ϳ�ͨ��
    };
    class DeferredShading
    {
  
    public:

        /* ����Float���͵���������
        */
        static osg::TextureRectangle* createFloatTextureRectangle(int textureSize);
    
        /* ����Hud���
        */
        static osg::Camera* createHudCamera(double left = 0, double right = 1.0,
                                        double bottom = 0, double top = 1.0 );
        /* �����ƹ�
        */
        static osg::ref_ptr<osg::LightSource> createLight(const osg::Vec3& pos);

        /* ������Ⱦ�ϳ���
        @scene���������ڵ�
        @shadowScene�� ��Ӱ�ڵ�
        @lightPos���ƹ�λ��
        */
        static Pipeline createPipelineEffectCompositor(osg::ref_ptr<osg::Group> scene,
                        osg::ref_ptr<osgShadow::ShadowedScene> shadowScene,
                        const osg::Vec3 lightPos);

        /* ������Ⱦƽ̨
        @scene���������ڵ�
        @shadowScene�� ��Ӱ�ڵ�
        @lightPos���ƹ�λ��
        */
        //Pipeline createPipelinePlainOSG(osg::ref_ptr<osg::Group> scene, osg::ref_ptr<osgShadow::ShadowedScene> shadowScene, const osg::Vec3 lightPos)
       static  Pipeline createPipelinePlainOSG(osg::ref_ptr<osg::Group> scene,
                        osg::ref_ptr<osgShadow::ShadowedScene> shadowScene,
                        const osg::Vec3 lightPos);
    
        /* ����RTT(Render to Target) ���
        */
        static osg::Camera* createRTTCamera(osg::Camera::BufferComponent buffer,
                        osg::Texture* tex,
                        bool isAbsolute = false);
   
        /* �����ı�������
        */
        static osg::Geode* createScreenQuad(float width, float height,
                        float scale = 1, osg::Vec3 corner = osg::Vec3());
        /* ��������
        */
        static osg::Texture2D * createTexture(const std::string& fileName);

        /* ����������ʾ����Ļ������
        */
        static osg::ref_ptr<osg::Camera> createTextureDisplayQuad(const osg::Vec3& pos,
                        osg::StateAttribute* tex, float scale,
                        float width = 0.3, float height = 0.2);

        /* ������ɫ������
        */
        static osg::ref_ptr<osg::StateSet> setShaderProgram(osg::ref_ptr<osg::Camera> pass,
                        std::string vert, std::string frag);

    };

 

#endif // __DEFERRED_SHADING_H__