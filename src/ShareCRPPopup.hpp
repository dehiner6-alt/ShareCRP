#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ShareCRPPopup : public Popup<int> {
protected:
    int m_targetLevelID;
    TextInput* m_pointsInput;

    bool setup(int levelID) override;
    void onSubmitButton(CCObject* sender);

public:
    static ShareCRPPopup* create(int levelID);
};
