#include "ShareCRPPopup.hpp"

ShareCRPPopup* ShareCRPPopup::create(int accountID) {
    auto ret = new ShareCRPPopup();
    if (ret && ret->initAnchored(360.f, 180.f, accountID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ShareCRPPopup::setup(int accountID) {
    m_accountID = accountID;

    setTitle("Share Creator Points.");

    m_inputField = TextInput::create(280.f, 50.f, "Amount of CPs", "chatFont.fnt");
    m_inputField->setPosition({m_size.width / 2, m_size.height / 2 + 10.f});
    m_mainLayer->addChild(m_inputField);

    auto cancelBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Cancel", "goldFont.fnt", "btn_red_01.png", 0.8f),
        this,
        menu_selector(ShareCRPPopup::onCancel)
    );

    auto submitBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Submit", "goldFont.fnt", "btn_green_01.png", 0.8f),
        this,
        menu_selector(ShareCRPPopup::onSubmit)
    );

    auto menu = CCMenu::create();
    menu->addChild(cancelBtn);
    menu->addChild(submitBtn);
    
    menu->alignItemsHorizontallyWithPadding(40.f);
    menu->setPosition({m_size.width / 2, 35.f});
    m_mainLayer->addChild(menu);

    return true;
}

void ShareCRPPopup::onCancel(CCObject*) {
    this->onClose(nullptr);
}

void ShareCRPPopup::onSubmit(CCObject*) {
    std::string text = m_inputField->getString();
    if (text.empty()) {
        FLAlertLayer::create("Error", "Please enter an amount of CPs.", "OK")->show();
        return;
    }

    for (char c : text) {
        if (!std::isdigit(c)) {
            FLAlertLayer::create("Error", "Please use only valid numbers.", "OK")->show();
            return;
        }
    }

    int amount = std::stoi(text);

    // Dynamically grab the current GDPS base URL
    std::string gdpsBase = GameManager::sharedState()->m_gameServerURL;
    
    if (!gdpsBase.empty() && gdpsBase.back() == '/') {
        gdpsBase.pop_back();
    }

    std::string serverURL = gdpsBase + "/addcp?accountID=" + std::to_string(m_accountID) + "&cp=" + std::to_string(amount);
    std::string password = Mod::get()->getSettingValue<std::string>("password");

    web::WebRequest()
        .header("Authorization", password)
        .post(serverURL)
        .then([this](web::WebResponse* response) {
            if (response->ok()) {
                FLAlertLayer::create("Success", "Creator points sent successfully!", "OK")->show();
                this->onClose(nullptr);
            } else {
                FLAlertLayer::create("Error", "Failed to connect to the server or unauthorized.", "OK")->show();
            }
        });
}
