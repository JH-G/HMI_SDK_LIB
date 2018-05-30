#include "Home.h"
#include "Home/UI/HomeWindow.h"
#include "HMIFrameWork/HMIFrameWork.h"
Home* Home::m_pInst = NULL;
Home::Home()
{

    setAppType(AppType_App);
    setAppId(HOME_ID);
    InitViewFactory(HomeVFactory::Inst());
    setMain(reinterpret_cast<void*>(new HomeWindow()));
}

Home *Home::Inst()
{
    if(m_pInst == NULL)
    {
        m_pInst = new Home();
    }
    return m_pInst;
}

void Home::onAppShow(string appId, string viewId)
{
   connect(this,SIGNAL(SigAppShow(string,string)),this,SLOT(OnAppShow(string,string)),Qt::UniqueConnection);
   emit SigAppShow(appId,viewId);
}

void Home::onAppHide()
{
    connect(this,SIGNAL(SigAppHide()),this,SLOT(OnAppHide()),Qt::UniqueConnection);
    emit SigAppHide();
}
void Home::HomeClicked()
{
   HMIFrameWork::Inst()->AppShow(HOME_ID);
}

void Home::SDLAppsClicked()
{
    HMIFrameWork::Inst()->AppShow(SDLAPPS_ID);
}

void Home::HVACClicked()
{
    HMIFrameWork::Inst()->AppShow(HVAC_ID);
}

void Home::MediaClicked()
{
    HMIFrameWork::Inst()->AppShow(MEDIA_ID);
}

void Home::MessageClicked()
{
    HMIFrameWork::Inst()->AppShow(MESSAGE_ID);
}

void Home::NavClicked()
{
    HMIFrameWork::Inst()->AppShow(NAV_ID);
}

void Home::PhoneClicked()
{
    HMIFrameWork::Inst()->AppShow(PHONE_ID);
}

void Home::SettingsClicked()
{
    map<string,string> p;
    p.insert(make_pair("Show","SettingsMain"));
    HMIFrameWork::Inst()->Notify(HOME_ID,p);
}

void Home::WeatherClicked()
{
    HMIFrameWork::Inst()->AppShow(WEATHER_ID);
}
void Home::onNotify(string appId, map<string, string> parameter)
{
    connect(this,SIGNAL(SigNotify(string,map<string,string>)),this,SLOT(OnNotify(string,map<string,string>)),Qt::UniqueConnection);
    emit SigNotify(appId,parameter);
}

void Home::onReply(string appId, map<string, string> parameter)
{
    connect(this,SIGNAL(SigReply(string,map<string,string>)),this,SLOT(OnReply(string,map<string,string>)),Qt::UniqueConnection);
    emit SigNotify(appId,parameter);
}

void Home::OnAppShow(string appId, string viewId)
{
    INFO()<<"onAppShow" << QString::fromStdString(appId) << "viewid " <<QString::fromStdString(viewId);
    int state = getState();
    switch (state) {
    case AppStatus_Active:
    {
        if(viewId == "Main")
        {
            ViewForwardById(eViewId_Main);
        }else if(viewId == "SettingsMain")
        {
            ViewForwardById(eViewId_Settings_Main);
        }
        else if(viewId == "BootAnimation")
        {
            ViewForwardById(eViewId_BootAnimation);
        }
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

void Home::OnAppHide()
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

void Home::OnNotify(string appId, map<string, string> parameter)
{
    INFO("Home::onNotify appId=%s .",appId.c_str());

    map<string,string>::const_iterator it = parameter.find("AppClick");
    if(it!=parameter.end())
    {
        string type = it->second;
        if(HOME_ID == type)
        {
            HomeClicked();
        }else if(PHONE_ID == type)
        {
            PhoneClicked();
        }else if(SETTINGS_ID == type)
        {
            SettingsClicked();
        }else if(MEDIA_ID == type)
        {
            MediaClicked();
        }else if(HVAC_ID == type)
        {
            HVACClicked();
        }else if(WEATHER_ID == type)
        {
            WeatherClicked();
        }else if(SDLAPPS_ID == type)
        {
            SDLAppsClicked();
        }else if(MESSAGE_ID == type)
        {
            MessageClicked();
        }else if(NAV_ID == type)
        {
            NavClicked();
        }
    }
    it = parameter.find("QuickMove");
    if(it!=parameter.end())
    {
        string type = it->second;
        emit SigQuickMove(QString::fromStdString(type));
    }

    it = parameter.find("Show");
    if(it!=parameter.end())
    {

        string type = it->second;
        INFO()<<QString::fromStdString(type);

        if(type == "SettingsMain")
        {
            if(getState() == AppStatus_Active)
            {
                INFO()<<" SettingsMain";
                ViewForwardById(eViewId_Settings_Main);
            }else
            {
                HMIFrameWork::Inst()->AppShow(HOME_ID,"SettingsMain");
            }
        }
    }
    it = parameter.find("action");
    if(it!=parameter.end())
    {
        string type = it->second;
        if(type == "BootAnimationFinish")
        {
            ViewForwardById(eViewId_Main);
            HMIFrameWork::Inst()->AppShow(STATUSBAR_ID);
            HMIFrameWork::Inst()->AppShow(QUICKLANUCH_ID);
        }
    }
}

void Home::OnReply(string appId, map<string, string> parameter)
{

}
