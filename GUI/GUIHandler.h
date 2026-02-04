#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <string>
#include <QMainWindow>

#include "../MyShape.h"

// Forward declarations
class GUIPrivate;
class CanvasHelper;
class QGraphicsView;
class QToolBar;
class MouseHandler;
class QLabel;
class QHBoxLayout;
class QDockWidget;
class ColorPicker;
class QPushButton;
class GUIHandler : public QMainWindow{
    public: 
    GUIHandler();
    ~GUIHandler();

    void start();
    CanvasHelper* getCanvas();

    // Use for GUI display only
    inline static bool isFilled = false;
    inline static bool isStroked = true;
    inline static bool rgbEnable;
    inline static double gridLine = 0.0;
    inline static std::string userStrokeColor;
    inline static std::string userFillColor;
    inline static std::string startGradient, endGradient;
    inline static int r, g, b;
    inline static std::unique_ptr<QColor> rgbColor;
    inline static void updatePropertyCallback();

    private:
    GUIPrivate* m_guiPrivate;
    std::shared_ptr<CanvasHelper> m_canvas;
    std::unique_ptr<QGraphicsView> m_gv;
    std::unique_ptr<QToolBar> m_toolbar;
    std::unique_ptr<MouseHandler> mouseHandler;
    QLabel* coodinatePreview;
    QHBoxLayout* MainMenu, *shapeButton, *topContainer, *rightContainer;
    QDockWidget* gridsizebox, *lineWidthbox, *colorSeletionbox, *rgbStatusbox, *rgbSeletionbox, *propertyDisplay;
    // Buttons
    QPushButton* lineBtn , *triangleBtn, *squareBtn, *rectangleBtn, *circleBtn, *cancelBtn, *backBtn, *drawShapeBtn, *addShapeBtn;
};

#endif // GUIHANDLER_H