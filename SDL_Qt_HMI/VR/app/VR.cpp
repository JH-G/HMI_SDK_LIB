#include "VR.h"
#include "VR/UI/VRWindow.h"
#include "VR/Factory/VRVFactory.h"
#include "HMIFrameWork/log_interface.h"

VoiceRecognition* VoiceRecognition::m_pInst = NULL;

VoiceRecognition::VoiceRecognition()
{
    setAppType(AppType_App);
    setAppId(VR_ID);
    InitViewFactory(VRVFactory::Inst());
    setMain(reinterpret_cast<void*>(new VRWindow()));
}

VoiceRecognition *VoiceRecognition::Inst()
{
    if(NULL == m_pInst)
    {
        m_pInst = new VoiceRecognition();
    }
    return m_pInst;
}

void VoiceRecognition::onAppShow(string appId, string viewId)
{
    connect(this,SIGNAL(SigAppShow(string,string)),this,SLOT(OnAppShow(string,string)),Qt::UniqueConnection);
    emit SigAppShow(appId,viewId);
}

void VoiceRecognition::onAppHide()
{
    connect(this,SIGNAL(SigAppHide()),this,SLOT(OnAppHide()),Qt::UniqueConnection);
    emit SigAppHide();
}

void VoiceRecognition::onNotify(string appId, map<string, string> parameter)
{
    connect(this,SIGNAL(SigNotify(string,map<string,string>)),this,SLOT(OnNotify(string,map<string,string>)),Qt::UniqueConnection);
    emit SigNotify(appId,parameter);
}

void VoiceRecognition::onReply(string appId, map<string, string> parameter)
{
    connect(this,SIGNAL(SigReply(string,map<string,string>)),this,SLOT(OnReply(string,map<string,string>)),Qt::UniqueConnection);
    emit SigReply(appId,parameter);
}

const std::vector<VRAppInfo> &VoiceRecognition::GetAppList() const
{
    return m_vVRAppList;
}

void VoiceRecognition::OnAppShow(string appId, string viewId)
{
    INFO("onAppShow:%s, viewId:%s", appId.c_str(), viewId.c_str());
    int state = getState();
    switch (state) {
    case AppStatus_Active:
    {
        ViewForwardById(eViewId_Main);
        QWidget* mainwin = reinterpret_cast<QWidget*>(getMain());
        mainwin->show();


    }
        break;
     case AppStatus_Inactive:

        break;
    default:
        break;
    }
}

void VoiceRecognition::OnAppHide()
{
    int state = getState();
    switch (state) {
    case AppStatus_Active:
    {



    }
        break;
     case AppStatus_Inactive:
    {
        QWidget* mainwin = reinterpret_cast<QWidget*>(getMain());
        mainwin->hide();
    }
        break;
    default:
        break;
    }
}

void VoiceRecognition::OnNotify(string appId, map<string, string> parameter)
{
    INFO("VoiceRecognition::onNotify appId=%s .",appId.c_str());

    {
        map<string, string>::iterator iter = parameter.find("VRAppList");
        if(iter != parameter.end())
        {
            m_vVRAppList.clear();

            string str = iter->second;
            int appCount = QString::fromStdString(str).toInt();

            for(int i = 0; i < appCount; ++i)
            {
                VRAppInfo appInfo;

                map<string, string>::iterator iter = parameter.find(QString("appId").append(QString::number(i)).toStdString());
                if(iter != parameter.end())
                {
                    appInfo.appId = QString::fromStdString(iter->second).toInt();
                }

                iter = parameter.find(QString("appName").append(QString::number(i)).toStdString());
                if(iter != parameter.end())
                {
                    appInfo.appName = iter->second;
                }

                iter = parameter.find(QString("appIcon").append(QString::number(i)).toStdString());
                if(iter != parameter.end())
                {
                    appInfo.appIcon = iter->second;
                }

                m_vVRAppList.push_back(appInfo);
            }

            emit SigUpdateAppList();
        }
    }
}

void VoiceRecognition::OnReply(string appId, map<string, string> parameter)
{

}
