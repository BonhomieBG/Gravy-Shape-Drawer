#include "ShapeManager.h"
#include "MyShape.h"
#include "My1DShape.h"
#include "Utility.hpp"

#include <vector>

void ShapeManager::addShape(std::shared_ptr<MyShape> shape){
    shapes_.push_back(std::move(shape));
}

bool ShapeManager::removeShape(int index){
    if (index < 0 || index >= static_cast<int>(shapes_.size())){
        return false;
    }
    shapes_.erase(shapes_.begin() + index);
    return true;
}

void ShapeManager::removeAllShapes(){
    shapes_.clear();
}

void ShapeManager::sort1DShapes() const{
    std::vector<const My1DShape*> oneDShapes;
    oneDShapes.reserve(shapes_.size()); // reserve place for all shapes avaiable

    // Add method to display if needed

    for (const auto& up: shapes_){
        if (!up) continue;
        if (const My1DShape* first = dynamic_cast<const My1DShape*>(up.get())){
            oneDShapes.push_back(first);
        }
    }

    std::sort(oneDShapes.begin(), oneDShapes.end(),
        [](const My1DShape* a, const My1DShape* b){
            return compare1D(*a, *b); // Assending from top to buttom
        }
    );
}

void ShapeManager::sort2DShapes() const{
    std::vector<const My2DShape*> twoDShapes;
    twoDShapes.reserve(shapes_.size()); // reserve place for all shapes avaiable

    // Add method to display if needed

    for (const auto& up: shapes_){
        if (!up) continue;
        if (const My2DShape* first = dynamic_cast<const My2DShape*>(up.get())){
            twoDShapes.push_back(first);
        }
    }

    std::sort(twoDShapes.begin(), twoDShapes.end(),
        [](const My2DShape* a, const My2DShape* b){
            return compare2D(*a, *b);
        }
    );
}

void ShapeManager::findMax1DShape() const {
    std::shared_ptr<My1DShape> maxShape = nullptr;
    int count = 0;

    for (const auto& up: shapes_){
        count++;
        if (!up) continue;
        if (const My1DShape* shape = dynamic_cast<const My1DShape*>(up.get())){
            if (!maxShape || compare1D(*shape, *maxShape) > 0){
                maxShape = std::dynamic_pointer_cast<My1DShape>(up);
            }
        }
    }

    if (maxShape){
        std::ostringstream oss;
        oss.precision(2);
        oss<< "Shape: %s | Length= %f | Shape Number: %d",
            maxShape->getName(),
            maxShape->calculateLength(),
            count;
        showInfoAlert ("Longest 1D Shape",  oss.str().c_str());
    }
}

void ShapeManager::findMax2DShape() const {
    std::shared_ptr<My2DShape> maxShape = nullptr;
    int count = 0;

    for (const auto& up: shapes_){
        count++;
        if (!up) continue;
        if (const My2DShape* shape = dynamic_cast<const My2DShape*>(up.get())){
            if (!maxShape || compare2D(*shape, *maxShape) > 0){
                maxShape = std::dynamic_pointer_cast<My2DShape>(up);
            }
        }
    }

    if (maxShape){
        std::ostringstream oss;
        oss.precision(2);
        oss << "Shape: " << maxShape->getName() 
            << " | Area= " << std::fixed << maxShape->calculateArea()
            << " | Perimeter= " << maxShape->calculatePerimeter()
            << " | Shape Number: " << count;
        showInfoAlert ("Largest 2D Shape",  oss.str().c_str());
    }
}

std::vector<std::shared_ptr<MyShape>> ShapeManager::getAllShapes() const{
    std::vector<std::shared_ptr<MyShape>> shapeCopies;
    shapeCopies.reserve(shapes_.size());

    for (const auto& up: shapes_){
        if (up){
            shapeCopies.push_back(up);
        }
    }

    return shapeCopies;
}

std::shared_ptr<MyShape> ShapeManager::getShape(int index) const {
    if (index >= 0 && index < static_cast<int>(shapes_.size())) {
        return shapes_[index];
    }
    return nullptr;
}