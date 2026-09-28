#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class ShareCRPPopup : public Popup<int> {
protected:
    TextInput* m_inputField = nullptr;
    EventListener<web::WebTask> m_webListener;
    int m_accountID;

    bool setup(int accountID) override;
    void onSubmit(CCObject*);
public:
    static ShareCRPPopup* create(int accountID);
};
