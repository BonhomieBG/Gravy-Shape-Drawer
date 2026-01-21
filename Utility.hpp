// File name: Util.hpp
// Description: This utility provides Alphabet checking and trimming whitespace and other general necessities.
// 
// Time Spent: About 1h
// Challenges: Finding a similar function to Java's trim() for removing white space (example: smoke , this will be enterted as smoke.) and provide a check for alphabetic words where is nessessary for the input to return valid.
//
// Revision History
// Date: By: Action
// 11/18/2025 Created the StringUtil.h class
// 11/20/2025 Renamed StringUtil.h to Util.hpp
// 11/22/2025 Moving makeBottomRight function and other repeativtive function that is used multiples tiles to Util.hpp for general use.
// Add compare1D and compair2D functions for comparing 1D and 2D shapes respectively.

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "MyPoint.h"
#include "Comparable.h"
#include "My2DShape.h"
#include "MyShape.h"
#include "My1DShape.h"
#include "ShapeManager.h"

#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <QMessageBox>
#include <QComboBox>
#include <QPushButton>
#include <type_traits>
#include <exception>

/*
FOR STRING CLEARING AND VALIDATION UTILITY, USE ValidationCheckString FUNCTION TO AUTOMATICALLY TRIM, LOWERCASE AND CHECK FOR ALPHABETIC WORDS.
*/

inline auto showErrorAlert(const std::string& title, const std::string& message, std::exception& exception){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Critical);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    msg.exec();
}

inline auto showErrorAlert(const std::string& title, const std::string& message){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Critical);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    msg.exec();
}

inline auto showInfoAlert(const std::string& title, const std::string& message){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Information);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    msg.exec();
}

inline auto showWarningAlert(const std::string& title, const std::string& message){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Warning);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    msg.exec();
}

inline auto showQuestionAlert(const std::string& title, const std::string& message){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Question);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    msg.exec();
}

inline auto showWarningCustom(const std::string& title, const std::string& message, const std::string& btn1Text, const std::string& btn2Text, std::function<void(bool)> callback){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Warning);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    QPushButton* btn1 = msg.addButton(QString::fromStdString(btn1Text), QMessageBox::AcceptRole);
    QPushButton* btn2 = msg.addButton(QString::fromStdString(btn2Text), QMessageBox::RejectRole);
    msg.exec();
    if (msg.clickedButton() == btn1) {
        callback(true);
    } else {
        callback(false);
    }
}

inline auto showInfoCustom(const std::string& title, const std::string& message, const std::string& btn1Text, const std::string& btn2Text, bool callback){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Information);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    QPushButton* btn1 = msg.addButton(QString::fromStdString(btn1Text), QMessageBox::AcceptRole);
    QPushButton* btn2 = msg.addButton(QString::fromStdString(btn2Text), QMessageBox::RejectRole);
    msg.exec();
    if (msg.clickedButton() == btn1) {
        callback=true;
    } else {
        callback=false;
    }
}

inline auto showWarningCustom(const std::string& title, const std::string& message, const std::string& btn1Text, const std::string& btn2Text, bool callback){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Warning);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    QPushButton* btn1 = msg.addButton(QString::fromStdString(btn1Text), QMessageBox::AcceptRole);
    QPushButton* btn2 = msg.addButton(QString::fromStdString(btn2Text), QMessageBox::RejectRole);
    msg.exec();
    if (msg.clickedButton() == btn1) {
        callback=true;
    } else {
        callback=false;
    }
}

inline auto showInfoCustom(const std::string& title, const std::string& message, const std::string& btn1Text, const std::string& btn2Text, std::function<void(bool)> callback){
    QMessageBox msg;
    msg.setIcon (QMessageBox::Information);
    msg.setWindowTitle (QString::fromStdString(title));
    msg.setText (QString::fromStdString(message));
    QPushButton* btn1 = msg.addButton(QString::fromStdString(btn1Text), QMessageBox::AcceptRole);
    QPushButton* btn2 = msg.addButton(QString::fromStdString(btn2Text), QMessageBox::RejectRole);
    msg.exec();
    if (msg.clickedButton() == btn1) {
        callback(true);
    } else {
        callback(false);
    }
}
////
// trim helpers, Java trim function equivalent
inline std::string trim(const std::string& s) {
    const char* ws = " \t\n\r\f\v";
    auto first = s.find_first_not_of(ws);
    if (first == std::string::npos) return std::string();
    auto last = s.find_last_not_of(ws);
    return s.substr(first, last - first + 1);
}

//// Check if the entered value is an Alphabet or words. Return false if it is empty or contains number/special character/non alphabetic.
inline bool isAlphaWord(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Input string cannot be empty.");
    return std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isalpha(c); });
    // std member function "all_of" from <algorithm> needs C++11 and will be marked as error if not using C++ header (.HPP)
}

// Convert to lower case
inline std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}

inline std::string toUpperCase(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return upperStr;
}
////

// This check function is use to lowercase, check for alphabetic word and trim the input string automatically.
inline std::string validationCheckString(const std::string& input){
    try {
    std::string result = input;
    result = toLowerCase(input);
    result = trim(result);
    isAlphaWord(result);
    return result;
    } catch (std::exception &e){
        showErrorAlert("Error", e.what(), e);
        return "";
    }
}

inline std::string validationCheckStringUp(const std::string& input){
    try {
    std::string result = input;
    result = toUpperCase(input);
    result = trim(result);
    isAlphaWord(result);
    return result;
    } catch (std::exception &e){
        showErrorAlert("Error", e.what(), e);
        return "";
    }
}

// General Make Bottom Right Point function
inline MyPoint* makeBottomRight(const MyPoint* topLeft, const double width, const double height){
    if (topLeft == nullptr) {
        throw std::invalid_argument("Top left point cannot be null.");
    }
    return new MyPoint(topLeft->getX()+width, topLeft->getY()+height);
}

// Compair two 1D shapes and return -1 if a<b or 1 if a>b or 0 if equal
inline int compare1D(const Comparable& a, const Comparable& b){
    auto p = dynamic_cast<const My1DShape*>(&a);
    auto q = dynamic_cast<const My1DShape*>(&b);
    if (!p || !q) {
        throw std::invalid_argument("Invalid comparison between different types.");
    }
    double Lena = p->calculateLength();
    double Lenb = q->calculateLength();
    const double eps = 1e-12;
    if (std::fabs(Lena - Lenb) <= eps) {
        return 0;
    }
    return (Lena < Lenb) ? -1 : 1;
}

// Compair two 2D shapes by area and return -1 if a<b or 1 if a>b or 0 if equal
inline int compare2D(const Comparable& a, const Comparable& b){
    auto p = dynamic_cast <const My2DShape*>(&a);
    auto q = dynamic_cast <const My2DShape*>(&b);
    if (!p || !q) {
        throw std::invalid_argument("Invalid comparison between different types.");
    }
    double AreaA = p->calculateArea();
    double AreaB = q->calculateArea();
    const double eps = 1e-12;
    if (std::fabs(AreaA - AreaB) <= eps) {
        return 0;
    }
    return (AreaA < AreaB) ? -1 : 1;
}

// template <typename T>
// bool isValidType(const T& value) {
//     if constexpr (std::is_same_v<T, int> ||
//                   std::is_same_v<T, double> ||
//                   std::is_same_v<T, float> ||
//                   std::is_same_v<T, long> ||
//                   std::is_base_of_v<MyShape, T> ||
//                   std::is_base_of_v<ShapeManager, T> ||
//                   std::is_same_v<T, std::string>) {
//         return true;
//     } 
//     else if constexpr (std::is_pointer_v<T>) {
//         // check only if the type is valid as a point
//         return value != nullptr;
//     } else{
//         return false;
//     }
// }


#endif