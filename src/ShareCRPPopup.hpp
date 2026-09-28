#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ShareCRPPopup : public Popup<int> {
protected:
    int m_accountID;
    TextInput* m_inputField;

    bool setup(int accountID) override;
    void onCancel(CCObject*);
    void onSubmit(CCObject*);

public:
    static ShareCRPPopup* create(int accountID);
};
