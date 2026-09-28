#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ShareCRPPopup : public Popup {
protected:
    int m_targetAccountID = 0;
    CCLabelBMFont *m_crpLabel = nullptr;
    int m_selectedCRP = 0;

    bool init(int accountID);

    void onDecrease(CCObject *);
    void onIncrease(CCObject *);
    void onCancel(CCObject *);
    void onSubmit(CCObject *);

    void updateCRPVisuals(int crp);

public:
    static ShareCRPPopup *create(int accountID);
};
