#include "ShareCRPPopup.hpp"

bool ShareCRPPopup::init(int accountID) {
    if (!Popup::init(330.f, 170.f))
        return false;

    m_targetAccountID = accountID;

    this->setTitle("Share CRP");
    this->setID("share-crp-popup"_spr);

    /*
        LOWER CRP BUTTON LOGIC
    */
    auto *decreaseButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"),
        this,
        menu_selector(ShareCRPPopup::onDecrease)
    );
    decreaseButton->setRotation(-90.f);
    decreaseButton->setID("decrease-crp-button"_spr);
    m_buttonMenu->addChildAtPosition(decreaseButton, Anchor::Center, { -60.f, 0.f });

    /*
        CRP LABEL LOGIC
    */
    m_crpLabel = CCLabelBMFont::create("0", "bigFont.fnt");
    m_crpLabel->setID("crp-label"_spr);
    m_buttonMenu->addChildAtPosition(m_crpLabel, Anchor::Center, { 0.f, 0.f });

    /*
        INCREASE CRP BUTTON LOGIC
    */
    auto *increaseButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"),
        this,
        menu_selector(ShareCRPPopup::onIncrease)
    );
    increaseButton->setRotation(90.f);
    increaseButton->setID("increase-crp-button"_spr);
    m_buttonMenu->addChildAtPosition(increaseButton, Anchor::Center, { 60.f, 0.f });

    /*
        CANCEL BUTTON LOGIC
    */
    auto *cancelButton = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Cancel", "goldFont.fnt", "GJ_button_01.png"),
        this,
        menu_selector(ShareCRPPopup::onCancel)
    );
    cancelButton->setID("cancel-button"_spr);
    m_buttonMenu->addChildAtPosition(cancelButton, Anchor::Bottom, { -60.f, 25.f });
    
    /*
        SUBMIT BUTTON LOGIC
    */
    auto *submitButton = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Submit", "goldFont.fnt", "GJ_button_01.png"),
        this,
        menu_selector(ShareCRPPopup::onSubmit)
    );
    submitButton->setID("submit-button"_spr);
    m_buttonMenu->addChildAtPosition(submitButton, Anchor::Bottom, { 60.f, 25.f });

    return true;
}

void ShareCRPPopup::onDecrease(CCObject *) {
    if (m_selectedCRP <= 0)
        return;

    m_selectedCRP--;
    updateCRPVisuals(m_selectedCRP);
}

void ShareCRPPopup::onIncrease(CCObject *) {
    m_selectedCRP++;
    updateCRPVisuals(m_selectedCRP);
}

void ShareCRPPopup::onCancel(CCObject *) {
    this->onClose(nullptr);
}

void ShareCRPPopup::onSubmit(CCObject *) {
    log::debug("Enviando {} CRP para la cuenta ID: {}", m_selectedCRP, m_targetAccountID);

    FLAlertLayer::create("ShareCRP", fmt::format("¡Asignados {} CRP con éxito!", m_selectedCRP), "OK")->show();
    this->onClose(nullptr);
}

void ShareCRPPopup::updateCRPVisuals(int crp) {
    m_crpLabel->setString(std::to_string(crp).c_str());
}

ShareCRPPopup *ShareCRPPopup::create(int accountID) {
    auto ret = new ShareCRPPopup();
    if (ret && ret->init(accountID)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
