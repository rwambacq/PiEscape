#include "UI.h"

UIModel::UIModel() {

}

UIModel::~UIModel() {

}

UIView::UIView() {

}

UIView::~UIView() {

}

UIController::UIController() {

}

UIController::~UIController() {

}

void UIModel::setTime(uint64_t time) {
    this->time = time;
}

uint64_t UIModel::getTime() const {
    return time;
}
