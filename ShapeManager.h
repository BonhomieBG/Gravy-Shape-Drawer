#ifndef SHAPEMANAGER_H
#define SHAPEMANAGER_H

#include <vector>
#include <memory>

#include "DisableCopyMove.h"

class MyShape;

class ShapeManager {
    public:
        DISABLE_COPY_MOVE(ShapeManager)

        ShapeManager() = default;
        virtual ~ShapeManager() = default;

        void addShape(std::shared_ptr<MyShape> shape);
        bool removeShape(int index);
        void removeAllShapes();
        void sort1DShapes() const;
        void sort2DShapes() const;
        void findMax1DShape() const;
        void findMax2DShape() const;
        std::vector<std::shared_ptr<MyShape>> getAllShapes() const;
        std::shared_ptr<MyShape> getShape(int index) const;

    private:
        std::vector<std::shared_ptr<MyShape>> shapes_;
};


#endif // SHAPEMANAGER_H